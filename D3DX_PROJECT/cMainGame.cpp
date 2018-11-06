#include "stdafx.h"
#include "cMainGame.h"

cMainGame::cMainGame() :
	SCENENUM(1),
	Next_SCENEN(0),
	sceneChangeTriger(0),
	m_pScene_Ingame(NULL),
	m_pScene_Result(NULL),
	m_pScene_Title(NULL)
{
}


cMainGame::~cMainGame()
{
	SAFE_DELETE(m_pScene_Ingame);
	SAFE_DELETE(m_pScene_Result);
	SAFE_DELETE(m_pScene_Title);
}

void cMainGame::Setup()
{
	switch (getSceneNum())
	{
	case 0:
		m_pScene_Title = new cSCENE_TITLE();
		m_pScene_Title->Setup();
		break;
	case 1:
		m_pScene_Ingame = new cSCENE_INGAME();
		m_pScene_Ingame->Setup();
		break;
	case 3:
		m_pScene_Result = new cSCENE_RESULT();
		m_pScene_Result->Setup();
		break;
	default:
		break;
	}
}

void cMainGame::Update()
{
	switch (getSceneNum())
	{
	case 0:
		m_pScene_Title->Update();
		break;
	case 1:
		m_pScene_Ingame->Update();
		break;
	case 3:
		m_pScene_Result->Update();
		break;
	default:
		break;
	}
	if (sceneChangeTriger)
		this->SceneChangeNum();
}

void cMainGame::Render()
{
	g_pDevice->Clear(NULL, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255, 252, 255), 1.0f, 0);
	g_pDevice->BeginScene();
	//---------------------------------------------------------
	//<-------------------------CODE START---------------------
	switch (getSceneNum())
	{
	case 0:
		m_pScene_Title->Render();
		break;
	case 1:
		m_pScene_Ingame->Render();
		break;
	case 3:
		m_pScene_Result->Render();
		break;
	default:
		break;
	}



	//<-------------------------CODE END-----------------------
	//---------------------------------------------------------

	g_pDevice->EndScene();
	g_pDevice->Present(NULL, NULL, NULL, NULL);
}

void cMainGame::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (getSceneNum())
	{
	case 0:
		m_pScene_Title->WndProc(hWnd, message, wParam, lParam);
		break;
	case 1:
		m_pScene_Ingame->WndProc(hWnd, message, wParam, lParam);
		break;
	case 3:
		m_pScene_Result->WndProc(hWnd, message, wParam, lParam);
		break;
	default:
		break;
	}
	
}

void cMainGame::SetSceneChangeTriger(int sceneNum)
{
	sceneChangeTriger = true;
}

void cMainGame::SceneChangeNum()
{
	SAFE_DELETE(m_pScene_Ingame);
	SAFE_DELETE(m_pScene_Result);
	SAFE_DELETE(m_pScene_Title);

	SCENENUM = Next_SCENEN;

	switch (getSceneNum())
	{
	case 0:
		m_pScene_Title = new cSCENE_TITLE();
		m_pScene_Title->Setup();
		break;
	case 1:
		m_pScene_Ingame = new cSCENE_INGAME();
		m_pScene_Ingame->Setup();
		break;
	case 3:
		m_pScene_Result = new cSCENE_RESULT();
		m_pScene_Result->Setup();
		break;
	default:
		break;
	}
	sceneChangeTriger = FALSE;

}

int cMainGame::getSceneNum()
{
	return SCENENUM;
}
