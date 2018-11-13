#pragma once

struct ST_BONE : public D3DXFRAME
{
	D3DXMATRIXA16 CombinedTransformationMatrix;
};

struct ST_BONE_MESH : public D3DXMESHCONTAINER
{
	std::vector<LPDIRECT3DTEXTURE9> vecTexture;
	std::vector<D3DMATERIAL9>		vecMaterial;

	LPD3DXMESH				pWorkingMesh;			// : �۾��޽�
	LPD3DXMESH				pOrigMesh;				// : ���� �޽� CloneMeshFVF�� ����
	D3DXMATRIXA16**			ppBoneMatrixPtrs;		// : �� �޽��� ������ �ִ� ������'��'�� �����Ʈ���� ������ �迭
	D3DXMATRIXA16*			pBoneOffsetMatrices;	// : ���� �޽��� ���ý����̽��� ������ ��Ʈ������.

	DWORD					dwNumPaletteEntries;
	DWORD					dwMaxNumFaceInfls;
	DWORD					dwNumAttrGroups;
	LPD3DXBUFFER			pBufBoneCombos;
};

class cAllocateHierarchy : public ID3DXAllocateHierarchy
{
public:
	cAllocateHierarchy(void);
	~cAllocateHierarchy(void);
public:
	SYNTHESIZE(std::string, m_sFolder, Folder);

	// >> : OBB
	SYNTHESIZE(D3DXVECTOR3, m_vMin, Min);
	SYNTHESIZE(D3DXVECTOR3, m_vMax, Max);

	SYNTHESIZE(DWORD, m_dwDefaultPaletteSize, DefaultPaletteSize);
	SYNTHESIZE(DWORD, m_dwMaxPaletteSize, MaxPaletteSize);

	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name,
		LPD3DXFRAME *ppNewFrame) override;

	STDMETHOD(CreateMeshContainer)(THIS_
		LPCSTR Name,
		CONST D3DXMESHDATA *pMeshData,
		CONST D3DXMATERIAL *pMaterials,
		CONST D3DXEFFECTINSTANCE *pEffectInstances,
		DWORD NumMaterials,
		CONST DWORD *pAdjacency,
		LPD3DXSKININFO pSkinInfo,
		LPD3DXMESHCONTAINER *ppNewMeshContainer) override;

	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree) override;

	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree) override;
};

