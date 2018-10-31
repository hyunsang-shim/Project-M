#include "stdafx.h"
#include "cNewObject.h"

cNewObject::cNewObject() : m_pTexture(NULL), v_translation(0, 0, 0)
{
	D3DXMatrixIdentity(&m_matWorld);
}


cNewObject::~cNewObject()
{
}

void cNewObject::Setup(string filePath)
{
	m_mtlColor.Emissive.r = 0.0f;
	m_mtlColor.Emissive.g = 0.0f;
	m_mtlColor.Emissive.b = 0.0f;
	m_mtlColor.Emissive.a = 1.0f;
	//FileLoad("box.obj");

	char * writable = new char[filePath.size() + 1];
	std::copy(filePath.begin(), filePath.end(), writable);
	writable[filePath.size()] = '\0'; 

	FileLoad(writable);

	delete[] writable;

	D3DXCreateMeshFVF(
		m_vecVertex.size() / 3,
		m_vecVertex.size(),
		D3DXMESH_MANAGED,
		ST_PNT_VERTEX::FVF,
		g_pDevice,
		&m_pMesh
	);

	ST_PNT_VERTEX* vertex;
	m_pMesh->LockVertexBuffer(0, (void**)&vertex);
	memcpy(vertex, &m_vecVertex[0], m_vecVertex.size() * sizeof(ST_PNT_VERTEX));
	m_pMesh->UnlockVertexBuffer();

	WORD* Index = 0;
	m_pMesh->LockIndexBuffer(0, (void**)&Index);
	for (int i = 0; i < m_vecVertex.size(); i++)
	{
		Index[i] = i;
	}
	m_pMesh->UnlockIndexBuffer();


	DWORD* Attribute = 0;
	m_pMesh->LockAttributeBuffer(0, &Attribute);
	memcpy(Attribute, &m_vecAttribute[0], m_vecAttribute.size() * sizeof(DWORD));
	m_pMesh->UnlockAttributeBuffer();

}

void cNewObject::Updata()
{

}

void cNewObject::Render()
{
	D3DXMATRIXA16 matSRT, BoxR, BoxT, BoxS, BoxRY, tmp;

	D3DXMatrixScaling(&BoxS, 0.01, 0.01, 0.01);
	D3DXMatrixRotationYawPitchRoll(&BoxR, D3DX_PI / 2, -D3DX_PI / 2, 0);
	D3DXVec3TransformNormal(&v_BoxLookAt, &D3DXVECTOR3(0, 0, 1), &BoxR);
	D3DXMatrixTranslation(&BoxT, v_translation.x, v_translation.y, v_translation.z);

	tmp = BoxS * BoxR * BoxT* m_matWorld;

	g_pDevice->SetTransform(D3DTS_WORLD, &tmp);

	for (int i = 0; i < m_vecMLT.size(); i++)
	{
		g_pDevice->SetMaterial(&m_vecMLT[i].m_mtlColor);
		g_pDevice->SetTexture(0, m_vecMLT[i].m_pTexture);
		m_pMesh->DrawSubset(i);
	}
	g_pDevice->SetTexture(0, NULL);
}

void cNewObject::FileLoad(char * FileName)
{
	char buf[1024];

	char mtbuf[1024];

	FILE* fp;

	fopen_s(&fp, FileName, "r");

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
			ProcessMtl(mltBuf);
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

			for (int j = 0; j < m_vecMLT.size(); j++)
			{
				if (strcmp(mtbuf, m_vecMLT[j].c) == 0)
				{
					m_vecAttribute.push_back(j);
				}
			}
		}
	}
	fclose(fp);
}

void cNewObject::ProcessMtl(char * FileName)
{
	char buf[1024];

	FILE* fp;
	MLT_GROUP m;

	fopen_s(&fp, FileName, "r");

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

			wchar_t wt[1024] = { 0 };
			::MultiByteToWideChar(CP_ACP, NULL, c, -1, wt, strlen(c));

			D3DXCreateTextureFromFileW(g_pDevice, wt, &m_pTexture);

			m.m_mtlColor = m_mtlColor;
			m.m_pTexture = m_pTexture;

			m_vecMLT.push_back(m);
		}
	}

	fclose(fp);
}

bool cNewObject::StartWith(char * FindStr, char * SearchStr)
{
	char* temp = strstr(FindStr, SearchStr);
	if (temp == FindStr)
		return true;
	return false;
}

void cNewObject::SetSRT(D3DXVECTOR3 vScale, D3DXVECTOR3 vRot, D3DXVECTOR3 vPos)
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
