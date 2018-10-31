#include "stdafx.h"
#include "cSKY.h"


cSKY::cSKY()
	:m_pXMeshSKY(NULL)
{
	D3DXMatrixIdentity(&m_matWorld);
}

void cSKY::Setup()
{
	ST_PNT_VERTEX* vertex;
	D3DXCreateSphere(g_pDevice, 10, 30, 30, &m_pXMeshSKY, NULL);

	D3DXCOLOR c(0.7f, 0.7f, 1.0f, 1.0f);
	D3DXCOLOR black(0.0f, 0.0f, 0.0f, 1.0f);
	
	skyMaterial.Ambient = c;
	skyMaterial.Diffuse = c;
	skyMaterial.Specular = c;
	skyMaterial.Emissive = black;
	skyMaterial.Power = 0.0f;

}

void cSKY::Render()
{
	D3DXVECTOR3 eyePosition(0.0f, 0.0f, 0.0f);
	D3DXMATRIX view;
	g_pDevice->GetTransform(D3DTS_VIEW, &view);
	D3DXMATRIX viewlnverse;
	D3DXMatrixInverse(&viewlnverse, 0, &view);
	D3DXVec3TransformCoord(&eyePosition, &eyePosition, &viewlnverse);


	g_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	g_pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);


	D3DXMatrixTranslation(&m_matWorld, eyePosition.x, eyePosition.y, eyePosition.z);
	g_pDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
	g_pDevice->SetMaterial(&skyMaterial);
	g_pDevice->SetTexture(NULL, NULL);
	m_pXMeshSKY->DrawSubset(0);

	g_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
}



cSKY::~cSKY()
{
	SAFE_DELETE(m_pXMeshSKY);
}
