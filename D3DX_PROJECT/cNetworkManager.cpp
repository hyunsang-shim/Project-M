#include "stdafx.h"
#include "cNetworkManager.h"
#include "cOtherCharacter.h"
#include <string>
#include <iostream>
#include <queue>
using namespace std;

// ���� �ּ�
// �Ʒ� �� �ϳ��� Ȱ��ȭ ��Ű�� ���.
#define SERVER_ADDR "165.246.163.66"	// ��ȣ��
//#define SERVER_ADDR "165.246.163.71"	// Shim Hyunsang
//#define SERVER_ADDR "192.168.0.9"		// ������ (��Ʈ��/������)
//#define SERVER_ADDR "192.168.0.7"		// ������ (��)
//#define SERVER_ADDR "127.0.0.1"	// ������


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


void threadProcessRecv(void * str)
{
	while (1)
	{
		Sleep(1);
		string tmp;
		char* givenMessage;
		if (g_pNetworkManager->messageQueue.size() == 0)
		{
			continue;
		}
		tmp = g_pNetworkManager->messageQueue.front();
		g_pNetworkManager->messageQueue.pop();

		givenMessage = new char[tmp.size() + 1];
		std::copy(tmp.begin(), tmp.end(), givenMessage);
		givenMessage[tmp.size()] = '\0';
		printf("%s\n", givenMessage);

		if (StartWith(givenMessage, "userStatus"))
		{
			int ID;
			D3DXVECTOR3 Pos;
			float Dir;
			int Status;
			int CurHP;

			sscanf_s(givenMessage, "%*s %d %f %f %f %f %d %d", &ID, &Pos.x, &Pos.y, &Pos.z, &Dir, &Status, &CurHP);
			for (int i = 0; i < g_pOtherPlayerManager->otherPlayerInfo.size(); i++)
			{
				if (g_pOtherPlayerManager->otherPlayerInfo.at(i)->info.ID == ID)
				{
					g_pOtherPlayerManager->otherPlayerInfo.at(i)->Update(Pos, Dir, Status);
					g_pOtherPlayerManager->otherPlayerInfo.at(i)->info.CurHP = CurHP;
				}
			}
		}
		else if (StartWith(givenMessage, "shot"))
		{
			int ID;
			int monsterNum;
			int damage;

			sscanf_s(givenMessage, "%*s %d %d %d", &ID, &damage, &ID);

		}
		else if (StartWith(givenMessage, "select"))
		{
			int ID;
			int character;

			sscanf_s(givenMessage, "%*s %d %d", &ID, &character);
			/*	for (int i = 0; i < g_vUsers.size(); i++)
			{
			if (g_vUsers.at(i).ID == ID)
			{
			g_vUsers.at(i).Character_No = character;
			}
			}*/
		}
		else if (StartWith(givenMessage, "myNameIs"))
		{
			int ID;
			char name[16];
			sscanf_s(givenMessage, "%*s %d %s", &ID, &name, 16);

			if (g_pGameInfoManager->GetMyInfo()->ID == ID)
				continue;

			CharacterStatus_PC* tmp = new CharacterStatus_PC;
	

			tmp->ID = ID;
			strcpy(tmp->PlayerName, name);
			g_pOtherPlayerManager->newPlayer(tmp);
		}
		else if (StartWith(givenMessage, "SetID"))
		{
			int IDID;
			sscanf_s(givenMessage, "%*s %d", &IDID);
			g_pGameInfoManager->GetMyInfo()->ID = IDID;
		}
	}
}


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
	hThread = new HANDLE;
	hThread = (HANDLE)_beginthreadex(NULL, 0, (unsigned int(__stdcall*)(void *))threadProcessRecv, NULL, 0, NULL);


	return isConnected;
	
}

void cNetworkManager::SendData(CharacterStatus_PC strPC)
{
	if (isConnected)
	{		
		send(s, (char*)&strPC, sizeof(CharacterStatus_PC)+1, 0);
	}
}

int cNetworkManager::SendData( NETWORK_HEADER NH, CharacterStatus_PC *strPC)
{
	int result = 0;
	string sendString;

	if (isConnected)
	{
		if (NH == NH_USER_STATUS)
		{
 			sendString += "userStatus";
			sendString += ' ';
			sendString += to_string(strPC->ID);
			sendString += ' ';
			sendString += to_string(strPC->CurPos.x);
			sendString += ' ';
			sendString += to_string(strPC->CurPos.y);
			sendString += ' ';
			sendString += to_string(strPC->CurPos.z);
			sendString += ' ';
			sendString += to_string(strPC->Dir);
			sendString += ' ';
			sendString += to_string(strPC->Status);
			sendString += ' ';
			sendString += to_string(strPC->CurHP);
		}
		if (NH == NH_SHOT)
		{
			sendString += "shot";
			sendString += ' ';
			sendString += to_string(strPC->ID);
			sendString += ' ';
			int monsterNum = 0; // hit monster num
			sendString += to_string(monsterNum);
			sendString += ' ';
			sendString += to_string(strPC->Attack);
		}
		if (NH == NH_SELECT)
		{
			sendString += "select";
			sendString += ' ';
			sendString += to_string(strPC->ID);
			sendString += ' ';
			sendString += to_string(strPC->Character_No);
		}
		if (NH == NH_MY_NAME_IS)
		{
			sendString += "myNameIs";
			sendString += ' ';
			sendString += to_string(strPC->ID);
			sendString += ' ';
			sendString += string(strPC->PlayerName);
		}
		
		char * sendMessage = new char[sendString.size() + 1];
		copy(sendString.begin(), sendString.end(), sendMessage);
		sendMessage[sendString.size()] = '\0';

		result = send(s, sendMessage, sendString.size() + 1, 0);

	}

	return result;
}

void cNetworkManager::recvData()
{
	char buffer[64];
	recv(s, buffer, 64, 0);
	string tmp = string(buffer);
	messageQueue.push(tmp);
}

bool cNetworkManager::GetNetStatus()
{
	return isConnected;
}

SOCKET cNetworkManager::GetServerSocket()
{
	return s;
}

