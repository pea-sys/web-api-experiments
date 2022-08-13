#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

//IPƒAƒhƒŒƒXŽæ“¾
int main(int argc, char* argv[])
{
    WSAData wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != EXIT_SUCCESS) {
        cerr << "WSAStartup Error " << WSAGetLastError() << endl;
        return EXIT_FAILURE;
    }

    char ac[80];
    if (gethostname(ac, sizeof(ac)) == SOCKET_ERROR) {
        cerr << "gethostname Error " << WSAGetLastError() << endl;
        return EXIT_FAILURE;
    }
    cout << "Host name is " << ac << "." << endl;

    struct hostent* phe = gethostbyname(ac);
    if (phe == nullptr) {
        cerr << "gethostbyname Error" << WSAGetLastError() << endl;
        return EXIT_FAILURE;
    }

    for (int i = 0; phe->h_addr_list[i] != 0; ++i) {
        struct in_addr addr;
        memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
        cout << "Address " << i << ": " << inet_ntoa(addr) << endl;
    }

    WSACleanup();

    return EXIT_SUCCESS;
}