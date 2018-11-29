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


void cOtherPlayerManager::newPlayer(CharacterStatus_PC* info)
{
	cOtherCharacter *tmp = new cOtherCharacter;
	tmp->Setup();
	cCharacter* pCharacter = new cCharacter;
	tmp->SetCharacterController(pCharacter);
	tmp->info = *(info);
	otherPlayerInfo.push_back(tmp);
	g_pGameInfoManager->AddOtherPlayer(info);
}

void cOtherPlayerManager::render()
{
	for (int i = 0; i < otherPlayerInfo.size(); i++)
	{
		otherPlayerInfo.at(i)->Render();
	}
}

void cOtherPlayerManager::disconnectPlayer(int ID)
{
	for (int i = 0; i < otherPlayerInfo.size(); i++)
	{
		if (otherPlayerInfo[i]->info.ID == ID)
		{
			otherPlayerInfo.erase(otherPlayerInfo.begin() + i);
			g_pGameInfoManager->RemoveOtherPlayerByID(ID);
			break;
		}
	}
}
