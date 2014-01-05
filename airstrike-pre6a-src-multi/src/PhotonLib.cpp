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

void PhotonLib::startwork(const JString& ipAddr, char* airstrikeIP){
    while(true){
        //printf(".\n");
        switch(mState){
            case State::INITIALIZED:
                peer->connect(ipAddr);
                mState = State::CONNECTING;
                printf("Connecting \n");
                break;

            case State::CONNECTED:
                {
		            mState = State::SENDING_IPADRESS;
                    printf("Connected \n");
                    OperationRequestParameters op;
                    op.put((nByte)1, ValueObject<JString>("amphie 10"));
		            op.put((nByte)2, ValueObject<JString>(airstrikeIP));

		            OperationRequest opRequest = OperationRequest(1, op);
		            showMsg(opRequest.toString(true, true));
                    peer->opCustom(opRequest, true);
                    break;
                }

            case State::DISCONNECTED:
                printf("Disconnected \n");
                break;

            default:
                break;
        }
        SLEEP(100);
        peer->service();
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
		printf("unkown status");
		break;
	}
}

void PhotonLib::onOperationResponse(const OperationResponse& opResponse)
{
	switch((int)opResponse.getOperationCode())
	{
	case 1:		
        if (opResponse.getReturnCode() == -1) showMsg(opResponse.toString(true, true, true));
        else mState = State::SENDED;
		break;
	default:
		showMsg(opResponse.toString(true, true, true));
		break;
	}
}

void PhotonLib::onEvent(const EventData& eventData){
	printf("onEvent \n");
}

void PhotonLib::debugReturn(ExitGames::Common::DebugLevel::DebugLevel debugLevel, const ExitGames::Common::JString& string){
	printf("onDebug \n");
}

void NotifyPhotonServer(char * photonIP)
{	
	    char airStrikrIPport[25];
    	strcpy(airStrikrIPport, GetLocalIp());
    	strcat(airStrikrIPport, ":1234");

	    printf("AirStrike IP adress = %s\n",airStrikrIPport);
	    printf("Photon IP adress = %s\n",photonIP);

        JString ipAddrPhoton(photonIP);
        PhotonLib * photonLib = new PhotonLib();
        photonLib->startwork(ipAddrPhoton, airStrikrIPport);
        delete photonLib;
}

char* GetLocalIp()    
{          
    int MAXINTERFACES=16;    
    char *ip = NULL;    
    int fd, intrface, retn = 0;      
    struct ifreq buf[MAXINTERFACES];      
    struct ifconf ifc;      
  
    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) >= 0){      
        ifc.ifc_len = sizeof(buf);      
        ifc.ifc_buf = (caddr_t)buf;      
        if (!ioctl(fd, SIOCGIFCONF, (char *)&ifc)){      
            intrface = ifc.ifc_len / sizeof(struct ifreq);       
            while (intrface-- > 0){      
                if (!(ioctl (fd, SIOCGIFADDR, (char *) &buf[intrface]))){      
                    ip=(inet_ntoa(((struct sockaddr_in*)(&buf[intrface].ifr_addr))->sin_addr));      
                    break;    
                }                          
            }    
        }      
        close (fd);      
        return ip;      
    }    
}

void showMsg(JString s){
	int size = s.length();
	int i;
	for(i=0;i<size;i++)printf("%c",(char) s.charAt(i));
	printf("\n");
}
void PhotonLib::debug(){
	printf("\npeer id = %d\n",peer->getPeerId());
	printf("Peer Count = %d\n",peer->getPeerCount());
	printf("Queued Incoming Commands = %d\n",peer->getQueuedIncomingCommands());
	printf("Round Trip Time = %d\n\n",peer->getRoundTripTime());
}
