#include "stdafx.h"
#include "cXModelSurface.h"


cXModelSurface::cXModelSurface()
	:m_vPos(0, 0, 0)
	, m_vRot(0, 0, 0)
	, m_vScale(1, 1, 1)
	, m_pSXMesh(NULL)
{
}

cXModelSurface::cXModelSurface(string filePath)
{
	HRESULT hr = 0;

	ID3DXBuffer* adjBuffer = 0;
	ID3DXBuffer* mtrlBuffer = 0;
	DWORD numMtrls = 0;

	D3DXLoadMeshFromXA(
		filePath.c_str(),
		D3DXMESH_MANAGED | D3DXMESH_32BIT,
		g_pDevice,
		&adjBuffer,
		&mtrlBuffer,
		0,
		&numMtrls,
		&m_pSXMesh
	);



	if (mtrlBuffer != 0 && numMtrls != 0)
	{
		D3DXMATERIAL* mtrls = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();

		for (int i = 0; i < numMtrls; i++)
		{
			mtrls[i].MatD3D.Ambient = mtrls[i].MatD3D.Diffuse;
			m_vecMtl.push_back(mtrls[i].MatD3D);

			if (mtrls[i].pTextureFilename != 0)
			{
				string tmp;
				tmp += "map/textures/";
				tmp += mtrls[i].pTextureFilename;
				LPDIRECT3DTEXTURE9 tex = g_pTextureManager->GetTexture(tmp);
				m_vecTextuer.push_back(tex);
			}
			else
			{
				m_vecTextuer.push_back(0);
			}
		}
	}



	std::vector<DWORD> vecAdj(adjBuffer->GetBufferSize());
	HRESULT thr = m_pSXMesh->GenerateAdjacency(0.0f, &vecAdj[0]);

	m_pSXMesh->OptimizeInplace(
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT |
		D3DXMESHOPT_VERTEXCACHE,
		&vecAdj[0],
		0, 0, 0);


	SAFE_RELEASE(mtrlBuffer);
	SAFE_RELEASE(adjBuffer);

	D3DXMatrixIdentity(&m_matWorld);
}


cXModelSurface::~cXModelSurface()
{
	SAFE_RELEASE(m_pSXMesh);
}

void cXModelSurface::Render()
{
	g_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	/*g_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	g_pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);*/
	g_pDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
	for (int i = 0; i < m_vecMtl.size(); i++)
	{
		g_pDevice->SetMaterial(&m_vecMtl[i]);
		g_pDevice->SetTexture(0, m_vecTextuer[i]);
		m_pSXMesh->DrawSubset(i);
	}
}

bool cXModelSurface::GetY(IN float x, OUT float & y, IN float z, D3DXVECTOR3 HeadPos)
{
	BOOL hit = false;
	BOOL hit3 = false;
	float dist = 0.0f;
	float dist3 = 0.0f;
	//float start = y + 12.0f;
	float before = y;

	D3DXVECTOR3 HeadPosThree;

	HeadPosThree.x = HeadPos.x;
	HeadPosThree.y = HeadPos.y + 4.0f;
	HeadPosThree.z = HeadPos.z;

	D3DXIntersect(m_pSXMesh, &HeadPos, &D3DXVECTOR3(0, -1, 0), &hit, NULL, NULL, NULL, &dist, NULL, NULL);
	D3DXIntersect(m_pSXMesh, &HeadPosThree, &D3DXVECTOR3(0, -1, 0), &hit3, NULL, NULL, NULL, &dist3, NULL, NULL);

	if (dist > dist3)
	{
		return false;
	}
	
	if (hit)
	{
		y = float(HeadPos.y) - dist;
	}
	else
	{
		y = HeadPos.y;
	}
	return true;
}

bool cXModelSurface::GetAIY(IN float x, OUT float & y, IN float z, D3DXVECTOR3 HeadPos)
{
	BOOL hit = false;
	float dist = 0.0f;
	//float start = y + 12.0f;
	float before = y;

	D3DXIntersect(m_pSXMesh, &HeadPos, &D3DXVECTOR3(0, -1, 0), &hit, NULL, NULL, NULL, &dist, NULL, NULL);

	if (hit)
	{
		y = float(HeadPos.y) - dist;
	}
	else
	{
		y = HeadPos.y;
	}
	return true;
}
