#include "stdafx.h"
#include "cSCENE_TITLE.h"

#include "cUIImageView.h"
#include "cUIButton.h"
#include "cUITextView.h"
enum
{
	E_BUTTON_PLAY = 11,
	E_BUTTON_CANCLE,
	E_BUTTON_EXIT,
	E_TEXT_VIEW,
	E_BUTTON_OK
};

cSCENE_TITLE::cSCENE_TITLE() :
	m_pCamera(NULL),
	m_pSprite(NULL),
	m_pTextureUI(NULL),
	m_pUIRoot(NULL),
	m_pUIShadowRoot(NULL),
	enterNameState(0)
{
}


cSCENE_TITLE::~cSCENE_TITLE()
{
	SAFE_DELETE(m_pCamera);
	if (m_pUIRoot) m_pUIRoot->Destroy();
	if (m_pUIShadowRoot) m_pUIShadowRoot->Destroy();
	SAFE_RELEASE(m_pSprite);
	SAFE_RELEASE(m_pTextureUI);
}

void cSCENE_TITLE::Setup()
{
	//카메라 셋팅
	m_pCamera = new cCamera();
	m_pCamera->Setup();
	//UI셋팅
	BGSetup();
	UIsetup();

}

void cSCENE_TITLE::Update()
{
	if (m_pUIRoot)
		m_pUIRoot->Update();
	if (m_pUIShadowRoot)
		m_pUIShadowRoot->Update();
}

void cSCENE_TITLE::Render()
{
	BGRender();
	UIrender();
}

void cSCENE_TITLE::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	switch (message)
	{
	case WM_CHAR:
		if (m_pUIShadowRoot->m_isHidden == 0)
		{
			if (wParam == VK_BACK &&g_pGameInfoManager->namelength > 0) g_pGameInfoManager->namelength--;
			else if (g_pGameInfoManager->namelength < 10)g_pGameInfoManager->userName[g_pGameInfoManager->namelength++] = wParam;
			g_pGameInfoManager->userName[g_pGameInfoManager->namelength] = NULL;

			m_pNameInput->Settext(g_pGameInfoManager->userName);			
		}
		break;


	default:
		break;
	}


}

void cSCENE_TITLE::BGSetup()
{
	D3DXCreateSprite(g_pDevice, &m_pSprite);
	//m_pTextureUI = g_pTextureManager->GetTexture("UI/김태희.jpg");

	D3DXCreateTextureFromFileEx(
		g_pDevice,
		"UI/wait_srceen.jpg",
		D3DX_DEFAULT_NONPOW2,
		D3DX_DEFAULT_NONPOW2,
		D3DX_DEFAULT,
		0,
		D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED,
		D3DX_FILTER_NONE,
		D3DX_DEFAULT,
		0,
		&m_stBGImageInfo,
		NULL,
		&m_pTextureUI);
}

void cSCENE_TITLE::BGRender()
{
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

	D3DXMATRIXA16 matT, matR, matS, mat;
	D3DXMatrixTranslation(&matT, 0, 0, 0);

	{
		static float fAngle = 0.0f;
		D3DXMatrixScaling(&matS, (float)g_pGameInfoManager->GameScreenSizeX / (float)m_stBGImageInfo.Width, (float)g_pGameInfoManager->GameScreenSizeY / (float)m_stBGImageInfo.Height, 1.0f);
		D3DXMatrixRotationZ(&matR, fAngle);
		mat = matS*matR*matT;
	}


	m_pSprite->SetTransform(&mat);



	RECT rc;
	SetRect(&rc, 0, 0, m_stBGImageInfo.Width, m_stBGImageInfo.Height);
	m_pSprite->Draw(m_pTextureUI,
		&rc,
		&D3DXVECTOR3(0, 0, 0),
		&D3DXVECTOR3(0, 0, 0),
		D3DCOLOR_ARGB(255, 255, 255, 255));
	m_pSprite->End();
}

void cSCENE_TITLE::UIsetup()
{
	{
		cUIImageView* pImageView = new cUIImageView;
		pImageView->SetPosition(0, 0, 0);
		//pImageView->SetTexture("./UI/test2.png");
		m_pUIRoot = pImageView;


		cUIButton* pButtonOK = new cUIButton;
		pButtonOK->SetTexture("./UI/play_button.png",
			"./UI/play_button.png",
			"./UI/play_button.png",
			" ");
		pButtonOK->SetPosition(g_pGameInfoManager->getScreenXPosByPer(80), g_pGameInfoManager->getScreenYPosByPer(70));
		pButtonOK->SetDelegate(this);
		pButtonOK->SetTag(E_BUTTON_PLAY);
		m_pUIRoot->AddChild(pButtonOK);

		cUIButton* pButtonCancle = new cUIButton;
		pButtonCancle->SetTexture("./UI/title_image.png",
			"./UI/title_image.png",
			"./UI/title_image.png",
			" ");
		pButtonCancle->setSize(0.2f, 0.2f);
		pButtonCancle->SetPosition(g_pGameInfoManager->getScreenXPosByPer(85), g_pGameInfoManager->getScreenYPosByPer(5));
		pButtonCancle->SetDelegate(this);
		m_pUIRoot->AddChild(pButtonCancle);

		cUIButton* pButtonExit = new cUIButton;
		pButtonExit->SetTexture("./UI/EXIT.png",
			"./UI/EXIT.png",
			"./UI/EXIT.png");
		pButtonExit->SetPosition(g_pGameInfoManager->getScreenXPosByPer(80), g_pGameInfoManager->getScreenYPosByPer(77));
		pButtonExit->SetDelegate(this);
		pButtonExit->SetTag(E_BUTTON_EXIT);
		m_pUIRoot->AddChild(pButtonExit);


	}

	{
		cUIImageView* pImageView = new cUIImageView;
		pImageView->SetPosition(0, 0, 0);
		//pImageView->SetTexture("./UI/test2.png");
		m_pUIShadowRoot = pImageView;

		cUIButton* pButtontest = new cUIButton;
		pButtontest->SetTexture("./UI/test3.png",
			"./UI/test3.png",
			"./UI/test3.png");
		pButtontest->setSize(5.0f, 2.0f);
		pButtontest->SetPosition(g_pGameInfoManager->getScreenXPosByPer(0), g_pGameInfoManager->getScreenYPosByPer(0));
		pButtontest->SetDelegate(this);
		m_pUIShadowRoot->AddChild(pButtontest);


		cUITextView* pTextView = new cUITextView;
		pTextView->Settext("이름을 입력하세요");
		pTextView->SetSize(ST_SIZEN(500, 200));
		pTextView->SetPosition(g_pGameInfoManager->getScreenXPosByPer(35), g_pGameInfoManager->getScreenYPosByPer(40));
		pTextView->SetDrawTextFormat(DT_CENTER | DT_VCENTER);
		pTextView->SetTextColor(D3DCOLOR_XRGB(255, 255, 0));

		m_pUIShadowRoot->AddChild(pTextView);

		cUIButton* input_space = new cUIButton;
		input_space->SetTexture("./UI/input.png",
			"./UI/input.png",
			"./UI/input.png");
		input_space->setSize(1.15f, 1.0f);
		input_space->SetPosition(g_pGameInfoManager->getScreenXPosByPer(35), g_pGameInfoManager->getScreenYPosByPer(55));
		input_space->SetDelegate(this);
		m_pUIShadowRoot->AddChild(input_space);

		m_pNameInput = new cUITextView;
		m_pNameInput->Settext(g_pGameInfoManager->userName);
		m_pNameInput->SetSize(ST_SIZEN(300, 50));
		m_pNameInput->SetPosition(g_pGameInfoManager->getScreenXPosByPer(40), g_pGameInfoManager->getScreenYPosByPer(55));
		m_pNameInput->SetDrawTextFormat(DT_CENTER | DT_VCENTER | DT_WORDBREAK);
		m_pNameInput->SetTextColor(D3DCOLOR_XRGB(255, 255, 0));
		m_pNameInput->SetTag(E_TEXT_VIEW);
		m_pUIShadowRoot->AddChild(m_pNameInput);

		cUIButton* OK_button = new cUIButton;
		OK_button->SetTexture("./UI/OK.png",
			"./UI/OK.png",
			"./UI/OK.png");
		OK_button->setSize(1.0f, 1.0f);
		OK_button->SetPosition(g_pGameInfoManager->getScreenXPosByPer(35), g_pGameInfoManager->getScreenYPosByPer(65));
		OK_button->SetDelegate(this);
		OK_button->SetTag(E_BUTTON_OK);
		m_pUIShadowRoot->AddChild(OK_button);

		cUIButton* cancle_button = new cUIButton;
		cancle_button->SetTexture("./UI/cancle.png",
			"./UI/cancle.png",
			"./UI/cancle.png");
		cancle_button->setSize(0.5f, 1.0f);
		cancle_button->SetPosition(g_pGameInfoManager->getScreenXPosByPer(48), g_pGameInfoManager->getScreenYPosByPer(65));
		cancle_button->SetDelegate(this);
		cancle_button->SetTag(E_BUTTON_CANCLE);
		m_pUIShadowRoot->AddChild(cancle_button);


		m_pUIShadowRoot->m_isHidden = 1;
	}
}

void cSCENE_TITLE::UIrender()
{
	if (m_pUIRoot)
		m_pUIRoot->Render(m_pSprite);

	if (m_pUIShadowRoot)
		m_pUIShadowRoot->Render(m_pSprite);

}

void cSCENE_TITLE::OnClick(cUIButton * pSender)
{
	cUITextView* pTextView =
		(cUITextView*)m_pUIRoot->FindChildByTag(E_TEXT_VIEW);

	if (pSender->GetTag() == E_BUTTON_PLAY)
	{
		m_pUIShadowRoot->m_isHidden = !m_pUIShadowRoot->m_isHidden;
		m_pUIRoot->setUnable();
		m_pUIShadowRoot->setable();
	}

	else if (pSender->GetTag() == E_BUTTON_EXIT)
	{

	}
	else if (pSender->GetTag() == E_BUTTON_OK)
	{
		g_pGameInfoManager->SetMyName();
		g_pGameInfoManager->sceneChangeTriger = TRUE;
		g_pGameInfoManager->nextScene = 1;
		g_pNetworkManager->SendData("join", g_pGameInfoManager->GetMyInfo());

	}
	else if (pSender->GetTag() == E_BUTTON_CANCLE)
	{
		m_pUIShadowRoot->setUnable();
		m_pUIShadowRoot->m_isHidden = TRUE;
		m_pUIRoot->setable();
	}
}