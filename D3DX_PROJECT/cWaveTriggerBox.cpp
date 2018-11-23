#include "stdafx.h"
#include "cWaveTriggerBox.h"
#include "cOBB.h"

cWaveTriggerBox::cWaveTriggerBox() : m_WaveCount(0), m_NextWave(true), m_pOBB(NULL)
{
	D3DXMatrixIdentity(&m_matWorld);
}


cWaveTriggerBox::~cWaveTriggerBox()
{
	SAFE_DELETE(m_pOBB);
}

void cWaveTriggerBox::Setup()
{
	if (m_WaveCount == 0)
	{
		tb.Name = "FirstTriggerBox";
		tb.SetMin(D3DXVECTOR3(0, 0, 20));
		tb.SetMax(D3DXVECTOR3(10, 10, 30));
		tb.m_DisplayOrNot = true;
		m_pOBB = new cOBB;
		m_pOBB->Setup(&tb);
		m_NextWave = false;
		m_WaveCount = 1;
	}
	else if (m_WaveCount == 1)
	{
		tb.Name = "SecondTriggerBox";
		tb.SetMin(D3DXVECTOR3(10, 0, 20));
		tb.SetMax(D3DXVECTOR3(20, 10, 30));
		tb.m_DisplayOrNot = true;
		m_pOBB = new cOBB;
		m_pOBB->Setup(&tb);
		m_NextWave = false;
		m_WaveCount = 2;
	}
	else if (m_WaveCount == 2)
	{
		tb.Name = "ThirdTriggerBox";
		tb.SetMin(D3DXVECTOR3(20, 0, 20));
		tb.SetMax(D3DXVECTOR3(30, 10, 30));
		tb.m_DisplayOrNot = true;
		m_pOBB = new cOBB;
		m_pOBB->Setup(&tb);
		m_NextWave = false;
		m_WaveCount = 3;
	}
}

void cWaveTriggerBox::Update()
{
	if (GetKeyState('1') & 0x8000)
	{
		tb.m_DisplayOrNot = false;
	}
	else
	{
		tb.m_DisplayOrNot = true;
	}

	if (GetKeyState('N') & 0x8000)
	{
		m_NextWave = true;
		Sleep(100);
	}

	m_pOBB->Update(&m_matWorld);
}

void cWaveTriggerBox::Render()
{
	D3DCOLOR c = D3DCOLOR_XRGB(255, 255, 255);

	if (tb.m_DisplayOrNot)
	{
		m_pOBB->OBBBox_Render(c);
	}
}

vector<TriggerBox> cWaveTriggerBox::GetTriggerBox()
{
	return m_vecTrigger;
}
