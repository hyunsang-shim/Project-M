#pragma once

class cFrame;
class cMtlTex;

class cAseLoader
{
public:
	cAseLoader();
	~cAseLoader();

private:
	FILE*					m_fp;
	char					m_szToken[1024];
	vector<cMtlTex*>		m_vecMtlTex;
	map<string, cFrame*>	m_mapFrame;


public:
	cFrame*					Load(IN char* szFullPath);

private:
	char*	GetToken();
	int		GetInteger();
	float	GetFloat();
	bool	IsWhite(IN char c);
	bool	IsEqual(IN char* str1, IN char* str2);
	void	SkipBlock();
	void	ProcessMATERIAL_LIST();
	void	ProcessMATERIAL(OUT cMtlTex* pMtlTex);
	void	ProcessMAP_DIFFUSE(OUT cMtlTex* pMtlTex);
	cFrame*	ProcessGEOMOBJECT();
	void	ProcessMESH(OUT cFrame* pFrame);
	void	ProcessMESH_VERTEX_LIST(OUT vector<D3DXVECTOR3>&vecV);
	void	ProcessMESH_FACE_LIST(OUT vector<ST_PNT_VERTEX>&vecVertex,
		IN vector<D3DXVECTOR3>&vecV);
	void	ProcessMESH_TVERTLIST(OUT vector<D3DXVECTOR2>&vecVT);
	void	ProcessMESH_TFACELIST(OUT vector<ST_PNT_VERTEX>& vecVertex,
		IN vector<D3DXVECTOR2>& vecVT);
	void	ProcessMESH_NORMALS(OUT vector<ST_PNT_VERTEX>&vecVertex);
	void	ProcessNODE_TM(OUT cFrame* pFrame);

	//회전 관련
	void	ProcessTM_ANIMATION(OUT cFrame* pFrame);
	void	ProcessCONTROL_POS_TRACK(OUT cFrame* pFrame);
	void	ProcessCONTROL_ROT_TRACK(OUT cFrame* pFrame);

	DWORD	m_dwFirstFrame;
	DWORD	m_dwLastFrame;
	DWORD	m_dwFrameSpeed;
	DWORD	m_dwTicksPerFrame;

	void	ProcessScene();
	void	Set_SceneFrame(OUT cFrame* pRoot);




};

