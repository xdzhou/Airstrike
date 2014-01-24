#include "networkmanager.h"
#include "messages.h"
#include "keys.h"
#include <QKeyEvent>
#include <QTimer>
#include <QDebug>

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
}

NetworkManager * NetworkManager::getInstance() {
    if (!instance) {
        instance = new NetworkManager();
    };
    return instance;
};

NetworkManager::~NetworkManager()
{

}

void NetworkManager::getServerList(const char * primier_server_ip){
    char tcp_adress[100] = "tcp://";
    strcat(tcp_adress, primier_server_ip);
    strcat(tcp_adress, ":5008");
    printf("primier_server_ip : %s\n", tcp_adress);

    req_socket = zmq_socket (context, ZMQ_REQ);
    zmq_connect (req_socket, tcp_adress);

    int nb_server = 0;

    while(1){
        Message_server_t server_info;
        server_info.mess_type = MSG_GET_SERVER;
        server_info.flag = nb_server;
        s_send_server_msg(req_socket, &server_info);
        //assert(rc==0);
        Message_server_t *msg = s_recv_server_msg(req_socket);
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
    writeText("Initializing ZeroMQ.");
/*
    if (enet_initialize() != 0) {
        emit writeText("An error occurred while initializing ENet");
        return -1;
    }
    emit writeText("ENet started.");
    client = enet_host_create(NULL // create a client host ,
                              1 // only allow 1 outgoing connection  ,
                              2 // allow up 2 channels to be used, 0 and 1  ,
                              57600 / 8 // 56K modem with 56 Kbps downstream bandwidth  ,
                              14400 / 8 // 56K modem with 14 Kbps upstream bandwidth  );

    if (client == NULL) {
        writeText("An error occurred while trying to create an ENet client host.");
        return -1;
    }

    // Connect to some.server.net:1234. 
    // enet_address_set_host(&address, ip_addr.toStdString().c_str());
    //address.port = port;

    enet_address_set_host(&address, ip_addr.toStdString().c_str());
    address.port = port;

    // Wait up to 5 seconds for the connection attempt to succeed. 
    int trying_left = 5;
    int connect_succeeded=0;
    while (!connect_succeeded){
        writeText("Trying to connect to " + ip_addr + ":" + QString::number(port) + "...");
        // Initiate the connection, allocating the two channels 0 and 1. 
        peer = enet_host_connect(client, &address, 2, 0);

        if (peer == NULL) {
            writeText("No available peers for initiating an ENet connection.");
            emit disconnected();
            return -1;
        }

        if (enet_host_service(client, &event, 3000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
            writeText("Connection to " + ip_addr + ":" + QString::number(port) + "succeeded.");
            connect_succeeded=1;
           // sendMessage(MSG_HELLO,myClientId);
            sendMessage(MSG_HELLO,myClientId,requestedTeam);
        } else {
            // Either the 5 seconds are up or a disconnect event was 
            // received. Reset the peer in the event the 5 seconds   
            // had run out without any significant event.            
            enet_peer_reset(peer);
            //printf("Connection to %s:%d failed.\n",ip_addr,port);
            trying_left--;
            if(trying_left<=0){
                writeText("Could not connect to " + ip_addr + ":" + port);
                emit disconnected();
                return -1;
            }
        }
    }
    start();
*/
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
/*
    int serviceResult;
    // Keep doing host_service until no events are left 
    while ( (serviceResult=enet_host_service(client, &event, 0))!=0) {
        if (serviceResult > 0) {
            switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT:
                //printf("A new client connected from %x:%u.\n", event.peer->address.host, event.peer->address.port);
                event.peer->data = (void *)"New User";
                break;

            case ENET_EVENT_TYPE_RECEIVE:
                process_packet(&event);
                break;

            case ENET_EVENT_TYPE_DISCONNECT:
                writeText("Server disconected.\n");
                emit disconnected();
                //exit(EXIT_FAILURE);
                break;

            case ENET_EVENT_TYPE_NONE:
                break;
            }
        } else if (serviceResult < 0) {
            writeText("Error with servicing the client");
            emit disconnected();
            //exit(EXIT_FAILURE);
        }
*/
}

void NetworkManager::testFunction()
{
    emit writeText("Test");
}

void NetworkManager::sendMessage(int msgType, int clientId, int data)
{
    /*
    // ENetPeer *p = &server->peers[peerId];
    if (!(peer==NULL)){
        AS_message_t msg;
        msg.mess_type=msgType;
        msg.client_id = clientId;
        msg.data = data;
        //msg.name = "Noki";
        //strcpy(msg.name,"Noki\0");
        strcpy(msg.name,login.toUtf8().data());
        ENetPacket *packet = enet_packet_create(&msg, sizeof(AS_message_t), ENET_PACKET_FLAG_RELIABLE);
        enet_peer_send(peer, 0, packet);
    }
    */
}

void NetworkManager::setGameServerIP(QString game_server_ip)
{
    this->game_server_ip = game_server_ip;
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
