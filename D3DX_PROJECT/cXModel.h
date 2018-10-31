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
	ID3DXMesh* m_pXMesh;
public:
	void SetSRT(D3DXVECTOR3 vScale, D3DXVECTOR3 vRot, D3DXVECTOR3 vPos);
	void Update();
	void Render();
};

