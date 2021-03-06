#pragma once
#include "cObject.h"

class cAI;
class cSkinnedMesh;

class cAI_Controller :public cObject
{
public:
	cAI_Controller();
	virtual ~cAI_Controller(void);

	int target;

protected:
	float m_fRotY;
	float m_fRotX;
	float fDeltaX;
	float fDeltaY;
	D3DXVECTOR3 m_vDirection;
	D3DXVECTOR3 m_vPosition;

	D3DXMATRIXA16 m_matWorld;

	float angle;

	D3DXMATRIXA16 RotateToCharacter;

	int CurrentAnimNum;
	int beforeAnimNum;
	double TotalPeriod;
	double CurrentPeriod;
	int AttackCoolTime;

	DWORD CurrentTime;
	DWORD BeforeTime;



public:
	virtual void SetUP();

	virtual void Update(cAI* m_AI, bool b, D3DXVECTOR3 moveToCharacterDir, cSkinnedMesh* m_SkinnedMesh);
	//virtual void Update(float ROTY, D3DXVECTOR3 POSITION);
	virtual void Render();
	virtual D3DXVECTOR3& GetPosition();
	void SetPositionY(float y);
	void SetPosition(D3DXVECTOR3 pos);
	virtual D3DXMATRIXA16* GetTransform()
	{
		return &m_matWorld;
	}
};