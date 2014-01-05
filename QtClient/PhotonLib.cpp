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

void PhotonLib::startwork(const char * ipAddr){
    while(true){
        printf(".\n");
        switch(mState){
            case State::INITIALIZED:
                peer->connect(JString(ipAddr));
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
		mState = State::SENDED;
		emit addNewRadioBtn("192.168.2.101",0);
		emit addNewRadioBtn("192.168.2.102",1);
		emit addNewRadioBtn("192.168.2.103",2);
                break;

            default:
                break;
        }
        peer->service();
        SLEEP(500);
	if(mState == State::SENDED) break;
    }
    peer->disconnect();
}

void PhotonLib::onStatusChanged(int statusCode)
{
	switch(statusCode)
	{
	case StatusCode::CONNECT:
		mState = State::CONNECTED;
		break;
	case StatusCode::DISCONNECT:
		printf("onStatusChanged Disconnected \n");
		mState = State::DISCONNECTED;
		break;
	default:
		break;
	}
}

void PhotonLib::onOperationResponse(const OperationResponse& opResponse)
{
	Dictionary<JString,JString> servers;
	switch(opResponse.getOperationCode())
	{
	case 2:
		if (opResponse.getReturnCode() == -1) showMsg(opResponse.toString(true, true, true));
		else{
			servers = ((ExitGames::Common::ValueObject<Dictionary<JString,JString> >)opResponse.getParameterForCode(2)).getDataCopy();
			emitIpAdress(servers.toString());
			mState = State::SENDED;
		}
		
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
void PhotonLib::emitIpAdress(JString s){
	int size = s.length(), i;
	char * string = (char *)malloc(sizeof(char)*(size+1));
	for(i=0; i<size; i++) string[i] = (char) s.charAt(i);
	string[size] = '\0';
	int numPrime = 0, flag = 0, ID;
	char *p;
	for(i=0; i<size; i++){
		if(numPrime==3 && flag==0) {
			p = & string[i];
			flag = 1;
		}
		if(string[i]=='\"') numPrime ++;
		if(string[i]==':') string[i] = '\0';
		if(numPrime==4 && flag==1) {
			string[i] = '\0';
			flag = numPrime = 0;
			printf("---%s\n", p);
			emit addNewRadioBtn(p,ID++);
		}
	}
}