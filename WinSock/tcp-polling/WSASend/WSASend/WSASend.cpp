#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib") 
/// <summary>
/// TCPクライアント
/// </summary>
int main() {
	
	WSABUF wsaBuf;						// WSABUF型バッファ
	char szInputBuf[256];				// 入力されたメッセージ文字列を格納するバッファ
	DWORD dwSendLen = 0;				// 送信データの長さ
	DWORD dwFlags = 0;					// 送信時のフラグ

	// ホスト名の入力
	char hostname[256];
	std::cout << "hostname(127.0.0.1): ";
	std::cin >> hostname;
	// ポート番号の入力
	int port;
	std::cout << "port(4000): ";
	std::cin >> port;

	// ソケット初期化
	WSADATA wsaData;
	int iRet = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iRet != EXIT_SUCCESS) {
		std::cout << "WSAStartUp Error!" << iRet;
		WSACleanup();
		return EXIT_FAILURE;
	}
	// ホスト情報の取得
	struct hostent* host = gethostbyname(hostname);
	if (host == NULL) {
		std::cout << "gethostbyname Error!";
		WSACleanup();
		return EXIT_FAILURE;
	}
	// ソケットの作成
	SOCKET soc = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, 0);
	if (soc == INVALID_SOCKET) {
		std::cout << "WSASocket Error!" << WSAGetLastError() << std::endl;
		WSACleanup();
		return EXIT_FAILURE;
	}
	//ポート番号をネットワークバイトオ―ダに変換
	u_short ns_port;
	iRet = WSAHtons(soc, port, &ns_port);
	if (iRet != EXIT_SUCCESS) {
		std::cout << "WSAHtons Error=" << WSAGetLastError() << std::endl;
		WSACleanup();
		return EXIT_FAILURE;
	}
	// 接続先サーバ情報
	struct sockaddr_in saiTargetAddr;
	saiTargetAddr.sin_family = AF_INET;// IPv4
	saiTargetAddr.sin_port = ns_port;														
	saiTargetAddr.sin_addr.S_un.S_addr = *(u_long*)host->h_addr_list[0];					

	// 接続
	if (WSAConnect(soc, (struct sockaddr*)&saiTargetAddr, sizeof(saiTargetAddr), NULL, NULL, NULL, NULL) == SOCKET_ERROR) {
		std::cout << "WSAConnect Error!" << WSAGetLastError() << std::endl;
		closesocket(soc);
		WSACleanup();
		return EXIT_FAILURE;
	}

	// メッセージループ
	while (TRUE) {
		// バッファをクリア
		memset(szInputBuf, 0, sizeof(char) * 256);
		// 送信データ入力
		std::cout << ">";
		// 入力文字列をバッファに格納
		std::cin >> szInputBuf;
		wsaBuf.buf = szInputBuf;
		wsaBuf.len = strlen(szInputBuf);
		if (WSASend(soc, &wsaBuf, 1, &dwSendLen, dwFlags, NULL, NULL) != EXIT_SUCCESS) {
			std::cout << "WSASend Error !" << WSAGetLastError() << std::endl;
			break;
		}
	}

	closesocket(soc);
	WSACleanup();
	return EXIT_SUCCESS;
}