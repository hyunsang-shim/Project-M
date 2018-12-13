#pragma once
#include "cCharacter.h"
class cXModel
{
public:
	cXModel();
	cXModel(string filePath);
	~cXModel();
private:

	D3DXVECTOR3 m_vRot;
	D3DXVECTOR3 m_vPos;
	D3DXVECTOR3 m_vScale;
	D3DXMATRIXA16 m_matWorld;
	vector<D3DMATERIAL9> m_vecMtl;
	vector<LPDIRECT3DTEXTURE9> m_vecTextuer;
	SYNTHESIZE(LPD3DXMESH, m_pXMesh, XMESH);
public:
	void SetSRT(D3DXVECTOR3 vScale, D3DXVECTOR3 vRot, D3DXVECTOR3 vPos);
	void Update();
	void Render();
	bool GetY(IN float x, OUT float &y, IN float z, D3DXVECTOR3 HeadPos);
};

