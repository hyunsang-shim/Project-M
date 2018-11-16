#include "stdafx.h"
#include "cNetworkManager.h"


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
	// addr.sin_addr.S_un.S_addr = inet_addr("165.246.163.66");	// 은호씨
	addr.sin_addr.S_un.S_addr = inet_addr("165.246.163.71"); // 심현상
	// addr.sin_addr.S_un.S_addr = inet_addr("192.168.0.7"); // 심현상(집)
	 	
	int x = connect(s, (LPSOCKADDR)&addr, sizeof(addr));		// 성공하면 0 리턴, 아니면 에러 리턴.
	WSAAsyncSelect(s, hWnd, WM_ASYNC, FD_READ);

	// 연결에 성공하면 플래그를 켠다.
	// >>
	if (!x) isConnected = true;

	return isConnected;
	
	// <<
}

void cNetworkManager::SendData(CharacterStatus_PC strPC)
{
	if (isConnected)
	{		
		send(s, (char*)&strPC, sizeof(CharacterStatus_PC)+1, 0);
	}
}

void cNetworkManager::recvData()
{
	if (isConnected)
	{
		/*
		memset(buffer, 0, 200);
		bufferLen = recv(s, buffer, 200, 0);
		buffer[bufferLen] = NULL;
		OMOK_MSG_SYS* tmpsys = (OMOK_MSG_SYS*)buffer;

		*/
		memset(buffer, 0, sizeof(CharacterStatus_PC)+1);

		int bufferLen = recv(s, buffer, sizeof(CharacterStatus_PC) + 1, 0);
		buffer[bufferLen] = NULL;
		recv(s, buffer, sizeof(CharacterStatus_PC)+1, 0);
		CharacterStatus_PC* tmp = (CharacterStatus_PC*)buffer;


		if (strcmp(tmp->MsgHeader, "userData") == 0)
		{
			//float x, y, z, direc;
			//int actCount, act, userNum;
			//sscanf_s(buffer, "%*s %d %f %f %f %f %d %d", &userNum, &x, &y, &z, &direc, &act, &actCount);
			//tmp.userNum = userNum;
			//tmp.x = x;
			//tmp.y = y;
			//tmp.z = z;
			//tmp.direction = direc;
			//tmp.action = act;
			//tmp.actionCount = actCount;

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
		else if (strcmp(tmp->MsgHeader, "totalUser") == 0)
		{
			int num;
			sscanf_s(buffer, "%*s %d", &num);
			g_pOtherPlayerManager->userNum = num;
		}
		else if (strcmp(tmp->MsgHeader, "disconnect") == 0)
		{
			int num = tmp->ID;
			for (int i = 0; i < OtherPlayer.size(); i++)
			{
				if (OtherPlayer.at(i)->info.ID == num)
				{
					delete(OtherPlayer.at(i));
					OtherPlayer.erase(OtherPlayer.begin() + i);
					i--;
					break;
				}
			}
		}
		else if (strcmp(tmp->MsgHeader, "welcome") == 0)
		{
			MessageBox(NULL, _T("Server Said: Welcome!!"), _T("Message Recieved"), MB_OK);
			tmp->s = s;
			g_pGameInfoManager->UpdateMyInfo(*tmp);
		}
	}
}

bool cNetworkManager::GetNetStatus()
{
	return isConnected;
}

void cNetworkManager::SendData(char * MsgHeader, CharacterStatus_PC *strPC)
{
	if (isConnected)
	{

		strcpy(strPC->MsgHeader, MsgHeader);
		send(s, (char*)&strPC, sizeof(CharacterStatus_PC) + 1, 0);
		if (strcmp(MsgHeader, "join") == 0)
		{
			strcpy(strPC->MsgHeader, "userData");
		}
	}
}