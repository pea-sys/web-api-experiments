//IPv4��ping
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>

#include <winsock2.h>
//Iphlpapi.h�w�b�_�[ �t�@�C���̃C���N���[�h �f�B���N�e�B�u�́A
//Icmpapi.h�w�b�_�[ �t�@�C���̑O�ɔz�u����K�v������
#include <iphlpapi.h>
#include <icmpapi.h>

#pragma comment(lib,"iphlpapi.lib")
#pragma comment(lib, "Ws2_32.lib") 

int main()
{
	HANDLE hIcmp;
	char* SendData = "ICMP SEND DATA";
	LPVOID ReplyBuffer;
	DWORD dwRetVal;
	DWORD buflen;
	PICMP_ECHO_REPLY pIcmpEchoReply;

	//IPv4 ICMP�G�R�[���N�G�X�g���s�\�ȃn���h�����I�[�v������
	hIcmp = IcmpCreateFile();
	if (hIcmp == INVALID_HANDLE_VALUE) {
		printf("\tUnable to open handle.\n");
		printf("IcmpCreatefile returned error: %ld\n", GetLastError());
		return 1;
	}

	//ping���M�p�f�[�^�������T�C�Y�擾
	buflen = sizeof(ICMP_ECHO_REPLY) + strlen(SendData) + 1;
	//���������蓖��
	ReplyBuffer = (VOID*)malloc(buflen);
	//���������蓖�Ď��s
	if (ReplyBuffer == NULL) {
		return 1;
	}
	//ReplyBuffer���蓖�ă���������buflen�o�C�g������0�ŏ�����
	memset(ReplyBuffer, 0, buflen);
	pIcmpEchoReply = (PICMP_ECHO_REPLY)ReplyBuffer;
	//IPv4 ICMP �G�R�[�v���𑗐M���A�G�R�[����������Ԃ��܂�
	dwRetVal = IcmpSendEcho(hIcmp,
		inet_addr("127.0.0.1"),
		SendData, strlen(SendData),
		NULL, ReplyBuffer,
		buflen,
		1000);
	if (dwRetVal != 0) {
		printf("Received %ld messages.\n", dwRetVal);
		printf("\n");
		printf("RTT: %d\n", pIcmpEchoReply->RoundTripTime);
		printf("Data Size: %d\n", pIcmpEchoReply->DataSize);
		printf("Message: %s\n", pIcmpEchoReply->Data);
	}
	else {
		printf("Call to IcmpSendEcho() failed.\n");
		printf("Error: %ld\n", GetLastError());
	}

	if (!IcmpCloseHandle(hIcmp)) {
		printf("Error: %ld\n", GetLastError());
	}

	return 0;
}