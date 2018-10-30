#pragma once
#include "stdafx.h"

struct ST_BONE : public D3DXFRAME
{
	D3DXMATRIXA16 CombineTransformationMatrix; // 누적행렬
};

struct ST_BONE_MESH : public D3DXMESHCONTAINER
{
	vector<D3DMATERIAL9>			vecMtl;
	vector<LPDIRECT3DTEXTURE9>		vecTexture;
	LPD3DXMESH	pOrigMesh;
	D3DXMATRIX**	ppBoneMatrixPtrs;
	D3DXMATRIX*		pBoneOffsetMatrices;
	D3DXMATRIX*		pCurrentBoneMatrices;
};

class cAllocateHierarchy
	: public ID3DXAllocateHierarchy
{
public:
	cAllocateHierarchy();
	~cAllocateHierarchy();
protected:
	SYNTHESIZE(string, m_sFolder, Folder);
	SYNTHESIZE(D3DXVECTOR3, m_vMin, Min);
	SYNTHESIZE(D3DXVECTOR3, m_vMax, Max);
public:
	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name,
		LPD3DXFRAME* ppNewFrame) override;

	STDMETHOD(CreateMeshContainer) (
		THIS_ LPCSTR Name,
		CONST D3DXMESHDATA * pMeshData,
		CONST D3DXMATERIAL * pMaterials,
		CONST D3DXEFFECTINSTANCE * pEffectInstances,
		DWORD NumMaterials,
		CONST DWORD * pAdjacency,
		LPD3DXSKININFO pSkinInfo,
		LPD3DXMESHCONTAINER * ppNewMeshContainer) override;

	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree) override;
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree) override;
};

