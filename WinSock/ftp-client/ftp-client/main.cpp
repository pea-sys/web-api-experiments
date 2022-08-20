#include <winsock2.h>

#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;


////// Prototypes ////////////////////////////////////////////////////////

extern int DoWinsock(const char* pcHost, int nPort, const char* loginId, const char* loginPassword);


//// main //////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
    // Do we have enough command line arguments?
    if (argc != 5) {
        cerr << "usage: " << argv[0] << " <server-address> " <<
            "[server-port]" << " [login-Id] " << "[login-password]" << endl << endl;
        return 1;
    }

    // Get host and (optionally) port from the command line
    const char* pcHost = argv[1];
    const int nPort = atoi(argv[2]);
    const char* loginId = argv[3];
    const char* loginPassword = argv[4];

    // Start Winsock up
    WSAData wsaData;
    int nCode;
    if ((nCode = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0) {
        cerr << "WSAStartup() returned error code " << nCode << "." <<
            endl;
        return 255;
    }

    int retval = DoWinsock(pcHost, nPort, loginId, loginPassword);

    // Shut Winsock back down and take off.
    WSACleanup();
    return retval;
}

