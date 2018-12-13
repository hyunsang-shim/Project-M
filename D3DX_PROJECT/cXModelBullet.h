#pragma once

class cOBB;
struct MLT_GROUP;

class cXModelBullet
{
public:
	cXModelBullet();
	~cXModelBullet();
private:
	D3DXVECTOR3 m_vRot;
	D3DXVECTOR3 m_vPos;
	D3DXVECTOR3 m_vScale;
	D3DXMATRIXA16 m_matWorld;
	vector<D3DMATERIAL9> m_vecMtl;
	vector<LPDIRECT3DTEXTURE9> m_vecTextuer;
	SYNTHESIZE(LPD3DXMESH, m_pXMesh, XMESH);
	SYNTHESIZE(cOBB*, m_pOBB, OBB);
	SYNTHESIZE(D3DXVECTOR3, m_vMin, Min);
	SYNTHESIZE(D3DXVECTOR3, m_vMax, Max);

	vector<ST_PNT_VERTEX> m_vecVertex;
	vector<D3DXVECTOR3> m_vecPoint;
	vector<D3DXVECTOR2> m_vecTexture;
	vector<D3DXVECTOR3> m_vecNomal;
	vector<MLT_GROUP> m_vecMLT;
	vector<DWORD> m_vecAttribute;
	map<string, DWORD> m_MtlNameComp;
	D3DMATERIAL9 m_mtlColor;

public:
	void Setup(string filePath);
	void FileLoad(char* szFolder, char* szFile);
	void ProcessMtl(char* szFolder, char* szFile);
	void Update();
	void Render();
	void SetSRT(D3DXVECTOR3 vRot, D3DXVECTOR3 vPos);
	bool StartWith(char* FindStr, char* SearchStr);
};

