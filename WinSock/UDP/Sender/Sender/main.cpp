#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <iostream>
#include <array>
#include <string>

#pragma comment(lib, "Ws2_32.lib")

int main()
{
	WSAData wsaData;

	SOCKET sock;
	

	int iRet = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iRet != EXIT_SUCCESS) {
		std::cout << "Error!(iRet = %d." << std::endl << iRet;
		WSACleanup();
		return EXIT_FAILURE;
	}

	sock = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, 0);
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
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	std::array<std::string, 2> str_arr = { "Hello", "World" };
	for (size_t i = 0; i < str_arr.size(); ++i) {
		sendto(sock,str_arr[i].c_str(), 5, 0, (struct sockaddr*)&addr, sizeof(addr));
	}

	closesocket(sock);
	WSACleanup();
	return EXIT_SUCCESS;
}