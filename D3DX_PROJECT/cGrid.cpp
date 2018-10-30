#include "stdafx.h"
#include "cGrid.h"


cGrid::cGrid()
{
	D3DXMatrixIdentity(&g_tMat);
	m_pVB = NULL;
	m_nNumLine = 0;
}


cGrid::~cGrid()
{
	SAFE_RELEASE(m_pVB);
}

void cGrid::Setup()
{
	vector<ST_PC_VERTEX> m_vecVertex;
	vector<ST_PC_VERTEX> m_vecPiramidVertex;
	ST_PC_VERTEX temp;
	temp.c = D3DCOLOR_XRGB(160, 160, 160);
	
	for (int i = -GRID_SIZE; i <= GRID_SIZE; i++)
	{
		if (i % 5 == 0)
		{
			temp.c = D3DCOLOR_XRGB(0, 0, 0);
		}
		if (i == 0)
		{
			temp.c = D3DCOLOR_XRGB(255, 0, 0);
		}
		temp.p.x = -GRID_SIZE;
		temp.p.y = 0;
		temp.p.z = i;
		m_vecVertex.push_back(temp);
		temp.p.x = GRID_SIZE;
		m_vecVertex.push_back(temp);
		if (i % 5 == 0)
		{
			temp.c = D3DCOLOR_XRGB(160, 160, 160);
		}
	}
	for (int i = -GRID_SIZE; i <= GRID_SIZE; i++)
	{
		if (i % 5 == 0)
		{
			temp.c = D3DCOLOR_XRGB(0, 0, 0);
		}
		if (i == 0)
		{
			temp.c = D3DCOLOR_XRGB(0, 0, 255);
		}
		temp.p.z = -GRID_SIZE;
		temp.p.y = 0;
		temp.p.x = i;
		m_vecVertex.push_back(temp);
		temp.p.z = GRID_SIZE;
		m_vecVertex.push_back(temp);
		if (i % 5 == 0)
		{
			temp.c = D3DCOLOR_XRGB(160, 160, 160);
		}
	}
	

	temp.c = D3DCOLOR_XRGB(0, 255, 0);
	temp.p.x = 0.1;
	temp.p.y = 2;
	temp.p.z = 0.1;
	m_vecPiramidVertex.push_back(temp);
	temp.p.x = 0.1;
	temp.p.y = 2;
	temp.p.z = -0.1;
	m_vecPiramidVertex.push_back(temp);
	temp.p.x = -0.1;
	temp.p.y = 2;
	temp.p.z = 0.1;
	m_vecPiramidVertex.push_back(temp);

	temp.p.x = 0.1;
	temp.p.y = 2;
	temp.p.z = -0.1;
	m_vecPiramidVertex.push_back(temp);
	temp.p.x = -0.1;
	temp.p.y = 2;
	temp.p.z = -0.1;
	m_vecPiramidVertex.push_back(temp);
	temp.p.x = -0.1;
	temp.p.y = 2;
	temp.p.z = 0.1;
	m_vecPiramidVertex.push_back(temp);

	temp.p.x = 0.1;
	temp.p.y = 2;
	temp.p.z = 0.1;
	m_vecPiramidVertex.push_back(temp);
	temp.p.x = -0.1;
	temp.p.y = 2;
	temp.p.z = 0.1;
	m_vecPiramidVertex.push_back(temp);
	temp.p.x = 0;
	temp.p.y = 0;
	temp.p.z = 0;
	m_vecPiramidVertex.push_back(temp);

	temp.p.x = -0.1;
	temp.p.y = 2;
	temp.p.z = 0.1;
	m_vecPiramidVertex.push_back(temp);
	temp.p.x = -0.1;
	temp.p.y = 2;
	temp.p.z = -0.1;
	m_vecPiramidVertex.push_back(temp);
	temp.p.x = 0;
	temp.p.y = 0;
	temp.p.z = 0;
	m_vecPiramidVertex.push_back(temp);

	temp.p.x = -0.1;
	temp.p.y = 2;
	temp.p.z = -0.1;
	m_vecPiramidVertex.push_back(temp);
	temp.p.x = 0.1;
	temp.p.y = 2;
	temp.p.z = -0.1;
	m_vecPiramidVertex.push_back(temp);
	temp.p.x = 0;
	temp.p.y = 0;
	temp.p.z = 0;
	m_vecPiramidVertex.push_back(temp);

	temp.p.x = 0.1;
	temp.p.y = 2;
	temp.p.z = -0.1;
	m_vecPiramidVertex.push_back(temp);
	temp.p.x = 0.1;
	temp.p.y = 2;
	temp.p.z = 0.1;
	m_vecPiramidVertex.push_back(temp);
	temp.p.x = 0;
	temp.p.y = 0;
	temp.p.z = 0;
	m_vecPiramidVertex.push_back(temp);


	temp.c = D3DCOLOR_XRGB(255, 0, 0);
	temp.p.x = 2;
	temp.p.y = 0.1;
	temp.p.z = -0.1;
	m_vecPiramidVertex.push_back(temp);
	temp.p.x = 2;
	temp.p.y = 0.1;
	temp.p.z = 0.1;
	m_vecPiramidVertex.push_back(temp);
	temp.p.x = 2;
	temp.p.y = -0.1;
	temp.p.z = -0.1;
	m_vecPiramidVertex.push_back(temp);
	temp.p.x = 2;
	temp.p.y = 0.1;
	temp.p.z = 0.1;
	m_vecPiramidVertex.push_back(temp);
	temp.p.x = 2;
	temp.p.y = -0.1;
	temp.p.z = 0.1;
	m_vecPiramidVertex.push_back(temp);
	temp.p.x = 2;
	temp.p.y = -0.1;
	temp.p.z = -0.1;
	m_vecPiramidVertex.push_back(temp);

	temp.p.x = 2;
	temp.p.y = -0.1;
	temp.p.z = 0.1;
	m_vecPiramidVertex.push_back(temp);
	temp.p.x = 2;
	temp.p.y = 0.1;
	temp.p.z = 0.1;
	m_vecPiramidVertex.push_back(temp);
	temp.p.x = 0;
	temp.p.y = 0;
	temp.p.z = 0;
	m_vecPiramidVertex.push_back(temp);

	temp.p.x = 2;
	temp.p.y = 0.1;
	temp.p.z = 0.1;
	m_vecPiramidVertex.push_back(temp);
	temp.p.x = 2;
	temp.p.y = 0.1;
	temp.p.z = -0.1;
	m_vecPiramidVertex.push_back(temp);
	temp.p.x = 0;
	temp.p.y = 0;
	temp.p.z = 0;
	m_vecPiramidVertex.push_back(temp);

	temp.p.x = 2;
	temp.p.y = 0.1;
	temp.p.z = -0.1;
	m_vecPiramidVertex.push_back(temp);
	temp.p.x = 2;
	temp.p.y = -0.1;
	temp.p.z = -0.1;
	m_vecPiramidVertex.push_back(temp);
	temp.p.x = 0;
	temp.p.y = 0;
	temp.p.z = 0;
	m_vecPiramidVertex.push_back(temp);

	temp.p.x = 2;
	temp.p.y = -0.1;
	temp.p.z = -0.1;
	m_vecPiramidVertex.push_back(temp);
	temp.p.x = 2;
	temp.p.y = -0.1;
	temp.p.z = 0.1;
	m_vecPiramidVertex.push_back(temp);
	temp.p.x = 0;
	temp.p.y = 0;
	temp.p.z = 0;
	m_vecPiramidVertex.push_back(temp);

	temp.c = D3DCOLOR_XRGB(0, 0, 255);
	temp.p.x = 0.1;
	temp.p.y = 0.1;
	temp.p.z = 2;
	m_vecPiramidVertex.push_back(temp);
	temp.p.x = -0.1;
	temp.p.y = -0.1;
	temp.p.z = 2;
	m_vecPiramidVertex.push_back(temp);
	temp.p.x = 0.1;
	temp.p.y = -0.1;
	temp.p.z = 2;
	m_vecPiramidVertex.push_back(temp);

	temp.p.x = 0.1;
	temp.p.y = 0.1;
	temp.p.z = 2;
	m_vecPiramidVertex.push_back(temp);
	temp.p.x = -0.1;
	temp.p.y = 0.1;
	temp.p.z = 2;
	m_vecPiramidVertex.push_back(temp);
	temp.p.x = -0.1;
	temp.p.y = -0.1;
	temp.p.z = 2;
	m_vecPiramidVertex.push_back(temp);

	temp.p.x = -0.1;
	temp.p.y = 0.1;
	temp.p.z = 2;
	m_vecPiramidVertex.push_back(temp);
	temp.p.x = 0.1;
	temp.p.y = 0.1;
	temp.p.z = 2;
	m_vecPiramidVertex.push_back(temp);
	temp.p.x = 0;
	temp.p.y = 0;
	temp.p.z = 0;
	m_vecPiramidVertex.push_back(temp);

	temp.p.x = 0.1;
	temp.p.y = 0.1;
	temp.p.z = 2;
	m_vecPiramidVertex.push_back(temp);
	temp.p.x = 0.1;
	temp.p.y = -0.1;
	temp.p.z = 2;
	m_vecPiramidVertex.push_back(temp);
	temp.p.x = 0;
	temp.p.y = 0;
	temp.p.z = 0;
	m_vecPiramidVertex.push_back(temp);

	temp.p.x = 0.1;
	temp.p.y = -0.1;
	temp.p.z = 2;
	m_vecPiramidVertex.push_back(temp);
	temp.p.x = -0.1;
	temp.p.y = -0.1;
	temp.p.z = 2;
	m_vecPiramidVertex.push_back(temp);
	temp.p.x = 0;
	temp.p.y = 0;
	temp.p.z = 0;
	m_vecPiramidVertex.push_back(temp);

	temp.p.x = -0.1;
	temp.p.y = -0.1;
	temp.p.z = 2;
	m_vecPiramidVertex.push_back(temp);
	temp.p.x = -0.1;
	temp.p.y = 0.1;
	temp.p.z = 2;
	m_vecPiramidVertex.push_back(temp);
	temp.p.x = 0;
	temp.p.y = 0;
	temp.p.z = 0;
	m_vecPiramidVertex.push_back(temp);

	{
		m_nNumLine = m_vecVertex.size() / 2;
		g_pDevice->CreateVertexBuffer(
			m_vecVertex.size() * sizeof(ST_PC_VERTEX),
			0,
			ST_PC_VERTEX::FVF,
			D3DPOOL_MANAGED,
			&m_pVB,
			NULL);
		ST_PC_VERTEX* pV = NULL;
		m_pVB->Lock(0, 0, (LPVOID*)&pV, 0);

		memcpy(pV, &m_vecVertex[0], m_vecVertex.size() * sizeof(ST_PC_VERTEX));
		m_pVB->Unlock();

	}
	{
		m_nPNumLine = m_vecPiramidVertex.size() / 3;
		g_pDevice->CreateVertexBuffer(
			m_vecPiramidVertex.size() * sizeof(ST_PC_VERTEX),
			0,
			ST_PC_VERTEX::FVF,
			D3DPOOL_MANAGED,
			&m_pPVB,
			NULL);
		ST_PC_VERTEX* pV = NULL;
		m_pPVB->Lock(0, 0, (LPVOID*)&pV, 0);

		memcpy(pV, &m_vecPiramidVertex[0], m_vecPiramidVertex.size() * sizeof(ST_PC_VERTEX));
		m_pPVB->Unlock();

	}
}


D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p)
{
	D3DMATERIAL9 rntrl;
	rntrl.Ambient = a;
	rntrl.Diffuse = d;
	rntrl.Specular = s;
	rntrl.Emissive = e;
	rntrl.Power = p;
	return rntrl;
}

void cGrid::Render()
{

	g_pDevice->SetTransform(D3DTS_WORLD, &g_tMat);
	g_pDevice->SetFVF(ST_PC_VERTEX::FVF);
	
	D3DXCOLOR c(1.0f, 1.0f, 1.0f, 1.0f);
	D3DXCOLOR black(0.0f, 0.0f, 0.0f, 1.0f);
	D3DMATERIAL9 whiteMeterial = InitMtrl(c, c, c, black, 8.0f);
	g_pDevice->SetMaterial(&whiteMeterial);
	{
		g_pDevice->SetStreamSource(0, m_pVB, 0, sizeof(ST_PC_VERTEX));
		g_pDevice->DrawPrimitive(D3DPT_LINELIST, 0, m_nNumLine);
	}

	{
		g_pDevice->SetStreamSource(0, m_pPVB, 0, sizeof(ST_PC_VERTEX));
		g_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_nPNumLine);
	}
}
