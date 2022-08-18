#pragma once

#if !defined(WS_UTIL_H)
#define WS_UTIL_H

// Uncomment one.
//#include <winsock.h>
#include <winsock2.h>
#pragma comment(lib, "Ws2_32.lib")
extern const char* WSAGetLastErrorMessage(const char* pcMessagePrefix,
    int nErrorID = 0);
extern bool ShutdownConnection(SOCKET sd);

#endif // !defined (WS_UTIL_H)

