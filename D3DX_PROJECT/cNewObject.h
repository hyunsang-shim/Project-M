#pragma once

class cOBB;

struct MLT_GROUP
{
	char c[1024];
	D3DMATERIAL9 m_mtlColor;
	LPDIRECT3DTEXTURE9 m_pTexture;
	bool operator==(const char value[])
	{
		if (strcmp(value, c) == 0)
		{
			return true;
		}
		return false;
	}
};

class cNewObject
{
public:
	cNewObject();
	~cNewObject();
public:
	void Setup(string filePath);
	void Setup(char* Path, char* Filename);
	void Updata();
	void Render();
	void FileLoad(char* szFolder, char* szFile);
	void ProcessMtl(char* szFolder, char* szFile);
	bool StartWith(char* FindStr, char* SearchStr);
	void SetSRT(D3DXVECTOR3 vScale, D3DXVECTOR3 vRot, D3DXVECTOR3 vPos);
	cOBB * GetOBB();
	D3DMATERIAL9 m_mtlColor;
	bool GetY(IN float x, OUT float &y, IN float z, D3DXVECTOR3 HeadPos);		// 하이트맵 높이값

private:
	vector<MLT_GROUP> m_vecMLT;
	map<string, DWORD> m_MtlNameComp;
	vector<ST_PNT_VERTEX> m_vecVertex;
	vector<D3DXVECTOR3> m_vecPoint;
	vector<D3DXVECTOR2> m_vecTexture;
	vector<D3DXVECTOR3> m_vecNomal;
	LPDIRECT3DTEXTURE9 m_pTexture;
	D3DXMATRIXA16 m_matWorld;
	float Rotation = D3DX_PI;
	D3DXVECTOR3 v_BoxLookAt;
	D3DXVECTOR3 v_translation;

	vector<DWORD> m_vecAttribute;
	SYNTHESIZE(LPD3DXMESH, m_pMesh, MESH);

	D3DXVECTOR3 m_vRot;
	D3DXVECTOR3 m_vPos;
	D3DXVECTOR3 m_vScale;

	int m_nNumLine;
	float m_fFallSpeed;


	SYNTHESIZE(D3DXVECTOR3, m_vMin, Min);
	SYNTHESIZE(D3DXVECTOR3, m_vMax, Max);
	cOBB* m_pOBB;
};

