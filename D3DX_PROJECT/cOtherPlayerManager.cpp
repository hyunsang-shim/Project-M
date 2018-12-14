#include "stdafx.h"
#include "cOtherPlayerManager.h"


cOtherPlayerManager::cOtherPlayerManager()
{
}


cOtherPlayerManager::~cOtherPlayerManager()
{
}

cOtherCharacter* cOtherPlayerManager::GetOtherPlayerByID(int idx)
{
	for (int i = 0; i < otherPlayerInfo.size(); i++)
	{
		if (otherPlayerInfo.at(i)->info.ID == idx)
		{
			return otherPlayerInfo.at(i);
		}
	}
	
}


void cOtherPlayerManager::newPlayer(CharacterStatus_PC info)
{
 	cOtherCharacter* tmp  = new cOtherCharacter;
	cCharacter* pCharacter = new cCharacter;
	otherPlayerInfo.push_back(tmp);
	otherPlayerInfo.back()->Setup();
	otherPlayerInfo.back()->SetCharacterController(pCharacter);
	otherPlayerInfo.back()->info = info;
}

void cOtherPlayerManager::render()
{
	/*for (int i = 0; i < otherPlayerInfo.size(); i++)
	{
		otherPlayerInfo.at(i)->Render();
	}*/
}

void cOtherPlayerManager::update()
{
	for (int i = 0; i < otherPlayerInfo.size(); i++)
	{
		otherPlayerInfo.at(i)->Update();
	}
}

void cOtherPlayerManager::disconnectPlayer(int ID)
{
	for (int i = 0; i < otherPlayerInfo.size(); i++)
	{
		if (otherPlayerInfo.at(i)->info.ID == ID)
		{
			SAFE_DELETE(otherPlayerInfo.at(i));
			otherPlayerInfo.erase(otherPlayerInfo.begin() + i);
			break;
		}
	}
}
