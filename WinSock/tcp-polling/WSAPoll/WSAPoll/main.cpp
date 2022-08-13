#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib") 

/// <summary>
/// TCP�|�[�����O
/// </summary>
int main() {

	static const int PORT = 4000;
	static const int POLLING_INTERVAL = 5000;
	static const int LIMITED_CLIENT_CONNECTION = 5;


	WSADATA wsaData;					// WinSock�̏������ɕK�v��WSADATA�\���̕ϐ�
	u_short ns_port;					// �|�[�g�ԍ��̃l�b�g���[�N�o�C�g�I�\�_�l
	struct sockaddr_in saiServerAddr;	// �A�h���X��������sockaddr_in�\����
	int optval = 1;						// �I�v�V�����loptval��1�ɏ�����
	WSAPOLLFD fdArray[6];			    // WSAPoll�֐��Ŏg�p�����\�P�b�g��񂪊i�[
	int count = 0;						// �ڑ����N���C�A���g�̐�
	int poll_result;					// poll�̖߂�l
	struct sockaddr_in saiClientAddr;	// �A�N�Z�v�g�����N���C�A���g�̃A�h���X���
	int iClientAddrLen;					// saiClientAddr�̃T�C�Y
	char* client_ip_addr_str;			// �N���C�A���g��IP�A�h���X������
	u_short client_port;				// �A�N�Z�v�g�����N���C�A���g�̃|�[�g�ԍ�
	int pos;							// �󂫗v�f�C���f�b�N�X�ꎞ�ۑ��p
	WSABUF wsaBuf;						// WSABUF�^�ϐ�(revents�����o�[���N���A�s�v)
	BYTE btBuf[256];					
	DWORD dwRecvLen = 0;				
	DWORD dwFlags = 0;					// WSARecv�Ɏw�肷��t���O
	DWORD dwSendLen = 0;				


	// �\�P�b�g�̏�����
	int iRet = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iRet!= EXIT_SUCCESS) {
		std::cout << "Error!(iRet = %d." << std::endl << iRet;
		WSACleanup();
		return EXIT_FAILURE;
	}
	// �\�P�b�g�̍쐬
	SOCKET soc = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, 0);
	if (soc == INVALID_SOCKET) {
		std::cout << "WSASocket Error!" << std::endl;
		WSACleanup();
		return EXIT_FAILURE;
	}
	// �|�[�g�ԍ��̕ϊ�
	WSAHtons(soc, PORT, &ns_port);
	std::cout <<  "port = " << PORT <<  " ns_port = " <<  ns_port << std::endl;
	// �o�C���h����A�h���X���addr�̐ݒ�.
	saiServerAddr.sin_family = AF_INET;					// IPv4�t�@�~���[AF_INET
	saiServerAddr.sin_port = ns_port;					
	saiServerAddr.sin_addr.S_un.S_addr = INADDR_ANY;	// ���ׂẴ��[�J���C���^�[�t�F�C�X�Ƀo�C���h

	// SO_REUSEADDR�̗L����
	if (setsockopt(soc, SOL_SOCKET, SO_REUSEADDR, (const char*)&optval, sizeof(optval)) != EXIT_SUCCESS) {
		std::cout << "setsockopt(SO_REUSEADDR) Error!" << std::endl;
		closesocket(soc);
		WSACleanup();
		return EXIT_FAILURE;
	}
	// soc��saiServerAddr���o�C���h
	if (bind(soc, (struct sockaddr*)&saiServerAddr, sizeof(saiServerAddr)) != EXIT_SUCCESS) {
		std::cout << "bind Error!" << std::endl;
		closesocket(soc);
		WSACleanup();
		return EXIT_FAILURE;
	}
	// �ڑ��v�����i�[����L���[�̍쐬
	if (listen(soc, LIMITED_CLIENT_CONNECTION) != EXIT_SUCCESS) {
		std::cout << "listen Error!" << std::endl;
		closesocket(soc);
		WSACleanup();
		return EXIT_FAILURE;
	}

	// pollfd�\���̔z��̏�����
	fdArray[0].fd = soc;	// 0�Ԗڂ̓T�[�o�\�P�b�g
	fdArray[0].events = POLLIN;
	memset(&fdArray[1], 0, sizeof(WSAPOLLFD) * LIMITED_CLIENT_CONNECTION);
	for (int i = 1; i <= LIMITED_CLIENT_CONNECTION; i++) {
		fdArray[i].fd = -1;
	}
	count = 0;

	while (TRUE) {
		// �\�P�b�g�Ď�
		poll_result = WSAPoll(fdArray, count + 1, POLLING_INTERVAL);
		if (poll_result == SOCKET_ERROR) {
			std::cout << "WSAPoll Error! " << WSAGetLastError() << std::endl;
		}
		// �Ď��ΏۂȂ�
		else if (poll_result == 0) {
			continue;
		}
		// �Ď��Ώۂ���                  
		// �T�[�o�\�P�b�gsoc.
		if (fdArray[0].revents & POLLIN) {

			// �A�N�Z�v�g�̑O����
			iClientAddrLen = sizeof(saiClientAddr);
			// �A�N�Z�v�g�҂�
			SOCKET acc = WSAAccept(soc, (struct sockaddr*)&saiClientAddr, &iClientAddrLen, NULL, NULL);
			if (acc == INVALID_SOCKET) {
				std::cout << "WSAAccept Error !" << WSAGetLastError() << std::endl;
			}

			client_ip_addr_str = inet_ntoa(saiClientAddr.sin_addr);								// inet_ntoa�ŃN���C�A���g��saiClientAddr.sin_addr��IP�A�h���X������ɕϊ�
			WSANtohs(acc, saiClientAddr.sin_port, &client_port);								// WSANtohs�ŃN���C�A���g�̃|�[�g�ԍ����z�X�g�o�C�g�I�[�_�ɕϊ�
			std::cout << "accept!(IPAddress = " << client_ip_addr_str <<  "Port = " << client_port << std::endl;;

			// fds�ւ̊i�[
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
					// �N���C�A���g���烁�b�Z�[�W��M
					if (WSARecv(fdArray[i].fd, &wsaBuf, 1, &dwRecvLen, &dwFlags, NULL, NULL) != EXIT_SUCCESS) {
						std::cout << "WSARecv Error !" << WSAGetLastError() << std::endl;
					}
					std::cout << wsaBuf.buf << std::endl;

					//wsaBuf.buf�̓��e�𑗂�Ԃ�.
					if (WSASend(fdArray[i].fd, &wsaBuf, 1, &dwSendLen, dwFlags, NULL, NULL) != EXIT_SUCCESS) {
						std::cout << "WSASend Error !" << WSAGetLastError() << std::endl;
					}
				}
			}
		}
	}

	// wsaPollFds��S�ĕ���
	for (int i = LIMITED_CLIENT_CONNECTION; i >= 0; i--) {
		closesocket(fdArray[i].fd);
	}
	WSACleanup();
	return EXIT_SUCCESS;

}