#pragma once
#include "stdafx.h"
#define g_pNetworkManager cNetworkManager::GetInstance()

#define WM_ASYNC WM_USER+2
#pragma comment(lib,"ws2_32.lib")
#include <queue>
#include <process.h> 
#include "cCharacter.h"


class cNetworkManager
{
public:
	SINGLETON(cNetworkManager);

private:
	WSADATA wsadata;
	SOCKET s, MySocket;
	SOCKADDR_IN addr = { 0 };
	char buffer[1000];
	vector<UserInfo> user;
	bool isConnected = false;		// ���� ���� Ȯ��
	HANDLE hThread;


public:
	queue<string> messageQueue;
	bool SetupNetwork(HWND hWnd);
	void SendData(CharacterStatus_PC strPC);
	int SendData(enum NETWORK_HEADER, CharacterStatus_PC* strPC);
	void recvData();
	bool GetNetStatus();		// ��Ʈ��ũ Ȯ�ο�
	SOCKET GetServerSocket();

};

