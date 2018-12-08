#include "stdafx.h"
#include "cOBB.h"
#include "cSkinnedMesh.h"
#include "cNewObject.h"
#include "cWaveTriggerBox.h"

cOBB::cOBB()
	:m_pIndexBuffer(NULL)
	, m_pVertexBuffer(NULL)
{
}


cOBB::~cOBB()
{
	SAFE_RELEASE(m_pIndexBuffer);
	SAFE_RELEASE(m_pVertexBuffer);
}

void cOBB::Setup(cSkinnedMesh * pSkinnedMesh)
{
	D3DXVECTOR3 vMin = pSkinnedMesh->GetMin();
	D3DXVECTOR3 vMax = pSkinnedMesh->GetMax();
	m_vOrgCenterPos = (vMin + vMax) / 2.0f;

	m_vOrgAxisDir[0] = D3DXVECTOR3(1, 0, 0);
	m_vOrgAxisDir[1] = D3DXVECTOR3(0, 1, 0);
	m_vOrgAxisDir[2] = D3DXVECTOR3(0, 0, 1);

	m_fAxisLen[0] = fabs(vMax.x - vMin.x);
	m_fAxisLen[1] = fabs(vMax.y - vMin.y);
	m_fAxisLen[2] = fabs(vMax.z - vMin.z);

	m_fAxisHalfLen[0] = m_fAxisLen[0] / 2.0f;
	m_fAxisHalfLen[1] = m_fAxisLen[1] / 2.0f;
	m_fAxisHalfLen[2] = m_fAxisLen[2] / 2.0f;

	D3DXMatrixIdentity(&m_matWorldTM);

	vector<ST_PC_VERTEX> vecVertex;
	vector<int> vecIndex;


	ST_PC_VERTEX v;
	v.c = D3DCOLOR_XRGB(255, 255, 255);

	v.p = D3DXVECTOR3(m_vOrgCenterPos.x + m_fAxisHalfLen[0], m_vOrgCenterPos.y + m_fAxisHalfLen[1], m_vOrgCenterPos.z + m_fAxisHalfLen[2]);   //FRT
	vecVertex.push_back(v);
	v.p = D3DXVECTOR3(m_vOrgCenterPos.x + m_fAxisHalfLen[0], m_vOrgCenterPos.y - m_fAxisHalfLen[1], m_vOrgCenterPos.z + m_fAxisHalfLen[2]);      //FRB
	vecVertex.push_back(v);
	v.p = D3DXVECTOR3(m_vOrgCenterPos.x - m_fAxisHalfLen[0], m_vOrgCenterPos.y - m_fAxisHalfLen[1], m_vOrgCenterPos.z + m_fAxisHalfLen[2]);      //FLB
	vecVertex.push_back(v);
	v.p = D3DXVECTOR3(m_vOrgCenterPos.x - m_fAxisHalfLen[0], m_vOrgCenterPos.y + m_fAxisHalfLen[1], m_vOrgCenterPos.z + m_fAxisHalfLen[2]);      //FLT
	vecVertex.push_back(v);
	v.p = D3DXVECTOR3(m_vOrgCenterPos.x + m_fAxisHalfLen[0], m_vOrgCenterPos.y + m_fAxisHalfLen[1], m_vOrgCenterPos.z - m_fAxisHalfLen[2]);      //BRT
	vecVertex.push_back(v);
	v.p = D3DXVECTOR3(m_vOrgCenterPos.x + m_fAxisHalfLen[0], m_vOrgCenterPos.y - m_fAxisHalfLen[1], m_vOrgCenterPos.z - m_fAxisHalfLen[2]);      //BRB
	vecVertex.push_back(v);
	v.p = D3DXVECTOR3(m_vOrgCenterPos.x - m_fAxisHalfLen[0], m_vOrgCenterPos.y - m_fAxisHalfLen[1], m_vOrgCenterPos.z - m_fAxisHalfLen[2]);      //BLB
	vecVertex.push_back(v);
	v.p = D3DXVECTOR3(m_vOrgCenterPos.x - m_fAxisHalfLen[0], m_vOrgCenterPos.y + m_fAxisHalfLen[1], m_vOrgCenterPos.z - m_fAxisHalfLen[2]);      //BLT
	vecVertex.push_back(v);

	//
	// Vertex Indices for OBB (Triangle)
	//front
	vecIndex.push_back(0);	vecIndex.push_back(1);	vecIndex.push_back(2);
	vecIndex.push_back(0);	vecIndex.push_back(2);	vecIndex.push_back(3);

	// back
	vecIndex.push_back(4);	vecIndex.push_back(6);	vecIndex.push_back(5);
	vecIndex.push_back(4);	vecIndex.push_back(7);	vecIndex.push_back(6);

	//right
	vecIndex.push_back(4);	vecIndex.push_back(5);	vecIndex.push_back(1);
	vecIndex.push_back(4);	vecIndex.push_back(1);	vecIndex.push_back(0);

	// left
	vecIndex.push_back(3);	vecIndex.push_back(2);	vecIndex.push_back(6);
	vecIndex.push_back(3);	vecIndex.push_back(6);	vecIndex.push_back(7);

	// bottom
	vecIndex.push_back(1);	vecIndex.push_back(5);	vecIndex.push_back(6);
	vecIndex.push_back(1);	vecIndex.push_back(6);	vecIndex.push_back(2);

	//top
	vecIndex.push_back(4);	vecIndex.push_back(0);	vecIndex.push_back(3);
	vecIndex.push_back(4);	vecIndex.push_back(3);	vecIndex.push_back(7);

	int size = sizeof(ST_PC_VERTEX) * vecVertex.size();
	HRESULT asd = g_pDevice->CreateVertexBuffer(size, 0, ST_PC_VERTEX::FVF, D3DPOOL_DEFAULT, &m_pVertexBuffer, 0);
	HRESULT ede = g_pDevice->CreateIndexBuffer(vecIndex.size() * sizeof(WORD), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIndexBuffer, 0);

	ST_PC_VERTEX* vertices;
	m_pVertexBuffer->Lock(0, 0, (void**)&vertices, 0);
	for (int i = 0; i < vecVertex.size(); i++)
	{
		vertices[i] = vecVertex[i];
	}
	m_pVertexBuffer->Unlock();

	WORD* indexList;
	m_pIndexBuffer->Lock(0, 0, (void**)&indexList, 0);
	for (int i = 0; i < vecIndex.size(); i++)
	{
		indexList[i] = vecIndex[i];
	}
	m_pIndexBuffer->Unlock();

}

void cOBB::Setup(cNewObject * pObject)
{
	D3DXVECTOR3 vMin = pObject->GetMin();
	D3DXVECTOR3 vMax = pObject->GetMax();
	m_vOrgCenterPos = (vMin + vMax) / 2.0f;

	m_vOrgAxisDir[0] = D3DXVECTOR3(1, 0, 0);
	m_vOrgAxisDir[1] = D3DXVECTOR3(0, 1, 0);
	m_vOrgAxisDir[2] = D3DXVECTOR3(0, 0, 1);

	m_fAxisLen[0] = fabs(vMax.x - vMin.x);
	m_fAxisLen[1] = fabs(vMax.y - vMin.y);
	m_fAxisLen[2] = fabs(vMax.z - vMin.z);

	m_fAxisHalfLen[0] = m_fAxisLen[0] / 2.0f;
	m_fAxisHalfLen[1] = m_fAxisLen[1] / 2.0f;
	m_fAxisHalfLen[2] = m_fAxisLen[2] / 2.0f;

	D3DXMatrixIdentity(&m_matWorldTM);

	vector<ST_PC_VERTEX> vecVertex;
	vector<int> vecIndex;


	ST_PC_VERTEX v;
	v.p = D3DXVECTOR3(m_vOrgCenterPos.x + m_fAxisHalfLen[0], m_vOrgCenterPos.y + m_fAxisHalfLen[1], m_vOrgCenterPos.z + m_fAxisHalfLen[2]);	//FRT
	v.c = D3DCOLOR_XRGB(255, 255, 255);
	vecVertex.push_back(v);
	v.p = D3DXVECTOR3(m_vOrgCenterPos.x + m_fAxisHalfLen[0], m_vOrgCenterPos.y - m_fAxisHalfLen[1], m_vOrgCenterPos.z + m_fAxisHalfLen[2]);		//FRB
	vecVertex.push_back(v);
	v.p = D3DXVECTOR3(m_vOrgCenterPos.x - m_fAxisHalfLen[0], m_vOrgCenterPos.y - m_fAxisHalfLen[1], m_vOrgCenterPos.z + m_fAxisHalfLen[2]);		//FLB
	vecVertex.push_back(v);
	v.p = D3DXVECTOR3(m_vOrgCenterPos.x - m_fAxisHalfLen[0], m_vOrgCenterPos.y + m_fAxisHalfLen[1], m_vOrgCenterPos.z + m_fAxisHalfLen[2]);		//FLT
	vecVertex.push_back(v);
	v.p = D3DXVECTOR3(m_vOrgCenterPos.x + m_fAxisHalfLen[0], m_vOrgCenterPos.y + m_fAxisHalfLen[1], m_vOrgCenterPos.z - m_fAxisHalfLen[2]);		//BRT
	vecVertex.push_back(v);
	v.p = D3DXVECTOR3(m_vOrgCenterPos.x + m_fAxisHalfLen[0], m_vOrgCenterPos.y - m_fAxisHalfLen[1], m_vOrgCenterPos.z - m_fAxisHalfLen[2]);		//BRB
	vecVertex.push_back(v);
	v.p = D3DXVECTOR3(m_vOrgCenterPos.x - m_fAxisHalfLen[0], m_vOrgCenterPos.y - m_fAxisHalfLen[1], m_vOrgCenterPos.z - m_fAxisHalfLen[2]);		//BLB
	vecVertex.push_back(v);
	v.p = D3DXVECTOR3(m_vOrgCenterPos.x - m_fAxisHalfLen[0], m_vOrgCenterPos.y + m_fAxisHalfLen[1], m_vOrgCenterPos.z - m_fAxisHalfLen[2]);		//BLT
	vecVertex.push_back(v);

	//
	// Vertex Indices for OBB (Triangle)
	//front
	vecIndex.push_back(0);	vecIndex.push_back(1);	vecIndex.push_back(2);
	vecIndex.push_back(0);	vecIndex.push_back(2);	vecIndex.push_back(3);

	// back
	vecIndex.push_back(4);	vecIndex.push_back(6);	vecIndex.push_back(5);
	vecIndex.push_back(4);	vecIndex.push_back(7);	vecIndex.push_back(6);

	//right
	vecIndex.push_back(4);	vecIndex.push_back(5);	vecIndex.push_back(1);
	vecIndex.push_back(4);	vecIndex.push_back(1);	vecIndex.push_back(0);

	// left
	vecIndex.push_back(3);	vecIndex.push_back(2);	vecIndex.push_back(6);
	vecIndex.push_back(3);	vecIndex.push_back(6);	vecIndex.push_back(7);

	// bottom
	vecIndex.push_back(1);	vecIndex.push_back(5);	vecIndex.push_back(6);
	vecIndex.push_back(1);	vecIndex.push_back(6);	vecIndex.push_back(2);

	//top
	vecIndex.push_back(4);	vecIndex.push_back(0);	vecIndex.push_back(3);
	vecIndex.push_back(4);	vecIndex.push_back(3);	vecIndex.push_back(7);


	int size = sizeof(ST_PC_VERTEX) * vecVertex.size();
	HRESULT asd = g_pDevice->CreateVertexBuffer(size, 0, ST_PC_VERTEX::FVF, D3DPOOL_DEFAULT, &m_pVertexBuffer, 0);
	HRESULT ede = g_pDevice->CreateIndexBuffer(vecIndex.size() * sizeof(WORD), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIndexBuffer, 0);

	ST_PC_VERTEX* vertices;
	m_pVertexBuffer->Lock(0, 0, (void**)&vertices, 0);
	for (int i = 0; i < vecVertex.size(); i++)
	{
		vertices[i] = vecVertex[i];
	}
	m_pVertexBuffer->Unlock();

	WORD* indexList;
	m_pIndexBuffer->Lock(0, 0, (void**)&indexList, 0);
	for (int i = 0; i < vecIndex.size(); i++)
	{
		indexList[i] = vecIndex[i];
	}
	m_pIndexBuffer->Unlock();
}

void cOBB::Setup(TriggerBox* pTrigger)
{
	D3DXVECTOR3 vMin = pTrigger->GetMin();
	D3DXVECTOR3 vMax = pTrigger->GetMax();
	m_vOrgCenterPos = (vMin + vMax) / 2.0f;

	m_vOrgAxisDir[0] = D3DXVECTOR3(1, 0, 0);
	m_vOrgAxisDir[1] = D3DXVECTOR3(0, 1, 0);
	m_vOrgAxisDir[2] = D3DXVECTOR3(0, 0, 1);

	m_fAxisLen[0] = fabs(vMax.x - vMin.x);
	m_fAxisLen[1] = fabs(vMax.y - vMin.y);
	m_fAxisLen[2] = fabs(vMax.z - vMin.z);

	m_fAxisHalfLen[0] = m_fAxisLen[0] / 2.0f;
	m_fAxisHalfLen[1] = m_fAxisLen[1] / 2.0f;
	m_fAxisHalfLen[2] = m_fAxisLen[2] / 2.0f;

	D3DXMatrixIdentity(&m_matWorldTM);

	vector<ST_PC_VERTEX> vecVertex;
	vector<int> vecIndex;


	ST_PC_VERTEX v;
	v.p = D3DXVECTOR3(m_vOrgCenterPos.x + m_fAxisHalfLen[0], m_vOrgCenterPos.y + m_fAxisHalfLen[1], m_vOrgCenterPos.z + m_fAxisHalfLen[2]);	//FRT
	v.c = D3DCOLOR_XRGB(255, 255, 255);
	vecVertex.push_back(v);
	v.p = D3DXVECTOR3(m_vOrgCenterPos.x + m_fAxisHalfLen[0], m_vOrgCenterPos.y - m_fAxisHalfLen[1], m_vOrgCenterPos.z + m_fAxisHalfLen[2]);		//FRB
	vecVertex.push_back(v);
	v.p = D3DXVECTOR3(m_vOrgCenterPos.x - m_fAxisHalfLen[0], m_vOrgCenterPos.y - m_fAxisHalfLen[1], m_vOrgCenterPos.z + m_fAxisHalfLen[2]);		//FLB
	vecVertex.push_back(v);
	v.p = D3DXVECTOR3(m_vOrgCenterPos.x - m_fAxisHalfLen[0], m_vOrgCenterPos.y + m_fAxisHalfLen[1], m_vOrgCenterPos.z + m_fAxisHalfLen[2]);		//FLT
	vecVertex.push_back(v);
	v.p = D3DXVECTOR3(m_vOrgCenterPos.x + m_fAxisHalfLen[0], m_vOrgCenterPos.y + m_fAxisHalfLen[1], m_vOrgCenterPos.z - m_fAxisHalfLen[2]);		//BRT
	vecVertex.push_back(v);
	v.p = D3DXVECTOR3(m_vOrgCenterPos.x + m_fAxisHalfLen[0], m_vOrgCenterPos.y - m_fAxisHalfLen[1], m_vOrgCenterPos.z - m_fAxisHalfLen[2]);		//BRB
	vecVertex.push_back(v);
	v.p = D3DXVECTOR3(m_vOrgCenterPos.x - m_fAxisHalfLen[0], m_vOrgCenterPos.y - m_fAxisHalfLen[1], m_vOrgCenterPos.z - m_fAxisHalfLen[2]);		//BLB
	vecVertex.push_back(v);
	v.p = D3DXVECTOR3(m_vOrgCenterPos.x - m_fAxisHalfLen[0], m_vOrgCenterPos.y + m_fAxisHalfLen[1], m_vOrgCenterPos.z - m_fAxisHalfLen[2]);		//BLT
	vecVertex.push_back(v);
	
	//
	// Vertex Indices for OBB (Triangle)
	//front
	vecIndex.push_back(0);	vecIndex.push_back(1);	vecIndex.push_back(2);
	vecIndex.push_back(0);	vecIndex.push_back(2);	vecIndex.push_back(3);

	// back
	vecIndex.push_back(4);	vecIndex.push_back(6);	vecIndex.push_back(5);
	vecIndex.push_back(4);	vecIndex.push_back(7);	vecIndex.push_back(6);

	//right
	vecIndex.push_back(4);	vecIndex.push_back(5);	vecIndex.push_back(1);
	vecIndex.push_back(4);	vecIndex.push_back(1);	vecIndex.push_back(0);

	// left
	vecIndex.push_back(3);	vecIndex.push_back(2);	vecIndex.push_back(6);
	vecIndex.push_back(3);	vecIndex.push_back(6);	vecIndex.push_back(7);

	// bottom
	vecIndex.push_back(1);	vecIndex.push_back(5);	vecIndex.push_back(6);
	vecIndex.push_back(1);	vecIndex.push_back(6);	vecIndex.push_back(2);

	//top
	vecIndex.push_back(4);	vecIndex.push_back(0);	vecIndex.push_back(3);
	vecIndex.push_back(4);	vecIndex.push_back(3);	vecIndex.push_back(7);

	

	int size = sizeof(ST_PC_VERTEX) * vecVertex.size();
	HRESULT asd = g_pDevice->CreateVertexBuffer(size, 0, ST_PC_VERTEX::FVF, D3DPOOL_DEFAULT, &m_pVertexBuffer, 0);
	HRESULT ede = g_pDevice->CreateIndexBuffer(vecIndex.size() * sizeof(WORD), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIndexBuffer, 0);

	ST_PC_VERTEX* vertices;
	m_pVertexBuffer->Lock(0, 0, (void**)&vertices, 0);
	for (int i = 0; i < vecVertex.size(); i++)
	{
		vertices[i] = vecVertex[i];
	}
	m_pVertexBuffer->Unlock();

	WORD* indexList;
	m_pIndexBuffer->Lock(0, 0, (void**)&indexList, 0);
	for (int i = 0; i < vecIndex.size(); i++)
	{
		indexList[i] = vecIndex[i];
	}
	m_pIndexBuffer->Unlock();
}

void cOBB::Update(D3DXMATRIXA16 * pmatWorld)
{
	if (pmatWorld)
		m_matWorldTM = *pmatWorld;

	for (int i = 0; i < 3; ++i)
	{
		D3DXVec3TransformNormal(&m_vAxisDir[i], &m_vOrgAxisDir[i], &m_matWorldTM);
	}

	D3DXVec3TransformCoord(&m_vCenterPos, &m_vOrgCenterPos, &m_matWorldTM);
}

bool cOBB::isCollision(cOBB * pOBB1, cOBB * pOBB2)
{
	float cos[3][3];
	float absCos[3][3];
	float dist[3];
	float r0, r1, r;

	const float cutOff = 0.9999999f;
	bool existsParallelPair = false;

	D3DXVECTOR3 D = pOBB2->m_vCenterPos - pOBB1->m_vCenterPos;

	for (int a = 0; a < 3; a++)
	{
		for (int b = 0; b < 3; b++)
		{
			cos[a][b] = D3DXVec3Dot(&pOBB1->m_vAxisDir[a], &pOBB2->m_vAxisDir[b]);
			absCos[a][b] = abs(cos[a][b]);
			if (absCos[a][b] > cutOff)
				existsParallelPair = true;
		}

		dist[a] = D3DXVec3Dot(&pOBB1->m_vAxisDir[a], &D);
		r = abs(dist[a]);

		r0 = pOBB1->m_fAxisHalfLen[a];

		r1 = pOBB2->m_fAxisHalfLen[0] * absCos[a][0] + pOBB2->m_fAxisHalfLen[1] * absCos[a][1] + pOBB2->m_fAxisHalfLen[2] * absCos[a][2];

		if (r > r0 + r1)
			return false;
	}

	for (int b = 0; b < 3; b++)
	{
		r = abs(D3DXVec3Dot(&pOBB2->m_vAxisDir[b], &D));
		r0 = pOBB1->m_fAxisHalfLen[0] * absCos[0][b] + pOBB1->m_fAxisHalfLen[1] * absCos[1][b] + pOBB1->m_fAxisHalfLen[2] * absCos[2][b];
		r1 = pOBB2->m_fAxisHalfLen[b];
		if (r > r0 + r1)
			return false;
	}

	if (existsParallelPair)
		return true;

	{
		r = abs(dist[0] * cos[2][0] - dist[2] * cos[0][0]);
		r0 = pOBB1->m_fAxisHalfLen[0] * absCos[2][0] + pOBB1->m_fAxisHalfLen[2] * absCos[0][0];
		r1 = pOBB2->m_fAxisHalfLen[1] * absCos[1][2] + pOBB2->m_fAxisHalfLen[2] * absCos[1][1];
		if (r > r0 + r1)
			return false;

		r = abs(dist[0] * cos[2][1] - dist[2] * cos[0][1]);
		r0 = pOBB1->m_fAxisHalfLen[0] * absCos[2][1] + pOBB1->m_fAxisHalfLen[2] * absCos[0][1];
		r1 = pOBB2->m_fAxisHalfLen[0] * absCos[1][2] + pOBB2->m_fAxisHalfLen[2] * absCos[1][0];
		if (r > r0 + r1)
			return false;

		r = abs(dist[0] * cos[2][2] - dist[2] * cos[0][2]);
		r0 = pOBB1->m_fAxisHalfLen[0] * absCos[2][2] + pOBB1->m_fAxisHalfLen[2] * absCos[0][2];
		r1 = pOBB2->m_fAxisHalfLen[0] * absCos[1][1] + pOBB2->m_fAxisHalfLen[1] * absCos[1][0];
		if (r > r0 + r1)
			return false;
	}

	{
		r = abs(dist[1] * cos[0][0] - dist[0] * cos[1][0]);
		r0 = pOBB1->m_fAxisHalfLen[0] * absCos[1][0] + pOBB1->m_fAxisHalfLen[1] * absCos[0][0];
		r1 = pOBB2->m_fAxisHalfLen[1] * absCos[2][2] + pOBB2->m_fAxisHalfLen[2] * absCos[2][1];
		if (r > r0 + r1)
			return false;

		r = abs(dist[1] * cos[0][1] - dist[0] * cos[1][1]);
		r0 = pOBB1->m_fAxisHalfLen[0] * absCos[1][1] + pOBB1->m_fAxisHalfLen[1] * absCos[0][1];
		r1 = pOBB2->m_fAxisHalfLen[0] * absCos[2][2] + pOBB2->m_fAxisHalfLen[2] * absCos[2][0];
		if (r > r0 + r1)
			return false;

		r = abs(dist[1] * cos[0][2] - dist[0] * cos[1][2]);
		r0 = pOBB1->m_fAxisHalfLen[0] * absCos[1][2] + pOBB1->m_fAxisHalfLen[1] * absCos[0][2];
		r1 = pOBB2->m_fAxisHalfLen[0] * absCos[2][1] + pOBB2->m_fAxisHalfLen[1] * absCos[2][0];
		if (r > r0 + r1)
			return false;
	}

	{
		r = abs(dist[2] * cos[1][0] - dist[1] * cos[2][0]);
		r0 = pOBB1->m_fAxisHalfLen[1] * absCos[2][0] + pOBB1->m_fAxisHalfLen[2] * absCos[1][0];
		r1 = pOBB2->m_fAxisHalfLen[1] * absCos[0][2] + pOBB2->m_fAxisHalfLen[2] * absCos[0][1];
		if (r > r0 + r1)
			return false;

		r = abs(dist[2] * cos[1][1] - dist[1] * cos[2][1]);
		r0 = pOBB1->m_fAxisHalfLen[1] * absCos[1][1] + pOBB1->m_fAxisHalfLen[2] * absCos[1][1];
		r1 = pOBB2->m_fAxisHalfLen[0] * absCos[0][2] + pOBB2->m_fAxisHalfLen[2] * absCos[0][0];
		if (r > r0 + r1)
			return false;

		r = abs(dist[2] * cos[1][2] - dist[1] * cos[2][2]);
		r0 = pOBB1->m_fAxisHalfLen[1] * absCos[2][2] + pOBB1->m_fAxisHalfLen[2] * absCos[1][2];
		r1 = pOBB2->m_fAxisHalfLen[0] * absCos[0][1] + pOBB2->m_fAxisHalfLen[1] * absCos[0][0];
		if (r > r0 + r1)
			return false;
	}


	return true;
}

void cOBB::OBBBox_Render(D3DXCOLOR c)
{
	ST_PC_VERTEX* vertices;
	m_pVertexBuffer->Lock(0, 0, (void**)&vertices, 0);
	for (int i = 0; i < 8; i++)
	{
		vertices[i].c = c;
	}
	m_pVertexBuffer->Unlock();

	g_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	g_pDevice->SetFVF(ST_PC_VERTEX::FVF);
	g_pDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(ST_PC_VERTEX));
	g_pDevice->SetIndices(m_pIndexBuffer);
	g_pDevice->SetTransform(D3DTS_WORLD, &m_matWorldTM);
	g_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	g_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 36);
	g_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	//m_pMeshBOX->DrawSubset(0);
	g_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

D3DXMATRIXA16 cOBB::GetMatrix_Collision()
{	
	return m_matWorldTM;
}
