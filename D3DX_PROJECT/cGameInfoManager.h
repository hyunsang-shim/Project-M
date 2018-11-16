#pragma once
#define g_pGameInfoManager cGameInfoManager::GetInstance()

class cNewObject;

class cGameInfoManager
{
public:
	SINGLETON(cGameInfoManager);

	BOOL isESCPushed;
	int mouseMoveX;
	int mouseMoveY;
	bool canGo;

	cNewObject* m_pMap;

	void setup_Map(string filePath);

	int MaxBulletCount;
};

