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

void PhotonLib::startwork(const JString& ipAddr, char* airstrikeIP){
    while(true){
        printf(".\n");
        switch(mState){
            case State::INITIALIZED:
                peer->connect(ipAddr);
                mState = State::CONNECTING;
                printf("Connecting \n");
                break;

            case State::CONNECTED:
                {
                    printf("Connected \n");
                    OperationRequestParameters op;
                    op.put(1, ValueObject<JString>(airstrikeIP));
                    peer->opCustom(OperationRequest(5, op), true);
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

void PhotonLib::onOperationResponse(const OperationResponse& operationResponse)
{
	switch(operationResponse.getOperationCode())
	{
	case 5:
        	printf("ip adress send success \n");
		mState = State::SENDED;
		break;
	default:
		break;
	}
}

void PhotonLib::onEvent(const EventData& eventData){

}

void PhotonLib::debugReturn(ExitGames::Common::DebugLevel::DebugLevel debugLevel, const ExitGames::Common::JString& string){

}

void NotifyPhotonServer(char * photonIP)
{	
	char airstrikeIP[20];
    	strcpy(airstrikeIP, GetLocalIp());
    	strcat(airstrikeIP, ":1234");	
	printf("AirStrike IP adress = %s\n",airstrikeIP);

        JString ipAddrPhoton(photonIP);
        PhotonLib * photonLib = new PhotonLib();
        photonLib->startwork(ipAddrPhoton, airstrikeIP);
        delete photonLib;
}

char* GetLocalIp()    
{          
    int MAXINTERFACES=16;    
    char *ip;    
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
