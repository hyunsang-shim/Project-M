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
	WORD			Mag_Size;		// 장탄 수
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
	D3DXVECTOR3 GetMyHeadPos();	// 머리 좌표 구하기 (하이트 맵)
};

