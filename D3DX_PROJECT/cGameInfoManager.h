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

	///// Sound



	void SoundSystem();
	void Play(int Sound_num, int channel_num);
	void Stop(int channel_num);
	void channel_volum_set(int channel, float volum);

	void destroy();
	void music_update();
	void music_speed_set(int Sound_num, float speed);

	void sound_infinity_set(int sound_track_nums, int sound_repeat_nums);


private:


};

