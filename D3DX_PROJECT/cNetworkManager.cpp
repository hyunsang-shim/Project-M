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
	// addr.sin_addr.S_un.S_addr = inet_addr("165.246.163.66");	// 은호씨
	// addr.sin_addr.S_un.S_addr = inet_addr("165.246.163.71"); // 심현상
	 addr.sin_addr.S_un.S_addr = inet_addr("192.168.0.7"); // 심현상(집)
	 	
	int x = connect(s, (LPSOCKADDR)&addr, sizeof(addr));		// 성공하면 0 리턴, 아니면 에러 리턴.
	WSAAsyncSelect(s, hWnd, WM_ASYNC, FD_READ);

	// 연결에 성공하면 플래그를 켠다.
	// >>
	if (!x)
	{
		isConnected = true;
	}

	return isConnected;
	
	// <<
}

void cNetworkManager::SendData(string str)
{
	if (isConnected)
	{
		char info[1000];
		strcpy(info, str.c_str());
		send(s, info, 1000, 0);
	}
}

void cNetworkManager::recvData()
{
	if (isConnected)
	{
		recv(s, buffer, 1000, 0);
		UserInfo tmp;
		if (StartWith(buffer, "userData"))
		{
			float x, y, z, direc;
			int actCount, act, userNum;
			sscanf_s(buffer, "%*s %d %f %f %f %f %d %d", &userNum, &x, &y, &z, &direc, &act, &actCount);
			tmp.userNum = userNum;
			tmp.x = x;
			tmp.y = y;
			tmp.z = z;
			tmp.direction = direc;
			tmp.action = act;
			tmp.actionCount = actCount;

			int check = 0;
			for (int i = 0; i < OtherPlayer.size(); i++)
			{
				if (OtherPlayer.at(i)->info.userNum == tmp.userNum)
				{
					OtherPlayer.at(i)->Update(tmp.x, tmp.y, tmp.z, tmp.direction, tmp.action, tmp.actionCount);
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
			sscanf_s(buffer, "%*s %d", &num);
			for (int i = 0; i < OtherPlayer.size(); i++)
			{
				if (OtherPlayer.at(i)->info.userNum == num)
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

