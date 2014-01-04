#include "PhotonLib.h"
#include <iostream>

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

void PhotonLib::startwork(const string ipAddr){
    while(true){
        printf(".\n");
        switch(mState){
            case State::INITIALIZED:
                peer->connect(JString(ipAddr.c_str()));
                mState = State::CONNECTING;
                printf("Connecting \n");
                break;

            case State::CONNECTED:
                {
                    printf("Connected \n");
		    OperationRequest opRequest = OperationRequest((nByte)2);
		    showMsg(opRequest.toString(true, true));
                    peer->opCustom(opRequest, true);
		    mState = State::SENDING_IPADRESS;
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
	if(mState == State::SENDED) break;
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

void PhotonLib::onOperationResponse(const OperationResponse& opResponse)
{
	JString msg("Test");
	switch(opResponse.getOperationCode())
	{
	case 1:
        	cout << opResponse.getParameters().toString() << endl;
		//msg = ((ExitGames::Common::ValueObject<JString>)opResponse.getParameterForCode(1)).getDataCopy();
		//msg = opResponse.getParameters().getValue(1);
		msg = opResponse.toString(true, true, true);
		showMsg(msg);
		mState = State::SENDED;
		break;
	default:
		showMsg(opResponse.toString(true, true, true));
		break;
	}
}

void PhotonLib::onEvent(const EventData& eventData){

}

void PhotonLib::debugReturn(ExitGames::Common::DebugLevel::DebugLevel debugLevel, const ExitGames::Common::JString& string){

}

void showMsg(JString s){
	int size = s.length();
	int i;
	for(i=0;i<size;i++)printf("%c",(char) s.charAt(i));
	printf("\n");
}
