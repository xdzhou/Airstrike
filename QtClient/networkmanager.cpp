#include "networkmanager.h"
#include "messages.h"
#include "keys.h"
#include <QKeyEvent>
#include <QTimer>
#include <QDebug>

void showMsg(AS_message_t * msg){
    printf("[clientID=%d, msgType=%d, dada=%d, name=%s]\n", msg->client_id, msg->mess_type, msg->data, msg->name);
}

NetworkManager::NetworkManager()
{
    //this->game_server_ip = NULL;
    context = zmq_ctx_new ();
    next_time = 0;
    keep_running = 1;
    myClientId = -1;
    //this->login;
    startTime = 0;
    requestedTeam=0;
    req_socket=NULL;
    req_game_socket=NULL;
    sub_socket=NULL;
    push_socket=NULL;
}

NetworkManager* NetworkManager::getInstance() {
    return new NetworkManager;
};

NetworkManager::~NetworkManager()
{

}

void NetworkManager::getServerList(const char * primier_server_ip){
    int rc;
    char tcp_adress[100] = "tcp://";
    strcat(tcp_adress, primier_server_ip);
    strcat(tcp_adress, ":5008");
    printf("primier_server_ip : %s\n", tcp_adress);

    req_socket = zmq_socket (context, ZMQ_REQ);
    rc = zmq_connect (req_socket, tcp_adress);
    assert(rc==0);
    int nb_server = 0;

    while(1){
        Message_server_t server_info;
        server_info.mess_type = MSG_GET_SERVER;
        server_info.flag = nb_server;
        rc = s_send_server_msg(req_socket, &server_info);
        assert(rc!=-1);
        printf("get server info request sended\n");
        Message_server_t *msg = s_recv_server_msg(req_socket);
        //printf("serverName:%s ip:%s flag:%d\n",msg->server_info.name, msg->server_info.ipadress, msg->flag);
        if(msg->flag == 1){
            printf("[%s] = %s\n", msg->server_info.name, msg->server_info.ipadress);
            //list_servers[nb_server] = msg->server_info;
            emit addNewRadioBtn(msg->server_info.name, msg->server_info.ipadress, nb_server);
            nb_server++;
        }else{
            zmq_close (req_socket);
            break;
        }
    }

}


int NetworkManager::network_init(){
    printf("NetworkManager - network init\n");
    //Hello Msg to send
    AS_message_t msg;
    msg.mess_type = MSG_HELLO;
    msg.client_id = myClientId;
    msg.data = requestedTeam;
    strcpy(msg.name, login.toStdString().c_str());
    s_send_msg(req_game_socket, &msg);

    AS_message_t *as_msg = s_recv_msg(req_game_socket);
    process_packet(as_msg);
    setSubSocket();
    free(as_msg);
    //zmq_close (req_game_socket);

    network_loop();   
    return 0;
}

void NetworkManager::network_loop(){
    printf("NetworkManager - network loop\n");
    int flag = 0; 
    keep_running = 1;
    while(keep_running==1){
        AS_message_t *msg = s_recv_sub(sub_socket);
        process_packet(msg);
        free(msg);
        if(flag==0){
            flag = 1;
            sendMessage(MSG_READY, myClientId, 0);
        }
    }
    printf("keep_running == 0 \n");
}

void NetworkManager::process_packet(AS_message_t * msg){
    showMsg(msg);
    switch (msg->mess_type) {
    case MSG_POINTS:
        emit newPlayerScore(msg->data);
        break;
    case MSG_HELLO:       
        myClientId = msg->client_id;
        emit newPlayerId(myClientId);
        writeText("Connected");
        break;
    case MSG_NO_SPACE:
        writeText("Plus de place, reconnectez-vous plus tard.");
        emit disconnected();
        break;
    case MSG_TIME2START:
        //emit newStartTime(msg->data);
        if(msg->data != startTime){
            writeText("Decollage dans " + QString::number(msg->data) + " seconde(s)");
            startTime = msg->data;
        }
        break;
    case MSG_DAMAGE:
        emit newHealthPoints(100 - msg->data);
        break;
    case MSG_TEAM_ID:
        writeText("Recu team id");
        emit newTeamId(msg->data);
        break;
    case MSG_ID_IN_TEAM:
        emit newIdInTeam(msg->data);
        qDebug()<< "Recu id in team"<< msg->data;
        break;
    case MSG_DISCONNECTED:
        keep_running = 0;
        writeText("Disconnected");
        break;
    default:
        break;
    }
    //enet_packet_destroy(event->packet);
}

void NetworkManager::sendMessage(int msgType, int clientId, int data)
{
    AS_message_t msg;
    msg.mess_type = msgType;
    msg.client_id = clientId;
    msg.data = data;
    //msg.name = "Noki";
    //strcpy(msg.name,"Noki\0");
    strcpy(msg.name, login.toUtf8().data());
    //printf("client send msg: ");
    showMsg(&msg);
    s_send_msg(push_socket, &msg);   
}

void NetworkManager::set_rand_key(){
    set_key(3);
    int num = rand();
    int key;
    if (num<(RAND_MAX/3)){
        key = KEY_FIRE;
        if (rand()>(RAND_MAX/2)){
            sendMessage(MSG_KEY, myClientId, key);
        }else{
            sendMessage(MSG_KEY, myClientId, -key);
        }
    }
    if (num<(RAND_MAX/3)) {
        key = KEY__UP;
        if (rand()<(RAND_MAX/4)){
            sendMessage(MSG_KEY, myClientId, key);
        }else{
            sendMessage(MSG_KEY, myClientId, -key);
        }
    }
    if (num<(RAND_MAX/3)) {
        key = KEY__DOWN;
        if (rand()<(RAND_MAX/3)){
            sendMessage(MSG_KEY, myClientId, key);
        }else{
            sendMessage(MSG_KEY, myClientId, -key);
        }
    }
}

void NetworkManager::setLogin(QString newLogin)
{
    printf("NetworkManager - set login\n");
    this->login = newLogin;
}

void NetworkManager::set_key(int key){
    printf("NetworkManager - set_key\n");
    sendMessage(MSG_KEY, myClientId, key);
}

void NetworkManager::setSubSocket()
{
    assert(myClientId != -1);
    if(sub_socket==NULL){
        int rc;
        char sub_adress[60];
        strcpy(sub_adress,"tcp://");
        strcat(sub_adress,game_server_ip.toStdString().c_str());
        strcat(sub_adress,":5009");
        sub_socket =zmq_socket (context, ZMQ_SUB);
        rc = zmq_connect (sub_socket, sub_adress);
        assert(rc==0);
        zmq_setsockopt (sub_socket, ZMQ_SUBSCRIBE,&myClientId, sizeof(int));
        printf("sub_socket - %s  with topic(clientId)- %d\n", sub_adress, myClientId);
    }
    
}

void NetworkManager::setGameServerIP(QString game_server_ip)
{
    printf("NetworkManager - set game server ip\n");
    this->game_server_ip = game_server_ip;

    if(myClientId==-1){
        int rc;
        char tcp_adress[60] = "tcp://";
        strcat(tcp_adress, game_server_ip.toStdString().c_str());
        strcat(tcp_adress, ":5008");   
        req_game_socket = zmq_socket (context, ZMQ_REQ);
        rc = zmq_connect (req_game_socket, tcp_adress);
        assert(rc==0);
        printf("req_game_socket : %s\n", tcp_adress);

        push_socket = zmq_socket (context, ZMQ_PUSH);
        char push_adress[60];
        strcpy(push_adress,"tcp://");
        strcat(push_adress,game_server_ip.toStdString().c_str());
        strcat(push_adress,":5010");
        rc = zmq_connect (push_socket, push_adress);
        assert(rc==0);
        printf("push_socket - %s\n", push_adress);
    }
}

void NetworkManager::setRequestedTeam(int team)
{
    this->requestedTeam = team;
}

void NetworkManager::disconnectClient()
{

    if(keep_running == 1){
        printf("client dis connecting\n");
        sendMessage(MSG_DISCONNECTED, myClientId, 0);
    }
}

void NetworkManager::process_key(QKeyEvent * event, int key_status){
    //writeText("Process key");
    if(keep_running == 1){
        if (event->key() == Qt::Key_Right )
            set_key(key_status*KEY__DOWN);
        if (event->key() == Qt::Key_Left)
            set_key(key_status*KEY__UP);
        if (event->key() == Qt::Key_Down || event->key() == Qt::Key_Up)
            set_key(key_status*KEY__ACCELERATE);
        if (event->key() == Qt::Key_Control)
            set_key(key_status*KEY_FIRE);
        if (event->key() == Qt::Key_Shift)
            set_key(key_status*KEY_BOMB);
    }
    
}

void NetworkManager::start()
{
    network_loop();
    QTimer::singleShot(20, this, SLOT(start()));
}
