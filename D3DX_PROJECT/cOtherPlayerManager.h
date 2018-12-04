#pragma once
#define g_pOtherPlayerManager cOtherPlayerManager::GetInstance()
#define OtherPlayer g_pOtherPlayerManager->otherPlayerInfo 
#include "cOtherCharacter.h"

class cOtherPlayerManager
{
public:

	SINGLETON(cOtherPlayerManager);

	int userNum;
	vector<cOtherCharacter> otherPlayerInfo;
	CharacterStatus_PC* GetOtherPlayerByID(int idx);


	void newPlayer(CharacterStatus_PC info);
	void render();
	void update();
	void disconnectPlayer(int idx);
};