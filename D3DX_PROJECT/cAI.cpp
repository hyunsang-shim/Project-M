#include "stdafx.h"
#include "cAI.h"
#include "cSkinnedMesh.h"
#include "cOBB.h"

cAI::cAI()
	: m_pSkinnedMesh(NULL)
	, m_pAIController(NULL)
	, m_pOBB(NULL),
	MonsterNum(-1)
	, load(0)
{
}

cAI::~cAI()
{
	SAFE_RELEASE(m_pAIController);
	SAFE_DELETE(m_pSkinnedMesh);
	SAFE_DELETE(m_pOBB);
}

void cAI::Setup(char * szFolder, char * szFileName)
{
	m_pSkinnedMesh = new cSkinnedMesh(szFolder, szFileName);
	m_pSkinnedMesh->SetRandomTrackPosition();

	m_pOBB = new cOBB;
	m_pOBB->Setup(m_pSkinnedMesh);
	load = 1;
}

void cAI::Update(bool b, D3DXVECTOR3 moveToCharacterDir)
{

	
	moveToCharacterDir = g_pOtherPlayerManager->GetOtherPlayerByID(target)->CurPos;
	

	if (m_pAIController)
	{
		m_pAIController->Update(this, b, moveToCharacterDir, m_pSkinnedMesh);
	}

	if (m_pOBB)
		m_pOBB->Update(m_pAIController ? m_pAIController->GetTransform() : NULL);
	//	m_pSkinnedMesh->Update();
	m_vBulletPos = m_pSkinnedMesh->GetBulletPos();
}

void cAI::Render(D3DCOLOR c)
{
	if (m_pAIController)
		m_pSkinnedMesh->setTransform(m_pAIController->GetTransform());

	m_pSkinnedMesh->UpdateAndRender();

	if (m_pOBB)
		m_pOBB->OBBBox_Render(c);
}

cOBB * cAI::GetOBB()
{
	return m_pOBB;
}

D3DXVECTOR3 cAI::GetPosition()
{
	return m_pAIController->GetPosition();
}

void cAI::SetPosition(D3DXVECTOR3 pos)
{
	m_pAIController->SetPosition(pos);
}

void cAI::SetAnimationIndexBlend(int nIndex)
{
	m_pSkinnedMesh->SetAnimationIndex(nIndex);
}