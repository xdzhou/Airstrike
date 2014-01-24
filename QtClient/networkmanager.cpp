#include "networkmanager.h"
#include "messages.h"
#include "keys.h"
#include <QKeyEvent>
#include <QTimer>
#include <QDebug>

NetworkManager* NetworkManager::instance = NULL; 

NetworkManager::NetworkManager()
{
    //this->game_server_ip = NULL;
    context = zmq_ctx_new ();
    next_time = 0;
    keep_running = 42;
    myClientId = -1;
    //this->login;
    startTime = 0;
    requestedTeam=0;
    //instance = NULL;
}

NetworkManager* NetworkManager::getInstance() {
    if (!instance) instance = new NetworkManager;
    return instance;

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
    //Hello Msg to send
    AS_message_t msg;
    msg.mess_type = MSG_HELLO;
    msg.client_id = myClientId;
    msg.data = requestedTeam;
    strcpy(msg.name, login.toStdString().c_str());
    s_send_msg(push_socket, &msg);
    printf("Hello Msg Send\n");

    network_loop();
    
    return 0;
}

void NetworkManager::process_packet(AS_message_t * msg){
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
    default:
        break;
    }
    //enet_packet_destroy(event->packet);
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
    this->login = newLogin;
}

void NetworkManager::set_key(int key){
    sendMessage(MSG_KEY, myClientId, key);
}

void NetworkManager::network_loop(){
    while(1){
        AS_message_t *msg = s_recv_sub(sub_socket);
        process_packet(msg);
        free(msg);
    }
}

void NetworkManager::testFunction()
{
    emit writeText("Test");
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
    s_send_msg(push_socket, &msg);
    printf ("Client PUSH msg = %s\n", msg.name);
    
}

void NetworkManager::setGameServerIP(QString game_server_ip)
{
    this->game_server_ip = game_server_ip;

    char sub_adress[60];
    strcpy(sub_adress,"tcp://");
    strcat(sub_adress,game_server_ip.toStdString().c_str());
    strcat(sub_adress,"5009");
    sub_socket =zmq_socket (context, ZMQ_SUB);
    zmq_connect (sub_socket, sub_adress);
    zmq_setsockopt (sub_socket, ZMQ_SUBSCRIBE,&myClientId, sizeof(int));
    printf("sub_socket - %s\n", sub_adress);

    push_socket = zmq_socket (context, ZMQ_PUSH);
    char push_adress[60];
    strcpy(push_adress,"tcp://");
    strcat(push_adress,game_server_ip.toStdString().c_str());
    strcat(push_adress,"5010");
    zmq_connect (push_socket, push_adress);
    printf("push_socket - %s\n", push_adress);

}

void NetworkManager::setRequestedTeam(int team)
{
    this->requestedTeam = team;
}

void NetworkManager::disconnectClient()
{
    /*
    ENetEvent event;

    enet_peer_disconnect (peer, 0);

    // Allow up to 3 seconds for the disconnect to succeed and drop any packets received packets.
    while (enet_host_service (client, & event, 3000) > 0)
    {
        switch (event.type)
        {
        case ENET_EVENT_TYPE_RECEIVE:
            enet_packet_destroy (event.packet);
            break;

        case ENET_EVENT_TYPE_DISCONNECT:
            writeText("Disconnection succeeded.");
            emit disconnected();
            return;
        }
    }

    // We've arrived here, so the disconnect attempt didn't 
    // succeed yet.  Force the connection down.             
    enet_peer_reset (peer);
    */
}

void NetworkManager::process_key(QKeyEvent * event, int key_status){
    //writeText("Process key");
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

void NetworkManager::start()
{
    network_loop();
    QTimer::singleShot(20, this, SLOT(start()));
}
