#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "ws-util.h"
#include <iostream>
#include <string.h>

using namespace std;

// Comment this out to disable the shutdown-delay functionality.
#define SHUTDOWN_DELAY


struct NTP_Packet {
    int Control_Word;
    int root_delay;
    int root_dispersion;
    int reference_identifier;
    __int64 reference_timestamp;
    __int64 originate_timestamp;
    __int64 receive_timestamp;
    int transmit_timestamp_seconds;
    int transmit_timestamp_fractions;

    void Initialize() {
       Control_Word = htonl(0x0B000000);
       root_delay = 0;
       root_dispersion = 0;
       reference_identifier = 0;
       reference_timestamp = 0;
       originate_timestamp = 0;
       receive_timestamp = 0;
       transmit_timestamp_seconds = 0;
       transmit_timestamp_fractions = 0;
    }
};
////////////////////////////////////////////////////////////////////////
// Constants

// kBufferSize must be larger than the length of kpcEchoMessage.
const int kBufferSize = 1024;
const char* kpcEchoMessage = "This is a test of the emergency data "
"transfer system.  If this had been real a real emergency, we "
"would have sent this data out-of-band.";
const int kEchoMessageLen = strlen(kpcEchoMessage);

#if defined(SHUTDOWN_DELAY)
// How long to wait after we do the echo before shutting the connection
// down, to give the user time to start other clients, for testing 
// multiple simultaneous connections.
const int kShutdownDelay = 3;
#endif


////////////////////////////////////////////////////////////////////////
// Prototypes

u_long LookupAddress(const char* pcHost);
SOCKET EstablishConnection();
bool SendNtp(SOCKET sd, sockaddr_in address);
int RcvTime(SOCKET sd, sockaddr_in address);
sockaddr_in GetServerInfo(const char* pcHost);

//// DoWinsock /////////////////////////////////////////////////////////
// The module's driver function -- we just call other functions and
// interpret their results.

int DoWinsock(const char* pcHost, int nPort)
{
    SOCKET sd = EstablishConnection();
    if (sd == INVALID_SOCKET) {
        cerr << endl << WSAGetLastErrorMessage("connect to server") <<
            endl;
        return 3;
    }
    cout << "connected, socket " << sd << "." << endl;

    sockaddr_in serverAddr = GetServerInfo(pcHost);

    // Send the echo packet to the server
    cout << "Sending ntp packet " <<  flush;
    int ntpTime;
    if (SendNtp(sd, serverAddr)) {
        cout << endl;
        ntpTime = RcvTime(sd, serverAddr);
        cout << "RcvTime:" << ntohl(ntpTime);
        
    }
    else {
        cerr << endl << WSAGetLastErrorMessage("send echo packet") <<
            endl;
        return 3;
    }

#if defined(SHUTDOWN_DELAY)
    // Delay for a bit, so we can start other clients.  This is strictly
    // for testing purposes, so you can convince yourself that the 
    // server is handling more than one connection at a time.
    cout << "Will shut down in " << kShutdownDelay <<
        " seconds... (one dot per second): " << flush;
    for (int i = 0; i < kShutdownDelay; ++i) {
        Sleep(1000);
        cout << '.' << flush;
    }
    cout << endl;
#endif

    // Shut connection down
    cout << "Shutting connection down..." << flush;
    if (ShutdownConnection(sd)) {
        cout << "Connection is down." << endl;
    }
    else {
        cout << endl << WSAGetLastErrorMessage("Shutdown connection") <<
            endl;
    }

    cout << "All done!" << endl;

    return 0;
}


u_long LookupAddress(const char* pcHost)
{
    u_long nRemoteAddr = inet_addr(pcHost);
    if (nRemoteAddr == INADDR_NONE) {
        // pcHost isn't a dotted IP, so resolve it through DNS
        hostent* pHE = gethostbyname(pcHost);
        if (pHE == 0) {
            return INADDR_NONE;
        }
        nRemoteAddr = *((u_long*)pHE->h_addr_list[0]);
    }

    return nRemoteAddr;
}

SOCKET EstablishConnection()
{
    // Create a stream socket
    SOCKET sd = socket(PF_INET, SOCK_DGRAM, 0);
    if (sd != INVALID_SOCKET) {
        sockaddr_in sinRemote;
        sinRemote.sin_family = AF_INET;
        sinRemote.sin_addr.s_addr = INADDR_ANY;
        sinRemote.sin_port = htons((unsigned short)1024);
        memset(sinRemote.sin_zero, (int)0, sizeof(sinRemote.sin_zero));
        if (bind(sd, (sockaddr*)&sinRemote, sizeof(sinRemote)) ==
            SOCKET_ERROR) {
            sd = INVALID_SOCKET;
        }
    }
    return sd;
}


sockaddr_in GetServerInfo(const char* pcHost)
{
    struct sockaddr_in serverAddr;
    struct hostent* serverhostent = gethostbyname((char*)pcHost);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = *((unsigned long*)((serverhostent->h_addr_list)[0]));
    serverAddr.sin_port = htons(123);
    memset(serverAddr.sin_zero, (int)0, sizeof(serverAddr.sin_zero));
    return serverAddr;
}

//// SendEcho //////////////////////////////////////////////////////////
// Sends the echo packet to the server.  Returns true on success,
// false otherwise.

bool SendNtp(SOCKET sd, sockaddr_in serverAddr)
{
    struct NTP_Packet	NTP_Send;				/* 送信するNTPパケット */
    NTP_Send.Initialize();

    // Send the string to the server
    if (sendto(sd, (const char*)&NTP_Send, sizeof(NTP_Send), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) != SOCKET_ERROR) {
        return true;
    }
    else {
        return false;
    }
}

int RcvTime(SOCKET sd, sockaddr_in address)
{
    struct NTP_Packet NTP_Recv;				/* 受信するNTPパケット */
    NTP_Recv.Initialize();
    fd_set rdps;
    struct timeval waittime;
    waittime.tv_sec = 10;		/* タイムアウト秒数を設定する */
    waittime.tv_usec = 0;

    int nTotalBytes = 0;

    FD_ZERO(&rdps);
    FD_SET(sd, &rdps);

    int selret = select(FD_SETSIZE, &rdps, (fd_set*)0, (fd_set*)0, &waittime);
    if (selret == SOCKET_ERROR) {
    }
    else if(selret == 0) {				/* タイムアウトの場合 */
    }
    else if (FD_ISSET(sd, &rdps) == FALSE) {	/* 受信はしたが、指定のソケットではない場合 */
    }
    int sockaddr_Size = sizeof(address);
    if (recvfrom(sd, (char*)&NTP_Recv, sizeof(NTP_Recv), 0, (struct sockaddr*)&address, &sockaddr_Size) == SOCKET_ERROR) {
        fprintf(stderr, "Error: サーバからの受信失敗\n");
        /* ソケットを破棄する */
        closesocket(sd);
        return -1;
    }
    return NTP_Recv.transmit_timestamp_seconds;
}
