#pragma once
#define g_pGameInfoManager cGameInfoManager::GetInstance()

class cNewObject;

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

	void setup_Map(string filePath);

	BOOL sceneChangeTriger;
	int nextScene;

	float aimSize;
};

