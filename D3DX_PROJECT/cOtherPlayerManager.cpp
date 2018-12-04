#include "stdafx.h"
#include "cOtherPlayerManager.h"


cOtherPlayerManager::cOtherPlayerManager()
{
}


cOtherPlayerManager::~cOtherPlayerManager()
{
}

CharacterStatus_PC* cOtherPlayerManager::GetOtherPlayerByID(int idx)
{

	return &g_pGameInfoManager->GetOthersInfo()->at(idx);
}


void cOtherPlayerManager::newPlayer(CharacterStatus_PC info)
{
	cOtherCharacter tmp ;
	cCharacter* pCharacter = new cCharacter;
	otherPlayerInfo.push_back(tmp);
	otherPlayerInfo.back().Setup();
	otherPlayerInfo.back().SetCharacterController(pCharacter);
	otherPlayerInfo.back().info = info;
	g_pGameInfoManager->AddOtherPlayer(info);
}

void cOtherPlayerManager::render()
{
	for (int i = 0; i < otherPlayerInfo.size(); i++)
	{
		otherPlayerInfo.at(i).Render();
	}
}

void cOtherPlayerManager::update()
{
	for (int i = 0; i < otherPlayerInfo.size(); i++)
	{
		otherPlayerInfo.at(i).Update();
	}
}

void cOtherPlayerManager::disconnectPlayer(int ID)
{
	for (int i = 0; i < otherPlayerInfo.size(); i++)
	{
		if (otherPlayerInfo.at(i).info.ID == ID)
		{
			otherPlayerInfo.erase(otherPlayerInfo.begin() + i);
			g_pGameInfoManager->RemoveOtherPlayerByID(ID);
			break;
		}
	}
}
