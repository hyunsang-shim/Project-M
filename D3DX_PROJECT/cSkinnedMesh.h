#pragma once
#include "stdafx.h"
struct ST_BONE;

class cSkinnedMesh
{
	friend class cSkinnedMeshManager;

private:
	ST_BONE*					m_pRootFrame;
	DWORD						m_dwWorkingPaletteSize;
	D3DXMATRIX*					m_pmWorkingPalette;
	LPD3DXEFFECT				m_pEffect;

	LPD3DXANIMATIONCONTROLLER	m_pAnimController;
	D3DXMATRIXA16				m_matWorldTM;

	/// >> : OBB -
	SYNTHESIZE(D3DXVECTOR3, m_vMin, Min);
	SYNTHESIZE(D3DXVECTOR3, m_vMax, Max);
	SYNTHESIZE(D3DXVECTOR3, m_vBulletPos, BulletPos);
	SYNTHESIZE(D3DXVECTOR3, m_vMyHeadPos, HeadPos);

	char* BulletStartBoneName;
	char* HeadBoneName;
public:
	cSkinnedMesh(char* szFolder, char* szFilename);
	~cSkinnedMesh(void);

	void UpdateAndRender();
	void SetAnimationIndex(int nIndex);

	void SetRandomTrackPosition();
	void SetTransform(D3DXMATRIXA16* pmat)
	{
		m_matWorldTM = *pmat;
	}

	void setTransform(D3DXMATRIXA16* pmat);
	LPD3DXANIMATIONCONTROLLER GetAnimController();
private:
	cSkinnedMesh();
	void Load(char* szFolder, char* szFilename);
	LPD3DXEFFECT LoadEffect(char* szFilename);
	void Update(ST_BONE* pCurrent, D3DXMATRIXA16* pmatParent);
	void Render(ST_BONE* pBone = NULL);
	void SetupBoneMatrixPtrs(ST_BONE* pBone);
	void Destroy();
};