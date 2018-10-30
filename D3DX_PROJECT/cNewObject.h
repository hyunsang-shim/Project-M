#pragma once
struct MLT_GROUP
{
	char c[1024];
	D3DMATERIAL9 m_mtlColor;
	LPDIRECT3DTEXTURE9 m_pTexture;
};

class cNewObject
{
public:
	cNewObject();
	~cNewObject();
public:
	void Setup();
	void Updata();
	void Render();
	void FileLoad(char* FileName);
	void ProcessMtl(char* FileName);
	bool StartWith(char* FindStr, char* SearchStr);
	D3DMATERIAL9 m_mtlColor;
private:
	vector<MLT_GROUP> m_vecMLT;
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
	LPD3DXMESH m_pMesh;


	int m_nNumLine;
};

