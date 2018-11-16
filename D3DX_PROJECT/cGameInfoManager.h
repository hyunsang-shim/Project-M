#pragma once
#define g_pGameInfoManager cGameInfoManager::GetInstance()
#include "stdafx.h"

class cNewObject;

class cGameInfoManager
{
public:
	SINGLETON(cGameInfoManager);

	int GameScreenSizeX;
	int GameScreenSizeY;
	int getScreenXPosByPer(int x);
	int getScreenYPosByPer(int y);

	char userName[25] = { 0 };
	int namelength;

	BOOL isESCPushed;
	int mouseMoveX;
	int mouseMoveY;
	bool canGo;

	cNewObject* m_pMap;

	void setup_Map(string filePath);

	BOOL sceneChangeTriger;
	int nextScene;

	// Network
	// >>
	void UpdateMyInfo(CharacterStatus_PC &newInfo);
	void UpdateOtherPlayers(vector<CharacterStatus_PC> &othersInfo);
	void UpdateNPCs(vector<CharacterStatus_NPC> &npcInfo);
	CharacterStatus_PC* GetMyInfo();
	vector<CharacterStatus_PC>* GetOthersInfo();
	vector<CharacterStatus_NPC>* GetNpcsInfo();
	void SetMyCharacter(int idx);
	void SetMyName();

	// <<

private:
	CharacterStatus_PC m_strMyCharacter;
	vector<CharacterStatus_PC> m_vOtherCharacters;
	vector<CharacterStatus_NPC> m_vNpcCharacters;
};

