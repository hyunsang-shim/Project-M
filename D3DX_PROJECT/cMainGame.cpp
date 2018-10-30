#include "stdafx.h"
#include "cMainGame.h"

#include "cGrid.h"
#include "cNewObject.h"
#include "cXModel.h"
#include "cAseLoader.h"

cMainGame::cMainGame()		
	:m_pCamera(NULL),
	m_pGrid(NULL),
	m_pMap(NULL),
	m_pObject(NULL),
	m_pXmodel(NULL)
{
}


cMainGame::~cMainGame()
{
	m_pMap->Destroy();
	SAFE_DELETE(m_pMap);
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pGrid);
	SAFE_DELETE(m_pObject);
	
}

void cMainGame::Setup()
{
	// 카메라 세팅
	m_pCamera = new cCamera();
	m_pCamera->Setup();
	
	// 그리스 세팅
	m_pGrid = new cGrid();
	m_pGrid->Setup();

	// 메인 광원 셋팅
	D3DXVECTOR3 dir(0,-1,0);
	D3DXCOLOR c(1.0f, 1.0f, 1.0f,1.0f);
	ZeroMemory(&DirectLight, sizeof(D3DLIGHT9));
	DirectLight = InitDirectionalLight(&dir, &c);
	g_pDevice->SetLight(0, &DirectLight);
	g_pDevice->SetLight(1, &DirectLight);
	g_pDevice->LightEnable(0, TRUE);
	g_pDevice->LightEnable(1, TRUE);
	g_pDevice->SetRenderState(D3DRS_LIGHTING, true);

	// 하이트맵 셋팅
	m_pMap = new cHeightMap();
	m_pMap->Setup("map/", "HeightMap.raw", "terrain.jpg", 1);

	//테스트 오브젝트 셋팅
	m_pObject = new cNewObject;
	m_pObject->Setup();

	loader = new cAseLoader();
	m_pRootFrame = loader->Load("woman/woman_01_all.ASE");
	 

	//테스트 엑스모델 셋팅
	m_pXmodel = new cXModel("Xfile/bigship1.x");
	m_pXmodel->SetSRT(D3DXVECTOR3(10, 10, 10), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 15, 0));

}

void cMainGame::Update()
{
	m_pCamera->Update(D3DXVECTOR3(0,0,0));
	if (m_pRootFrame)
		m_pRootFrame->Update(m_pRootFrame->GetKeyFrame(), NULL);

}

void cMainGame::Render()
{
	g_pDevice->Clear(NULL, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255, 252, 255), 1.0f, 0);
	g_pDevice->BeginScene();
	//---------------------------------------------------------
	//<-------------------------CODE START---------------------
	m_pGrid->Render();
	m_pMap->Render();
	m_pObject->Render();
	m_pXmodel->Render();

	if (m_pRootFrame)
		m_pRootFrame->Render();

	//<-------------------------CODE END-----------------------
	//---------------------------------------------------------
	g_pDevice->EndScene();
	g_pDevice->Present(NULL, NULL, NULL, NULL);
}

void cMainGame::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pCamera)
	{
		m_pCamera->WndProc(hWnd, message, wParam, lParam);
	}
}

void cMainGame::Setup_HeightMap()
{
	cHeightMap* pMap = new cHeightMap;
	pMap->Setup("HeightMapData/", "HeightMap.raw","terrain.jpg");
	m_pMap = pMap;
}


D3DLIGHT9 cMainGame::InitDirectionalLight(D3DXVECTOR3 * direction, D3DXCOLOR * color)
{
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));

	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Ambient = *color * 0.4f;
	light.Diffuse = *color;
	light.Specular = *color *0.6f;
	light.Direction = *direction;
	return light;
}

D3DLIGHT9 cMainGame::InitPointLight(D3DXVECTOR3 * position, D3DXCOLOR * color)
{
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));

	light.Type = D3DLIGHT_POINT;
	light.Ambient = *color * 0.4f;
	light.Diffuse = *color;
	light.Specular = *color *0.6f;
	light.Position = *position;
	light.Attenuation0 = 1.0f;
	light.Range = 10.0;
	return light;
}

D3DLIGHT9 cMainGame::InitSpotLight(D3DXVECTOR3 * position, D3DXVECTOR3 * direction, D3DXCOLOR * color)
{
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));

	light.Type = D3DLIGHT_SPOT;
	light.Ambient = *color * 0.4f;
	light.Diffuse = *color;
	light.Specular = *color *0.6f;
	light.Direction = *direction;
	light.Position = *position;
	light.Attenuation0 = 0.001f;
	light.Range = 400.0;
	light.Falloff = 1.0f;
	light.Theta = D3DX_PI / 4.0;
	light.Phi = D3DX_PI / 2.0;
	return light;
}