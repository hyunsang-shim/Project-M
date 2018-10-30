#pragma once

#define g_pSkinnedMeshManager cSkinnedMeshManager::GetInstance()

class cSkinnedMesh;

class cSkinnedMeshManager
{
private:
	map<string, cSkinnedMesh* > m_mapSkinnedMesh;

public:
	SINGLETON(cSkinnedMeshManager);
	cSkinnedMesh* GetSkinnedMesh(char *szFolder, char* szFilename);
	void Destroy();

};

