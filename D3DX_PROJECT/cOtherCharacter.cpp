#include "stdafx.h"
#include "cOtherCharacter.h"
#include "cSkinnedMesh.h"
#include "cOBB.h"

cOtherCharacter::cOtherCharacter()
	: m_pSkinnedMesh(NULL)
	, m_pCharacterController(NULL)
	, m_pOBB(NULL)
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
	m_pSkinnedMesh = new cSkinnedMesh("Xfile", "zealot.X");
	m_pSkinnedMesh->SetRandomTrackPosition();

	m_pOBB = new cOBB;
	m_pOBB->Setup(m_pSkinnedMesh);
}

void cOtherCharacter::Update()
{
	if (m_pCharacterController)
		m_pCharacterController->Update();

	if (m_pOBB)
		m_pOBB->Update(m_pCharacterController ? m_pCharacterController->GetTransform() : NULL);
	m_pSkinnedMesh->Update();
}

void cOtherCharacter::Render(D3DCOLOR c)
{
	if (m_pCharacterController)
		m_pSkinnedMesh->setTransform(m_pCharacterController->GetTransform());

	m_pSkinnedMesh->UpdateAndRender();

	if (m_pOBB)
		m_pOBB->OBBBox_Render(c);
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
	m_pSkinnedMesh->SetAnimationIndexBlend(nIndex);
}
