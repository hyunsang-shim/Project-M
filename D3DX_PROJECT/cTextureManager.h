#pragma once

#define g_pTextureManager cTextureManager::GetInstance()

class cTextureManager
{
//public:
//	cTextureManager();
//	~cTextureManager();
private:
	SINGLETON(cTextureManager);
private:
	map<string, LPDIRECT3DTEXTURE9>m_mapTexture;
public:
	LPDIRECT3DTEXTURE9 GetTexture(char* szFullPath);
	LPDIRECT3DTEXTURE9 GetTexture(string& sFullPath);
	void Destroy();

	// >> :
private:
	map<string, D3DXIMAGE_INFO>		m_mapImageInfo;
public:
	LPDIRECT3DTEXTURE9 GetTexture(char* szFullPath, D3DXIMAGE_INFO* pImageInfo);
	LPDIRECT3DTEXTURE9 GetTexture(string& szFullPath, D3DXIMAGE_INFO* pImageInfo);
	// << :


};

