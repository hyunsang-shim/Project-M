#include "stdafx.h"
#include "cMyCharacter.h"
#include "cSkinnedMesh.h"
#include "cOBB.h"

cMyCharacter::cMyCharacter()
	: m_pSkinnedMesh(NULL)
	, m_pCharacterController(NULL)
	, m_pOBB(NULL)
{
}


cMyCharacter::~cMyCharacter()
{
	SAFE_RELEASE(m_pCharacterController);
	SAFE_DELETE(m_pSkinnedMesh);
	SAFE_DELETE(m_pOBB);
}

void cMyCharacter::Setup(char * szFolder, char * szFileName)
{
	m_pSkinnedMesh = new cSkinnedMesh(szFolder, szFileName);
	m_pSkinnedMesh->SetRandomTrackPosition();

	m_pOBB = new cOBB;
	m_pOBB->Setup(m_pSkinnedMesh);
}

void cMyCharacter::Update(D3DXVECTOR3 camDirection)
{
	if (m_pCharacterController)
	{	
		m_pCharacterController->Update(this, m_pSkinnedMesh);
	
	}

	if (m_pOBB)
		m_pOBB->Update(m_pCharacterController ? m_pCharacterController->GetTransform() : NULL);
	m_pSkinnedMesh->Update();
	m_vBulletPos = m_pSkinnedMesh->GetBulletPos();
}

void cMyCharacter::Render(D3DCOLOR c)
{
	if (m_pCharacterController)
		m_pSkinnedMesh->setTransform(m_pCharacterController->GetTransform());

	m_pSkinnedMesh->UpdateAndRender();

	if (m_pOBB)
		m_pOBB->OBBBox_Render(c);
}

cOBB * cMyCharacter::GetOBB()
{
	return m_pOBB;
}

D3DXVECTOR3 cMyCharacter::GetPosition()
{
	return m_pCharacterController->GetPosition();
}

void cMyCharacter::SetAnimationIndexBlend(int nIndex)
{
	m_pSkinnedMesh->SetAnimationIndexBlend(nIndex);
}

void cMyCharacter::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	m_pCharacterController->WndProc(hWnd, message, wParam, lParam);
}

string cMyCharacter::sendData()
{
	return m_pCharacterController->getUserData();
}

