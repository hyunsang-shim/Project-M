#include "stdafx.h"
#include "cTextureManager.h"


cTextureManager::cTextureManager()
{
}


cTextureManager::~cTextureManager()
{
}

LPDIRECT3DTEXTURE9 cTextureManager::GetTexture(char * szFullPath)
{
	HRESULT hr;
	if (m_mapTexture.find(szFullPath) == m_mapTexture.end())
	{
		hr = D3DXCreateTextureFromFileA(g_pDevice, szFullPath,
			&m_mapTexture[szFullPath]);
	}
	return m_mapTexture[szFullPath];
}

LPDIRECT3DTEXTURE9 cTextureManager::GetTexture(string & sFullPath)
{
	return GetTexture((char*) sFullPath.c_str());
}

void cTextureManager::Destroy()
{
	for each(auto it in m_mapTexture)
		SAFE_RELEASE(it.second);
	m_mapTexture.clear();
}
