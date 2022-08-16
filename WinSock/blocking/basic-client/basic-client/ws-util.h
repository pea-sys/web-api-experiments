#pragma once
#if !defined(WS_UTIL_H)
#define WS_UTIL_H


#include <winsock2.h>

extern const char* WSAGetLastErrorMessage(const char* pcMessagePrefix,
    int nErrorID = 0);
extern bool ShutdownConnection(SOCKET sd);

#endif // !defined (WS_UTIL_H)

#pragma once
