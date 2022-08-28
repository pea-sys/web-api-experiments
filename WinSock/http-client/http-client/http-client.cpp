#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "ws-util.h"
#include <iostream>
#include <string.h>

using namespace std;

// Comment this out to disable the shutdown-delay functionality.
#define SHUTDOWN_DELAY


////////////////////////////////////////////////////////////////////////
// Constants

// kBufferSize must be larger than the length of kpcEchoMessage.
const int kBufferSize = 1024;
const char* kpcMessage = "GET % s HTTP / 1.0\r\nHost: % s : % d\r\nUser - Agent: httpc / 0.0\r\n\r\n";
const int kpcMessageLen = strlen(kpcMessage);

#if defined(SHUTDOWN_DELAY)
// How long to wait after we do the echo before shutting the connection
// down, to give the user time to start other clients, for testing 
// multiple simultaneous connections.
const int kShutdownDelay = 3;
#endif
#define RECVSIZE	4096

////////////////////////////////////////////////////////////////////////
// Prototypes

u_long LookupAddress(const char* pcHost);
SOCKET EstablishConnection(u_long nRemoteAddr, u_short nPort);
bool Send(SOCKET sd);
int ReadReply(SOCKET sd);


//// DoWinsock /////////////////////////////////////////////////////////
// The module's driver function -- we just call other functions and
// interpret their results.

int DoWinsock(const char* pcHost, int nPort)
{
    // Find the server's address
    cout << "Looking up address..." << flush;
    u_long nRemoteAddress = LookupAddress(pcHost);
    if (nRemoteAddress == INADDR_NONE) {
        cerr << endl << WSAGetLastErrorMessage("lookup address") <<
            endl;
        return 3;
    }
    in_addr Address;
    memcpy(&Address, &nRemoteAddress, sizeof(u_long));
    cout << inet_ntoa(Address) << ":" << nPort << endl;

    // Connect to the server
    cout << "Connecting to remote host..." << flush;
    SOCKET sd = EstablishConnection(nRemoteAddress, htons(nPort));
    if (sd == INVALID_SOCKET) {
        cerr << endl << WSAGetLastErrorMessage("connect to server") <<
            endl;
        return 3;
    }
    cout << "connected, socket " << sd << "." << endl;


    // Send the echo packet to the server

    int nBytes;
    if (Send(sd)) {
        cout << endl;
        if ((nBytes = ReadReply(sd)) > 0) {
            if (nBytes == kBufferSize) {
                cerr << "FYI, likely data overflow." << endl;
            }
        }
        else if (nBytes == 0) {
            cerr << endl << "Server unexpectedly closed the connection" <<
                endl;
        }
        else {
            cerr << endl << WSAGetLastErrorMessage("read reply") <<
                endl;
            return 3;
        }
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


//// LookupAddress /////////////////////////////////////////////////////
// Given an address string, determine if it's a dotted-quad IP address
// or a domain address.  If the latter, ask DNS to resolve it.  In
// either case, return resolved IP address.  If we fail, we return
// INADDR_NONE.

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


//// EstablishConnection ///////////////////////////////////////////////
// Connects to a given address, on a given port, both of which must be
// in network byte order.  Returns newly-connected socket if we succeed,
// or INVALID_SOCKET if we fail.

SOCKET EstablishConnection(u_long nRemoteAddr, u_short nPort)
{
    // Create a stream socket
    SOCKET sd = socket(PF_INET, SOCK_STREAM, 0);
    if (sd != INVALID_SOCKET) {
        sockaddr_in sinRemote;
        sinRemote.sin_family = PF_INET;
        sinRemote.sin_addr.s_addr = nRemoteAddr;
        sinRemote.sin_port = nPort;
        if (connect(sd, (sockaddr*)&sinRemote, sizeof(sockaddr_in)) ==
            SOCKET_ERROR) {
            sd = INVALID_SOCKET;
        }
    }

    return sd;
}


//// SendEcho //////////////////////////////////////////////////////////
// Sends the echo packet to the server.  Returns true on success,
// false otherwise.

bool Send(SOCKET sd)
{
    // Send the string to the server
    if (send(sd, kpcMessage, kpcMessageLen, 0) != SOCKET_ERROR) {
        return true;
    }
    else {
        return false;
    }
}


//// ReadReply /////////////////////////////////////////////////////////
// Read the reply packet and check it for sanity.  Returns -1 on 
// error, 0 on connection closed, > 0 on success.

int ReadReply(SOCKET sd)
{
    //// Read reply from server
    char acReadBuffer[kBufferSize];
    int  buf_len;
    char recvbuf[RECVSIZE];
    while (1) {
        /* サーバからデータを受信します */
        buf_len = recv(sd, recvbuf, RECVSIZE - 1, 0);
        if (buf_len == SOCKET_ERROR) {	/* 受信失敗の場合 */
            fprintf(stderr, "Error: サーバからの受信失敗\n");
            break;
        }
        else if (buf_len == 0) {			/* 受信終わり */
            break;
        }
        recvbuf[buf_len] = '\0';	/* 受信バッファの後ろにNULLを付加する */
        printf("%s", recvbuf);		/* 標準出力する */
    }
    return strlen(recvbuf);
}
