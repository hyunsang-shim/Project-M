#pragma once

#include "resource.h"

enum character_status {
	CS_IDLE,
	CS_FRONT,
	CS_FRONT_LEFT,
	CS_FRONT_RIGHT,
	CS_FRONT_LEFT_SHIFT,
	CS_FRONT_RIGHT_SHIFT,
	CS_LEFT,
	CS_RIGHT,
	CS_BACK,
	CS_BACK_LEFT,
	CS_BACK_RIGHT,
	CS_RELOAD,
	CS_DOWN,
	CS_DEAD
};


struct CharacterStatus_PC
{
	char			MsgHeader[64];			// 메시지 헤더
	int			ID;				// 세션 ID	
	char			PlayerName[16];	// 유저이름
	int			Character_No;	// 캐릭터 종류
	int			Attack;			// 공력력
	int			MaxHP;			// 최대 체력
	int			CurHP;			// 현재 체력
	int			HP_Regen;		// 체력 재생
	int			MoveSpeed;		// 이동 속도
	int			Mag_Cnt;		// 장탄 수
	int			Mag_Max;			// 최대 장전 수
	int			ShootSpeed;		// 연사속도
	int			BulletTime;		// 총알 속도
	D3DXVECTOR3		CurPos;			// 현재 위치값
	float			Dir;				// 캐릭터가 바라보는 방향
	int			Status;			// 캐릭터 상태
	int				TargetID;		// 공격 한 대상
	int				FailCnt;		// 접속 여부
	SOCKET			s;				// 소켓
};

struct CharacterStatus_NPC
{
	char			MsgHeader[64];			// 메시지 헤더
	int			ID;				// 세션 ID	
	char			CharacterName[16];	// 캐릭터 이름
	int			Character_No;	// 캐릭터 종류
	int			Attack;			// 공력력
	int			MaxHP;			// 최대 체력
	int			CurHP;			// 현재 체력
	int			HP_Regen;		// 체력 재생
	int			MoveSpeed;		// 이동 속도
	int			Mag_Cnt;		// 장탄 수
	int			Mag_Max;		// 최대 장전 수
	int			ShootSpeed;		// 연사속도
	int			BulletTime;		// 총알 속도
	D3DXVECTOR3		CurPos;			// 현재 위치값
	float			Dir;			// 캐릭터가 바라보는 방향
	int			Status;			// 캐릭터 상태
	int				TargetID;		// 공격 한 대상
};
