#pragma once
#include "cCharacter.h"
class cXModelSurface
{
public:
	cXModelSurface();
	cXModelSurface(string filePath);
	~cXModelSurface();
private:
	D3DXVECTOR3 m_vRot;
	D3DXVECTOR3 m_vPos;
	D3DXVECTOR3 m_vScale;
	D3DXMATRIXA16 m_matWorld;
	vector<D3DMATERIAL9> m_vecMtl;
	vector<LPDIRECT3DTEXTURE9> m_vecTextuer;
	SYNTHESIZE(LPD3DXMESH, m_pSXMesh, SXMESH);
public:
	void Render();
	bool GetY(IN float x, OUT float &y, IN float z, D3DXVECTOR3 HeadPos);
	bool GetAIY(IN float x, OUT float &y, IN float z, D3DXVECTOR3 HeadPos);
};

