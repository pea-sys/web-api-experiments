#include <winsock2.h>

#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;


////// Prototypes ////////////////////////////////////////////////////////

extern int DoWinsock(const char* pcHost, int nPort);


//// main //////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
    // Do we have enough command line arguments?
    if (argc < 2) {
        cerr << "usage: " << argv[0] << " <server-address> " <<
            "[server-port]" << endl << endl;
        return 1;
    }

    // Get host and (optionally) port from the command line
    const char* pcHost = argv[1];


    // Do a little sanity checking.
    int nNumArgsIgnored = (argc - 3);
    if (nNumArgsIgnored > 0) {
        cerr << nNumArgsIgnored << " extra argument" <<
            (nNumArgsIgnored == 1 ? "" : "s") <<
            " ignored.  FYI." << endl;
    }

    // Start Winsock up
    WSAData wsaData;
    int nCode;
    if ((nCode = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0) {
        cerr << "WSAStartup() returned error code " << nCode << "." <<
            endl;
        return 255;
    }

    // Call the main example routine.
    int retval = DoWinsock(pcHost, 80);

    // Shut Winsock back down and take off.
    WSACleanup();
    return retval;
}

