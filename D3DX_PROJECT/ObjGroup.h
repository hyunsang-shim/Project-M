#pragma once
class ObjGroup
{
public:
	ObjGroup();
	~ObjGroup();


	vector<ST_PNT_VERTEX> m_vecPoint;
	D3DMATERIAL9 mtrl;
	string mtrlName;
	string textureName;
	LPDIRECT3DTEXTURE9 m_pTexture;
	void setup();
	void render(D3DXMATRIXA16& m_matTotal);
};

