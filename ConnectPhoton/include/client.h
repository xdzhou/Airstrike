#ifndef CLIENT_H
#define CLIENT_H
#include "Photon-cpp/inc/PhotonPeer.h"
#include "Photon-cpp/inc/OperationRequest.h"
#include "Photon-cpp/inc/OperationResponse.h"
#include "Photon-cpp/inc/PhotonListener.h"
#include "Common-cpp/inc/Dictionary.h"

class client : public ExitGames::Photon::PhotonListener
{
    public:
        client(void);
        virtual ~client(void);
        void startwork(void);

    protected:

    private:
        void onOperationResponse(const ExitGames::Photon::OperationResponse& operationResponse);
        void onStatusChanged(int statusCode);
        void onEvent(const ExitGames::Photon::EventData& eventData);
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
        ExitGames::Photon::PhotonPeer* peer;

};

#endif // CLIENT_H
