#include "stdafx.h"
#include "cGameInfoManager.h"
#include "cNewObject.h"


cGameInfoManager::cGameInfoManager() :
	isESCPushed(0),
	mouseMoveX(0),
	mouseMoveY(0),
	canGo(1),
	m_pMap(NULL),
	MaxBulletCount(30)
{
}

cGameInfoManager::~cGameInfoManager()
{
	SAFE_DELETE(m_pMap);
}

void cGameInfoManager::setup_Map(string filePath)
{
	SAFE_DELETE(m_pMap);
	m_pMap = new cNewObject;
	m_pMap->Setup("test_map_obj.obj");

}
