#pragma once
#define g_pGameInfoManager cGameInfoManager::GetInstance()


class cGameInfoManager
{
public:
	SINGLETON(cGameInfoManager);

	BOOL isESCPushed;
	int mouseMoveX;
	int mouseMoveY;
};

