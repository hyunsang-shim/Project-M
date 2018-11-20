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
	char			MsgHeader[64];			// �޽��� ���
	int			ID;				// ���� ID	
	char			PlayerName[16];	// �����̸�
	int			Character_No;	// ĳ���� ����
	int			Attack;			// ���·�
	int			MaxHP;			// �ִ� ü��
	int			CurHP;			// ���� ü��
	int			HP_Regen;		// ü�� ���
	int			MoveSpeed;		// �̵� �ӵ�
	int			Mag_Cnt;		// ��ź ��
	int			Mag_Max;			// �ִ� ���� ��
	int			ShootSpeed;		// ����ӵ�
	int			BulletTime;		// �Ѿ� �ӵ�
	D3DXVECTOR3		CurPos;			// ���� ��ġ��
	float			Dir;				// ĳ���Ͱ� �ٶ󺸴� ����
	int			Status;			// ĳ���� ����
	int				TargetID;		// ���� �� ���
	int				FailCnt;		// ���� ����
	SOCKET			s;				// ����
};

struct CharacterStatus_NPC
{
	char			MsgHeader[64];			// �޽��� ���
	int			ID;				// ���� ID	
	char			CharacterName[16];	// ĳ���� �̸�
	int			Character_No;	// ĳ���� ����
	int			Attack;			// ���·�
	int			MaxHP;			// �ִ� ü��
	int			CurHP;			// ���� ü��
	int			HP_Regen;		// ü�� ���
	int			MoveSpeed;		// �̵� �ӵ�
	int			Mag_Cnt;		// ��ź ��
	int			Mag_Max;		// �ִ� ���� ��
	int			ShootSpeed;		// ����ӵ�
	int			BulletTime;		// �Ѿ� �ӵ�
	D3DXVECTOR3		CurPos;			// ���� ��ġ��
	float			Dir;			// ĳ���Ͱ� �ٶ󺸴� ����
	int			Status;			// ĳ���� ����
	int				TargetID;		// ���� �� ���
};
