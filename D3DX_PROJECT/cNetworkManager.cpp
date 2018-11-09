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

void cNetworkManager::SetupNetwork(HWND hWnd)
{
	WSAStartup(MAKEWORD(2, 2), &wsadata);
	s = socket(AF_INET, SOCK_STREAM, 0);
	addr.sin_family = AF_INET;
	addr.sin_port = 20;
	addr.sin_addr.s_addr = inet_addr("165.246.163.66");
	int x = connect(s, (LPSOCKADDR)&addr, sizeof(addr));
	WSAAsyncSelect(s, hWnd, WM_ASYNC, FD_READ);


}

void cNetworkManager::SendData(string str)
{
	char info[1000];
	strcpy(info, str.c_str());
	send(s, info, 1000, 0);
}

void cNetworkManager::recvData()
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

