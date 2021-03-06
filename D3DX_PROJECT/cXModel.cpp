#include "stdafx.h"
#include "cXModel.h"
#include "cDeviceManager.h"

cXModel::cXModel()
	:m_vPos(0, 0, 0)
	, m_vRot(0, 0, 0)
	, m_vScale(1, 1, 1)
	, m_pXMesh(NULL)
{
}

LPCWSTR s2ws(string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r.c_str();
}
cXModel::cXModel(string filePath)
{
	HRESULT hr = 0;

	ID3DXBuffer* adjBuffer = 0;
	ID3DXBuffer* mtrlBuffer = 0;
	DWORD numMtrls = 0;
	m_pXMesh = NULL;

	D3DXLoadMeshFromX(
		filePath.c_str(),
		D3DXMESH_MANAGED | D3DXMESH_32BIT,
		g_pDevice,
		&adjBuffer,
		&mtrlBuffer,
		0,
		&numMtrls,
		&m_pXMesh
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
	HRESULT thr = m_pXMesh->GenerateAdjacency(0.0f, &vecAdj[0]);

	m_pXMesh->OptimizeInplace(
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT |
		D3DXMESHOPT_VERTEXCACHE,
		&vecAdj[0],
		0, 0, 0);


	SAFE_RELEASE(mtrlBuffer);
	SAFE_RELEASE(adjBuffer);

	D3DXMatrixIdentity(&m_matWorld);
}


cXModel::~cXModel()
{
	SAFE_RELEASE(m_pXMesh);
}

void cXModel::SetSRT(D3DXVECTOR3 vScale, D3DXVECTOR3 vRot, D3DXVECTOR3 vPos)
{
	m_vPos = vPos;
	m_vScale = vScale;
	m_vRot = vRot;
	D3DXMATRIXA16 matT, matS, matR, matX, matY, matZ;
	D3DXMatrixTranslation(&matT, m_vPos.x, m_vPos.y, m_vPos.z);
	D3DXMatrixScaling(&matS, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixRotationX(&matX, m_vRot.x);
	D3DXMatrixRotationY(&matY, m_vRot.y);
	D3DXMatrixRotationZ(&matZ, m_vRot.z);
	matR = matZ * matX * matY;
	m_matWorld = matS * matR * matT;
}

void cXModel::Update()
{

}

void cXModel::Render()
{
	g_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	/*g_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	g_pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);*/
	g_pDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
	for (int i = 0; i < m_vecMtl.size(); i++)
	{
		g_pDevice->SetMaterial(&m_vecMtl[i]);
		g_pDevice->SetTexture(0, m_vecTextuer[i]);
		m_pXMesh->DrawSubset(i);
	}
}

bool cXModel::GetY(IN float x, OUT float & y, IN float z, D3DXVECTOR3 HeadPos)
{
	BOOL hit = false;
	BOOL hit2 = false;
	float dist = 0.0f;
	float dist2 = 0.0f;
	//float start = y + 12.0f;
	float before = y;

	D3DXVECTOR3 HeadPosDouble;
	HeadPosDouble.x = HeadPos.x;
	HeadPosDouble.y = HeadPos.y + 10.0f;
	HeadPosDouble.z = HeadPos.z;

	D3DXIntersect(m_pXMesh, &HeadPos, &D3DXVECTOR3(0, -1, 0), &hit, NULL, NULL, NULL, &dist, NULL, NULL);
	D3DXIntersect(m_pXMesh, &HeadPosDouble, &D3DXVECTOR3(0, -1, 0), &hit2, NULL, NULL, NULL, &dist2, NULL, NULL);
	
	if (dist > dist2)
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
