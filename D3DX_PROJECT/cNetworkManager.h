#pragma once
#define g_pNetworkManager cNetworkManager::GetInstance()

#define WM_ASYNC WM_USER+2
#pragma comment(lib,"ws2_32.lib")
#include <queue>
#include <process.h> 

#include "stdafx.h"


class cNetworkManager
{
public:
	SINGLETON(cNetworkManager);

private:
	WSADATA wsadata;
	SOCKET s, MySocket;
	SOCKADDR_IN addr = { 0 };
	char buffer[1000];
	vector<CharacterStatus_PC> user;
	bool isConnected = false;		// 접속 상태 확인
	HANDLE hThread;


public:
	queue<string> messageQueue;
	bool SetupNetwork(HWND hWnd);
	void SendData(CharacterStatus_PC strPC);
	int SendData(enum NETWORK_HEADER, CharacterStatus_PC* strPC);
	void recvData();
	bool GetNetStatus();		// 네트워크 확인용
	SOCKET GetServerSocket();

};

