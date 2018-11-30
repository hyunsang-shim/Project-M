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
