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
	m_vOtherCharacters(NULL),
	m_vNpcCharacters(NULL)
{
}


cGameInfoManager::~cGameInfoManager()
{
	SAFE_DELETE(m_pMap);
}

int cGameInfoManager::getScreenXPosByPer(int x)
{
	return (float)GameScreenSizeX / 100.0f*(float)x;
}

int cGameInfoManager::getScreenYPosByPer(int y)
{
	return  (float)GameScreenSizeY / 100.0f*(float)y;
}

void cGameInfoManager::setup_Map(string filePath)
{
	SAFE_DELETE(m_pMap);
	m_pMap = new cNewObject;
	m_pMap->Setup("test_map_obj.obj");

}

void cGameInfoManager::UpdateMyInfo(CharacterStatus_PC newInfo)
{
	m_strMyCharacter = newInfo;
}

void cGameInfoManager::UpdateOtherPlayers(vector<CharacterStatus_PC> othersInfo)
{
	m_vOtherCharacters = othersInfo;
}

void cGameInfoManager::UpdateNPCs(vector<CharacterStatus_NPC> npcInfo)
{
	m_vNpcCharacters = npcInfo;
}

CharacterStatus_PC cGameInfoManager::GetMyInfo()
{
	return m_strMyCharacter;
}

vector<CharacterStatus_PC> cGameInfoManager::GetOthersInfo()
{
	return m_vOtherCharacters;
}

vector<CharacterStatus_NPC> cGameInfoManager::GetNpcsInfo()
{
	return m_vNpcCharacters;
}

void cGameInfoManager::SetMyName()
{
	strcpy(m_strMyCharacter.PlayerName, userName);
}
