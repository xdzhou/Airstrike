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

using namespace ExitGames::Common;
using namespace ExitGames::Photon;
using namespace std;

class PhotonLib : public PhotonListener
{
    public:
        PhotonLib(void);
        virtual ~PhotonLib(void);
        void startwork(const JString& ipAddrPhoton, char* airstrikeIP);

    protected:

    private:
	void debug();
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

};

extern "C" void NotifyPhotonServer(char * ipadressPhoton);
char* GetLocalIp();
void showMsg(JString s);
