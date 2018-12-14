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
		tb.SetMin(D3DXVECTOR3(432.631, 6.127, -273.694));
		tb.SetMax(D3DXVECTOR3(382.757, 10.332, -212.889));
		m_SpawnXPos = 311.390;
		m_SpawnYPos = 10.080;
		m_SpawnZPos = -253.620;
		tb.m_DisplayOrNot = true;
		tb.MakeMonster = 10;
		m_pOBB = new cOBB;
		m_pOBB->Setup(&tb);
		m_NextWave = false;
		m_WaveCount = 1;

		g_pGameInfoManager->monsterTriggerBoxNum = 0;

	}
	else if (m_WaveCount == 1)
	{
		tb.Name = "SecondTriggerBox";
		tb.SetMin(D3DXVECTOR3(105.857, 11.741, -38.861));
		tb.SetMax(D3DXVECTOR3(61.710, 24.790, 24.675));
		m_SpawnXPos = -1.630;
		m_SpawnYPos = 14.259;
		m_SpawnZPos = -30.925;
		tb.m_DisplayOrNot = true;
		tb.MakeMonster = 20;
		m_pOBB = new cOBB;
		m_pOBB->Setup(&tb);
		m_NextWave = false;
		m_WaveCount = 2;

		g_pGameInfoManager->monsterTriggerBoxNum = 1;
	}
	else if (m_WaveCount == 2)
	{
		tb.Name = "ThirdTriggerBox";
		tb.SetMin(D3DXVECTOR3(-235.656, 14.206, 89.316));
		tb.SetMax(D3DXVECTOR3(-191.437, 20.502, 113.468));
		m_SpawnXPos = -256.878;
		m_SpawnYPos = 28.366;
		m_SpawnZPos = 120.544;
		tb.m_DisplayOrNot = true;
		tb.MakeMonster = 25;
		m_pOBB = new cOBB;
		m_pOBB->Setup(&tb);
		m_NextWave = false;
		m_WaveCount = 3;

		g_pGameInfoManager->monsterTriggerBoxNum = 2;
	}
	else if (m_WaveCount == 3)
	{
		tb.Name = "ThirdTriggerBox";
		tb.SetMin(D3DXVECTOR3(-0, 0, -0));
		tb.SetMax(D3DXVECTOR3(-0, 0, -0));
		m_SpawnXPos = -317;
		m_SpawnYPos = 18;
		m_SpawnZPos = -102;
		tb.m_DisplayOrNot = true;
		tb.MakeMonster = 25;
		m_pOBB = new cOBB;
		m_pOBB->Setup(&tb);
		m_NextWave = false;
		m_WaveCount = 3;

		g_pGameInfoManager->monsterTriggerBoxNum = 3;
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
