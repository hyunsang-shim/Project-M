#pragma once
#include "cCharacter.h"
class cSkinnedMesh;
class cOBB;
class iMap;

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
	Hit,
	Down,
	Sleep,
	Stand_Up,
	Dead,
	NumSize
};

struct CharacterStatus_PC
{
	WORD			ID;				// ���� ID
	char			PlayerName[16];	// �����̸�
	WORD			Character_No;	// ĳ���� ����
	WORD			Attack;			// ���·�
	DWORD			MaxHP;			// �ִ� ü��
	DWORD			CurHP;			// ���� ü��
	WORD			HP_Regen;		// ü�� ���
	DWORD			MoveSpeed;		// �̵� �ӵ�
	WORD			Mag_Size;		// ��ź ��
	WORD			MaxMag;			// �ִ� ���� ��
	DWORD			ShootSpeed;		// ����ӵ�
	WORD			BulletTime;		// �Ѿ� �ӵ�
	D3DXVECTOR3		CurPos;			// ���� ��ġ��
	D3DXVECTOR3		Dir;				// ĳ���Ͱ� �ٶ󺸴� ����
	WORD			Status;
};

struct CharacterStatus_NPC
{
	WORD			ID;				// ���� ID
	char			MonsterName[16];	// �����̸�
	WORD			Character_No;	// ĳ���� ����
	WORD			Attack;			// ���·�
	DWORD			MaxHP;			// �ִ� ü��
	DWORD			CurHP;			// ���� ü��
	WORD			HP_Regen;		// ü�� ���
	DWORD			MoveSpeed;		// �̵� �ӵ�
	WORD			Mag_Size;		// ��ź ��
	WORD			MaxMag;			// �ִ� ���� ��
	DWORD			ShootSpeed;		// ����ӵ�
	WORD			BulletTime;		// �Ѿ� �ӵ�
	D3DXVECTOR3		CurPos;			// ���� ��ġ��
	D3DXVECTOR3		Dir;				// ĳ���Ͱ� �ٶ󺸴� ����
	WORD			Status;
};

class cMyCharacter
{
public:
	cMyCharacter();
	virtual ~cMyCharacter();

private:
	cSkinnedMesh* m_pSkinnedMesh;
	cOBB* m_pOBB;	
	SYNTHESIZE_ADD_REF(cCharacter*, m_pCharacterController, CharacterController);
	SYNTHESIZE(D3DXVECTOR3, m_vBulletPos, BulletPos);
public:
	void Setup(char * szFolder, char * szFileName);
	void Update(D3DXVECTOR3 camDirection);
	void Render(D3DCOLOR c = D3DCOLOR_XRGB(255, 255, 255));
	cOBB* GetOBB();
	D3DXVECTOR3 GetPosition();
	void SetAnimationIndexBlend(int nIndex);
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);	
	string sendData();
	D3DXVECTOR3 GetMyHeadPos();	// �Ӹ� ��ǥ ���ϱ� (����Ʈ ��)
};

