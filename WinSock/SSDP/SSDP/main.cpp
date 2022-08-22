# define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#define SERVERPORT 1900
#include <iostream>


char buff[] = "M-SEARCH * HTTP/1.1\r\nHOST: 239.255.255.250:1900\r\nMAN: \"ssdp:discover\"\r\nMX: 3\r\nST: upnp:rootdevice\r\n\r\n";

int main()
{
    //char buff[400]; 
    char rcvdbuff[1000];
    int len, Ret = 2;

    WSADATA wsaData;
    struct sockaddr_in their_addr;
    struct hostent* he;
    SOCKET sock;
    int nCode;
    if ((nCode = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0) {
        std::cerr << "WSAStartup() returned error code " << nCode << "." << std::endl;
        return 255;
    }
    sock = socket(AF_INET, SOCK_DGRAM, 0);

    their_addr.sin_family = AF_INET;
    their_addr.sin_addr.s_addr = inet_addr("239.255.255.250");   
    their_addr.sin_port = htons(SERVERPORT);

    len = sizeof(struct sockaddr_in);

    while (1)
    {
        printf("buff:\n%s\n", buff);

        Ret = sendto(sock, buff, strlen(buff), 0, (struct sockaddr*)&their_addr, len);
        if (Ret < 0)
        {
            printf("error in SENDTO() function");
            closesocket(sock);
            return 0;
        }

        //Receiving Text from server
        printf("\n\nwaiting to recv:\n");
        memset(rcvdbuff, 0, sizeof(rcvdbuff));
        Ret = recvfrom(sock, rcvdbuff, sizeof(rcvdbuff), 0, (struct sockaddr*)&their_addr, &len);
        if (Ret < 0)
        {
            printf("Error in Receiving");
            return 0;
        }
        rcvdbuff[Ret - 1] = '\0';
        printf("RECEIVED MESSAGE FROM SERVER\t: %s\n", rcvdbuff);

        //Delay for testing purpose
        Sleep(3 * 1000);
    }
    closesocket(sock);
    WSACleanup();
}