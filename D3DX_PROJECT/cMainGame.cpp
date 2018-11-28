#include "stdafx.h"
#include "cMainGame.h"
#include <process.h>
#include <time.h>
using namespace std;

cMainGame::cMainGame() :
	SCENENUM(0)
{
}



cMainGame::~cMainGame()
{
	SAFE_DELETE(g_pGameInfoManager->m_pScene_Ingame);
	SAFE_DELETE(g_pGameInfoManager->m_pScene_Result);
	SAFE_DELETE(g_pGameInfoManager->m_pScene_Title);
}

void ThreadPorc()
{
	g_pGameInfoManager->m_pScene_Ingame = new cSCENE_INGAME();
	g_pGameInfoManager->m_pScene_Ingame->Setup();
}

void cMainGame::Setup()
{
	static HANDLE hThread;
	switch (getSceneNum())
	{
	case 0:
	{
		g_pGameInfoManager->m_pScene_Title = new cSCENE_TITLE();
		g_pGameInfoManager->m_pScene_Title->Setup();
		hThread = (HANDLE)_beginthreadex(NULL, 0, (unsigned(__stdcall *)(void*))ThreadPorc, NULL, 0, NULL);
	}
		break;
	case 1:
	{
		g_pGameInfoManager->m_pScene_Ingame = new cSCENE_INGAME();
		g_pGameInfoManager->m_pScene_Ingame->Setup();
	}
		break;
	case 3:
		g_pGameInfoManager->m_pScene_Result = new cSCENE_RESULT();
		g_pGameInfoManager->m_pScene_Result->Setup();
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
		g_pGameInfoManager->m_pScene_Title->Update();
		break;
	case 1:
		if (g_pGameInfoManager->m_pScene_Ingame->load)
			g_pGameInfoManager->m_pScene_Ingame->Update();
		break;
	case 3:
		g_pGameInfoManager->m_pScene_Result->Update();
		break;
	default:
		break;
	}
	if (g_pGameInfoManager->sceneChangeTriger)
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
		g_pGameInfoManager->m_pScene_Title->Render();
		break;
	case 1:
		if (g_pGameInfoManager->m_pScene_Ingame->load)
			g_pGameInfoManager->m_pScene_Ingame->Render();
		break;
	case 3:
		g_pGameInfoManager->m_pScene_Result->Render();
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
		g_pGameInfoManager->m_pScene_Title->WndProc(hWnd, message, wParam, lParam);
		break;
	case 1:
		g_pGameInfoManager->m_pScene_Ingame->WndProc(hWnd, message, wParam, lParam);
		break;
	case 3:
		g_pGameInfoManager->m_pScene_Result->WndProc(hWnd, message, wParam, lParam);
		break;
	default:
		break;
	}
	
}


void cMainGame::SetSceneChangeTriger(int sceneNum)
{
	g_pGameInfoManager->nextScene = sceneNum;
	g_pGameInfoManager->sceneChangeTriger = true;
}

void cMainGame::SceneChangeNum()
{
	//SAFE_DELETE(g_pGameInfoManager->m_pScene_Ingame);
	SAFE_DELETE(g_pGameInfoManager->m_pScene_Result);
	SAFE_DELETE(g_pGameInfoManager->m_pScene_Title);

	SCENENUM = g_pGameInfoManager->nextScene;

	switch (getSceneNum())
	{
	case 0:
		g_pGameInfoManager->m_pScene_Title = new cSCENE_TITLE();
		g_pGameInfoManager->m_pScene_Title->Setup();
		break;
	case 1:
		/*g_pGameInfoManager->m_pScene_Ingame = new cSCENE_INGAME();
		g_pGameInfoManager->m_pScene_Ingame->Setup();*/
		break;
	case 3:
		g_pGameInfoManager->m_pScene_Result = new cSCENE_RESULT();
		g_pGameInfoManager->m_pScene_Result->Setup();
		break;
	default:
		break;
	}
	g_pGameInfoManager->sceneChangeTriger = FALSE;

}

int cMainGame::getSceneNum()
{
	return SCENENUM;
}
