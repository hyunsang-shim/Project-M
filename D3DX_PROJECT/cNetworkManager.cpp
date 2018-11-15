#include "stdafx.h"
#include "cNetworkManager.h"


cNetworkManager::cNetworkManager()
{
}


cNetworkManager::~cNetworkManager()
{
}

bool StartWith(char * FindStr, char * SearchStr)
{
	char* temp = strstr(FindStr, SearchStr);
	if (temp == FindStr)
		return true;
	return false;
}

bool cNetworkManager::SetupNetwork(HWND hWnd)
{
	WSAStartup(MAKEWORD(2, 2), &wsadata);
	s = socket(AF_INET, SOCK_STREAM, 0);
	addr.sin_family = AF_INET;
	addr.sin_port = 20;
	// addr.sin_addr.S_un.S_addr = inet_addr("165.246.163.66");	// ��ȣ��
	addr.sin_addr.S_un.S_addr = inet_addr("165.246.163.71"); // ������
	// addr.sin_addr.S_un.S_addr = inet_addr("192.168.0.7"); // ������(��)
	 	
	int x = connect(s, (LPSOCKADDR)&addr, sizeof(addr));		// �����ϸ� 0 ����, �ƴϸ� ���� ����.
	WSAAsyncSelect(s, hWnd, WM_ASYNC, FD_READ);

	// ���ῡ �����ϸ� �÷��׸� �Ҵ�.
	// >>
	if (!x)
	{
		isConnected = true;
	}

	return isConnected;
	
	// <<
}

void cNetworkManager::SendData(CharacterStatus_PC strPC)
{
	if (isConnected)
	{		
		send(s, (char*)&strPC, 1000, 0);
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
		
		recv(s, buffer, sizeof(CharacterStatus_PC)+1, 0);
		CharacterStatus_PC* tmp = (CharacterStatus_PC*)buffer;

		if (StartWith(buffer, "userData"))
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
		else if (StartWith(buffer, "totalUser"))
		{
			int num;
			sscanf_s(buffer, "%*s %d", &num);
			g_pOtherPlayerManager->userNum = num;
		}
		else if (StartWith(buffer, "disconnect"))
		{
			int num;
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
	}
}

bool cNetworkManager::GetNetStatus()
{
	return isConnected;
}

