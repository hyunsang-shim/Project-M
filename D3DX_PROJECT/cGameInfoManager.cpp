#include "stdafx.h"
#include "cGameInfoManager.h"
#include "cNewObject.h"


cGameInfoManager::cGameInfoManager() :
	isESCPushed(0),
	mouseMoveX(0),
	mouseMoveY(0),
	canGo(1),
	m_pMap(NULL),
	GameScreenSizeX(1920),
	GameScreenSizeY(1080),
	namelength(0), 
	sceneChangeTriger(FALSE),
	nextScene(0),
	aimSize(0.0f)
{
}


cGameInfoManager::~cGameInfoManager()
{
	SAFE_DELETE(m_pMap);
}

int cGameInfoManager::getScreenXPosByPer(float x)
{
	return (float)GameScreenSizeX / 100.0f*(float)x;
}

int cGameInfoManager::getScreenYPosByPer(float y)
{
	return  (float)GameScreenSizeY / 100.0f*(float)y;
}

void cGameInfoManager::setup_Map(string filePath)
{
	SAFE_DELETE(m_pMap);
	m_pMap = new cNewObject;
	m_pMap->Setup("test_map_obj.obj");

}
