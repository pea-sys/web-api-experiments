#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

int main()
{
	WSAData wsaData;

	int iRet = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iRet != EXIT_SUCCESS) {
		std::cout << "Error!(iRet = " << iRet << std::endl;
		WSACleanup();
		return EXIT_FAILURE;
	}
	SOCKET sock = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, 0);
	if (sock == INVALID_SOCKET) {
		std::cout << "WSASocket Error!" << WSAGetLastError() << std::endl;
		WSACleanup();
		return EXIT_FAILURE;
	}
	u_short ns_port;
	iRet = WSAHtons(sock, 4000, &ns_port);
	if (iRet != EXIT_SUCCESS) {
		std::cout << "WSAHtons Error=" << WSAGetLastError() << std::endl;
		WSACleanup();
		return EXIT_FAILURE;
	}
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = ns_port;
	addr.sin_addr.S_un.S_addr = INADDR_ANY;

	if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
		std::cout << "bind Error!" << std::endl;
		closesocket(sock);
		WSACleanup();
		return EXIT_FAILURE;
	}

	char buf[2048];

	while (TRUE)
	{
		memset(buf, 0, sizeof(buf));
		if (recv(sock, buf, sizeof(buf), 0) == SOCKET_ERROR)
		{
			std::cout << "recv Error!" << std::endl;
			closesocket(sock);
			WSACleanup();
			return EXIT_FAILURE;
		}
		else
		{
			std::cout << buf << std::endl;
		}
	}
	closesocket(sock);
	WSACleanup();
	return EXIT_SUCCESS;
}