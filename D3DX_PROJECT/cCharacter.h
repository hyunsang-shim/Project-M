#pragma once
#include "cObject.h"

class cMyCharacter;
class cSkinnedMesh;

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

	POINT m_ptPrevMouse;



public:
	virtual void SetUP();
	virtual void Update(cMyCharacter* m_MyCharacter, cSkinnedMesh* m_SkinnedMesh);
	virtual void Update(float ROTY, D3DXVECTOR3 POSITION);
	virtual void Render();
	virtual D3DXVECTOR3& GetPosition();
	void SetPositionY(float y);

	virtual D3DXMATRIXA16* GetTransform()
	{
		return &m_matWorld;
	}
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

