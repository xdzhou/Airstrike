#include "PhotonLib.h"

#if defined _EG_WINDOWS_PLATFORM
#	define SLEEP(ms) Sleep(ms)
#else
#	if defined _EG_LINUX_PLATFORM || defined _EG_MARMALADE_PLATFORM || defined _EG_ANDROID_PLATFORM || defined _EG_BLACKBERRY_PLATFORM
#		include "unistd.h"
#	endif
#	define SLEEP(ms) usleep(ms*1000)
#endif

PhotonLib::PhotonLib(void)
{
    peer = new PhotonPeer(*this);
    mState = State::INITIALIZED;
}

PhotonLib::~PhotonLib(void)
{
    delete peer;
}

void PhotonLib::startwork(void){

    while(true){
        printf(".\n");
        switch(mState){
            case State::INITIALIZED:
                peer->connect(JString(L"192.168.1.24:5055"));
                mState = State::CONNECTING;
                printf("Connecting \n");
                break;

            case State::CONNECTED:
                {
                    printf("Connected \n");
                    char * login = "test";
                    char * password = "1234";
                    OperationRequestParameters op;
                    op.put(1, ValueObject<JString>(login));
                    op.put(2, ValueObject<JString>(password));
                    peer->opCustom(OperationRequest(5, op), true);
                    break;
                }

            case State::DISCONNECTED:
                printf("Disconnected \n");
                break;

            default:
                break;
        }
        peer->service();
        SLEEP(500);
    }

}

void PhotonLib::onStatusChanged(int statusCode)
{
	switch(statusCode)
	{
	case StatusCode::CONNECT:
		mState = State::CONNECTED;
		break;
	case StatusCode::DISCONNECT:
		mState = State::DISCONNECTED;
		break;
	default:
		break;
	}
}

void PhotonLib::onOperationResponse(const OperationResponse& operationResponse)
{
	switch(operationResponse.getOperationCode())
	{
	case 5:
        printf("Login success \n");
		break;
	default:
		break;
	}
}

void PhotonLib::onEvent(const EventData& eventData){

}

void PhotonLib::debugReturn(ExitGames::Common::DebugLevel::DebugLevel debugLevel, const ExitGames::Common::JString& string){

}
