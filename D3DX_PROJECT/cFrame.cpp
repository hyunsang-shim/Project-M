#include "stdafx.h"
#include "cFrame.h"


cFrame::cFrame()
	: m_pMtlTex(NULL)
{
	D3DXMatrixIdentity(&m_matLocalTM);
	D3DXMatrixIdentity(&m_matWorldTM);
	D3DXMatrixIdentity(&m_matWorld);
}

cFrame::~cFrame()
{
	SAFE_RELEASE(m_pMtlTex);
}

void cFrame::Update(int nKeyFrame, D3DXMATRIXA16 * pmatParent)
{
	D3DXMATRIXA16 matR, matT;

	CalcLocalR(nKeyFrame, matR);
	CalcLocalT(nKeyFrame, matT);

	m_matLocalTM = matR*matT;
	m_matWorldTM = m_matLocalTM;
	if(pmatParent)
	{
		m_matWorldTM *= (*pmatParent);
	}  
	for each(auto c in m_vecChild)
	{
		c->Update(nKeyFrame, &m_matWorldTM);
		c->m_matWorld = m_matWorld;
	}

}

void cFrame::Render()
{
	if (m_pMtlTex)
	{
		D3DXMATRIXA16 tmp;
		tmp = m_matWorldTM * m_matWorld;
		g_pDevice->SetTransform(D3DTS_WORLD, &tmp);
		g_pDevice->SetTexture(0, m_pMtlTex->GetTexture());
		g_pDevice->SetMaterial(&m_pMtlTex->GetMaterial());
		g_pDevice->SetFVF(ST_PNT_VERTEX::FVF);
		g_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
			m_vecVertex.size() / 3,
			&m_vecVertex[0],
			sizeof(ST_PNT_VERTEX));
	}

	for each(auto c in m_vecChild)
	{
		c->Render();
	}
}

void cFrame::AddChild(cFrame * pChild)
{
	m_vecChild.push_back(pChild);
}

void cFrame::Destroy()
{
	for each(auto c in m_vecChild)
	{
		c->Destroy();
	}
	this->Release();
}

void cFrame::CalcOriginLocalTM(D3DXMATRIXA16 * pmatParent)
{
	//LTN = WTM * InvParentWTM
	m_matLocalTM = m_matWorldTM;
	if (pmatParent)
	{
		D3DXMATRIXA16 matInvParent;
		D3DXMatrixInverse(&matInvParent, 0, pmatParent);
		m_matLocalTM = m_matWorldTM * matInvParent;
	}

	for each(auto c in m_vecChild)
	{
		c->CalcOriginLocalTM(&m_matWorldTM);
	}
}

void cFrame::SetSRT(D3DXVECTOR3 vScale, D3DXVECTOR3 vRot, D3DXVECTOR3 vPos)
{
	m_vPos = vPos;
	m_vScale = vScale;
	m_vRot = vRot;
	D3DXMATRIXA16 matT, matS, matR, matX, matY, matZ;
	D3DXMatrixTranslation(&matT, m_vPos.x, m_vPos.y, m_vPos.z);
	D3DXMatrixScaling(&matS, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixRotationX(&matX, m_vRot.x);
	D3DXMatrixRotationY(&matY, m_vRot.y);
	D3DXMatrixRotationZ(&matZ, m_vRot.z);
	matR = matZ * matX * matY;
	this->m_matWorld = matS * matR * matT;
}

void cFrame::CalcLocalT(IN int nKeyFrame, OUT D3DXMATRIXA16 & matT)
{
	D3DXMatrixIdentity(&matT);
	if (m_vecPosTrack.empty())
	{
		matT._41 = m_matLocalTM._41; //4행 1열
		matT._42 = m_matLocalTM._42; //4행 2열
		matT._43 = m_matLocalTM._43; //4행 3열
		return;
	}
	if (nKeyFrame <= m_vecPosTrack.front().n)
	{
		matT._41 = m_vecPosTrack.front().v.x;
		matT._42 = m_vecPosTrack.front().v.y;
		matT._43 = m_vecPosTrack.front().v.z;
		return;
	}
	if (nKeyFrame >= m_vecPosTrack.back().n)
	{
		matT._41 = m_vecPosTrack.back().v.x;
		matT._42 = m_vecPosTrack.back().v.y;
		matT._43 = m_vecPosTrack.back().v.z;
		return;
	}
	int nNextIndex = 0;
	for (size_t i = 0; i < m_vecPosTrack.size(); ++i)
	{
		if (nKeyFrame < m_vecPosTrack[i].n)
		{
			nNextIndex = i;
			break;
		}
	}
	int nPrevIndex = nNextIndex - 1;
	float t = (nKeyFrame - m_vecPosTrack[nPrevIndex].n) /
		(float)(m_vecPosTrack[nNextIndex].n - m_vecPosTrack[nPrevIndex].n);

	D3DXVECTOR3 v;
	D3DXVec3Lerp(&v,
		&m_vecPosTrack[nPrevIndex].v,
		&m_vecPosTrack[nNextIndex].v,
		t);

	matT._41 = v.x;
	matT._42 = v.y;
	matT._43 = v.z;
}

void cFrame::CalcLocalR(IN int nKeyFrame, OUT D3DXMATRIXA16 & matR)
{
	D3DXMatrixIdentity(&matR);
	if (m_vecRotTrack.empty())
	{
		matR = m_matLocalTM;
		matR._41 = 0.0f;
		matR._42 = 0.0f;
		matR._43 = 0.0f;
		return;
	}

	if (nKeyFrame <= m_vecRotTrack.front().n)
	{
		D3DXMatrixRotationQuaternion(&matR, &m_vecRotTrack.front().q);
		return;
	}
	if (nKeyFrame >= m_vecRotTrack.back().n)
	{
		D3DXMatrixRotationQuaternion(&matR, &m_vecRotTrack.back().q);
		return;
	}

	int nNextIndex = 0;
	for (size_t i = 0; i < m_vecRotTrack.size(); i++)
	{
		if (nKeyFrame < m_vecRotTrack[i].n)
		{
			nNextIndex = i;
			break;
		}
	}

	int nPrevIndex = nNextIndex - 1;

	float t = (nKeyFrame - m_vecRotTrack[nPrevIndex].n) /
		(float)(m_vecRotTrack[nNextIndex].n - m_vecRotTrack[nPrevIndex].n);

	D3DXQUATERNION q;
	D3DXQuaternionSlerp(&q,
		&m_vecRotTrack[nPrevIndex].q,
		&m_vecRotTrack[nNextIndex].q,
		t);

	D3DXMatrixRotationQuaternion(&matR, &q);//팔을 휘두를때 보간이 제대로 되도록(포물선을 그리도록). 안하면 팔이 움푹 패인 형태로 휘둘린다.
}

int cFrame::GetKeyFrame()
{
	int first = m_dwFirstFrame*m_dwTicksPerFrame;
	int last = m_dwLastFrame*m_dwTicksPerFrame;
	return GetTickCount() % (last - first) + first;
}
