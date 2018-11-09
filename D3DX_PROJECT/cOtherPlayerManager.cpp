#include "stdafx.h"
#include "cOtherPlayerManager.h"


cOtherPlayerManager::cOtherPlayerManager()
{
}


cOtherPlayerManager::~cOtherPlayerManager()
{
}

void cOtherPlayerManager::newPlayer(UserInfo info)
{
	cOtherCharacter *tmp = new cOtherCharacter;
	tmp->Setup();
	cCharacter* pCharacter = new cCharacter;
	tmp->SetCharacterController(pCharacter);
	tmp->info = info;
	otherPlayerInfo.push_back(tmp);


}

void cOtherPlayerManager::render()
{
	for (int i = 0; i < otherPlayerInfo.size(); i++)
	{
		otherPlayerInfo.at(i)->Render();
	}
}
