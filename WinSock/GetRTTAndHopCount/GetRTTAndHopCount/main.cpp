#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>

#include <winsock2.h>
#include <iphlpapi.h>

#pragma comment(lib,"iphlpapi.lib")
#pragma comment(lib, "Ws2_32.lib") 

int main()
{
	IPAddr ipaddr = inet_addr("8.8.8.8"); //Google DNS
	ULONG hopCount = 0;
	ULONG maxHops = 64;
	ULONG rtt = 0;

	if (GetRTTAndHopCount(ipaddr, &hopCount, maxHops, &rtt) == TRUE) {
		/*Hops: 14 (中継地点の数。転送によるオーバーヘッドが大きいので少ない方が良い)
		  RTT:  77 msec*/
		printf("Hops: %ld\n", hopCount);
		printf("RTT:  %ld msec\n", rtt);
	}
	else {
		printf("GetRTTAndHopCount failed : %d\n", GetLastError());
	}

	return 0;
}
