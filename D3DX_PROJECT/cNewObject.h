#pragma once

class cOBB;

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
	void Setup(string filePath);
	void Updata();
	void Render();
	void FileLoad(char* FileName);
	void ProcessMtl(char* FileName);
	bool StartWith(char* FindStr, char* SearchStr);
	void SetSRT(D3DXVECTOR3 vScale, D3DXVECTOR3 vRot, D3DXVECTOR3 vPos);
	cOBB * GetOBB();
	D3DMATERIAL9 m_mtlColor;
	bool GetY(IN float x, OUT float &y, IN float z);

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

	D3DXVECTOR3 m_vRot;
	D3DXVECTOR3 m_vPos;
	D3DXVECTOR3 m_vScale;

	int m_nNumLine;
	int						m_nTileN;		// 캐릭터 높이 찾기


	SYNTHESIZE(D3DXVECTOR3, m_vMin, Min);
	SYNTHESIZE(D3DXVECTOR3, m_vMax, Max);
	cOBB* m_pOBB;
};

