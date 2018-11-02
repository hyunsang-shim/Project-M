#include "stdafx.h"
#include "cUIObject.h"


cUIObject::cUIObject()
	: m_vPosition(0,0,0),
	m_pParent(NULL),
	m_stSize(0,0),
	m_nTag(0),
	m_isHidden(false)
{
	D3DXMatrixIdentity(&m_matWorld);
}


cUIObject::~cUIObject()
{
}

void cUIObject::SetPosition(float x, float y, float z)
{
	m_vPosition.x = x;
	m_vPosition.y = y;
	m_vPosition.z = z;
}

void cUIObject::AddChild(cUIObject * pChild)
{
	pChild->SetParent(this);
	m_vecChild.push_back(pChild);
}

void cUIObject::Update()
{
	if (m_isHidden) return;

	D3DXMatrixIdentity(&m_matWorld);
	m_matWorld._41 = m_vPosition.x;
	m_matWorld._42 = m_vPosition.y;
	m_matWorld._43 = m_vPosition.z;

	if (m_pParent)
	{
		m_matWorld._41 += m_pParent->m_matWorld._41;
		m_matWorld._42 += m_pParent->m_matWorld._42;
		m_matWorld._43 += m_pParent->m_matWorld._43;
	}

	for each(auto c in m_vecChild)
	{
		c->Update();
	}
}

void cUIObject::Render(LPD3DXSPRITE pSprite)
{
	if (m_isHidden) return;

	for each(auto c in m_vecChild)
		c->Render(pSprite);
}

void cUIObject::Destroy()
{
	m_isHidden = true;
	for each(auto c in m_vecChild)
		c->Destroy();
	this->Release();
}

cUIObject * cUIObject::FindChildByTag(int nTag)
{
	if (m_nTag == nTag)
		return this;

	for each(auto c in m_vecChild)
	{
		cUIObject* p = c->FindChildByTag(nTag);
		if (p) return p;
	}
	return nullptr;
}
