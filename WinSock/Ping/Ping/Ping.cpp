//IPv4でping
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>

#include <winsock2.h>
//Iphlpapi.hヘッダー ファイルのインクルード ディレクティブは、
//Icmpapi.hヘッダー ファイルの前に配置する必要がある
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

	//IPv4 ICMPエコーリクエスト発行可能なハンドルをオープンする
	hIcmp = IcmpCreateFile();
	if (hIcmp == INVALID_HANDLE_VALUE) {
		printf("\tUnable to open handle.\n");
		printf("IcmpCreatefile returned error: %ld\n", GetLastError());
		return 1;
	}

	//ping送信用データメモリサイズ取得
	buflen = sizeof(ICMP_ECHO_REPLY) + strlen(SendData) + 1;
	//メモリ割り当て
	ReplyBuffer = (VOID*)malloc(buflen);
	//メモリ割り当て失敗
	if (ReplyBuffer == NULL) {
		return 1;
	}
	//ReplyBuffer割り当てメモリからbuflenバイト分をを0で初期化
	memset(ReplyBuffer, 0, buflen);
	pIcmpEchoReply = (PICMP_ECHO_REPLY)ReplyBuffer;
	//IPv4 ICMP エコー要求を送信し、エコー応答応答を返します
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