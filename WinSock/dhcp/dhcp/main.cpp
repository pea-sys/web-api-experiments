#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <iostream> 

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x)) 
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

// DHCPを通じて取得したIPアドレスの解法と再取得(IPv4のみ有効)

void __cdecl main()
{
    ULONG ulOutBufLen = 0;
    DWORD dwRetVal = 0;
    PIP_INTERFACE_INFO pInfo;

    pInfo = (IP_INTERFACE_INFO*)MALLOC(sizeof(IP_INTERFACE_INFO));

    // Make an initial call to GetInterfaceInfo to get
    // the necessary size into the ulOutBufLen variable
    if (GetInterfaceInfo(pInfo, &ulOutBufLen) == ERROR_INSUFFICIENT_BUFFER) {
        FREE(pInfo);
        pInfo = (IP_INTERFACE_INFO*)MALLOC(ulOutBufLen);
    }

    // Make a second call to GetInterfaceInfo to get the
    // actual data we want
    if ((dwRetVal = GetInterfaceInfo(pInfo, &ulOutBufLen)) == NO_ERROR) {
        printf("\tAdapter Name: %ws\n", pInfo->Adapter[0].Name);
        printf("\tAdapter Index: %ld\n", pInfo->Adapter[0].Index);
        printf("\tNum Adapters: %ld\n", pInfo->NumAdapters);
    }
    else if (dwRetVal == ERROR_NO_DATA) {
        printf("There are no network adapters with IPv4 enabled on the local system\n");
        return;
    }
    else {
        LPVOID lpMsgBuf;
        printf("GetInterfaceInfo failed.\n");

        if (FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            dwRetVal,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
            (LPTSTR)&lpMsgBuf,
            0,
            NULL)) {
            printf("\tError: %s", lpMsgBuf);
        }
        LocalFree(lpMsgBuf);
        return;
    }

    if ((dwRetVal = IpReleaseAddress(&pInfo->Adapter[0])) == NO_ERROR) {
        printf("IP release succeeded.\n");
    }
    else {
        printf("IP release failed: %ld\n", dwRetVal);
    }

    if ((dwRetVal = IpRenewAddress(&pInfo->Adapter[0])) == NO_ERROR) {
        printf("IP renew succeeded.\n");
    }
    else {
        printf("IP renew failed: %ld\n", dwRetVal);
    }

    // Free memory for IP_INTERFACE_INFO 
    if (pInfo != NULL) {
        FREE(pInfo);
    }
    std::cout << ("\n Finish\n");
    std::system("PAUSE");

    return;
}