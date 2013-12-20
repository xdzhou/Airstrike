#include "Photon-cpp/inc/PhotonPeer.h"
#include "Photon-cpp/inc/OperationRequest.h"
#include "Photon-cpp/inc/OperationResponse.h"
#include "Photon-cpp/inc/PhotonListener.h"
#include "Common-cpp/inc/Dictionary.h"

using namespace ExitGames::Common;
using namespace ExitGames::Photon;

class PhotonLib : public PhotonListener
{
    public:
        PhotonLib(void);
        virtual ~PhotonLib(void);
        void startwork(void);

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
                JOINING,
                JOINED,
                SENT_DATA,
                RECEIVED_DATA,
                LEAVING,
                LEFT,
                DISCONNECTING,
                DISCONNECTED
            };
        };

        State::States mState;
        PhotonPeer* peer;

};


