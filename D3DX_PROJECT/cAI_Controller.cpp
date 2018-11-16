#include "stdafx.h"
#include "cAI_Controller.h"
#include "cAI.h"
#include "cNewObject.h"
#include "cSkinnedMesh.h"

cAI_Controller::cAI_Controller()
	:m_fRotY(0.0f)
	, m_vDirection(0, 0, 0)
	, m_vPosition(50, 0, 15)
{
	D3DXMatrixIdentity(&m_matWorld);
}


cAI_Controller::~cAI_Controller()
{
}

void cAI_Controller::SetUP()
{
	
}

void cAI_Controller::Update(cAI * m_AI, bool b, D3DXVECTOR3 moveToCharacterDir, cSkinnedMesh* m_SkinnedMesh)
{
	static int CurrentAnimNum = 9;
	static int beforeAnimNum = 0;
	static double TotalPeriod = 0.0;
	static double CurrentPeriod = 0.0;
	int AttackTime = 0;
	static int AttackCoolTime = 0;
	bool OnlyLeftOrRight = true;
	D3DXVECTOR3 m_vLeftDirection;
	D3DXVECTOR3 m_vUp(0, 1, 0);
	D3DXVec3Cross(&m_vLeftDirection, &m_vUp, &m_vDirection);


	AttackTime = GetTickCount();

	D3DXVECTOR3 m_vBeforePosition = m_vPosition;
	
	float AI_To_Distance = D3DXVec3Length(&moveToCharacterDir);



	if (AttackTime - AttackCoolTime < 2000)
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
			CurrentAnimNum = 3;
			D3DXVec3Normalize(&moveToCharacterDir, &moveToCharacterDir);
			m_vPosition += moveToCharacterDir / 10;
		}
		else
		{
			CurrentAnimNum = 7;
			D3DXVec3Normalize(&moveToCharacterDir, &moveToCharacterDir);
			AttackCoolTime = AttackTime;
		}
	}
	else
	{
		if (TotalPeriod <= CurrentPeriod + 0.05)
		{
			CurrentAnimNum = 9;
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


	D3DXMATRIXA16 matR, matT;
	if (b)
	{
		m_fRotY = -acos(moveToCharacterDir.z);
	}
	D3DXMatrixRotationY(&matR, m_fRotY);
	m_vDirection = D3DXVECTOR3(0, 0, 1);

	D3DXVec3TransformNormal(&m_vDirection, &m_vDirection, &matR);
	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	m_matWorld = matR * matT;
}


void cAI_Controller::Update(float ROTY, D3DXVECTOR3 POSITION)
{
	D3DXMATRIXA16 matR, matT;
	m_vPosition = POSITION;
	D3DXMatrixRotationY(&matR, m_fRotY);

	m_vDirection = D3DXVECTOR3(0, 0, 1);

	D3DXVec3TransformNormal(&m_vDirection, &m_vDirection, &matR);
	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	m_matWorld = matR * matT;
}

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