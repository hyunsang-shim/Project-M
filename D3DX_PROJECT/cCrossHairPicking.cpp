#include "stdafx.h"
#include "cCrossHairPicking.h"


cCrossHairPicking::cCrossHairPicking()
{
	GetClientRect(g_hWnd, &rc);
	cp.x = rc.right / 2;
	cp.y = rc.bottom / 2;
}


cCrossHairPicking::~cCrossHairPicking()
{
}

void cCrossHairPicking::Setup()
{
	m_vMiddlePos = D3DXVECTOR3(cp.x, cp.y, 1.0f);
	CalcPosition();
}

void cCrossHairPicking::CalcPosition()
{
	D3DXMATRIXA16 matInvView, matProj, matWorld;
	D3DVIEWPORT9 Viewport;

	D3DXMatrixIdentity(&matInvView);
	D3DXMatrixIdentity(&matProj);

	g_pDevice->GetTransform(D3DTS_WORLD, &matWorld);
	g_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	g_pDevice->GetViewport(&Viewport);
	g_pDevice->GetTransform(D3DTS_VIEW, &matInvView);

	D3DXMatrixInverse(&matInvView, 0, &matInvView);

	double x = m_vMiddlePos.x;
	double y = m_vMiddlePos.y;

	x = ( ( (2.0f * x) / Viewport.Width ) - 1.0f ) / matProj._11;
	y = (((-2.0f * y) / Viewport.Height) + 1.0f) / matProj._22;

	m_Origin = D3DXVECTOR3(0,0,0);
	m_Direction = D3DXVECTOR3(x, y, 1.0f);

	D3DXVec3TransformNormal(&m_Direction, &m_Direction, &matInvView);
	D3DXVec3Normalize(&m_Direction, &m_Direction);
	m_Origin = D3DXVECTOR3(matInvView._41, matInvView._42, matInvView._43);
}
