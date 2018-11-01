#pragma once
class cSkinnedMesh;
class cNewObject;

class cOBB
{
public:
	cOBB();
	~cOBB();
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;
	LPDIRECT3DINDEXBUFFER9 m_pIndexBuffer;
	D3DXVECTOR3 m_vOrgCenterPos;
	D3DXVECTOR3 m_vOrgAxisDir[3];
	D3DXVECTOR3 m_vCenterPos;
	D3DXVECTOR3 m_vAxisDir[3];
	float m_fAxisLen[3];
	float m_fAxisHalfLen[3];

	D3DXMATRIXA16 m_matWorldTM;

public:
	void Setup(cSkinnedMesh* pSkinnedMesh);
	void Setup(cNewObject* pObject);
	void Update(D3DXMATRIXA16* pmatWorld);
	static bool isCollision(cOBB* pOBB1, cOBB* pOBB2);
	void OBBBox_Render(D3DXCOLOR c);

};

