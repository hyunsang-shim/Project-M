#include "stdafx.h"
#include "cSkinnedMesh.h"
#include "cAllocateHierarchy.h"
#include "cSkinnedMeshManager.h"

cSkinnedMesh::cSkinnedMesh()
	: m_pAnimController(NULL)
	, m_fBlendTime(0.3f)
	, m_fPassedBlendTime(0.0f)
	, m_isAnimBlend(false)
	, m_vMax(0, 0, 0)
	, m_vMin(0, 0, 0)
{
	D3DXMatrixIdentity(&m_matWorldTM);
}


cSkinnedMesh::~cSkinnedMesh()
{
	/*cAllocateHierarchy ah;
	D3DXFrameDestroy(m_pRoot, &ah);*/
	SAFE_RELEASE(m_pAnimController);
}

void cSkinnedMesh::Setup(char * szFolder, char * szFile)
{
	string sFullPath(szFolder);
	sFullPath =
		sFullPath + string("/") + string(szFile);

	cAllocateHierarchy ah;
	ah.SetFolder(szFolder);

	HRESULT  hr =D3DXLoadMeshHierarchyFromXA(sFullPath.c_str(),
		D3DXMESH_MANAGED,
		g_pDevice,
		&ah,
		NULL,
		&m_pRoot,
		&m_pAnimController);

	{
		m_vMin = ah.GetMin();
		m_vMax = ah.GetMax();
	}

	SetupBoneMatrixPtrs(m_pRoot);
}

void cSkinnedMesh::Update()
{
	{
		if (m_isAnimBlend)
		{
			m_fPassedBlendTime += g_pTimeManager->GetElapsedTime();
			if (m_fPassedBlendTime >= m_fBlendTime)
			{
				m_isAnimBlend = false;
				m_pAnimController->SetTrackWeight(0, 1.0f);
				m_pAnimController->SetTrackWeight(1, false);
			}
			else
			{
				float fweight = m_fPassedBlendTime / m_fBlendTime;
				m_pAnimController->SetTrackWeight(0, fweight);
				m_pAnimController->SetTrackWeight(1, 1.0f - fweight);

			}
		}

	}


	m_pAnimController->AdvanceTime(g_pTimeManager->GetElapsedTime(), NULL);
	Update(m_pRoot, NULL);
	UpdateSKinnedMesh(m_pRoot);
}

void cSkinnedMesh::Update(LPD3DXFRAME pFrame, LPD3DXFRAME pParent)
{
	if (pFrame == NULL)
		pFrame = m_pRoot;

	ST_BONE* pBone = (ST_BONE*)pFrame;
	pBone->CombineTransformationMatrix = pBone->TransformationMatrix;
	if (pParent)
	{
		pBone->CombineTransformationMatrix *=
			((ST_BONE*)pParent)->CombineTransformationMatrix;
	}
	if (pFrame->pFrameFirstChild)
	{
		Update(pFrame->pFrameFirstChild, pFrame);
	}
	if (pFrame->pFrameSibling)
	{
		Update(pFrame->pFrameSibling, pParent);
	}
}

void cSkinnedMesh::Render(LPD3DXFRAME pFrame)
{
	if (pFrame == NULL)
		pFrame = m_pRoot;

	ST_BONE* pBone = (ST_BONE*)pFrame;
	if (pBone->pMeshContainer)
	{
		ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pBone->pMeshContainer;
		if (pBoneMesh->MeshData.pMesh)
		{
			g_pDevice->SetTransform(D3DTS_WORLD,
				&pBone->CombineTransformationMatrix);
			for (size_t i = 0; i < pBoneMesh->vecMtl.size(); ++i)
			{
				g_pDevice->SetTexture(0, pBoneMesh->vecTexture[i]);
				g_pDevice->SetMaterial(&pBoneMesh->vecMtl[i]);
				pBoneMesh->MeshData.pMesh->DrawSubset(i);
			}
		}
	}

	if (pFrame->pFrameFirstChild)
	{
		Render(pFrame->pFrameFirstChild);
	}
	if (pFrame->pFrameSibling)
	{
		Render(pFrame->pFrameSibling);
	}
}

void cSkinnedMesh::SetupBoneMatrixPtrs(LPD3DXFRAME pFrame)
{
	if (pFrame && pFrame->pMeshContainer)
	{
		ST_BONE_MESH* pBoneMesh =
			(ST_BONE_MESH*)pFrame->pMeshContainer;
		if (pBoneMesh->pSkinInfo)
		{
			LPD3DXSKININFO pSkinInfo = pBoneMesh->pSkinInfo;
			DWORD dwNumBones = pSkinInfo->GetNumBones();
			for (DWORD i = 0; i < dwNumBones; ++i)
			{
				ST_BONE* pBone =
					(ST_BONE*)D3DXFrameFind(
						m_pRoot, pSkinInfo->GetBoneName(i));
				pBoneMesh->ppBoneMatrixPtrs[i] =
					&(pBone->CombineTransformationMatrix);
			}
		}
	}

	if (pFrame->pFrameFirstChild)
		SetupBoneMatrixPtrs(pFrame->pFrameFirstChild);

	if (pFrame->pFrameSibling)
		SetupBoneMatrixPtrs(pFrame->pFrameSibling);
}

void cSkinnedMesh::UpdateSKinnedMesh(LPD3DXFRAME pFrame)
{
	if (pFrame && pFrame->pMeshContainer)
	{
		ST_BONE_MESH* pBoneMesh =
			(ST_BONE_MESH*)pFrame->pMeshContainer;
		if (pBoneMesh->pSkinInfo)
		{
			LPD3DXSKININFO pSkinInfo = pBoneMesh->pSkinInfo;
			DWORD dwNumBones = pSkinInfo->GetNumBones();
			for (DWORD i = 0; i < dwNumBones; ++i)
			{
				pBoneMesh->pCurrentBoneMatrices[i] =
					pBoneMesh->pBoneOffsetMatrices[i] *
					*(pBoneMesh->ppBoneMatrixPtrs[i]);
			}
		}

		BYTE* src = NULL;
		BYTE* dest = NULL;

		pBoneMesh->pOrigMesh->LockVertexBuffer(D3DLOCK_READONLY,
			(void**)&src);
		pBoneMesh->MeshData.pMesh->LockVertexBuffer(0, (void**)&dest);

		pBoneMesh->pSkinInfo->UpdateSkinnedMesh(
			pBoneMesh->pCurrentBoneMatrices, NULL, src, dest);

		pBoneMesh->MeshData.pMesh->UnlockVertexBuffer();
		pBoneMesh->pOrigMesh->UnlockVertexBuffer();
	}

	if (pFrame->pFrameFirstChild)
		UpdateSKinnedMesh(pFrame->pFrameFirstChild);
	if (pFrame->pFrameSibling)
		UpdateSKinnedMesh(pFrame->pFrameSibling);
}

void cSkinnedMesh::SetAnimationIndex(int nIndex)
{
	int num = m_pAnimController->GetNumAnimationSets();
	if (nIndex > num) nIndex = nIndex % num;

	LPD3DXANIMATIONSET pAnimSet = NULL;
	m_pAnimController->GetAnimationSet(nIndex, &pAnimSet);

	m_pAnimController->SetTrackAnimationSet(0, pAnimSet);
	m_pAnimController->GetPriorityBlend();

	SAFE_RELEASE(pAnimSet);

}

void cSkinnedMesh::SetAnimationIndexBlend(int nIndex)
{
	m_isAnimBlend = true;
	m_fPassedBlendTime = 0.0f; // 블렌딩 시간 초기화

	int num = m_pAnimController->GetNumAnimationSets();
	if (nIndex > num) nIndex = nIndex % num;


	LPD3DXANIMATIONSET pPrevAnimSet = NULL;
	LPD3DXANIMATIONSET pNextAnimSet = NULL;

	D3DXTRACK_DESC stTrackDesc;
	m_pAnimController->GetTrackDesc(0, &stTrackDesc);

	m_pAnimController->GetTrackAnimationSet(0, &pPrevAnimSet);
	m_pAnimController->SetTrackAnimationSet(1, pPrevAnimSet);
	m_pAnimController->SetTrackDesc(1, &stTrackDesc);

	m_pAnimController->GetAnimationSet(nIndex, &pNextAnimSet);
	m_pAnimController->SetTrackAnimationSet(0, pNextAnimSet);
	m_pAnimController->SetTrackPosition(0, 0.0f);

	m_pAnimController->SetTrackWeight(0, 0.0f);
	m_pAnimController->SetTrackWeight(1, 1.0f);

	SAFE_RELEASE(pPrevAnimSet);
	SAFE_RELEASE(pNextAnimSet);
}

cSkinnedMesh::cSkinnedMesh(char * szFolder, char * szFileName)
{
	cSkinnedMesh* pSkinnedMesh =
		g_pSkinnedMeshManager->GetSkinnedMesh(szFolder, szFileName);
	m_pRoot = pSkinnedMesh->m_pRoot;

	m_vMin = pSkinnedMesh->m_vMin;
	m_vMax = pSkinnedMesh->m_vMax;

	pSkinnedMesh->m_pAnimController->CloneAnimationController(
		pSkinnedMesh->m_pAnimController->GetMaxNumAnimationOutputs(),
		pSkinnedMesh->m_pAnimController->GetMaxNumAnimationSets(),
		pSkinnedMesh->m_pAnimController->GetMaxNumTracks(),
		pSkinnedMesh->m_pAnimController->GetMaxNumEvents(),
		&m_pAnimController);

	BulletStartBoneName = "BONE_0009F";
}

LPCWSTR s2ws1(string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r.c_str();
}

void cSkinnedMesh::Load(char * szFolder, char * szFileName)
{
	cAllocateHierarchy ah;
	ah.SetFolder(szFolder);

	string sFullPath(szFolder);
	sFullPath += string("/") + string(szFileName);

	HRESULT  hr = D3DXLoadMeshHierarchyFromX(sFullPath.c_str(),
		D3DXMESH_MANAGED,
		g_pDevice,
		&ah,
		NULL,
		(LPD3DXFRAME*)&m_pRoot,
		&m_pAnimController);


	m_vMin = ah.GetMin();
	m_vMax = ah.GetMax();

	if (m_pRoot) SetupBoneMatrixPtrs(m_pRoot);
}

void cSkinnedMesh::Destroy()
{
	cAllocateHierarchy ah;
	D3DXFrameDestroy((LPD3DXFRAME)m_pRoot, &ah);
}

void cSkinnedMesh::UpdateAndRender()
{
	if (m_pAnimController)
	{
		m_pAnimController->AdvanceTime(g_pTimeManager->GetElapsedTime(), NULL);
	}
	if (m_pRoot)
	{
		Update((ST_BONE*)m_pRoot, &m_matWorldTM);
		Render(m_pRoot);
	}

}

void cSkinnedMesh::Update(ST_BONE * pCurrent, D3DXMATRIXA16 *pmatParent)
{
	if (pCurrent == NULL)
		pCurrent = (ST_BONE*)m_pRoot;

	pCurrent->CombineTransformationMatrix = pCurrent->TransformationMatrix;
		
	if (pmatParent)
	{
		pCurrent->CombineTransformationMatrix =
			pCurrent->CombineTransformationMatrix * (*pmatParent);
	}

	// 높이 맵 판단용 머리 본 찾기
	// >>
	if (pCurrent->Name == m_sHeadName)
	{
		m_vecHeadPos.x = pCurrent->CombineTransformationMatrix._41;
		m_vecHeadPos.y = pCurrent->CombineTransformationMatrix._42;
		m_vecHeadPos.z = pCurrent->CombineTransformationMatrix._43;
	}
	// <<

	if (strcmp(pCurrent->Name, BulletStartBoneName) == 0)
	{
		m_vBulletPos = D3DXVECTOR3(pCurrent->CombineTransformationMatrix._41, pCurrent->CombineTransformationMatrix._42, pCurrent->CombineTransformationMatrix._43);
	}

	if (pCurrent->pFrameSibling)
	{
		Update((ST_BONE*)pCurrent->pFrameSibling, pmatParent);
	}

	if (pCurrent->pFrameFirstChild)
	{
		Update((ST_BONE*)pCurrent->pFrameFirstChild, &(pCurrent->CombineTransformationMatrix));
	}

	

}

void cSkinnedMesh::SetRandomTrackPosition()
{
	m_pAnimController->SetTrackPosition(0, (rand() % 100) / 10.0f);
}

void cSkinnedMesh::setTransform(D3DXMATRIXA16 * pmat)
{
	m_matWorldTM = *pmat;
}

D3DXVECTOR3 cSkinnedMesh::GetHeadPos()
{
	return m_vecHeadPos;
}

LPD3DXANIMATIONCONTROLLER cSkinnedMesh::GetAnimController()
{
	return m_pAnimController;
}
