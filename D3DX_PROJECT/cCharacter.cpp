#include "stdafx.h"
#include "cCharacter.h"
#include "cMyCharacter.h"
#include "cSkinnedMesh.h"

cCharacter::cCharacter()
	:m_fRotY(0.0f)
	, m_vDirection(0, 0, 0)
	, m_vPosition(0, 0, 0)

{
	D3DXMatrixIdentity(&m_matWorld);
	m_ptPrevMouse.x = 0;
	m_ptPrevMouse.y = 0;
}


cCharacter::~cCharacter()
{
}

void cCharacter::SetUP()
{
}

void cCharacter::Update(cMyCharacter* m_MyCharacter, cSkinnedMesh* m_SkinnedMesh)
{
	static int CurrentAnimNum = 1;
	static int beforeAnimNum = 0;
	static double TotalPeriod = 0.0;
	static double CurrentPeriod = 0.0;
	bool OnlyLeftOrRight = true;
	D3DXVECTOR3 m_vLeftDirection;
	D3DXVECTOR3 m_vUp(0, 1, 0);
	D3DXVec3Cross(&m_vLeftDirection, &m_vUp, &m_vDirection);

	LPD3DXANIMATIONSET pCurrentAnimSet = NULL;
	D3DXTRACK_DESC stTrackDesc;

	m_SkinnedMesh->GetAnimController()->GetTrackDesc(0, &stTrackDesc);
	m_SkinnedMesh->GetAnimController()->GetAnimationSet(CurrentAnimNum, &pCurrentAnimSet);

	CurrentPeriod = pCurrentAnimSet->GetPeriodicPosition(stTrackDesc.Position);
	TotalPeriod = pCurrentAnimSet->GetPeriod();

	if (GetKeyState('R') & 0x8000)
	{
		CurrentAnimNum = 2;

		beforeAnimNum = CurrentAnimNum;

		m_MyCharacter->SetAnimationIndexBlend(beforeAnimNum);
	}
	else
	{
		if (GetKeyState('W') & 0x8000)
		{
			m_vPosition = m_vPosition + (m_vDirection * 0.1f);
			CurrentAnimNum = 6;
			if (GetKeyState(VK_SHIFT) & 0x8000)
			{
				m_vPosition = m_vPosition + (m_vDirection * 0.15f);

				if ((GetKeyState('A') & 0x8000) || (GetKeyState('D') & 0x8000))
				{
					OnlyLeftOrRight = false;
				}
			}
			if ((GetKeyState('A') & 0x8000) || (GetKeyState('D') & 0x8000))
			{
				OnlyLeftOrRight = false;
			}
		}
		else if (GetKeyState('S') & 0x8000)
		{
			m_vPosition = m_vPosition - (m_vDirection * 0.1f);
			CurrentAnimNum = 3;
			if ((GetKeyState('A') & 0x8000) || (GetKeyState('D') & 0x8000))
			{
				OnlyLeftOrRight = false;
			}
		}
		else
		{
			if (TotalPeriod <= CurrentPeriod + 0.1)
			{
				CurrentAnimNum = 1;
			}
		}

		if (GetKeyState('A') & 0x8000)
		{
			m_vPosition = m_vPosition - (m_vLeftDirection * 0.1f);
			if (OnlyLeftOrRight)
			{
				CurrentAnimNum = 5;
			}
		}

		if (GetKeyState('D') & 0x8000)
		{
			m_vPosition = m_vPosition + (m_vLeftDirection * 0.1f);
			if (OnlyLeftOrRight)
			{
				CurrentAnimNum = 4;
			}

		}

		if (GetKeyState(VK_LBUTTON) & 0x8000)
		{

		}

		if (beforeAnimNum != CurrentAnimNum)
		{
			beforeAnimNum = CurrentAnimNum;
			m_MyCharacter->SetAnimationIndexBlend(beforeAnimNum);
		}
	}


	RECT rc;
	GetClientRect(g_hWnd, &rc);

	D3DXMATRIXA16 matR, matT;
	D3DXMatrixRotationY(&matR, m_fRotY);
	m_vDirection = D3DXVECTOR3(0, 0, 1);

	D3DXVec3TransformNormal(&m_vDirection, &m_vDirection, &matR);
	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	m_matWorld = matR * matT;
}

void cCharacter::Update(float ROTY, D3DXVECTOR3 POSITION)
{
	D3DXMATRIXA16 matR, matT;
	m_vPosition = POSITION;
	D3DXMatrixRotationY(&matR, m_fRotY);

	m_vDirection = D3DXVECTOR3(0, 0, 1);

	D3DXVec3TransformNormal(&m_vDirection, &m_vDirection, &matR);
	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	m_matWorld = matR * matT;
}

void cCharacter::Render()
{
}

D3DXVECTOR3 & cCharacter::GetPosition()
{
	return this->m_vPosition;
}

void cCharacter::SetPositionY(float y)
{
	this->m_vPosition.y = y;
}

void cCharacter::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_MOUSEMOVE:
		POINT ptCurrMouse;
		ptCurrMouse.x = LOWORD(lParam);
		ptCurrMouse.y = HIWORD(lParam);

		float fDeltaX = (float)ptCurrMouse.x - m_ptPrevMouse.x;
		float fDeltaY = (float)ptCurrMouse.y - m_ptPrevMouse.y;

		m_fRotY += (fDeltaX / 100.f);
		m_vDirection.x += (fDeltaY / 100.f);

		m_ptPrevMouse = ptCurrMouse;
		break;
	}
}