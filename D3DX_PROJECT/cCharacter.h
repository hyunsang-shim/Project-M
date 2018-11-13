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
	Hit,
	Down,
	Down_idle,
	Stand_Up,
	Dead,
	NumSize
};

struct CharacterStatus_PC
{
	WORD			ID;				// 세션 ID
	char			PlayerName[16];	// 유저이름
	WORD			Character_No;	// 캐릭터 종류
	WORD			Attack;			// 공력력
	DWORD			MaxHP;			// 최대 체력
	DWORD			CurHP;			// 현재 체력
	WORD			HP_Regen;		// 체력 재생
	DWORD			MoveSpeed;		// 이동 속도
	WORD			MagCnt;		// 장탄 수
	WORD			MaxMag;			// 최대 장전 수
	DWORD			ShootSpeed;		// 연사속도
	WORD			BulletTime;		// 총알 속도
	D3DXVECTOR3		CurPos;			// 현재 위치값
	D3DXVECTOR3		Dir;				// 캐릭터가 바라보는 방향
	WORD			Status;
};

struct CharacterStatus_NPC
{
	WORD			ID;				// 세션 ID
	char			MonsterName[16];	// 유저이름
	WORD			Character_No;	// 캐릭터 종류
	WORD			Attack;			// 공력력
	DWORD			MaxHP;			// 최대 체력
	DWORD			CurHP;			// 현재 체력
	WORD			HP_Regen;		// 체력 재생
	DWORD			MoveSpeed;		// 이동 속도
	WORD			Mag_Size;		// 장탄 수
	WORD			MaxMag;			// 최대 장전 수
	DWORD			ShootSpeed;		// 연사속도
	WORD			BulletTime;		// 총알 속도
	D3DXVECTOR3		CurPos;			// 현재 위치값
	D3DXVECTOR3		Dir;				// 캐릭터가 바라보는 방향
	WORD			Status;
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
	virtual void UpdateOtherPlayer(float x, float y, float z, float degree, int action, int actionCount);

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

