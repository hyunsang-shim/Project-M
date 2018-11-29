#include "stdafx.h"
#include "cGameInfoManager.h"
#include "cNewObject.h"
#include "cXModel.h"
#include "cXModelSurface.h"

cGameInfoManager::cGameInfoManager() :
	isESCPushed(0),
	mouseMoveX(0),
	mouseMoveY(0),
	canGo(1),
	m_pMap(NULL),
	m_pXMap(NULL),
	GameScreenSizeX(1920),
	GameScreenSizeY(1080),
	namelength(0), 
	sceneChangeTriger(FALSE),
	nextScene(0),
	aimSize(0.0f),
	m_vOtherCharacters(NULL),
	m_vNpcCharacters(NULL),  
	MaxBulletCount(30),
	loading(0)
{
}


cGameInfoManager::~cGameInfoManager()
{
	SAFE_DELETE(m_pMap);
	SAFE_DELETE(m_pXMap);
}

int cGameInfoManager::getScreenXPosByPer(float x)
{
	return (float)GameScreenSizeX / 100.0f*(float)x;
}

int cGameInfoManager::getScreenYPosByPer(float y)
{
	return  (float)GameScreenSizeY / 100.0f*(float)y;
}

void cGameInfoManager::setup_Map(char* Path, char* Filename)
{
	SAFE_DELETE(m_pMap);
	m_pMap = new cNewObject;
	m_pMap->Setup(Path, Filename);

}

void cGameInfoManager::setup_XMap(string filePath)
{
	m_pXMap = new cXModel(filePath);
}

void cGameInfoManager::setup_SXMap(string filePath)
{
	m_pSXMap = new cXModelSurface(filePath);
}

void cGameInfoManager::UpdateMyInfo(CharacterStatus_PC &newInfo)
{
	memcpy(&m_strMyCharacter, (CharacterStatus_PC*)&newInfo, sizeof(CharacterStatus_PC));
}

void cGameInfoManager::UpdateOtherPlayers(vector<CharacterStatus_PC>& othersInfo)
{
	memcpy(&m_vOtherCharacters, (vector<CharacterStatus_PC>*)&othersInfo, sizeof(vector<CharacterStatus_PC>));
}

void cGameInfoManager::UpdateNPCs(vector<CharacterStatus_NPC>& npcInfo)
{
	memcpy(&m_vNpcCharacters, (vector<CharacterStatus_NPC>*)&npcInfo, sizeof(vector<CharacterStatus_NPC>));
}

CharacterStatus_PC* cGameInfoManager::GetMyInfo()
{
	return &m_strMyCharacter;
}

vector<CharacterStatus_PC>* cGameInfoManager::GetOthersInfo()
{
	return &m_vOtherCharacters;
}

vector<CharacterStatus_NPC>* cGameInfoManager::GetNpcsInfo()
{
	return &m_vNpcCharacters;
}

void cGameInfoManager::SetMyCharacter(int idx)
{
	switch (idx)
	{
	case PC_Soldier:
	default:
		m_strMyCharacter.BulletTime = 1000.0f;
		m_strMyCharacter.Attack = 10;
		m_strMyCharacter.Character_No = 0;
		m_strMyCharacter.CurHP = 100;
		m_strMyCharacter.MaxHP = 100;
		m_strMyCharacter.HP_Regen = 1;
		m_strMyCharacter.CurPos = D3DXVECTOR3(0, 0, 0);
		m_strMyCharacter.Dir = 0.0f;
		m_strMyCharacter.Mag_Cnt = 25;
		m_strMyCharacter.Mag_Max = 25;
		m_strMyCharacter.MoveSpeed = 2.5f;
		m_strMyCharacter.ShootSpeed = 4000.0f;
		m_strMyCharacter.Status = PC_Stand;
		break;
	}
}

void cGameInfoManager::SetMyName()
{
	strcpy(m_strMyCharacter.PlayerName, userName);
}