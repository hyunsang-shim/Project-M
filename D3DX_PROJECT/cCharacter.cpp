#include "stdafx.h"
#include "cCharacter.h"
#include "cMyCharacter.h"
#include "cSkinnedMesh.h"
#include "cNewObject.h"

cCharacter::cCharacter()
	:m_fRotY(0.0f)
	, m_vDirection(0, 0, 0)
	, m_vPosition(0, 0, 0)

{
	D3DXMatrixIdentity(&m_matWorld);
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

	static bool reloading = false;
	
	m_SkinnedMesh->GetAnimController()->GetTrackDesc(0, &stTrackDesc);
	m_SkinnedMesh->GetAnimController()->GetAnimationSet(CurrentAnimNum, &pCurrentAnimSet);

	CurrentPeriod = pCurrentAnimSet->GetPeriodicPosition(stTrackDesc.Position);
	TotalPeriod = pCurrentAnimSet->GetPeriod();

	D3DXVECTOR3 m_vBeforePosition = m_vPosition;

	if (GetKeyState('W') & 0x8000)
	{
		this->KEY_W = 1;
	}
	else
	{
		this->KEY_W = 0;
	}
	if (GetKeyState('S') & 0x8000)
	{
		this->KEY_S = 1;
	}
	else
	{
		this->KEY_S = 0;
	}
	if (GetKeyState('A') & 0x8000)
	{
		this->KEY_A = 1;
	}
	else
	{
		this->KEY_A = 0;
	}
	if (GetKeyState('D') & 0x8000)
	{
		this->KEY_D = 1;
	}
	else
	{
		this->KEY_D = 0;
	}
	if (GetKeyState(VK_SHIFT) & 0x8000)
	{
		this->KEY_SHIFT = 1;
	}
	else
	{
		this->KEY_SHIFT = 0;
	}

	beforeAnimNum = CurrentAnimNum;

	if (KEY_W && !KEY_S && !KEY_A &&! KEY_D && !KEY_SHIFT) // front
	{
		m_vPosition = m_vPosition + (m_vDirection * 0.1f);
		CurrentAnimNum = 8;
	}
	else if (KEY_W && !KEY_S && !KEY_A && !KEY_D && KEY_SHIFT) // front + shift
	{
		m_vPosition = m_vPosition + (m_vDirection * 0.25f);
		CurrentAnimNum = 4;
	}
	else if (KEY_W && !KEY_S && KEY_A && !KEY_D && !KEY_SHIFT) // front + left
	{
		m_vPosition = m_vPosition + (m_vDirection * 0.1f);
		m_vPosition = m_vPosition - (m_vLeftDirection * 0.1f);
		CurrentAnimNum = 8;
	}
	else if (KEY_W && !KEY_S && !KEY_A && KEY_D && !KEY_SHIFT) // front + right
	{
		m_vPosition = m_vPosition + (m_vDirection * 0.1f);
		m_vPosition = m_vPosition + (m_vLeftDirection * 0.1f);
		CurrentAnimNum = 8;
	}
	else if (KEY_W && !KEY_S && KEY_A && !KEY_D && KEY_SHIFT) // front + left + shift
	{
		m_vPosition = m_vPosition + (m_vDirection * 0.2f);
		m_vPosition = m_vPosition - (m_vLeftDirection * 0.2f);
		CurrentAnimNum = 4;
	}
	else if (KEY_W && !KEY_S && !KEY_A && KEY_D && KEY_SHIFT) // front + right + shift
	{
		m_vPosition = m_vPosition + (m_vDirection * 0.2f);
		m_vPosition = m_vPosition + (m_vLeftDirection * 0.2f);
		CurrentAnimNum = 4;
	}
	else if (!KEY_W && !KEY_S && KEY_A && !KEY_D ) // left
	{
		m_vPosition = m_vPosition - (m_vLeftDirection * 0.1f);
		CurrentAnimNum = 7;

	}
	else if (!KEY_W && !KEY_S && !KEY_A && KEY_D ) // right
	{
		m_vPosition = m_vPosition + (m_vLeftDirection * 0.1f);
		CurrentAnimNum = 6;
	}
	else if (!KEY_W && KEY_S && !KEY_A && !KEY_D ) // back
	{
		m_vPosition = m_vPosition - (m_vDirection * 0.1f);
		CurrentAnimNum = 5;
	}
	else if (!KEY_W && KEY_S && KEY_A && !KEY_D ) // back + left
	{
		m_vPosition = m_vPosition - (m_vDirection * 0.1f);
		m_vPosition = m_vPosition - (m_vLeftDirection * 0.1f);
		CurrentAnimNum = 5;
	}
	else if (!KEY_W && KEY_S && !KEY_A && KEY_D ) // back + riight
	{
		m_vPosition = m_vPosition - (m_vDirection * 0.1f);
		m_vPosition = m_vPosition + (m_vLeftDirection * 0.1f);
		CurrentAnimNum = 5;
	}
	else if(!KEY_W && !KEY_S && !KEY_A && !KEY_D &&! reloading)
	{
		CurrentAnimNum = 10;
		int i = 0;
	}

	if (reloading)
	{
		if (TotalPeriod <= CurrentPeriod+0.15)
		{
			reloading = false;
		}
	}

	if (GetKeyState('R') & 0x8000 && !KEY_W && !KEY_S && !KEY_A && !KEY_D)
	{
		if (!reloading)
		{
			reloading = true;
			CurrentAnimNum = 1;
			beforeAnimNum = CurrentAnimNum;
			m_MyCharacter->SetAnimationIndexBlend(CurrentAnimNum);
			g_pGameInfoManager->GetMyInfo()->Status = CurrentAnimNum;
			g_pNetworkManager->SendData(NH_USER_STATUS, g_pGameInfoManager->GetMyInfo());
		}
	}

	if (CurrentAnimNum == beforeAnimNum)
	{

	}
	else
	{
		beforeAnimNum = CurrentAnimNum;
		m_MyCharacter->SetAnimationIndexBlend(beforeAnimNum);
		g_pGameInfoManager->GetMyInfo()->Status = CurrentAnimNum;
		g_pNetworkManager->SendData(NH_USER_STATUS, g_pGameInfoManager->GetMyInfo());
	}



	//if (GetKeyState('R') & 0x8000)
	//{
	//	CurrentAnimNum = 2;

	//	beforeAnimNum = CurrentAnimNum;

	//	m_MyCharacter->SetAnimationIndexBlend(beforeAnimNum);
	//}
	//else
	//{
	//	if (GetKeyState('W') & 0x8000)
	//	{
	//		m_vPosition = m_vPosition + (m_vDirection * 0.1f);
	//		CurrentAnimNum = 6;
	//		if (GetKeyState(VK_SHIFT) & 0x8000)
	//		{
	//			m_vPosition = m_vPosition + (m_vDirection * 0.15f);

	//			if ((GetKeyState('A') & 0x8000) || (GetKeyState('D') & 0x8000))
	//			{
	//				OnlyLeftOrRight = false;
	//			}
	//		}
	//		if ((GetKeyState('A') & 0x8000) || (GetKeyState('D') & 0x8000))
	//		{
	//			OnlyLeftOrRight = false;
	//		}
	//	}
	//	else if (GetKeyState('S') & 0x8000)
	//	{
	//		m_vPosition = m_vPosition - (m_vDirection * 0.1f);
	//		CurrentAnimNum = 3;
	//		if ((GetKeyState('A') & 0x8000) || (GetKeyState('D') & 0x8000))
	//		{
	//			OnlyLeftOrRight = false;
	//		}
	//	}
	//	else
	//	{
	//		if (TotalPeriod <= CurrentPeriod + 0.1)
	//		{
	//			CurrentAnimNum = 1;
	//		}
	//	}

	//	if (GetKeyState('A') & 0x8000)
	//	{
	//		m_vPosition = m_vPosition - (m_vLeftDirection * 0.1f);
	//		if (OnlyLeftOrRight)
	//		{
	//			CurrentAnimNum = 5;
	//		}
	//	}

	//	if (GetKeyState('D') & 0x8000)
	//	{
	//		m_vPosition = m_vPosition + (m_vLeftDirection * 0.1f);
	//		if (OnlyLeftOrRight)
	//		{
	//			CurrentAnimNum = 4;
	//		}

	//	}

	//	if (GetKeyState(VK_LBUTTON) & 0x8000)
	//	{

	//	}

	//	if (beforeAnimNum != CurrentAnimNum)
	//	{
	//		beforeAnimNum = CurrentAnimNum;
	//		m_MyCharacter->SetAnimationIndexBlend(beforeAnimNum);
	//	}
	//}

	if (g_pGameInfoManager->m_pMap)
	{
		float y = 0;
		D3DXVECTOR3 head = m_vPosition + D3DXVECTOR3(0, 2, 0);
		if(!g_pGameInfoManager->m_pMap->GetY(m_vPosition.x, y, m_vPosition.z, head))
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
	D3DXMatrixRotationY(&matR, m_fRotY);
	m_vDirection = D3DXVECTOR3(0, 0, 1);

	D3DXVec3TransformNormal(&m_vDirection, &m_vDirection, &matR);
	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	m_matWorld = matR * matT;

	g_pGameInfoManager->GetMyInfo()->CurPos.x = m_vPosition.x;
	g_pGameInfoManager->GetMyInfo()->CurPos.y = m_vPosition.y;
	g_pGameInfoManager->GetMyInfo()->CurPos.z = m_vPosition.z;

	g_pGameInfoManager->GetMyInfo()->Dir = m_fRotY;
}

void cCharacter::UpdateOtherPlayer(D3DXVECTOR3 CurPos, float Direction, WORD status)
{

	D3DXMATRIXA16 matR, matT;
	D3DXMatrixRotationY(&matR, Direction);
	D3DXVec3TransformNormal(&m_vDirection, &m_vDirection, &matR);
	D3DXMatrixTranslation(&matT, CurPos.x, CurPos.y, CurPos.z);
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

CharacterStatus_PC cCharacter::getUserData()
{
	CharacterStatus_PC myData;
	 
	// myData.PlayerName
	// myData.Character_No
	// myData.Attack
	// myData.MaxHP
	// myData.CurHP
	// myData.HP_Regen
	// myData.MoveSpeed
	// myData.MagCnt
	// myData.MaxMag
	// myData.ShootSpeed
	// myData.BulletTime
	myData.CurPos = this->m_vPosition;
	myData.Dir = this->m_fRotY;
	// myData.Status

	return myData;
}

void cCharacter::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_MOUSEMOVE:
		if (!g_pGameInfoManager->isESCPushed)
		{
			m_fRotY += (g_pGameInfoManager->mouseMoveX / 100.f);
			m_vDirection.x += (g_pGameInfoManager->mouseMoveY / 100.f);
		}
		break;
	
	}
}