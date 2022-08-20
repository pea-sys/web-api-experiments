#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "ws-util.h"
#include <iostream>
#include <string.h>
#include <vector>

using namespace std;

// Comment this out to disable the shutdown-delay functionality.
#define SHUTDOWN_DELAY


////////////////////////////////////////////////////////////////////////
// Constants

// kBufferSize must be larger than the length of kpcEchoMessage.
const int kBufferSize = 1024;
const int ftpResponseCodeDigit = 3;
#if defined(SHUTDOWN_DELAY)
// How long to wait after we do the echo before shutting the connection
// down, to give the user time to start other clients, for testing 
// multiple simultaneous connections.
const int kShutdownDelay = 3;
#endif


////////////////////////////////////////////////////////////////////////
// Prototypes

u_long LookupAddress(const char* pcHost);
SOCKET EstablishConnection(u_long nRemoteAddr, u_short nPort);
bool SendEcho(SOCKET sd,  char* kpcEchoMessage, int kEchoMessageLen);
string ReadReply(SOCKET sd, char* kpcEchoMessage);
vector<string> StringSplit(string str, string separator);

//// DoWinsock /////////////////////////////////////////////////////////
// The module's driver function -- we just call other functions and
// interpret their results.

int DoWinsock(const char* pcHost, int nPort, const char* loginId, const char* loginPassword)
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
    char kpcEchoMessage[256];
    string response;
    SOCKET controlConnection = EstablishConnection(nRemoteAddress, htons(nPort));
    if (controlConnection == INVALID_SOCKET) {
        cerr << endl << WSAGetLastErrorMessage("connect to server") <<
            endl;
        return 3;
    }
    else
    {
        response = ReadReply(controlConnection, kpcEchoMessage).substr(0, ftpResponseCodeDigit);
        if (response == "220") {
            cout << "Service ready for new user." << endl;
        }
    }

    sprintf_s(kpcEchoMessage, sizeof(kpcEchoMessage), "USER %s\r\n", loginId);
    if (SendEcho(controlConnection, kpcEchoMessage, strlen(kpcEchoMessage))) {
        cout << endl;
        response = ReadReply(controlConnection, kpcEchoMessage).substr(0, ftpResponseCodeDigit);
        if (response == "331") {
            cout << "User name okay, need password." << endl;
        }
    }
    else {
        cerr << endl << WSAGetLastErrorMessage("send echo packet") <<
            endl;
        return 3;
    }

    sprintf_s(kpcEchoMessage, sizeof(kpcEchoMessage), "PASS %s\r\n", loginPassword);
    if (SendEcho(controlConnection, kpcEchoMessage, strlen(kpcEchoMessage))) {
        cout << endl;
        response = ReadReply(controlConnection, kpcEchoMessage).substr(0, ftpResponseCodeDigit);
        if (response == "230") {
            cout << "User logged in, proceed." << endl;
        }
    }
    else {
        cerr << endl << WSAGetLastErrorMessage("send echo packet") <<
            endl;
        return 3;
    }

    sprintf_s(kpcEchoMessage, sizeof(kpcEchoMessage), "PASV\r\n");
    vector<string> ary;
    if (SendEcho(controlConnection, kpcEchoMessage, strlen(kpcEchoMessage))) {
        response = ReadReply(controlConnection, kpcEchoMessage);
        response = response.substr(response.rfind("(") + 1, response.rfind(")") - response.rfind("(") - 1);
        cout << response << endl;
        ary = StringSplit(response, ",");
    }

    nRemoteAddress = LookupAddress((ary[0] + "." + ary[1] + "." + ary[2] + "." + ary[3]).c_str());
    if (nRemoteAddress == INADDR_NONE) {
        cerr << endl << WSAGetLastErrorMessage("lookup address") <<
            endl;
        return 3;
    }
    memcpy(&Address, &nRemoteAddress, sizeof(u_long));
    cout << inet_ntoa(Address) << ":" << atoi(ary[4].c_str()) * 256 + atoi(ary[5].c_str()) << endl;
    SOCKET dataConnection = EstablishConnection(nRemoteAddress, htons(atoi(ary[4].c_str()) * 256 + atoi(ary[5].c_str())));
    if (dataConnection == INVALID_SOCKET) {
        cerr << endl << WSAGetLastErrorMessage("connect to server") <<
            endl;
        return 3;
    }

    sprintf_s(kpcEchoMessage, sizeof(kpcEchoMessage), "LIST\r\n");
    if (SendEcho(controlConnection, kpcEchoMessage, strlen(kpcEchoMessage))) {
        cout << endl;
        response = ReadReply(controlConnection, kpcEchoMessage);
        if (response == "226") {
            cout << "	Closing data connection." << endl;
        }
    }
    else {
        cerr << endl << WSAGetLastErrorMessage("send echo packet") <<
            endl;
        return 3;
    }

    sprintf_s(kpcEchoMessage, sizeof(kpcEchoMessage), "QUIT\r\n");
    if (SendEcho(controlConnection, kpcEchoMessage, strlen(kpcEchoMessage))) {
        cout << endl;
        response = ReadReply(controlConnection, kpcEchoMessage);
        if (response == "226") {
            cout << "	Closing data connection." << endl;
        }
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
    if (ShutdownConnection(controlConnection)) {
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
    SOCKET sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd != INVALID_SOCKET) {
        sockaddr_in sinRemote;
        sinRemote.sin_family = AF_INET;
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

bool SendEcho(SOCKET sd, char* kpcEchoMessage, int kEchoMessageLen)
{
    // Send the string to the server
    if (send(sd, kpcEchoMessage, kEchoMessageLen, 0) != SOCKET_ERROR) {
        return true;
    }
    else {
        return false;
    }
}


//// ReadReply /////////////////////////////////////////////////////////
// Read the reply packet and check it for sanity.  Returns -1 on 
// error, 0 on connection closed, > 0 on success.

string ReadReply(SOCKET sd, char* kpcEchoMessage)
{
	// Read reply from server
	char acReadBuffer[kBufferSize];
	if (recv(sd, acReadBuffer, kBufferSize, 0) == SOCKET_ERROR) {
		return "NG";
	}
	return string{ acReadBuffer };
}

vector<string> StringSplit(string str, string separator) {
    if (separator == "") return { str };
    vector<string> result;
    string tstr = str + separator;
    long l = tstr.length(), sl = separator.length();
    string::size_type pos = 0, prev = 0;

    for (; pos < l && (pos = tstr.find(separator, pos)) != string::npos; prev = (pos += sl)) {
        result.emplace_back(tstr, prev, pos - prev);
    }
    return result;
}