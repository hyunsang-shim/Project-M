#pragma once
class cSKY
{
public:
	cSKY();
	LPD3DXMESH m_pXMeshSKY;
	

	
	void Setup();
	void Render();
private :
	D3DXVECTOR3 eyePosition;
	D3DXMATRIXA16 m_matWorld;
	D3DMATERIAL9 skyMaterial;

	~cSKY();
};

