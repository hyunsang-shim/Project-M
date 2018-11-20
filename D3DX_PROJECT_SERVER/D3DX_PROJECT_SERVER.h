#pragma once

#include "resource.h"
enum character_status {
	Stand,
	Stand_Shoot,
	Run_Front,
	Run_Front_Shoot,
	Run_Left,
	Run_Left_Shoot,
	Run_Right,
	Run_Right_Shoot,
	Run_Back,
	Run_Back_Shoot,
	Dash,
	Reload,
	Hit,
	Down,
	Down_idle,
	Stand_Up,
	Dead,
	NumSize
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
