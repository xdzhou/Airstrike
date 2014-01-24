#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QString>
#include <QWidget>
#include "zmqhelper.h"

class NetworkManager : public QObject
{
    Q_OBJECT

private:
    QString game_server_ip;
    int next_time;
    int keep_running;
    int myClientId;
    QString login;
    int startTime;
    int requestedTeam;
    NetworkManager();
    static NetworkManager* instance;
    void *context;
    void *req_socket;
    void *sub_socket;
    void *push_socket;

public:
    static NetworkManager* getInstance();
    ~NetworkManager();
    void set_key(int key);
    void network_loop();
    void process_packet(AS_message_t * msg);
    void update_state();
    void testFunction();
    void sendMessage(int msgType,int clientId,int data = 0);


public slots:
    void start();
    void process_key(QKeyEvent * event, int key_status);
    int network_init();
    void set_rand_key();
    void setLogin(QString newLogin);
    void disconnectClient();
    void setGameServerIP(QString game_server_ip);
    void setRequestedTeam(int team);
    void getServerList(const char * primier_server_ip);

signals:
    void writeText(QString text);
    void newPlayerScore(int score);
    void newHealthPoints(int value);
    void newPlayerId(int value);
    void newTeamId(int value);
    void newIdInTeam(int value);
    //void newStartTime(int value);
    void disconnected();
    void addNewRadioBtn(char *name, char *ip, int id);

};

#endif // NETWORKMANAGER_H
