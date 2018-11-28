#include "stdafx.h"
#include "cNetworkManager.h"

// ���� �ּ�
// �Ʒ� �� �ϳ��� Ȱ��ȭ ��Ű�� ���.
//#define SERVER_ADDR "165.246.163.66"	// ��ȣ��
#define SERVER_ADDR "165.246.163.71"	// Shim Hyunsang
//#define SERVER_ADDR "192.168.0.9"		// ������ (��Ʈ��/������)
//#define SERVER_ADDR "192.168.0.7"		// ������ (��)
//#define SERVER_ADDR "127.0.0.1"	// ������


cNetworkManager::cNetworkManager()
{
}

cNetworkManager::~cNetworkManager()
{	
}

//bool StartWith(char * FindStr, char * SearchStr)
//{
//	char* temp = strstr(FindStr, SearchStr);
//	if (temp == FindStr)
//		return true;
//	return false;
//}

bool cNetworkManager::SetupNetwork(HWND hWnd)
{
	WSAStartup(MAKEWORD(2, 2), &wsadata);
	s = socket(AF_INET, SOCK_STREAM, 0);
	addr.sin_family = AF_INET;
	addr.sin_port = 20;
	addr.sin_addr.S_un.S_addr = inet_addr(SERVER_ADDR);
	WSAAsyncSelect(s, hWnd, WM_ASYNC, FD_READ | FD_CLOSE);

	static int ConnectRes;
	ConnectRes = connect(s, (LPSOCKADDR)&addr, sizeof(addr));

	if (ConnectRes != -1)
	{
		MessageBox(NULL, _T("Connection Failed!"), _T("Error!!"), MB_OK);
		isConnected = false;
	}
	else
		isConnected = true;
	// <<

	return isConnected;
	
}

void cNetworkManager::SendData(CharacterStatus_PC strPC)
{
	if (isConnected)
	{		
		send(s, (char*)&strPC, sizeof(CharacterStatus_PC)+1, 0);
	}
}

int cNetworkManager::SendData(char * MsgHeader, CharacterStatus_PC *strPC)
{
	int result = 0;
	if (isConnected)
	{
		
		strcpy(strPC->MsgHeader, MsgHeader);
		if (MySocket != strPC->s)
			strPC->s = MySocket;

		result = send(s, (char*)&strPC, sizeof(CharacterStatus_PC) + 1, 0);

		if (strcmp(MsgHeader, "join") == 0)
		{
			strcpy(strPC->MsgHeader, "userData");
		}
	}

	return result;
}

void cNetworkManager::recvData()
{
	if (isConnected)
	{
		memset(buffer, 0, sizeof(CharacterStatus_PC)+1);
		
		int bufferLen = recv(s, buffer, sizeof(CharacterStatus_PC)+1, 0);
		buffer[bufferLen] = NULL;
		CharacterStatus_PC* tmp = (CharacterStatus_PC*)&buffer;
		if (strcmp(tmp->MsgHeader, "join") == 0)
		{
			g_pOtherPlayerManager->newPlayer(tmp);
		}
		else if (strcmp(tmp->MsgHeader, "userData") == 0)
		{

			int check = 0;
			for (int i = 0; i < OtherPlayer.size(); i++)
			{
				if (OtherPlayer.at(i)->info.ID == tmp->ID)
				{
					OtherPlayer.at(i)->Update(tmp->CurPos, tmp->Dir, tmp->Status);
					check = 1;
					break;
				}
			}

			if (check == 0)
			{
				g_pOtherPlayerManager->newPlayer(tmp);
			}
		}
		else if (strcmp(tmp->MsgHeader, "disconnect") == 0)
		{
			int num = tmp->ID;
			g_pOtherPlayerManager->disconnectPlayer(num);	// erase user from OtherPlayerManager and also GameInfoManager.
			/*for (int i = 0; i < OtherPlayer.size(); i++)
			{
				if (OtherPlayer.at(i)->info.ID == num)
				{
					delete(OtherPlayer.at(i));
					OtherPlayer.erase(OtherPlayer.begin() + i);
					i--;
					break;
				}
			}*/
		}
		else if (strcmp(tmp->MsgHeader, "welcome") == 0)
		{
			//MessageBox(NULL, _T("Server Said: Welcome!!"), _T("Message Recieved"), MB_OK);		
			tmp->s = s;
			g_pGameInfoManager->UpdateMyInfo(*tmp);
			g_pNetworkManager->SendData("TitleScene", g_pGameInfoManager->GetMyInfo());
		}
	}
}

bool cNetworkManager::GetNetStatus()
{
	return isConnected;
}

SOCKET cNetworkManager::GetServerSocket()
{
	return s;
}

