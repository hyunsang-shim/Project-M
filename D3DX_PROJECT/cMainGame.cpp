#include "stdafx.h"
#include "cMainGame.h"

#include "cGrid.h"
#include "cNewObject.h"
#include "cXModel.h"
#include "cAseLoader.h"
#include "cSKY.h"
#include "cSkinnedMesh.h"
#include "cCharacter.h"
#include "cMyCharacter.h"
#include "cFontManager.h"
#include "cOBB.h"

#include "cUIImageView.h"
#include "cUIButton.h"
#include "cUITextView.h"

enum
{
	E_BUTTON_OK = 11,
	E_BUTTON_CANCLE,
	E_BUTTON_EXIT,
	E_TEXT_VIEW,
};

cMainGame::cMainGame()
	:m_pCamera(NULL),
	m_pGrid(NULL),
	m_pMap(NULL),
	m_pObject(NULL),
	m_pXmodel(NULL),
	m_pSKY(NULL),
	m_pMyCharacter(NULL),
	m_pFont(NULL),
	m_p3DText(NULL),
	m_pSprite(NULL),
	m_pTextureUI(NULL),
	m_pUIRoot(NULL)
{
}


cMainGame::~cMainGame()
{
	if (m_pUIRoot) m_pUIRoot->Destroy();
	SAFE_RELEASE(m_pSprite);
	SAFE_RELEASE(m_pTextureUI);

	m_pMap->Destroy();
	SAFE_RELEASE(m_pFont);
	SAFE_RELEASE(m_p3DText);

	g_pFontManager->Destroy();
	SAFE_DELETE(m_pMap);
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pGrid);
	SAFE_DELETE(m_pObject);
	SAFE_DELETE(m_pMyCharacter);
}

void cMainGame::Setup()
{
	// Ä«¸Þ¶ó ¼¼ÆÃ
	m_pCamera = new cCamera();
	m_pCamera->Setup();
	
	// ±×¸®½º ¼¼ÆÃ
	m_pGrid = new cGrid();
	m_pGrid->Setup();

	// ¸ÞÀÎ ±¤¿ø ¼ÂÆÃ
	D3DXVECTOR3 dir(0,-1,0);
	D3DXCOLOR c(1.0f, 1.0f, 1.0f,1.0f);
	ZeroMemory(&DirectLight, sizeof(D3DLIGHT9));
	DirectLight = InitDirectionalLight(&dir, &c);
	g_pDevice->SetLight(0, &DirectLight);
	g_pDevice->SetLight(1, &DirectLight);
	g_pDevice->SetLight(2, &DirectLight);
	g_pDevice->SetLight(3, &DirectLight);
	g_pDevice->LightEnable(0, TRUE);
	g_pDevice->LightEnable(1, TRUE);
	g_pDevice->LightEnable(2, TRUE);
	g_pDevice->LightEnable(3, TRUE);
	g_pDevice->SetRenderState(D3DRS_LIGHTING, true);

	// ÇÏÀÌÆ®¸Ê ¼ÂÆÃ
	m_pMap = new cHeightMap();
	m_pMap->Setup("map/", "HeightMap.raw", "terrain.jpg", 1);

	//Å×½ºÆ® ¿ÀºêÁ§Æ® ¼ÂÆÃ
	m_pObject = new cNewObject;
	m_pObject->Setup("box.obj");
	//m_pObject->SetSRT(D3DXVECTOR3(5.0f, 5.0f, 5.0f), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(-10, 0, 10));
	//
	loader = new cAseLoader();
	m_pRootFrame = loader->Load("woman/woman_01_all.ASE");
	m_pRootFrame->SetSRT(D3DXVECTOR3(5.0f, 5.0f, 5.0f), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(10, 0, 10));

	//Å×½ºÆ® ¿¢½º¸ðµ¨ ¼ÂÆÃ
	m_pXmodel = new cXModel("Xfile/bigship1.x");
	m_pXmodel->SetSRT(D3DXVECTOR3(1.0f, 1.0f,1.0f), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(-15, 0, -15));
	
	//ÇÏ´Ã ¼ÂÆÃ
	m_pSKY = new cSKY();
	m_pSKY->Setup();

	//Áú·µ ¼ÂÆÃ
	m_pMyCharacter = new cMyCharacter;
	m_pMyCharacter->Setup("Xfile" , "Soldier4.x");
	cCharacter* pCharacter = new cCharacter;
	m_pMyCharacter->SetCharacterController(pCharacter);

	//ÆùÆ® ¼ÂÆÃ
	//Creat_Font();

	//UI ¼¼ÆÃ
	Setup_UI();
}

void cMainGame::Update()
{
	g_pTimeManager->Update();

	m_pCamera->Update(m_pMyCharacter->GetPosition());
	if (m_pRootFrame)
		m_pRootFrame->Update(m_pRootFrame->GetKeyFrame(), NULL);

	if (m_pObject)
		m_pObject->Updata();

	if (m_pMyCharacter)
		m_pMyCharacter->Update(m_pCamera->getDirection());


	float y = m_pMyCharacter->GetPosition().y;
	if(m_pMap)
		m_pMap->GetHeight(m_pMyCharacter->GetPosition().x, y, m_pMyCharacter->GetPosition().z);

	m_pMyCharacter->GetCharacterController()->SetPositionY(y);
	if(m_pUIRoot)
		m_pUIRoot->Update();
}

void cMainGame::Render()
{
	g_pDevice->Clear(NULL, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255, 252, 255), 1.0f, 0);
	g_pDevice->BeginScene();
	//---------------------------------------------------------
	//<-------------------------CODE START---------------------
	m_pSKY->Render();
	m_pGrid->Render();
	m_pMap->Render();
	m_pObject->Render();
	m_pXmodel->Render();

	if (m_pRootFrame)
		m_pRootFrame->Render();
	Render_UI();
	//Render_Text();

	g_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	if (cOBB::isCollision(m_pMyCharacter->GetOBB(), m_pObject->GetOBB()))
	{
		if (m_pMyCharacter)
			m_pMyCharacter->Render(NULL);
	}
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
	if (m_pMyCharacter)
	{
		m_pMyCharacter->WndProc(hWnd, message, wParam, lParam);
	}
	switch (message)
	{
	case WM_LBUTTONDOWN:
	{
		static int n = 0;
		//m_pSkinnedMesh->SetAnimationIndex(n++);
		m_pMyCharacter->SetAnimationIndexBlend(n++);
	}
	break;
	}
	
}

void cMainGame::Setup_HeightMap()
{
	cHeightMap* pMap = new cHeightMap;
	pMap->Setup("HeightMapData/", "HeightMap.raw","terrain.jpg");
	m_pMap = pMap;
}

void cMainGame::SetMousePosToCenter(HWND hWnd)
{
	POINT MOUSE;
	RECT client;
	GetClientRect(hWnd, &client);
	MOUSE.y = (client.bottom - client.top) / 2;
	MOUSE.x = (client.right - client.left) / 2;
	ClientToScreen(hWnd, &MOUSE);
	SetCursorPos(MOUSE.x, MOUSE.y);
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

void cMainGame::Creat_Font()
{
	{
		D3DXFONT_DESC fd;
		ZeroMemory(&fd, sizeof(D3DXFONT_DESC));
		fd.Height = 50;
		fd.Width = 25;
		fd.Weight = FW_MEDIUM;
		fd.Italic = false;
		fd.CharSet = DEFAULT_CHARSET;
		fd.OutputPrecision = OUT_DEFAULT_PRECIS;
		fd.PitchAndFamily = FF_DONTCARE;

		{
			AddFontResource("font/umberto.ttf");
			strcpy(fd.FaceName, "umberto");
		}
		D3DXCreateFontIndirect(g_pDevice, &fd, &m_pFont);
	}
	{
		HDC hdc = CreateCompatibleDC(0);
		LOGFONT lf;
		ZeroMemory(&lf, sizeof(LOGFONT));
		lf.lfHeight = 25;
		lf.lfWidth = 12;
		lf.lfWeight = 500;
		lf.lfItalic = false;
		lf.lfUnderline = false;
		lf.lfStrikeOut = false;
		strcpy(lf.lfFaceName, "±¼¸²Ã¼");

		HFONT hFont;
		HFONT hFontOld;
		hFont = CreateFontIndirect(&lf);
		hFontOld = (HFONT)SelectObject(hdc, hFont);
		D3DXCreateText(g_pDevice, hdc, "Direct3DÇÑ±Û", 0.001f, 0.01f, &m_p3DText, 0,0);
		SelectObject(hdc, hFontOld);
		DeleteObject(hFont);
		DeleteDC(hdc);
	}

}

void cMainGame::Render_Text()
{
	{
		string sText("ABC 123 !@#!@#$% °¡³ª´Ù¶ó");
		RECT rc;
		SetRect(&rc, 100, 100, 101, 101);
		m_pFont->DrawTextA(NULL, sText.c_str(), sText.length(), &rc, DT_LEFT | DT_TOP | DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 0));
	}
	{
		string sText("Quest");
		LPD3DXFONT pFont = g_pFontManager->GetFont(cFontManager::E_QUEST);
		RECT rc;
		SetRect(&rc, 100, 200, 101, 101);

		pFont->DrawTextA(NULL, sText.c_str(), strlen(sText.c_str()), &rc, DT_LEFT | DT_TOP | DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 0));

	}
	{
		D3DXMATRIXA16 matWorld, matS, matR, matT;
		D3DXMatrixIdentity(&matS);
		D3DXMatrixIdentity(&matR);
		D3DXMatrixIdentity(&matT);

		D3DXMatrixScaling(&matS, 1.0f, 1.0f, 100.0f);
		D3DXMatrixRotationX(&matR, -D3DX_PI / 4.0f);
		D3DXMatrixTranslation(&matT, -2.0f, 2.0f, 0.0f);
		matWorld = matS * matR * matT;
		g_pDevice->SetTransform(D3DTS_WORLD, &matWorld);
		m_p3DText->DrawSubset(0);
	}


}

void cMainGame::Setup_UI()
{
	D3DXCreateSprite(g_pDevice, &m_pSprite);
	//m_pTextureUI = g_pTextureManager->GetTexture("UI/±èÅÂÈñ.jpg");

	D3DXCreateTextureFromFileEx(
		g_pDevice,
		"UI/±èÅÂÈñ.jpg",
		D3DX_DEFAULT_NONPOW2,
		D3DX_DEFAULT_NONPOW2,
		D3DX_DEFAULT,
		0,
		D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED,
		D3DX_FILTER_NONE,
		D3DX_DEFAULT,
		0,
		&m_stImageInfo,
		NULL,
		&m_pTextureUI);

	{
		cUIImageView* pImageView = new cUIImageView;
		pImageView->SetPosition(0, 0, 0);
		pImageView->SetTexture("./UI/panel-info.png");
		m_pUIRoot = pImageView;

		cUITextView* pTextView = new cUITextView;
		pTextView->Settext("¹ÎÇüÀÇ Äèº¯À» ºô¸ç!!");
		pTextView->SetSize(ST_SIZEN(300, 200));
		pTextView->SetPosition(100, 100);
		pTextView->SetDrawTextFormat(DT_CENTER | DT_VCENTER | DT_WORDBREAK);
		pTextView->SetTextColor(D3DCOLOR_XRGB(255, 255, 0));
		pTextView->SetTag(E_TEXT_VIEW);
		m_pUIRoot->AddChild(pTextView);

		cUIButton* pButtonOK = new cUIButton;
		pButtonOK->SetTexture("./UI/btn-med-up.png",
			"./UI/btn-med-over.png",
			"./UI/btn-med-down.png",
			"OK");
		pButtonOK->SetPosition(150, 330);
		pButtonOK->SetDelegate(this);
		pButtonOK->SetTag(E_BUTTON_OK);
		m_pUIRoot->AddChild(pButtonOK);

		cUIButton* pButtonCancle = new cUIButton;
		pButtonCancle->SetTexture("./UI/btn-med-up.png",
			"./UI/btn-med-over.png",
			"./UI/btn-med-down.png",
			"cancle");
		pButtonCancle->SetPosition(150, 380);
		pButtonCancle->SetDelegate(this);
		pButtonCancle->SetTag(E_BUTTON_CANCLE);
		m_pUIRoot->AddChild(pButtonCancle);

		cUIButton* pButtonExit = new cUIButton;
		pButtonExit->SetTexture("./UI/btn-main-menu.png",
			"./UI/btn-main-menu.png",
			"./UI/btn-main-menu.png");
		pButtonExit->SetPosition(400, 75);
		pButtonExit->SetDelegate(this);
		pButtonExit->SetTag(E_BUTTON_EXIT);
		m_pUIRoot->AddChild(pButtonExit);
	}


}

void cMainGame::Render_UI()
{
	/*m_pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

	D3DXMATRIXA16 matT, matR, matS, mat;
	D3DXMatrixTranslation(&matT, 100, 100, 0);
	
	{
		static float fAngle = 0.0f;
		fAngle += 0.01f;
		D3DXMatrixRotationZ(&matR, fAngle);
		mat = matR*matT;
	}



	m_pSprite->SetTransform(&mat);



	RECT rc;
	SetRect(&rc, 0, 0, m_stImageInfo.Width, m_stImageInfo.Height);
	m_pSprite->Draw(m_pTextureUI,
		&rc,
		&D3DXVECTOR3(0, 0, 0),
		&D3DXVECTOR3(0, 0, 0),
		D3DCOLOR_ARGB(255, 255, 255, 255));
	m_pSprite->End();*/


	if (m_pUIRoot)
		m_pUIRoot->Render(m_pSprite);

}

void cMainGame::OnClick(cUIButton * pSender)
{
	cUITextView* pTextView =
		(cUITextView*)m_pUIRoot->FindChildByTag(E_TEXT_VIEW);

	if (pSender->GetTag() == E_BUTTON_OK)
	{
		pTextView->Settext("¶Ë Àß ½Ó!");
	}
	else if (pSender->GetTag() == E_BUTTON_CANCLE)
	{
		pTextView->Settext("´ÙÀ½ ±âÈ¸¿¡!");
	}
	else if (pSender->GetTag() == E_BUTTON_EXIT)
	{
		m_pUIRoot->m_isHidden = true;
	}
}
