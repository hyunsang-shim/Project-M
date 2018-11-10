#pragma once
#define g_pOtherPlayerManager cOtherPlayerManager::GetInstance()
#define OtherPlayer g_pOtherPlayerManager->otherPlayerInfo 
#include "cOtherCharacter.h"

class cOtherPlayerManager
{
public:

	SINGLETON(cOtherPlayerManager);

	int userNum;
	vector<cOtherCharacter*> otherPlayerInfo;

	void newPlayer(UserInfo info);
	void render();

};

