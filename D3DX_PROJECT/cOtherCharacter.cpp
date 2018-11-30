#include "stdafx.h"
#include "cOtherCharacter.h"
#include "cSkinnedMesh.h"
#include "cOBB.h"

cOtherCharacter::cOtherCharacter()
	: m_pSkinnedMesh(NULL)
	, m_pCharacterController(NULL)
	, m_pOBB(NULL)
	, status(CS_IDLE)
{
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

	m_pCharacterController->UpdateOtherPlayer(status);

}

void cOtherCharacter::Update(D3DXVECTOR3 CurPos, float rotY, WORD Status)
{
	SetAnimationIndexBlend(Status);
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

void cOtherCharacter::SetAnimationIndexBlend(int nIndex)
{
	m_pSkinnedMesh->SetAnimationIndex(nIndex);
}
