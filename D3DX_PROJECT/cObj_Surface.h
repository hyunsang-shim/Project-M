#pragma once

class cObj_Surface
{
public:
	cObj_Surface();
	~cObj_Surface();
public:
	void Setup();
	void Updata();
	void Render();
	void FileLoad(char* FileName);
	void ProcessMtl(char* FileName);
	bool StartWith(char* FindStr, char* SearchStr);
	D3DMATERIAL9 m_mtlColor;
	vector<ST_PNT_VERTEX> m_vecVertex;
private:
	vector<D3DXVECTOR3> m_vecPoint;
	vector<D3DXVECTOR2> m_vecTexture;
	vector<D3DXVECTOR3> m_vecNomal;
	LPDIRECT3DTEXTURE9 m_pTexture;
	D3DXMATRIXA16 m_matWorld;
	float Rotation = D3DX_PI;
	D3DXVECTOR3 v_BoxLookAt;
	D3DXVECTOR3 v_translation;
};

