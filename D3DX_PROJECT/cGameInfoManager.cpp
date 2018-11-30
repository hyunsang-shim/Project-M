#include "stdafx.h"
#include "cGameInfoManager.h"
#include "cNewObject.h"


cGameInfoManager::cGameInfoManager() :
	isESCPushed(0),
	mouseMoveX(0),
	mouseMoveY(0),
	canGo(1),
	m_pMap(NULL),
	GameScreenSizeX(1366),
	GameScreenSizeY(768),
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
		m_strMyCharacter.Status = CS_IDLE;
		break;
	}
}

void cGameInfoManager::SetMyName()
{
	strcpy(m_strMyCharacter.PlayerName, userName);
}

int cGameInfoManager::GetNumTotalUser()
{
	return m_vOtherCharacters.size();
}

void cGameInfoManager::AddOtherPlayer(CharacterStatus_PC * info)
{
	CharacterStatus_PC tmp = *info;
	m_vOtherCharacters.push_back(tmp);	
}

void cGameInfoManager::RemoveOtherPlayerByID(int ID)
{
	for (int i = 0; i < m_vOtherCharacters.size(); i++)
	{
		if (m_vOtherCharacters[i].ID == ID)
		{
			m_vOtherCharacters.erase(m_vOtherCharacters.begin() + i);
			break;
		}
	}
}
