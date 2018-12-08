#pragma once
#define g_pGameInfoManager cGameInfoManager::GetInstance()
#include "stdafx.h"

class cNewObject;
class cXModel;
class cXModelSurface;
class cOBB;

class cGameInfoManager
{
public:
	SINGLETON(cGameInfoManager);

	int GameScreenSizeX;
	int GameScreenSizeY;
	int getScreenXPosByPer(float x);
	int getScreenYPosByPer(float y);

	char userName[25] = { 0 };
	int namelength;

	BOOL isESCPushed;
	int mouseMoveX;
	int mouseMoveY;
	bool canGo;

	cNewObject* m_pMap;
	cXModel* m_pXMap;
	cXModelSurface* m_pSXMap;

	void setup_Map(char* Path, char* Filename);
	void setup_XMap(string filePath);
	void setup_SXMap(string filePath);

	int MaxBulletCount;
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
	int GetNumTotalUser();
	void AddOtherPlayer(CharacterStatus_PC info);
	void RemoveOtherPlayerByID(int ID);

	vector<CharacterStatus_PC> m_vOtherCharacters;
	vector<CharacterStatus_NPC> m_vNpcCharacters;
	void AddNPC(cAI* NPC);

	// <<

	float aimSize;


	CharacterStatus_PC m_strMyCharacter;

	cSCENE_INGAME* m_pScene_Ingame;
	cSCENE_RESULT* m_pScene_Result;
	cSCENE_TITLE* m_pScene_Title;

	int loading;


private:


};

