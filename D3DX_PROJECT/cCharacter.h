#pragma once
#include "cObject.h"

class cMyCharacter;
class cSkinnedMesh;
class cOtherCharacter;


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
	
	D3DXMATRIXA16 m_matR;
	D3DXMATRIXA16 m_matWorld;




public:
	D3DXVECTOR3 m_vPosition;
	virtual void SetUP();
	virtual void UpdateOtherPlayer(D3DXVECTOR3 CurPos, float Direction, WORD status);
	void UpdateOtherPlayer(int status, cOtherCharacter* m_OtherCharacter);
	virtual void Update(cMyCharacter* m_MyCharacter, cSkinnedMesh* m_SkinnedMesh);
	virtual void Update(float ROTY, D3DXVECTOR3 POSITION);
	virtual void Render();
	virtual D3DXVECTOR3 GetPosition();
	void SetPositionY(float y);
	CharacterStatus_PC getUserData();
	virtual D3DXMATRIXA16* GetTransform()
	{
		return &m_matWorld;
	}
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


	BOOL KEY_W;
	BOOL KEY_A;
	BOOL KEY_S;
	BOOL KEY_D;
	BOOL KEY_SHIFT;




};

