#ifndef PHOTONLIB_H
#define PHOTONLIB_H

#include <sys/ioctl.h>  
#include <net/if.h>  
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include "Photon-cpp/inc/PhotonPeer.h"
#include "Photon-cpp/inc/OperationRequest.h"
#include "Photon-cpp/inc/OperationResponse.h"
#include "Photon-cpp/inc/PhotonListener.h"
#include "Common-cpp/inc/Dictionary.h"
#include <QObject>

using namespace ExitGames::Common;
using namespace ExitGames::Photon;
using namespace std;

class PhotonLib : public QObject, public PhotonListener
{
    Q_OBJECT

    public:
        PhotonLib(void);
        virtual ~PhotonLib(void);
        

    protected:

    private:
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
	string serverList[5];

    public slots:
	void startwork(const char * ipAddrPhoton);

    signals:
    	void addNewRadioBtn(const char * name, int id);
};

void showMsg(JString s);
#endif 
