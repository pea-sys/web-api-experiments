#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;
//ネットワークインターフェースを取得する
int main()
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != EXIT_SUCCESS) {
        cerr << "WSAStartup Error " << WSAGetLastError() << endl;
        return EXIT_FAILURE;
    }

    SOCKET sd = WSASocket(AF_INET, SOCK_DGRAM, 0, 0, 0, 0);
    if (sd == SOCKET_ERROR) {
        cerr << "WSASocket Error " << WSAGetLastError() << endl;
        return EXIT_FAILURE;
    }

    INTERFACE_INFO InterfaceList[20];
    unsigned long nBytesReturned;
    if (WSAIoctl(sd, SIO_GET_INTERFACE_LIST, 0, 0, &InterfaceList, sizeof(InterfaceList), &nBytesReturned, 0, 0) == SOCKET_ERROR) {
        cerr << "WSAIoctl Error " << WSAGetLastError() << endl;
        return EXIT_FAILURE;
    }

    int nNumInterfaces = nBytesReturned / sizeof(INTERFACE_INFO);
    cout << "There are " << nNumInterfaces << " interfaces:" << endl;
    for (int i = 0; i < nNumInterfaces; ++i) {
        sockaddr_in* pAddress;
        pAddress = (sockaddr_in*)&(InterfaceList[i].iiAddress);
        cout << " " << inet_ntoa(pAddress->sin_addr);

        pAddress = (sockaddr_in*)&(InterfaceList[i].iiBroadcastAddress);
        cout << " has bcast " << inet_ntoa(pAddress->sin_addr);

        pAddress = (sockaddr_in*)&(InterfaceList[i].iiNetmask);
        cout << " and netmask " << inet_ntoa(pAddress->sin_addr) << endl;

        cout << " Interface is ";
        u_long nFlags = InterfaceList[i].iiFlags;
        if (nFlags & IFF_UP){
            cout << "up";
        }
        else {
            cout << "down";
        }
        if (nFlags & IFF_POINTTOPOINT) cout << ", is point-to-point";
        if (nFlags & IFF_LOOPBACK)     cout << ", is loopback";
        cout << ", and can do: ";
        if (nFlags & IFF_BROADCAST) cout << "bcast ";
        if (nFlags & IFF_MULTICAST) cout << "multicast ";
        cout << endl;
    }

    WSACleanup();

    return EXIT_SUCCESS;
}
