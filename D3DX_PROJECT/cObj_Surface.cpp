#include "stdafx.h"
#include "cObj_Surface.h"


cObj_Surface::cObj_Surface()
	: m_pTexture(NULL), v_translation(0, 0, 0)
{
	D3DXMatrixIdentity(&m_matWorld);
}


cObj_Surface::~cObj_Surface()
{
}

void cObj_Surface::Setup()
{
	m_mtlColor.Emissive.r = 0.0f;
	m_mtlColor.Emissive.g = 0.0f;
	m_mtlColor.Emissive.b = 0.0f;
	m_mtlColor.Emissive.a = 1.0f;
	//FileLoad("box.obj");
	FileLoad("map_surface.obj");
	
	D3DXMATRIXA16 matSRT, BoxR, BoxT, BoxS, BoxRY;

	//D3DXMatrixScaling(&BoxS , 0.2, 0.2, 0.2);
	D3DXMatrixScaling(&BoxS, 0.01, 0.01, 0.01);
	D3DXMatrixRotationYawPitchRoll(&BoxR, D3DX_PI / 2, -D3DX_PI / 2, 0);
	D3DXVec3TransformNormal(&v_BoxLookAt, &D3DXVECTOR3(0, 0, 1), &BoxR);
	//D3DXVec3Normalize(&v_BoxLookAt, &v_BoxLookAt);
	D3DXMatrixTranslation(&BoxT, v_translation.x, v_translation.y, v_translation.z);

	m_matWorld = BoxS * BoxR * BoxT;

	for (int i = 0; i < m_vecVertex.size(); i++)
	{
		D3DXVec3TransformCoord(&m_vecVertex[i].p, &m_vecVertex[i].p, &m_matWorld);
	}
}

void cObj_Surface::Updata()
{

}

void cObj_Surface::Render()
{
	g_pDevice->SetFVF(ST_PNT_VERTEX::FVF);
	g_pDevice->SetTransform(D3DTS_WORLD, &m_matWorld);

	g_pDevice->SetMaterial(&m_mtlColor);
	g_pDevice->SetTexture(0, m_pTexture);

	g_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_vecVertex.size() / 3, &m_vecVertex[0], sizeof(ST_PNT_VERTEX));

	g_pDevice->SetTexture(0, NULL);
}

void cObj_Surface::FileLoad(char * FileName)
{
	char buf[1024];

	FILE* fp;

	fopen_s(&fp, FileName, "r");

	while (!feof(fp))
	{
		fgets(buf, 1024, fp);

		if (buf[0] == '#')
		{
			continue;
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
		}
	}
	fclose(fp);
}

void cObj_Surface::ProcessMtl(char * FileName)
{
	char buf[1024];

	FILE* fp;

	fopen_s(&fp, FileName, "r");

	while (!feof(fp))
	{

		fgets(buf, 1024, fp);
		if (buf[0] == '#')
		{
			continue;
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
		}
	}

	fclose(fp);
}

bool cObj_Surface::StartWith(char * FindStr, char * SearchStr)
{
	char* temp = strstr(FindStr, SearchStr);
	if (temp == FindStr)
		return true;
	return false;
}
