#pragma once
#include "cCharacter.h"
#include "stdafx.h"

class cSkinnedMesh;
class cOBB;
class iMap;

class cOtherCharacter
{
public:
	cOtherCharacter();
	virtual ~cOtherCharacter();

private:
	cSkinnedMesh* m_pSkinnedMesh;
	cOBB* m_pOBB;
	SYNTHESIZE_ADD_REF(cCharacter*, m_pCharacterController, CharacterController);
	int status;

public:
	void Setup();
	void Update();
	void Update(D3DXVECTOR3 CurPos, float rotY, WORD Status);
	void Render(D3DCOLOR c = D3DCOLOR_XRGB(255, 255, 255));
	cOBB* GetOBB();
	D3DXVECTOR3 GetPosition();
	void SetAnimationIndexBlend(int nIndex);
	CharacterStatus_PC info;

};
