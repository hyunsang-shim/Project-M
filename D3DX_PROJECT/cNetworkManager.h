#pragma once
#define g_pNetworkManager cNetworkManager::GetInstance()

#define WM_ASYNC WM_USER+2
#pragma comment(lib,"ws2_32.lib")


#include "stdafx.h"


class cNetworkManager
{
public:
	SINGLETON(cNetworkManager);

private:
	WSADATA wsadata;
	SOCKET s, cs;
	SOCKADDR_IN addr = { 0 };
	char buffer[1000];
	vector<CharacterStatus_PC> user;
	bool isConnected = false;		// ���� ���� Ȯ��


public:
	bool SetupNetwork(HWND hWnd);
	void SendData(CharacterStatus_PC strPC);
	void SendData(char* MsgHeader, CharacterStatus_PC strPC);
	void recvData();
	bool GetNetStatus();		// ��Ʈ��ũ Ȯ�ο�

};

