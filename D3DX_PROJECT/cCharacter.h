#pragma once
#include "cObject.h"

class cMyCharacter;
class cSkinnedMesh;

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
	WORD			ID;				// ���� ID	
	char			PlayerName[16];	// �����̸�
	WORD			Character_No;	// ĳ���� ����
	WORD			Attack;			// ���·�
	DWORD			MaxHP;			// �ִ� ü��
	DWORD			CurHP;			// ���� ü��
	WORD			HP_Regen;		// ü�� ���
	DWORD			MoveSpeed;		// �̵� �ӵ�
	WORD			Mag_Cnt;		// ��ź ��
	WORD			Mag_Max;			// �ִ� ���� ��
	DWORD			ShootSpeed;		// ����ӵ�
	WORD			BulletTime;		// �Ѿ� �ӵ�
	D3DXVECTOR3		CurPos;			// ���� ��ġ��
	float			Dir;				// ĳ���Ͱ� �ٶ󺸴� ����
	WORD			Status;			// ĳ���� ����
	int				TargetID;		// ���� �� ���
	int				FailCnt;		// ���� ����
	SOCKET			s;				// ����
};

struct CharacterStatus_NPC
{
	char			MsgHeader[64];			// �޽��� ���
	WORD			ID;				// ���� ID	
	char			CharacterName[16];	// ĳ���� �̸�
	WORD			Character_No;	// ĳ���� ����
	WORD			Attack;			// ���·�
	DWORD			MaxHP;			// �ִ� ü��
	DWORD			CurHP;			// ���� ü��
	WORD			HP_Regen;		// ü�� ���
	DWORD			MoveSpeed;		// �̵� �ӵ�
	WORD			Mag_Cnt;		// ��ź ��
	WORD			Mag_Max;		// �ִ� ���� ��
	DWORD			ShootSpeed;		// ����ӵ�
	WORD			BulletTime;		// �Ѿ� �ӵ�
	D3DXVECTOR3		CurPos;			// ���� ��ġ��
	float			Dir;			// ĳ���Ͱ� �ٶ󺸴� ����
	WORD			Status;			// ĳ���� ����
	int				TargetID;		// ���� �� ���
};

class cCharacter:public cObject
{
public:
	cCharacter();
	virtual ~cCharacter(void);

protected:
	float m_fRotY;
	float m_fRotX;
	float fDeltaX;
	float fDeltaY;
	D3DXVECTOR3 m_vDirection;
	D3DXVECTOR3 m_vPosition;
	
	D3DXMATRIXA16 m_matWorld;




public:
	virtual void SetUP();
	virtual void UpdateOtherPlayer(D3DXVECTOR3 CurPos, float Direction, WORD status);

	virtual void Update(cMyCharacter* m_MyCharacter, cSkinnedMesh* m_SkinnedMesh);
	virtual void Update(float ROTY, D3DXVECTOR3 POSITION);
	virtual void Render();
	virtual D3DXVECTOR3& GetPosition();
	void SetPositionY(float y);
	CharacterStatus_PC getUserData();
	virtual D3DXMATRIXA16* GetTransform()
	{
		return &m_matWorld;
	}
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

