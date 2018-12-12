#include "stdafx.h"
#include "cNetworkManager.h"
#include "cOtherCharacter.h"
#include <string>
#include <iostream>
#include <queue>
#include "cAI.h"
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") 
using namespace std;


// ?�쏙?�占?�옙 ?�쌍?�옙
// ?�싣뤄옙 ?�쏙???�싹?�옙?�쏙???�占?�옙???�쏙?�키?�쏙???�쏙?�占?
#define SERVER_ADDR "165.246.163.66"	// ?�쏙?�호?�쏙??
//#define SERVER_ADDR "165.246.163.71"	// Shim Hyunsang
//#define SERVER_ADDR "192.168.0.9"		// ?�쏙?�占?�옙?�쏙??(?�쏙?�트?�쏙???�쏙?�占?�옙?�쏙??
//#define SERVER_ADDR "192.168.0.7"		// ?�쏙?�占?�옙?�쏙??(?�쏙??
//#define SERVER_ADDR "127.0.0.1"	// ?�쏙?�占?�옙?�쏙??


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
		EnterCriticalSection(&g_pNetworkManager->crit);
		tmp = g_pNetworkManager->messageQueue.front();
		g_pNetworkManager->messageQueue.pop();
		LeaveCriticalSection(&g_pNetworkManager->crit);

		givenMessage = new char[tmp.size() + 1];
		std::copy(tmp.begin(), tmp.end(), givenMessage);
		givenMessage[tmp.size()] = '\0';


		if (!StartWith(givenMessage, "ping"))
			printf("%s\n", givenMessage);


		if (StartWith(givenMessage, "ping"))
		{
			continue;
		}
		else if (StartWith(givenMessage, "userStatus"))
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

			CharacterStatus_PC tmp;


			tmp.ID = ID;
			strcpy(tmp.PlayerName, name);
			g_pOtherPlayerManager->newPlayer(tmp);
		}
		else if (StartWith(givenMessage, "SetID"))
		{
			int IDID;
			sscanf_s(givenMessage, "%*s %d", &IDID);
			g_pGameInfoManager->GetMyInfo()->ID = IDID;
		}
		else if (StartWith(givenMessage, "disconnect"))
		{
			int IDID;
			sscanf_s(givenMessage, "%*s %d", &IDID);
			for (int i = 0; i < g_pOtherPlayerManager->otherPlayerInfo.size(); i++)
			{
				if (g_pOtherPlayerManager->otherPlayerInfo.at(i)->info.ID == IDID)
				{
					SAFE_DELETE(g_pOtherPlayerManager->otherPlayerInfo.at(i));
					g_pOtherPlayerManager->otherPlayerInfo.erase(g_pOtherPlayerManager->otherPlayerInfo.begin() + i);


					break;
				}
			}
		}
		else if (StartWith(givenMessage, "setMonster"))
		{
			int IDID, MonsterID;
			float x, y, z;
			sscanf_s(givenMessage, "%*s %d %d %f %f %f", &IDID, &MonsterID, &x, &y, &z);

			cAI *tmp;

			g_pGameInfoManager->m_pVecAI.push_back(tmp);
			g_pGameInfoManager->m_pVecAI.back() = new cAI;
			g_pGameInfoManager->m_pVecAI.back()->Setup("NPCS", "slicer.X");
			cAI_Controller* m_pVecAI_Controller = new cAI_Controller;
			g_pGameInfoManager->m_pVecAI.back()->SetAIController(m_pVecAI_Controller);
			g_pGameInfoManager->m_pVecAI.back()->SetPosition(D3DXVECTOR3(x,y,z));
			g_pGameInfoManager->m_pVecAI.back()->MonsterNum = MonsterID;

			for (int i = 0; i < g_pOtherPlayerManager->otherPlayerInfo.size(); i++)
			{
				if (g_pOtherPlayerManager->otherPlayerInfo.at(i)->info.ID == IDID)
				{
					g_pGameInfoManager->m_pVecAI.back()->target = g_pOtherPlayerManager->otherPlayerInfo.at(i)->GetPositionPoint();
				}
			}
		}
		else if (StartWith(givenMessage, "dieMonster"))
		{
			int monsterID;
			sscanf_s(givenMessage, "%*s %d", &monsterID);

			for (int i = 0; i < g_pGameInfoManager->m_pVecAI.size(); i++)
			{
				if (g_pGameInfoManager->m_pVecAI.at(i)->MonsterNum == monsterID)
				{
					delete(g_pGameInfoManager->m_pVecAI.at(i));
					g_pGameInfoManager->m_pVecAI.erase(g_pGameInfoManager->m_pVecAI.begin() + i);
					break;
				}
			}

		}
		else
		{
			printf("error\n");
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

	InitializeCriticalSection(&crit);

	int test = WSAGetLastError();
	printf("%d", test);
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

	// <<
}

void cNetworkManager::SendData(CharacterStatus_PC strPC)
{
	if (isConnected)
	{
		send(s, (char*)&strPC, sizeof(CharacterStatus_PC) + 1, 0);
	}
}

int cNetworkManager::SendData(NETWORK_HEADER NH, CharacterStatus_PC *strPC)
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
		if (NH == NH_SPAWN_TRIGGER)
		{
			sendString += "triggerBoxNum";
			sendString += ' ';
			sendString += to_string(g_pGameInfoManager->monsterTriggerBoxNum);
		}

		char * sendMessage = new char[sendString.size() + 1];
		copy(sendString.begin(), sendString.end(), sendMessage);
		sendMessage[sendString.size()] = '\0';

		result = send(s, sendMessage, 128, 0);

	}

	return result;
}

void cNetworkManager::recvData()
{
	char* buffer = new char[128];
	recv(s, buffer, 128, 0);
	string tmp = string(buffer);

	EnterCriticalSection(&crit);
	messageQueue.push(tmp);
	LeaveCriticalSection(&crit);
}

bool cNetworkManager::GetNetStatus()
{
	return isConnected;
}

SOCKET cNetworkManager::GetServerSocket()
{
	return s;
}

