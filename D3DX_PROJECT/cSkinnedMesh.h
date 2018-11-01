#pragma once
#include "stdafx.h"

struct ST_BONE;

class cSkinnedMesh
{
public:
	cSkinnedMesh();
	~cSkinnedMesh();
protected:
	LPD3DXFRAME			m_pRoot;
	LPD3DXANIMATIONCONTROLLER m_pAnimController;

	float	m_fBlendTime;
	float	m_fPassedBlendTime;
	bool	m_isAnimBlend;

public:

	void Setup(char* szFoler, char* szFile);
	void Update();
	void Update(LPD3DXFRAME pFrame, LPD3DXFRAME pParent);

	void Render(LPD3DXFRAME pFrame);
	void SetupBoneMatrixPtrs(LPD3DXFRAME pFrame);
	void UpdateSKinnedMesh(LPD3DXFRAME pFrame);

	void SetAnimationIndex(int nIndex);
	void SetAnimationIndexBlend(int nIndex);

private:
	SYNTHESIZE(D3DXVECTOR3, m_vMin, Min);
	SYNTHESIZE(D3DXVECTOR3, m_vMax, Max);
	D3DXMATRIXA16		m_matWorldTM;

public:
	cSkinnedMesh(char* szFolder, char* szFileName);
	void Load(char* szFolder, char* szFileName);
	void Destroy();
	void UpdateAndRender();
	void Update(ST_BONE* pCurrent, D3DXMATRIXA16* pmatParent);
	void SetRandomTrackPosition();
	void setTransform(D3DXMATRIXA16* pmat);
};

