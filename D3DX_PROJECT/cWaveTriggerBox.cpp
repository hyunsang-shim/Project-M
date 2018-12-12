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
		tb.SetMin(D3DXVECTOR3(292, 2, -372));
		tb.SetMax(D3DXVECTOR3(260, 12, -417));
		m_SpawnXPos = 200;
		m_SpawnYPos = 5;
		m_SpawnZPos = -395;
		tb.m_DisplayOrNot = true;
		tb.MakeMonster = 10;
		m_pOBB = new cOBB;
		m_pOBB->Setup(&tb);
		m_NextWave = false;
		m_WaveCount = 1;
		g_pGameInfoManager->monsterTriggerBoxNum = 0;
		g_pNetworkManager->SendData(NH_SPAWN_TRIGGER, g_pGameInfoManager->GetMyInfo());

	}
	else if (m_WaveCount == 1)
	{
		tb.Name = "SecondTriggerBox";
		tb.SetMin(D3DXVECTOR3(22, 11, -185));
		tb.SetMax(D3DXVECTOR3(-11, 16, -229));
		m_SpawnXPos = -60;
		m_SpawnYPos = 5;
		m_SpawnZPos = -229;
		tb.m_DisplayOrNot = true;
		tb.MakeMonster = 20;
		m_pOBB = new cOBB;
		m_pOBB->Setup(&tb);
		m_NextWave = false;
		m_WaveCount = 2;
	}
	else if (m_WaveCount == 2)
	{
		tb.Name = "ThirdTriggerBox";
		tb.SetMin(D3DXVECTOR3(-208, 13, -108));
		tb.SetMax(D3DXVECTOR3(-241, 18, -102));
		m_SpawnXPos = -317;
		m_SpawnYPos = 18;
		m_SpawnZPos = -102;
		tb.m_DisplayOrNot = true;
		tb.MakeMonster = 25;
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
