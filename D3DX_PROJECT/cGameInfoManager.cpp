#include "stdafx.h"
#include "cGameInfoManager.h"
#include "cNewObject.h"
#include "cXModel.h"
#include "cXModelSurface.h"
#include "cAI.h"
#include "cOBB.h"

#include "fmod.hpp"
#include "fmod_errors.h"

using namespace FMOD;

System* pSystem;
Sound* pSound[100]; //sound num setting
Channel* pChannel[100];

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
	m_vNpcCharacters(NULL),  
	MaxBulletCount(30),
	loading(0),
	monsterTriggerBoxNum(0),
	timer(9999)
{
	System_Create(&pSystem);
	pSystem->init(100, FMOD_INIT_NORMAL, NULL); // 4 is max channel
}


cGameInfoManager::~cGameInfoManager()
{
	destroy();
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



void cGameInfoManager::UpdateNPCs(vector<CharacterStatus_NPC>& npcInfo)
{
	memcpy(&m_vNpcCharacters, (vector<CharacterStatus_NPC>*)&npcInfo, sizeof(vector<CharacterStatus_NPC>));
}

CharacterStatus_PC* cGameInfoManager::GetMyInfo()
{
	return &m_strMyCharacter;
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
		m_strMyCharacter.MaxHP = 150;
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

void cGameInfoManager::AddOtherPlayer(CharacterStatus_PC info)
{
	CharacterStatus_PC tmp = info;
	m_vOtherCharacters.push_back(tmp);
}

void cGameInfoManager::SoundSystem()
{
	// 0~9 bgm , other sound
	pSystem->createStream("sound/bgms/loadings.mp3", FMOD_LOOP_NORMAL, NULL, &pSound[0]); // loading
	pSystem->createSound("sound/bgms/main_bgm.mp3", FMOD_LOOP_NORMAL, NULL, &pSound[1]); //main
	pSystem->createStream("sound/bgms/rialto_bgm.mp3", FMOD_LOOP_NORMAL, NULL, &pSound[2]); //main


																						 // 10~19 UI sound
	pSystem->createSound("sound/chr/sd.mp3", FMOD_DEFAULT, NULL, &pSound[10]);


	pSystem->createSound("sound/Effectsounds/click.mp3", FMOD_DEFAULT, NULL, &pSound[19]);


	// 20~29 skill sound
	pSystem->createSound("sound/Effectsounds/heal.mp3", FMOD_DEFAULT, NULL, &pSound[20]);



	//30~99 effect and null sound
	pSystem->createSound("sound/Effectsounds/pl_gunsound.mp3", FMOD_DEFAULT, NULL, &pSound[30]);
	pSystem->createSound("sound/Effectsounds/moves.mp3", FMOD_LOOP_NORMAL, NULL, &pSound[31]);
	pSystem->createSound("sound/Effectsounds/bullet_re.mp3", FMOD_DEFAULT, NULL, &pSound[32]);
	pSystem->createSound("sound/Effectsounds/pl_damege.mp3", FMOD_DEFAULT, NULL, &pSound[33]);

	//pSystem->createSound("sound/gunsound.wav", FMOD_DEFAULT, NULL, &pSound[track_num]);
}

void cGameInfoManager::Play(int Sound_num, int channel_num)
{

	pSystem->playSound(pSound[Sound_num], NULL, 0, &pChannel[channel_num]);
}

void cGameInfoManager::Stop(int channel_num)
{
	pChannel[channel_num]->stop();
}

void cGameInfoManager::channel_volum_set(int channel, float volum)
{
	pChannel[channel]->setVolume(volum);
}

void cGameInfoManager::destroy()
{
	if (pSound)
		for (int i = 0; i < 100; i++)
			pSound[i]->release();
	if (pSystem)
	{
		pSystem->release();
		pSystem->close();
	}
}

void cGameInfoManager::music_update()
{
	if (!pSystem)
		pSystem->update();
}

void cGameInfoManager::music_speed_set(int Sound_num, float speed)
{
	pSound[Sound_num]->setMusicSpeed(speed);
}

void cGameInfoManager::sound_infinity_set(int sound_track_nums, int sound_repeat_nums)
{
	pSound[sound_track_nums]->setLoopCount(sound_repeat_nums);
}

void cGameInfoManager::AddNPC(cAI * NPC)
{
	CharacterStatus_NPC tmp;
	strcpy(tmp.MsgHeader, "idle");
	tmp.ID = m_vNpcCharacters.size();

	tmp.CharacterName[16];
	tmp.Character_No = m_vNpcCharacters.size();;
	tmp.Attack = 1;
	tmp.MaxHP = 50;
	tmp.CurHP = 50;
	tmp.HP_Regen = 0;
	tmp.MoveSpeed = 100;
	tmp.Mag_Cnt = 0;
	tmp.Mag_Max = 0;
	tmp.ShootSpeed = 0;
	tmp.BulletTime = 0;
	tmp.CurPos = D3DXVECTOR3(0, 0, 0);
	tmp.Dir = 0.0f;
	tmp.Status = CS_IDLE;
	tmp.TargetID = NULL;
	m_vNpcCharacters.push_back(tmp);
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

