#pragma once
#include "iMap.h"
class cHeightMap :public iMap
{
public:
	cHeightMap();
	virtual ~cHeightMap();

private:
	LPD3DXMESH				m_pMesh;
	vector<D3DXVECTOR3>		m_vecVertex;
	D3DMATERIAL9			m_stMtl;
	LPDIRECT3DTEXTURE9		m_pTexture;
	int						m_nTileN;

public:
	void Setup(char* szFolder, char* szRaw, char* szTex,
		DWORD dwBytePerPixel = 1);
	virtual void Render() override;
	virtual bool GetHeight(IN float x, OUT float &y, IN float z) override;
	void Destroy();
};

