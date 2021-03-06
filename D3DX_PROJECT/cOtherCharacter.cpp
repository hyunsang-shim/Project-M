#include "stdafx.h"
#include "cOtherCharacter.h"
#include "cSkinnedMesh.h"
#include "cOBB.h"

cOtherCharacter::cOtherCharacter()
	: m_pSkinnedMesh(NULL)
	, m_pCharacterController(NULL)
	, m_pOBB(NULL)
	, status(CS_IDLE)
	, motionStatus(10)
{
	info.CurPos = D3DXVECTOR3(330, 5, -410);
}


cOtherCharacter::~cOtherCharacter()
{
	SAFE_RELEASE(m_pCharacterController);
	SAFE_DELETE(m_pSkinnedMesh);
	SAFE_DELETE(m_pOBB);
}

void cOtherCharacter::Setup()
{
	m_pSkinnedMesh = new cSkinnedMesh("Xfile", "Soldier76_with_gun.x");
	m_pSkinnedMesh->SetRandomTrackPosition();

	/*m_pOBB = new cOBB;
	m_pOBB->Setup(m_pSkinnedMesh);*/
}

void cOtherCharacter::Update()
{ 
	if (m_pCharacterController)
		m_pCharacterController->UpdateOtherPlayer(status, this);
}

void cOtherCharacter::Update(D3DXVECTOR3 CurPos, float rotY, WORD Status)
{
	int tmpStatus = 0;

	if (Status == CS_IDLE)
	{
		//SetAnimationIndexBlend(10);
		tmpStatus = 10;
	}
	else if (Status == CS_FRONT || Status == CS_FRONT_LEFT || Status == CS_FRONT_RIGHT)
	{
		//SetAnimationIndexBlend(8);
		tmpStatus = 8;
	}
	else if (Status == CS_FRONT_SHIFT || Status == CS_FRONT_LEFT_SHIFT || Status == CS_FRONT_RIGHT_SHIFT)
	{
		//SetAnimationIndexBlend(4);
		tmpStatus = 4;
	}
	else if (Status == CS_BACK || Status == CS_BACK_LEFT || Status == CS_BACK_RIGHT)
	{
		//SetAnimationIndexBlend(5);
		tmpStatus = 5;
	}
	else if (Status == CS_LEFT)
	{
		//SetAnimationIndexBlend(7);
		tmpStatus = 7;

	}
	else if (Status == CS_RIGHT)
	{
		//SetAnimationIndexBlend(6);
		tmpStatus = 6;
	}

	if (this->motionStatus != tmpStatus)
	{
		SetAnimationIndexBlend(tmpStatus);
		this->motionStatus = tmpStatus;
		
	}
	this->info.CurPos = CurPos;
	this->status = Status;
	if (m_pCharacterController)
		m_pCharacterController->UpdateOtherPlayer(CurPos, rotY, Status);

}

void cOtherCharacter::Render(D3DCOLOR c)
{
	if (m_pCharacterController)
		m_pSkinnedMesh->setTransform(m_pCharacterController->GetTransform());

	m_pSkinnedMesh->UpdateAndRender();

	//if (m_pOBB)
	//	m_pOBB->OBBBox_Render(c);
}

cOBB * cOtherCharacter::GetOBB()
{
	return m_pOBB;
}

D3DXVECTOR3 cOtherCharacter::GetPosition()
{
	return m_pCharacterController->GetPosition();
}

D3DXVECTOR3 * cOtherCharacter::GetPositionPoint()
{
	return &m_pCharacterController->m_vPosition;
}

void cOtherCharacter::SetAnimationIndexBlend(int nIndex)
{
	m_pSkinnedMesh->SetAnimationIndex(nIndex);
}

D3DXVECTOR3 cOtherCharacter::GetMyHeadPos()
{
	return m_pSkinnedMesh->GetHeadPos();
}