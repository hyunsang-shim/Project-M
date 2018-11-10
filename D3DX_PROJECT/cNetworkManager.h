#pragma once
#define g_pNetworkManager cNetworkManager::GetInstance()

#define WM_ASYNC WM_USER+2
#pragma comment(lib,"ws2_32.lib")

#include <winsock.h>



class cNetworkManager
{
public:
	SINGLETON(cNetworkManager);

private:
	WSADATA wsadata;
	SOCKET s, cs;
	SOCKADDR_IN addr = { 0 };
	char buffer[1000];
	vector<UserInfo> user;
	bool isConnected = false;		// 접속 상태 확인


public:
	bool SetupNetwork(HWND hWnd);
	void SendData(string str);
	void recvData();
	bool GetNetStatus();		// 네트워크 확인용

};

