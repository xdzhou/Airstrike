#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <error.h>
#include "network.h"
#include "messages.h"
#include "prototype.h"
#include "players.h"
#include "keys.h"
#include "zmqhelper.h"
#include "sprite.h"
#include <sys/time.h>
#include <sys/resource.h>
#include <net/if.h>  
#include <arpa/inet.h>
#include <sys/ioctl.h>

void *thread_function( void *arg );
void network_loop();
void process_packet(AS_message_t * msg);
void sendMessage(int msgType,int clientId,int data);
void sendMessageLoad(int peerId, int sizeInByte);
void sendServerInfo(char* primaryServerIP, Server_info_t* data);
char* GetLocalIp();
void showMsg(AS_message_t * msg);
void printAllServers();

int  clientConnected[MAXPLAYERS];
int  clientCount= 0;

unsigned int counterOut = 0;
unsigned int counterIn = 0;

unsigned int lastCounterOut = 0;
unsigned int lastCounterIn = 0;

volatile int netStop=0 ;//place to 1 to stop the network
volatile int netStopped=0 ;//place to 1 when network stopped
int fps=0;

int networkLoad = 0;
int networkLoadinterval = 0;
int networkport = 1234;
void * loadData;

void *rep_socket;
void *pub_socket;
void *pull_socket;
void *context;
int nb_server = 0;
// Server_info_t *list_servers;

struct servers_list_record * servers_list;
char* primaryServer = NULL;

void network_init(char* primaryServerIP, char* serverName){
	//copy primary server ip info
	if(primaryServerIP!=NULL){
		primaryServer = (char *) malloc(sizeof(char) * strlen(primaryServerIP) + 1);
		strcpy(primaryServer, primaryServerIP);
	}	
	//  Socket to talk to clients
    context = zmq_ctx_new ();
    int rc;
    rep_socket = zmq_socket (context, ZMQ_REP);
    rc = zmq_bind (rep_socket, "tcp://*:5008");
    assert (rc == 0);

    pub_socket = zmq_socket (context, ZMQ_PUB);
    rc = zmq_bind (pub_socket, "tcp://*:5009");
    assert (rc == 0);

    pull_socket = zmq_socket (context, ZMQ_PULL);
    rc = zmq_bind (pull_socket, "tcp://*:5010");
    assert (rc == 0);

    // list_servers = (Server_info_t *)malloc(sizeof(Server_info_t)*10);
	pthread_t thread;
	rc = pthread_create( &thread, NULL, thread_function, NULL);
	if(rc) error(EXIT_FAILURE, rc, "pthread_create");

	// I am the primary server
	Server_info_t* data = malloc(sizeof(Server_info_t));
	strcpy(data->name, serverName);
	strcpy(data->ipadress, GetLocalIp());
	if (primaryServerIP == NULL){
		printf("I'm a primier Server\n");
		struct servers_list_record* primary = malloc(sizeof(struct servers_list_record));	
		primary->data = data;
		primary->next = NULL;
		servers_list = primary;
		nb_server ++;
	}else{
		sendServerInfo(primaryServerIP, data);
		free(data);
	}
	printAllServers();
}

void *thread_function( void *arg ){
	int i;
	for (i=0; i<MAXPLAYERS;i++){
		clientConnected[i]=0;
	}

	char cb[250];
	time_t h = 0;
	h = time(NULL);
	sprintf(cb,"log/airlog_%dB_%d_ms_%d.log",networkLoad,networkLoadinterval,h);
	logOpen(cb);
	mylog(LOG_INFO,"Starting the game",0);
	sprintf(cb,"%d players; %d teams; load %d Bytes each %dms",playerCount,teamCount,networkLoad,networkLoadinterval);
	mylog(LOG_INFO,cb,0);
	network_loop();
	mylog(LOG_INFO,"Closing the game",0);
	logClose();
	netStopped=1;
}

void network_loop(){
	int prevTimeStat = sprite_global.game_clock;
	int prevTimeLoad = prevTimeStat;
	int prevTimePlay = prevTimeStat;

	int pingCnt ;
	int prev_utime=0;
	int cur_utime=0;
	int prevClock=0;
	int currClock=0;
	struct timeval tim;
	struct rusage usage;
	int tdiff;

	while (netStop!=1) {
		int k;
		if (sprite_global.game_clock-prevTimeStat >= 1000){
			mylog(LOG_NETWORK_TOTAL_OUT,"Total Output", counterOut);
			mylog(LOG_NETWORK_TOTAL_IN,"Total Input", counterIn);
			mylog(LOG_NETWORK_OUT,"Output", counterOut-lastCounterOut);
			mylog(LOG_NETWORK_IN,"Input", counterIn-lastCounterIn);
			mylog(LOG_FPS,"fps", fps);
			mylog(LOG_CLIENT_COUNT,"Clients", clientCount);

			lastCounterOut = counterOut;
			lastCounterIn = counterIn;

			if (!getrusage(RUSAGE_SELF, &usage) && !gettimeofday(&tim, NULL)){
				prev_utime = cur_utime;
				cur_utime = clock();
				prevClock = currClock;
				currClock = tim.tv_sec * 1000000 + tim.tv_usec;
				if ((tdiff = currClock - prevClock) !=0)
					mylog(LOG_CPU,"CPU %",(cur_utime-prev_utime)/(tdiff/100));
			}

			prevTimeStat = sprite_global.game_clock;
		}

		

		zmq_pollitem_t items [] = {{ rep_socket, 0, ZMQ_POLLIN, 0 },{ pull_socket, 0, ZMQ_POLLIN, 0 }};
		while (netStop!=1) {
			//send msg
			if (sprite_global.game_clock-prevTimePlay >= 1000){
				for(k=0;k<playerCount;k++){
					if (clientConnected[k]){
						sendMessage(MSG_POINTS,k,players[k].points);
						sendMessage(MSG_DAMAGE,k,players[k].damage);
						if (players[k].spawnTimer){
							sendMessage(MSG_TIME2START,k,1+(players[k].spawnTimer-sprite_global.game_clock)/1000);
						}
					}
				}
				prevTimePlay = sprite_global.game_clock;
			}
			//receive msg
			int rc = zmq_poll (items, 2, 300);
	        if (rc == -1) break;
			if (items [0].revents & ZMQ_POLLIN) {
				//printf("Request of type REQ.\n");
	            Message_server_t * msg = s_recv_server_msg(rep_socket);
	            //printf("message type: %d flag:%d\n", msg->mess_type, msg->flag);
	            if(msg->mess_type == MSG_GET_SERVER){
	                if(msg->flag < nb_server){
	                	printf("client get server info.\n");
	                    msg->mess_type = MSG_SET_SERVER;
	                    struct servers_list_record* temp = servers_list;
	                    int i;
	                    for (i = 0; i < msg->flag; i++)
	                    	temp = temp->next;
	                    msg->server_info = *(temp->data);
	                    //msg->server_info = list_servers[msg->flag];
	                    msg->flag = 1;
	                }else{
	                    msg->mess_type = MSG_SET_SERVER;
	                    msg->flag = 0;
	                }
	                s_send_server_msg(rep_socket, msg);
	            }else if(msg->mess_type == MSG_SET_SERVER && primaryServer == NULL){             
	                // //list_servers[nb_server] = msg->server_info;
    				struct servers_list_record* new_serv = malloc(sizeof(struct servers_list_record));
					Server_info_t* data = malloc(sizeof(Server_info_t));
					strcpy(data->name, msg->server_info.name);
					strcpy(data->ipadress, msg->server_info.ipadress);
					new_serv->data = data;
					new_serv->next = servers_list;
					servers_list = new_serv;
	                nb_server++;
	                // ?? why are we resending the msg ??
	                s_send_server_msg(rep_socket, msg);
	                printAllServers();
	                free(msg);
	            }else if (msg->mess_type == MSG_HELLO){
	                //as_msg of type HELLO
	                AS_message_t *as_msg = (AS_message_t *)msg;                
	                process_packet(as_msg);
	                //s_send_msg(rep_socket, as_msg);
	            }
	            else{
	                printf("unknown server info message!\n");
	            }
	        }
	        if (items [1].revents & ZMQ_POLLIN) {
	        	AS_message_t * msg = s_recv_msg(pull_socket);
	            process_packet(msg);
	        }

		}
	}
}


void sendMessage(int msgType,int clientId,int data){
	AS_message_t msg;
	msg.mess_type = msgType;
	msg.client_id = clientId;
	msg.data = data;
	msg.name[0] = '\0';
	s_send_pub(pub_socket, &msg, clientId);
	counterOut += sizeof(AS_message_t);
	//printf("server send msg: ");
    //showMsg(&msg);
}

void process_packet(AS_message_t * msg){
	showMsg(msg);
	counterIn += sizeof(AS_message_t);
	switch (msg->mess_type) {
	case MSG_HELLO:
		mylog(LOG_MESSAGE,"MSG_HELLO message received from",msg->client_id);
		int client_id;
		int success=0;
		int teamRequested = msg->data;
		if (msg->client_id>=0 && msg->client_id<playerCount && !clientConnected[msg->client_id] && players[msg->client_id].team->id == teamRequested){
			client_id = msg->client_id;
			/////////////
			s_send_msg(rep_socket, msg);
			/////////////
			clientCount++;
			clientConnected[client_id]=1;
			players[client_id].isConnected=1;
			strncpy(players[client_id].name,msg->name,32);
			printf("*******************************%s   %s\n",msg->name,players[client_id].name);
			players[client_id].name[31]='\0';
			//sendMessage(MSG_HELLO,client_id,client_id);
			//sendMessageTeam(MSG_TEAM_ID,client_id,players[client_id].team->id);
			//sendMessageTeam(MSG_ID_IN_TEAM,client_id,players[client_id].id_in_team);
			mylog(LOG_MESSAGE,"MSG_HELLO sent to",msg->client_id);
			success=1;
		}else{
			int i;
			for (i=0;i < playerCount; i++){
				if (!clientConnected[i] && players[i].team->id == teamRequested){
					client_id = i;
					/////////////
					msg->client_id = i;
					s_send_msg(rep_socket, msg);
					/////////////
					clientCount++;
					clientConnected[client_id]=1;
					players[client_id].isConnected=1;
					strncpy(players[client_id].name,msg->name,32);
					printf("*******************************%s   %s\n",msg->name,players[client_id].name);
					players[client_id].name[31]='\0';
					//sendMessage(MSG_HELLO,client_id,client_id);
					//sendMessageTeam(MSG_TEAM_ID,client_id,players[client_id].team->id);
					//sendMessageTeam(MSG_ID_IN_TEAM,client_id,players[client_id].id_in_team);
					mylog(LOG_MESSAGE,"MSG_HELLO sent to",msg->client_id);
					success=1;
					break;

				}
			}
		}
		if(!success){
			sendMessage(MSG_NO_SPACE,0,0);
			mylog(LOG_MESSAGE,"MSG_NO_SPACE sent to",msg->client_id);
			break;
		}
		break;
	case MSG_READY:
		assert(msg->client_id!=-1);
		sendMessage(MSG_TEAM_ID, msg->client_id, players[msg->client_id].team->id);
		sendMessage(MSG_ID_IN_TEAM, msg->client_id, players[msg->client_id].id_in_team);
		break;
	case MSG_KEY:
		//printf("Key %d message received from %d\n",msg->data,peerID);
		if (clientConnected[msg->client_id]){
			if (msg->data >= 0){
				network_keymap[msg->client_id][msg->data]=1;
			}else{
				network_keymap[msg->client_id][-msg->data]=0;
			}
		}
		break;
	case MSG_DISCONNECTED:
		printf("client %d disconected.\n", msg->client_id);
		sendMessage(MSG_DISCONNECTED, msg->client_id, 0);
		int i = msg->client_id;
		clientConnected[i]=0;
		strncpy(players[i].name," \0",32);
		players[i].isConnected=0;
		clientCount-=1;
		mylog(LOG_NETWORK_DISCONNECT,"A client disconnected ",i);
		break;
	default:
		break;
	}
	free(msg);
}

void sendServerInfo(char *primaryServerIP, Server_info_t* data){
	int rc;
	printf("I'm a normal Server\n");
	void *req_socket = zmq_socket (context, ZMQ_REQ);
    char tcp_adress[100] = "tcp://";
    strcat(tcp_adress, primaryServerIP);
    strcat(tcp_adress, ":5008");
    printf("temp req_socket - %s\n", tcp_adress);
    rc = zmq_connect (req_socket,  tcp_adress);
    assert(rc == 0);
    
	Message_server_t msg;
	msg.mess_type = MSG_SET_SERVER;
	msg.server_info = *data;
	msg.flag = 0;
	s_send_server_msg(req_socket, &msg);
	Message_server_t *msg_server = s_recv_server_msg(req_socket);
	free(msg_server);
	zmq_close (req_socket);
}

char* GetLocalIp(){          
    int MAXINTERFACES=16;    
    char *ip = NULL;    
    int fd, intrface, retn = 0;      
    struct ifreq buf[MAXINTERFACES];      
    struct ifconf ifc;      
  
    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) >= 0){      
        ifc.ifc_len = sizeof(buf);      
        ifc.ifc_buf = (caddr_t)buf;      
        if (!ioctl(fd, SIOCGIFCONF, (char *)&ifc)){      
            intrface = ifc.ifc_len / sizeof(struct ifreq);       
            while (intrface-- > 0){      
                if (!(ioctl (fd, SIOCGIFADDR, (char *) &buf[intrface]))){      
                    ip=(inet_ntoa(((struct sockaddr_in*)(&buf[intrface].ifr_addr))->sin_addr));      
                    break;    
                }                          
            }    
        }      
        close (fd);   
    }
    return ip;
}

void showMsg(AS_message_t * msg){
	printf("[clientID=%d, msgType=%d, dada=%d, name=%s]\n", msg->client_id, msg->mess_type, msg->data, msg->name);
}

void printAllServers() {
	struct servers_list_record * temp = servers_list;
	while(temp != NULL) {
		printf("[%s - %s]\n", temp->data->name, temp->data->ipadress);
		temp = temp->next;
	}
}