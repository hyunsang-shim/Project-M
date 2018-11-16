#pragma once
#include "cAI_Controller.h"
class cSkinnedMesh;
class cOBB;
class iMap;

class cAI
{
public:
	cAI();
	virtual ~cAI();
private:
	cSkinnedMesh* m_pSkinnedMesh;
	cOBB* m_pOBB;
	SYNTHESIZE_ADD_REF(cAI_Controller*, m_pAIController, AIController);
	SYNTHESIZE(D3DXVECTOR3, m_vBulletPos, BulletPos);
public:
	void Setup(char * szFolder, char * szFileName);
	void Update(bool b, D3DXVECTOR3 moveToCharacterDir);
	void Render(D3DCOLOR c = D3DCOLOR_XRGB(255, 255, 255));
	cOBB* GetOBB();
	D3DXVECTOR3 GetPosition();
	void SetAnimationIndexBlend(int nIndex);
};

