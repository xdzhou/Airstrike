#include "../include/client.h"

client::client(void)
{
    peer = new ExitGames::Photon::PhotonPeer(*this);
    mState = State::INITIALIZED;
}

client::~client(void)
{
    delete peer;
}

void client::startwork(void){

    while(true){
        printf(".\n");
        switch(mState){
            case State::INITIALIZED:
			peer->connect("localhost");
			mState = State::CONNECTING;
			break;
		case State::CONNECTED:
            ExitGames::Common::JString *login = new ExitGames::Common::JString("test");
            ExitGames::Common::JString  *password = new ExitGames::Common::JString("1234");
            ExitGames::Common::Dictionary<nByte, ExitGames::Common::Object> *directory = new ExitGames::Common::Dictionary<nByte, ExitGames::Common::Object>();
            directory->put((unsigned char)1);

            ExitGames::Photon::OperationRequestParameters paramettre = new ExitGames::Photon::OperationRequestParameters {{1, login}, {2, password}};;
			ExitGames::Photon::OperationRequest request = new ExitGames::Photon::OperationRequest(5, paramettre);
			peer->opCustom(request);
			break;
		case State::DISCONNECTED:
			mState = State::INITIALIZED;
			break;
		default:
			break;
        }
        peer->service();
        SLEEP(500);
    }

}

void client::onStatusChanged(int statusCode)
{
	switch(statusCode)
	{
	case ExitGames::Photon::StatusCode::CONNECT:
		mState = State::CONNECTED;
		break;
	case ExitGames::Photon::StatusCode::DISCONNECT:
		mState = State::DISCONNECTED;
		break;
	default:
		break;
	}
}

void client::onOperationResponse(const ExitGames::Photon::OperationResponse& operationResponse)
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
