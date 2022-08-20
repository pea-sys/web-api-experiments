
#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#pragma comment(lib, "mpr.lib")
/*
　UNCを取得
  
  Connection name: \\127.0.0.1\share
*/
int main(int argc, char* argv[]) {
    TCHAR szDeviceName[80];
    DWORD dwResult, cchBuff = sizeof(szDeviceName);
    NETRESOURCE nr;

    // Call the WNetGetConnection function.
    //ホスト名前解決にNetBIOSを使用している(NBNSパケットが飛ぶ)
    dwResult = WNetGetConnection(_T("z:"),
        szDeviceName,
        &cchBuff);

    switch (dwResult)
    {
        //
        // Print the connection name or process errors.
        //
    case NO_ERROR:
        printf("Connection name: %s\n", szDeviceName);
        break;
        //
        // The device is not a redirected device.
        //
    case ERROR_NOT_CONNECTED:
        printf("Device z: not connected.\n");
        break;
        //
        // The device is not currently connected, but it is a persistent connection.
        //
    case ERROR_CONNECTION_UNAVAIL:
        printf("Connection unavailable.\n");
        break;
        //
        // Handle the error.
        //
    default:
        printf("WNetGetConnection failed.\n");
    }
    

    ZeroMemory(&nr, sizeof(NETRESOURCE));

    nr.dwType = RESOURCETYPE_DISK;
    nr.lpLocalName = NULL;
    nr.lpRemoteName = _T(szDeviceName);
    nr.lpProvider = NULL;
    //SMBプロトコルを使用したサーバフォルダアクセス
    dwResult = WNetAddConnection2(&nr, _T("share-password"), _T("share-id"), 0);
    // Process errors.
    //  The local device is already connected to a network resource.
    //
    if (dwResult == ERROR_ALREADY_ASSIGNED)
    {
        printf("Already connected to specified resource.\n");
        return dwResult;
    }

    //  An entry for the local device already exists in the user profile.
    //
    else if (dwResult == ERROR_DEVICE_ALREADY_REMEMBERED)
    {
        printf("Attempted reassignment of remembered device.\n");
        return dwResult;
    }
    else if (dwResult != NO_ERROR)
    {
        //
        // Call an application-defined error handler.
        //
        printf("WNetAddConnection2 failed.\n");
        return dwResult;
    }
    return 0;
}