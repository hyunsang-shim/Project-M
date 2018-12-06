#include "stdafx.h"
#include "cAI_Controller.h"
#include "cAI.h"
#include "cNewObject.h"
#include "cSkinnedMesh.h"
#include "cXModelSurface.h"

cAI_Controller::cAI_Controller()
	:m_fRotY(0.0f)
	, m_vDirection(0, 0, 1)
	, m_vPosition(50, 0, 15)
{
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&RotateToCharacter);
	CurrentAnimNum = 0;
	beforeAnimNum = 0;
	TotalPeriod = 0.0;
	CurrentPeriod = 0.0;
	AttackCoolTime = 0;
	BeforeTime = 0;
}


cAI_Controller::~cAI_Controller()
{
}

void cAI_Controller::SetUP()
{

}

void cAI_Controller::Update(cAI * m_AI, bool b, D3DXVECTOR3 moveToCharacterDir, cSkinnedMesh* m_SkinnedMesh)
{
	int AttackTime = 0;
	bool OnlyLeftOrRight = true;
	D3DXVECTOR3 m_vLeftDirection;
	D3DXVECTOR3 m_vUp(0, 1, 0);
	//D3DXVec3Cross(&m_vLeftDirection, &m_vUp, &m_vDirection);

	AttackTime = GetTickCount();

	D3DXVECTOR3 m_vBeforePosition = m_vPosition;

	float AI_To_Distance = D3DXVec3Length(&moveToCharacterDir);



	if (AttackTime - AttackCoolTime < 5000)
	{
		b = false;

		LPD3DXANIMATIONSET pCurrentAnimSet = NULL;
		D3DXTRACK_DESC stTrackDesc;

		m_SkinnedMesh->GetAnimController()->GetTrackDesc(0, &stTrackDesc);
		m_SkinnedMesh->GetAnimController()->GetAnimationSet(CurrentAnimNum, &pCurrentAnimSet);

		CurrentPeriod = pCurrentAnimSet->GetPeriodicPosition(stTrackDesc.Position);
		TotalPeriod = pCurrentAnimSet->GetPeriod();
	}

	if (b)
	{
		if (AI_To_Distance > 4.0f)
		{
			CurrentAnimNum = 6;
			D3DXVec3Normalize(&moveToCharacterDir, &moveToCharacterDir);
			m_vPosition += moveToCharacterDir / 10;
		}
		else
		{
			CurrentAnimNum = 2;
			D3DXVec3Normalize(&moveToCharacterDir, &moveToCharacterDir);
			AttackCoolTime = AttackTime;
		}
	}
	else
	{
		if (TotalPeriod <= CurrentPeriod + 0.05)
		{
			CurrentAnimNum = 0;
		}
	}

	if (beforeAnimNum != CurrentAnimNum)
	{
		beforeAnimNum = CurrentAnimNum;
		m_AI->SetAnimationIndexBlend(beforeAnimNum);
	}

	if (g_pGameInfoManager->m_pMap)
	{
		float y = 0;
		D3DXVECTOR3 head = m_vPosition + D3DXVECTOR3(0, 2, 0);
		if (!g_pGameInfoManager->m_pMap->GetY(m_vPosition.x, y, m_vPosition.z, head))
			m_vPosition = m_vBeforePosition;
		else if (y - m_vPosition.y > 0.7)
		{

		}
		else
		{
			if (m_vPosition.y - y > 0.3)
				m_vPosition.y -= 0.3f;
			else
				m_vPosition.y = y;
		}
	}
	
	CurrentTime = GetTickCount();

	if (g_pGameInfoManager->m_pSXMap && CurrentTime - BeforeTime > 350)
	{
		BeforeTime = CurrentTime;
		float y = 0;
		D3DXVECTOR3 head = m_vPosition + D3DXVECTOR3(0, 2, 0);
		if (!g_pGameInfoManager->m_pSXMap->GetAIY(m_vPosition.x, y, m_vPosition.z, head))
			m_vPosition = m_vBeforePosition;
		else
		{
			if (m_vPosition.y - y > 0.3)
				m_vPosition.y -= 0.3f;
			else
				m_vPosition.y = y;
		}
	}

	D3DXMATRIXA16 matT;


	if (b)
	{
		D3DXVECTOR3 tmpVec = moveToCharacterDir;
		tmpVec.y = 0;
		D3DXVec3Normalize(&tmpVec, &tmpVec);
		float side = pow(tmpVec.x*tmpVec.x + tmpVec.z * tmpVec.z, 0.5);

		RotateToCharacter._11 = tmpVec.z / side;
		RotateToCharacter._13 = -tmpVec.x / side;
		RotateToCharacter._31 = tmpVec.x / side;
		RotateToCharacter._33 = tmpVec.z / side;
	}


	m_vDirection = D3DXVECTOR3(0, 0, 1);
	D3DXVec3TransformNormal(&m_vDirection, &m_vDirection, &RotateToCharacter);
	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	m_matWorld = RotateToCharacter * matT;
}


//void cAI_Controller::Update(float ROTY, D3DXVECTOR3 POSITION)
//{
//	D3DXMATRIXA16 matR, matT;
//	m_vPosition = POSITION;
//	D3DXMatrixRotationY(&matR, m_fRotY);
//
//	m_vDirection = D3DXVECTOR3(0, 0, 1);
//
//	D3DXVec3TransformNormal(&m_vDirection, &m_vDirection, &matR);
//	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
//	m_matWorld = matR * matT;
//}

void cAI_Controller::Render()
{

}

D3DXVECTOR3 & cAI_Controller::GetPosition()
{
	return this->m_vPosition;
}

void cAI_Controller::SetPositionY(float y)
{
	this->m_vPosition.y = y;
}

void cAI_Controller::SetPosition(D3DXVECTOR3 pos)
{
	m_vPosition = pos;
}
