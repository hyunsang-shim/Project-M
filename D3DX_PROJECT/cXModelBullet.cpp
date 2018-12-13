#include "stdafx.h"
#include "cXModelBullet.h"
#include "cOBB.h"
#include "cNewObject.h"

cXModelBullet::cXModelBullet()
{
}


cXModelBullet::~cXModelBullet()
{
}

void cXModelBullet::Setup(string filePath)
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

		D3DXVECTOR3 vMin(0, 0, 0), vMax(0, 0, 0);

	LPVOID pV = NULL;
	m_pXMesh->LockVertexBuffer(0, &pV);
	D3DXComputeBoundingBox((D3DXVECTOR3*)pV,
		m_pXMesh->GetNumVertices(),
		D3DXGetFVFVertexSize(m_pXMesh->GetFVF()),
		&vMin,
		&vMax);

	/*D3DXVec3Minimize(&m_vMin, &m_vMin, &vMin);
	D3DXVec3Maximize(&m_vMax, &m_vMax, &vMax);*/

	m_vMin = vMin;
	m_vMax = vMax;

	m_pXMesh->UnlockVertexBuffer();

	if (mtrlBuffer != 0 && numMtrls != 0)
	{
		D3DXMATERIAL* mtrls = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();

		for (int i = 0; i < numMtrls; i++)
		{
			mtrls[i].MatD3D.Ambient = mtrls[i].MatD3D.Diffuse;
			m_vecMtl.push_back(mtrls[i].MatD3D);

			if (mtrls[i].pTextureFilename != 0)
			{
				LPDIRECT3DTEXTURE9 tex = g_pTextureManager->GetTexture(mtrls[i].pTextureFilename);
				//D3DXCreateTextureFromFileA(g_pDevice, mtrls[i].pTextureFilename, &tex);
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


	//m_mtlColor.Emissive.r = 0.0f;
	//m_mtlColor.Emissive.g = 0.0f;
	//m_mtlColor.Emissive.b = 0.0f;
	//m_mtlColor.Emissive.a = 1.0f;

	//FileLoad(szFolder, szFile);

	//D3DXCreateMeshFVF(
	//	m_vecAttribute.size(),
	//	m_vecVertex.size(),
	//	D3DXMESH_MANAGED | D3DXMESH_32BIT,
	//	ST_PNT_VERTEX::FVF,
	//	g_pDevice,
	//	&m_pXMesh
	//);

	//ST_PNT_VERTEX* vertex;
	//m_pXMesh->LockVertexBuffer(0, (void**)&vertex);
	//memcpy(vertex, &m_vecVertex[0], m_vecVertex.size() * sizeof(ST_PNT_VERTEX));
	//m_pXMesh->UnlockVertexBuffer();

	//DWORD* Index = 0;
	//m_pXMesh->LockIndexBuffer(0, (void**)&Index);
	//for (int i = 0; i < m_vecVertex.size(); i++)
	//{
	//	Index[i] = i;
	//}
	//m_pXMesh->UnlockIndexBuffer();


	//DWORD* Attribute = 0;
	//m_pXMesh->LockAttributeBuffer(0, &Attribute);
	//memcpy(Attribute, &m_vecAttribute[0], m_vecAttribute.size() * sizeof(DWORD));
	//m_pXMesh->UnlockAttributeBuffer();

	//std::vector<DWORD> vecAdj(m_vecVertex.size());
	//m_pXMesh->GenerateAdjacency(0.0f, &vecAdj[0]);

	//m_pXMesh->OptimizeInplace(
	//	D3DXMESHOPT_ATTRSORT |
	//	D3DXMESHOPT_COMPACT |
	//	D3DXMESHOPT_VERTEXCACHE,
	//	&vecAdj[0],
	//	0, 0, 0);

	D3DXMatrixIdentity(&m_matWorld);

	m_pOBB = new cOBB;
	m_pOBB->Setup(this);
}

void cXModelBullet::FileLoad(char * szFolder, char * szFile)
{
	std::string sFullPath(szFolder);
	sFullPath += (std::string("/") + std::string(szFile));

	char buf[1024];

	char mtbuf[1024];

	FILE* fp;

	fopen_s(&fp, sFullPath.c_str(), "r");

	while (!feof(fp))
	{
		fgets(buf, 1024, fp);

		if (buf[0] == '#')
		{
			continue;
		}
		else if (StartWith(buf, "usemtl"))
		{
			sscanf_s(buf, "%*s %s", mtbuf, 1024);
		}
		else if (StartWith(buf, "mtllib"))
		{
			char mltBuf[1024];

			sscanf_s(buf, "%*s %s", mltBuf, 1024);

			ProcessMtl(szFolder, mltBuf);
		}
		else if (StartWith(buf, "vt"))
		{
			float u, v;
			sscanf_s(buf, "%*s %f %f %*f", &u, &v);
			m_vecTexture.push_back(D3DXVECTOR2(u, v));
		}
		else if (StartWith(buf, "vn"))
		{
			float x, y, z;
			sscanf_s(buf, "%*s %f %f %f", &x, &y, &z);
			m_vecNomal.push_back(D3DXVECTOR3(x, y, z));
		}
		else if (StartWith(buf, "v"))
		{
			float x, y, z;
			sscanf_s(buf, "%*s %f %f %f", &x, &y, &z);
			m_vecPoint.push_back(D3DXVECTOR3(x, y, z));
		}
		else if (StartWith(buf, "f"))
		{
			int Index[3][3];

			sscanf_s(buf, "%*s %d/%d/%d %d/%d/%d %d/%d/%d",
				&Index[0][0], &Index[0][1], &Index[0][2],
				&Index[1][0], &Index[1][1], &Index[1][2],
				&Index[2][0], &Index[2][1], &Index[2][2]
			);

			for (int i = 0; i < 3; i++)
			{
				ST_PNT_VERTEX v;
				v.p = m_vecPoint[Index[i][0] - 1];
				v.t = m_vecTexture[Index[i][1] - 1];
				v.n = m_vecNomal[Index[i][2] - 1];
				m_vecVertex.push_back(v);
			}

			m_vecAttribute.push_back(m_MtlNameComp[mtbuf]);
		}
	}
	fclose(fp);
}

void cXModelBullet::ProcessMtl(char * szFolder, char * szFile)
{
	std::string sFullPath(szFolder);
	sFullPath += (std::string("/") + std::string(szFile));

	char buf[1024];

	FILE* fp;
	MLT_GROUP m;
	DWORD count = 0;

	fopen_s(&fp, sFullPath.c_str(), "r");

	while (!feof(fp))
	{

		fgets(buf, 1024, fp);
		if (buf[0] == '#')
		{
			continue;
		}
		else if (StartWith(buf, "newmtl"))
		{
			char mltBuf[1024];

			sscanf_s(buf, "%*s %s", mltBuf, 1024);
			strcpy_s(m.c, mltBuf);

			if (find(m_vecMLT.begin(), m_vecMLT.end(), mltBuf) == m_vecMLT.end())
			{
				m_MtlNameComp.insert(pair<string, DWORD>(mltBuf, count));
				count++;
			}
		}
		else if (StartWith(buf, "Ka"))
		{
			float r, g, b;
			sscanf_s(buf, "%*s %f %f %f", &r, &g, &b);

			m_mtlColor.Ambient = D3DXCOLOR(r, g, b, 1.0f);
		}
		else if (StartWith(buf, "Kd"))
		{
			float r, g, b;
			sscanf_s(buf, "%*s %f %f %f", &r, &g, &b);
			m_mtlColor.Diffuse = D3DXCOLOR(r, g, b, 1.0f);
		}
		else if (StartWith(buf, "Ks"))
		{
			float r, g, b;
			sscanf_s(buf, "%*s %f %f %f", &r, &g, &b);
			m_mtlColor.Specular = D3DXCOLOR(r, g, b, 1.0f);
		}
		else if (StartWith(buf, "d"))
		{
			float power;
			sscanf_s(buf, "%*s %f", &power);
			m_mtlColor.Power = power;
		}
		else if (StartWith(buf, "map_Kd"))
		{
			char c[1024];
			sscanf_s(buf, "%*s %s", c, 1024);

			sFullPath = std::string(szFolder);
			sFullPath += (std::string("/") + std::string(c));

			/*wchar_t wt[1024] = { 0 };
			::MultiByteToWideChar(CP_ACP, NULL, c, -1, wt, strlen(c));

			D3DXCreateTextureFromFileW(g_pDevice, wt, &m_pTexture);*/

			LPDIRECT3DTEXTURE9 pTexture = g_pTextureManager->GetTexture(sFullPath);

			m.m_mtlColor = m_mtlColor;
			m.m_pTexture = pTexture;

			m_vecMLT.push_back(m);
		}
	}

	fclose(fp);
}

void cXModelBullet::Update()
{
	m_pOBB->Update(&m_matWorld);
}

void cXModelBullet::Render()
{
	g_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	g_pDevice->SetRenderState(D3DRS_LIGHTING, false);
	/*g_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	g_pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);*/
	g_pDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
	for (int i = 0; i < m_vecMtl.size(); i++)
	{
		g_pDevice->SetMaterial(&m_vecMtl[i]);
		g_pDevice->SetTexture(0, m_vecTextuer[i]);
		m_pXMesh->DrawSubset(i);
	}
	g_pDevice->SetTexture(0, NULL);
	g_pDevice->SetRenderState(D3DRS_LIGHTING, true);
	D3DCOLOR c = D3DCOLOR_XRGB(255, 255, 255);
	m_pOBB->OBBBox_Render(c);
}

void cXModelBullet::SetSRT(D3DXVECTOR3 vRot, D3DXVECTOR3 vPos)
{
	m_vPos = vPos;
	m_vRot = vRot;
	//D3DXVec3Normalize(&m_vRot, &m_vRot);

	D3DXMATRIXA16 matT, matS, matR, matX, matY, matZ;
	D3DXMatrixTranslation(&matT, m_vPos.x, m_vPos.y, m_vPos.z);
	D3DXMatrixRotationX(&matX, m_vRot.x);
	D3DXMatrixRotationY(&matY, m_vRot.y);
	D3DXMatrixRotationZ(&matZ, m_vRot.z);
	matR = matZ * matX;
	m_matWorld = matT;
}

bool cXModelBullet::StartWith(char * FindStr, char * SearchStr)
{
	char* temp = strstr(FindStr, SearchStr);
	if (temp == FindStr)
		return true;
	return false;
}
