#pragma once

#include "cCharacter.h"
class cSkinnedMesh;
class cOBB;
class iMap;

class cMyCharacter
{
public:
	cMyCharacter();
	virtual ~cMyCharacter();

private:
	cSkinnedMesh* m_pSkinnedMesh;
	cOBB* m_pOBB;
	SYNTHESIZE_ADD_REF(cCharacter*, m_pCharacterController, CharacterController);
	

public:
	void Setup(char * szFolder, char * szFileName);
	void Update(D3DXVECTOR3 camDirection);
	void Render(D3DCOLOR c = D3DCOLOR_XRGB(255, 255, 255));
	cOBB* GetOBB();
	D3DXVECTOR3 GetPosition();
	void SetAnimationIndexBlend(int nIndex);
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

