#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib") 

/// <summary>
/// TCPポーリング
/// </summary>
int main() {

	static const int PORT = 4000;
	static const int POLLING_INTERVAL = 5000;
	static const int LIMITED_CLIENT_CONNECTION = 5;


	WSADATA wsaData;					// WinSockの初期化に必要なWSADATA構造体変数
	u_short ns_port;					// ポート番号のネットワークバイトオ―ダ値
	struct sockaddr_in saiServerAddr;	// アドレス情報を持つsockaddr_in構造体
	int optval = 1;						// オプション値optvalを1に初期化
	WSAPOLLFD fdArray[6];			    // WSAPoll関数で使用されるソケット情報が格納
	int count = 0;						// 接続中クライアントの数
	int poll_result;					// pollの戻り値
	struct sockaddr_in saiClientAddr;	// アクセプトしたクライアントのアドレス情報
	int iClientAddrLen;					// saiClientAddrのサイズ
	char* client_ip_addr_str;			// クライアントのIPアドレス文字列
	u_short client_port;				// アクセプトしたクライアントのポート番号
	int pos;							// 空き要素インデックス一時保存用
	WSABUF wsaBuf;						// WSABUF型変数(reventsメンバーをクリア不要)
	BYTE btBuf[256];					
	DWORD dwRecvLen = 0;				
	DWORD dwFlags = 0;					// WSARecvに指定するフラグ
	DWORD dwSendLen = 0;				


	// ソケットの初期化
	int iRet = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iRet!= EXIT_SUCCESS) {
		std::cout << "Error!(iRet = %d." << std::endl << iRet;
		WSACleanup();
		return EXIT_FAILURE;
	}
	// ソケットの作成
	SOCKET soc = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, 0);
	if (soc == INVALID_SOCKET) {
		std::cout << "WSASocket Error!" << std::endl;
		WSACleanup();
		return EXIT_FAILURE;
	}
	// ポート番号の変換
	WSAHtons(soc, PORT, &ns_port);
	std::cout <<  "port = " << PORT <<  " ns_port = " <<  ns_port << std::endl;
	// バインドするアドレス情報addrの設定.
	saiServerAddr.sin_family = AF_INET;					// IPv4ファミリーAF_INET
	saiServerAddr.sin_port = ns_port;					
	saiServerAddr.sin_addr.S_un.S_addr = INADDR_ANY;	// すべてのローカルインターフェイスにバインド

	// SO_REUSEADDRの有効化
	if (setsockopt(soc, SOL_SOCKET, SO_REUSEADDR, (const char*)&optval, sizeof(optval)) != EXIT_SUCCESS) {
		std::cout << "setsockopt(SO_REUSEADDR) Error!" << std::endl;
		closesocket(soc);
		WSACleanup();
		return EXIT_FAILURE;
	}
	// socにsaiServerAddrをバインド
	if (bind(soc, (struct sockaddr*)&saiServerAddr, sizeof(saiServerAddr)) != EXIT_SUCCESS) {
		std::cout << "bind Error!" << std::endl;
		closesocket(soc);
		WSACleanup();
		return EXIT_FAILURE;
	}
	// 接続要求を格納するキューの作成
	if (listen(soc, LIMITED_CLIENT_CONNECTION) != EXIT_SUCCESS) {
		std::cout << "listen Error!" << std::endl;
		closesocket(soc);
		WSACleanup();
		return EXIT_FAILURE;
	}

	// pollfd構造体配列の初期化
	fdArray[0].fd = soc;	// 0番目はサーバソケット
	fdArray[0].events = POLLIN;
	memset(&fdArray[1], 0, sizeof(WSAPOLLFD) * LIMITED_CLIENT_CONNECTION);
	for (int i = 1; i <= LIMITED_CLIENT_CONNECTION; i++) {
		fdArray[i].fd = -1;
	}
	count = 0;

	while (TRUE) {
		// ソケット監視
		poll_result = WSAPoll(fdArray, count + 1, POLLING_INTERVAL);
		if (poll_result == SOCKET_ERROR) {
			std::cout << "WSAPoll Error! " << WSAGetLastError() << std::endl;
		}
		// 監視対象なし
		else if (poll_result == 0) {
			continue;
		}
		// 監視対象あり                  
		// サーバソケットsoc.
		if (fdArray[0].revents & POLLIN) {

			// アクセプトの前準備
			iClientAddrLen = sizeof(saiClientAddr);
			// アクセプト待ち
			SOCKET acc = WSAAccept(soc, (struct sockaddr*)&saiClientAddr, &iClientAddrLen, NULL, NULL);
			if (acc == INVALID_SOCKET) {
				std::cout << "WSAAccept Error !" << WSAGetLastError() << std::endl;
			}

			client_ip_addr_str = inet_ntoa(saiClientAddr.sin_addr);								// inet_ntoaでクライアントのsaiClientAddr.sin_addrをIPアドレス文字列に変換
			WSANtohs(acc, saiClientAddr.sin_port, &client_port);								// WSANtohsでクライアントのポート番号をホストバイトオーダに変換
			std::cout << "accept!(IPAddress = " << client_ip_addr_str <<  "Port = " << client_port << std::endl;;

			// fdsへの格納
			pos = -1;
			for (int i = 1; i < count + 1; i++) {
				if (fdArray[i].fd == -1) {
					pos = i;
					break;
				}
			}
			if (pos == -1) {
				if (count >= LIMITED_CLIENT_CONNECTION) {
					std::cout << "error!(fds full!" << std::endl;
					closesocket(acc);
				}
				else {
					pos = count + 1;
					count++;
					fdArray[pos].fd = acc;
					fdArray[pos].events = POLLIN;
				}
			}
		}

		for (int i = 1; i < count + 1; i++) {
			if (fdArray[i].fd != -1) {
				if (fdArray[i].revents & POLLIN) {
					memset(btBuf, 0, sizeof(BYTE) * 256);
					wsaBuf.buf = (char*)btBuf;			
					wsaBuf.len = sizeof(BYTE) * 256;
					// クライアントからメッセージ受信
					if (WSARecv(fdArray[i].fd, &wsaBuf, 1, &dwRecvLen, &dwFlags, NULL, NULL) != EXIT_SUCCESS) {
						std::cout << "WSARecv Error !" << WSAGetLastError() << std::endl;
					}
					std::cout << wsaBuf.buf << std::endl;

					//wsaBuf.bufの内容を送り返す.
					if (WSASend(fdArray[i].fd, &wsaBuf, 1, &dwSendLen, dwFlags, NULL, NULL) != EXIT_SUCCESS) {
						std::cout << "WSASend Error !" << WSAGetLastError() << std::endl;
					}
				}
			}
		}
	}

	// wsaPollFdsを全て閉じる
	for (int i = LIMITED_CLIENT_CONNECTION; i >= 0; i--) {
		closesocket(fdArray[i].fd);
	}
	WSACleanup();
	return EXIT_SUCCESS;

}