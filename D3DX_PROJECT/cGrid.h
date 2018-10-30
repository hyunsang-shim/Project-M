#pragma once
class cGrid
{
public:
	cGrid();
	~cGrid();
	//vector<ST_PC_VERTEX> m_vecVertex;
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	int m_nNumLine;
	//vector<ST_PC_VERTEX> m_vecPiramidVertex;
	LPDIRECT3DVERTEXBUFFER9 m_pPVB;
	int m_nPNumLine;
	ID3DXLine* g_pLine;
	D3DXMATRIX g_tMat;
public:
	void Setup();
	void Render();
};

