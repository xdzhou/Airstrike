#ifndef photonmanager_H
#define photonmanager_H

#include <enet/enet.h>
#include <QObject>
#include <QString>
#include <QWidget>
#include <arpa/inet.h>
#include "Photon-cpp/inc/PhotonPeer.h"
#include "Photon-cpp/inc/OperationRequest.h"
#include "Photon-cpp/inc/OperationResponse.h"
#include "Photon-cpp/inc/PhotonListener.h"
#include "Common-cpp/inc/Dictionary.h"

class photonmanager : public QObject, public PhotonListener
{
    Q_OBJECT

private:
    QString ip_addr;
    int port;
    int next_time;
    int keep_running;
    int myClientId;
    QString login;
    int startTime;
    int requestedTeam;
    //photon SDK
    void onOperationResponse(const OperationResponse& operationResponse);
    void onStatusChanged(int statusCode);
    void onEvent(const EventData& eventData);
    void debugReturn(ExitGames::Common::DebugLevel::DebugLevel debugLevel, const ExitGames::Common::JString& string);

        class State
        {
        public:
            enum States
            {
                INITIALIZED = 0,
                CONNECTING,
                CONNECTED,
                SENDING_IPADRESS,
                SENDED,
                DISCONNECTING,
                DISCONNECTED
            };
        };

    State::States mState;
    PhotonPeer* peer;

public:
    photonmanager();
    ~photonmanager();
    void set_key(int key);
    void network_loop();
    void process_packet(ENetEvent * event);
    void update_state();
    void testFunction();
    void sendMessage(int msgType,int clientId,int data = 0);
    //photon SDK
    void startwork(const JString& ipAddrPhoton, char* airstrikeIP);

public slots:
    void start();
    void process_key(QKeyEvent * event, int key_status);
    int network_init();
    void set_rand_key();
    void setLogin(QString newLogin);
    void disconnectClient();
    void setIP(QString ip_addr = "127.0.0.1", int port=1234);
    void setRequestedTeam(int team);

signals:
    void writeText(QString text);
    void newPlayerScore(int score);
    void newHealthPoints(int value);
    void newPlayerId(int value);
    void newTeamId(int value);
    void newIdInTeam(int value);
    //void newStartTime(int value);
    void disconnected();

};

#endif // photonmanager_H
