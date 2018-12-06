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
	E_BUTTON_OK,
	SELECT_DIVA,
	SELECT_SOLDIER,
	SELECT_HANZO,
	SELECT_TRACER,
	SELECT_REIN,
	SELECT_REAPER,
	E_BUTTON_READY,
	LOADING_BAR
};

cSCENE_TITLE::cSCENE_TITLE() :
	m_pCamera(NULL),
	m_pSprite(NULL),
	m_pTextureUI(NULL),
	m_pUIRoot(NULL),
	m_pUIShadowRoot(NULL),
	m_pUILoading(NULL),
	m_pUICharacterSelect(NULL),
	enterNameState(0),
	player1_character(NULL)
{
}


cSCENE_TITLE::~cSCENE_TITLE()
{
	SAFE_DELETE(m_pCamera);
	if (m_pUIRoot) m_pUIRoot->Destroy();
	if (m_pUIShadowRoot) m_pUIShadowRoot->Destroy();
	if (m_pUICharacterSelect) m_pUICharacterSelect->Destroy();
	if (m_pUILoading) m_pUILoading->Destroy();

	SAFE_RELEASE(m_pSprite);
	SAFE_RELEASE(m_pTextureUI);
	SAFE_RELEASE(m_pTextureUI);
}

void cSCENE_TITLE::Setup()
{
	//ī�޶� ����
	m_pCamera = new cCamera();
	m_pCamera->Setup();
	//UI����
	BGSetup();
	UIsetup();
	Creat_font();

}

void cSCENE_TITLE::Update()
{
	if (m_pUIRoot)
		m_pUIRoot->Update();
	if (m_pUIShadowRoot)
		m_pUIShadowRoot->Update();
	if (m_pUICharacterSelect)
		m_pUICharacterSelect->Update();
	if (m_pUILoading)
		m_pUILoading->Update();
}

void cSCENE_TITLE::Render()
{
	BGRender();
	UIrender();
	showTime();
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
	//m_pTextureUI = g_pTextureManager->GetTexture("UI/������.jpg");

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
		pTextView->Settext("�̸��� �Է��ϼ���");
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
	{
		cUIImageView* pImageView = new cUIImageView;
		pImageView->SetPosition(0, 0, 0);
		//pImageView->SetTexture("./UI/test2.png");
		m_pUICharacterSelect = pImageView;

		

		cUIButton* selectBack = new cUIButton;
		selectBack->SetTexture("./UI/selectBack.jpg",
			"./UI/selectBack.jpg",
			"./UI/selectBack.jpg");
		selectBack->setSize(1.0f,1.0f);
		selectBack->SetPosition(g_pGameInfoManager->getScreenXPosByPer(0), g_pGameInfoManager->getScreenYPosByPer(0));
		selectBack->SetDelegate(this);
		m_pUICharacterSelect->AddChild(selectBack);

	cUIButton* timerBack = new cUIButton;
		timerBack->SetTexture("./UI/timer_back.png",
			"./UI/timer_back.png",
			"./UI/timer_back.png");
		timerBack->setSize(0.3f, 0.3f);
		timerBack->SetPosition(g_pGameInfoManager->getScreenXPosByPer(44), g_pGameInfoManager->getScreenYPosByPer(0));
		timerBack->SetDelegate(this);
		m_pUICharacterSelect->AddChild(timerBack);

		cUIButton* selectCharacterBack = new cUIButton;
		selectCharacterBack->SetTexture("./UI/select_back.png",
			"./UI/select_back.png",
			"./UI/select_back.png");
		selectCharacterBack->setSize(0.65f, 0.65f);
		selectCharacterBack->SetPosition(g_pGameInfoManager->getScreenXPosByPer(0)-50, g_pGameInfoManager->getScreenYPosByPer(27));
		selectCharacterBack->SetDelegate(this);
		m_pUICharacterSelect->AddChild(selectCharacterBack);

		cUIButton* select_divide = new cUIButton;
		select_divide->SetTexture("./UI/select_divide.png",
			"./UI/select_divide.png",
			"./UI/select_divide.png");
		select_divide->setSize(1.0f, 1.0f);
		select_divide->SetPosition(g_pGameInfoManager->getScreenXPosByPer(10)-150 - 30, g_pGameInfoManager->getScreenYPosByPer(30));
		select_divide->SetDelegate(this);
		m_pUICharacterSelect->AddChild(select_divide);

		cUIButton* diva_select = new cUIButton;
		diva_select->SetTexture("./UI/diva_select.png",
			"./UI/diva_select.png",
			"./UI/diva_select.png");
		diva_select->setSize(0.98f, 0.98f);
		diva_select->SetPosition(g_pGameInfoManager->getScreenXPosByPer(3)+17 - 30, g_pGameInfoManager->getScreenYPosByPer(33)+2);
		diva_select->SetDelegate(this);
		diva_select->SetTag(SELECT_DIVA);
		m_pUICharacterSelect->AddChild(diva_select);

		cUIButton* hanzo_select = new cUIButton;
		hanzo_select->SetTexture("./UI/hanzo_select.png",
			"./UI/hanzo_select.png",
			"./UI/hanzo_select.png");
		hanzo_select->setSize(0.95f, 0.95f);
		hanzo_select->SetPosition(g_pGameInfoManager->getScreenXPosByPer(3)+17 - 30, g_pGameInfoManager->getScreenYPosByPer(45));
		hanzo_select->SetDelegate(this);
		hanzo_select->SetTag(SELECT_HANZO);
		m_pUICharacterSelect->AddChild(hanzo_select);

		cUIButton* tracer_select = new cUIButton;
		tracer_select->SetTexture("./UI/tracer_select.png",
			"./UI/tracer_select.png",
			"./UI/tracer_select.png");
		tracer_select->setSize(0.97f, 0.97f);
		tracer_select->SetPosition(g_pGameInfoManager->getScreenXPosByPer(3)+17 - 30, g_pGameInfoManager->getScreenYPosByPer(56));
		tracer_select->SetDelegate(this);
		tracer_select->SetTag(SELECT_TRACER);
		m_pUICharacterSelect->AddChild(tracer_select);

		cUIButton* ripper_select = new cUIButton;
		ripper_select->SetTexture("./UI/reaper_select.png",
			"./UI/reaper_select.png",
			"./UI/reaper_select.png");
		ripper_select->setSize(0.96f, 0.96f);
		ripper_select->SetPosition(g_pGameInfoManager->getScreenXPosByPer(9)+6 - 30, g_pGameInfoManager->getScreenYPosByPer(39));
		ripper_select->SetDelegate(this);
		ripper_select->SetTag(SELECT_REAPER);
		m_pUICharacterSelect->AddChild(ripper_select);

		cUIButton* Rein_select = new cUIButton;
		Rein_select->SetTexture("./UI/rein_select.png",
			"./UI/rein_select.png",
			"./UI/rein_select.png");
		Rein_select->setSize(0.93f, 0.93f);
		Rein_select->SetPosition(g_pGameInfoManager->getScreenXPosByPer(9)+13 - 30, g_pGameInfoManager->getScreenYPosByPer(51));
		Rein_select->SetDelegate(this);
		Rein_select->SetTag(SELECT_REIN);
		m_pUICharacterSelect->AddChild(Rein_select);

		cUIButton* soldier_select = new cUIButton;
		soldier_select->SetTexture("./UI/soldier_select.png",
			"./UI/soldier_select.png",
			"./UI/soldier_select.png");
		soldier_select->setSize(0.95f, 0.95f);
		soldier_select->SetPosition(g_pGameInfoManager->getScreenXPosByPer(15)+2 - 30, g_pGameInfoManager->getScreenYPosByPer(45));
		soldier_select->SetDelegate(this);
		soldier_select->SetTag(SELECT_SOLDIER);
		m_pUICharacterSelect->AddChild(soldier_select);

	
		player1_character = new cUIButton;
	

		player1_character->SetPosition(g_pGameInfoManager->getScreenXPosByPer(30), g_pGameInfoManager->getScreenYPosByPer(15));
		player1_character->SetTexture("./UI/trans.png",
			"./UI/trans.png",
			"./UI/trans.png"); 
		player1_character->setSize(1.5f, 1.5f);
		player1_character->SetDelegate(this);
		m_pUICharacterSelect->AddChild(player1_character);

	
	




		cUIButton* player2_back = new cUIButton;
		player2_back->SetTexture("./UI/otherPlayerBack.png",
			"./UI/otherPlayerBack.png",
			"./UI/otherPlayerBack.png");
		player2_back->setSize(0.7f, 0.7f);
		player2_back->SetPosition(g_pGameInfoManager->getScreenXPosByPer(82), g_pGameInfoManager->getScreenYPosByPer(34)+12);
		player2_back->SetDelegate(this);
		m_pUICharacterSelect->AddChild(player2_back);

		cUIButton* player3_back = new cUIButton;
		player3_back->SetTexture("./UI/otherPlayerBack.png",
			"./UI/otherPlayerBack.png",
			"./UI/otherPlayerBack.png");
		player3_back->setSize(0.7f, 0.7f);
		player3_back->SetPosition(g_pGameInfoManager->getScreenXPosByPer(82), g_pGameInfoManager->getScreenYPosByPer(49)+5 );
		player3_back->SetDelegate(this);
		m_pUICharacterSelect->AddChild(player3_back);

		cUIButton* player4_back = new cUIButton;
		player4_back->SetTexture("./UI/otherPlayerBack.png",
			"./UI/otherPlayerBack.png",
			"./UI/otherPlayerBack.png");
		player4_back->setSize(0.7f, 0.7f);
		player4_back->SetPosition(g_pGameInfoManager->getScreenXPosByPer(82), g_pGameInfoManager->getScreenYPosByPer(63) + 5);
		player4_back->SetDelegate(this);
		m_pUICharacterSelect->AddChild(player4_back);




		cUIButton* player2_chaback = new cUIButton;
		player2_chaback->SetTexture("./UI/chaback.png",
			"./UI/chaback.png",
			"./UI/chaback.png");
		player2_chaback->setSize(1.0f, 1.0f);
		player2_chaback->SetPosition(g_pGameInfoManager->getScreenXPosByPer(84), g_pGameInfoManager->getScreenYPosByPer(36));
		player2_chaback->SetDelegate(this);
		m_pUICharacterSelect->AddChild(player2_chaback);

		cUIButton* player3_chaback = new cUIButton;
		player3_chaback->SetTexture("./UI/chaback.png",
			"./UI/chaback.png",
			"./UI/chaback.png");
		player3_chaback->setSize(1.0f, 1.0f);
		player3_chaback->SetPosition(g_pGameInfoManager->getScreenXPosByPer(84), g_pGameInfoManager->getScreenYPosByPer(50));
		player3_chaback->SetDelegate(this);
		m_pUICharacterSelect->AddChild(player3_chaback);

		cUIButton* player4_chaback = new cUIButton;
		player4_chaback->SetTexture("./UI/chaback.png",
			"./UI/chaback.png",
			"./UI/chaback.png");
		player4_chaback->setSize(1.0f, 1.0f);
		player4_chaback->SetPosition(g_pGameInfoManager->getScreenXPosByPer(84), g_pGameInfoManager->getScreenYPosByPer(64));
		player4_chaback->SetDelegate(this);
		m_pUICharacterSelect->AddChild(player4_chaback);


		for (int i = 0; i < 3; i++)
		{
			cUIButton* newButton = new cUIButton;
			other_player_character.push_back(newButton);
			other_player_character.back()->SetTexture("./UI/default_portrait.png",
				"./UI/default_portrait.png",
				"./UI/default_portrait.png");
			other_player_character.back()->setSize(0.33f, 0.33f);
			other_player_character.back()->SetPosition(g_pGameInfoManager->getScreenXPosByPer(82)+9, g_pGameInfoManager->getScreenYPosByPer(31 + 14 * i) + 12);
			other_player_character.back()->SetDelegate(this);
			m_pUICharacterSelect->AddChild(other_player_character.back());
		}



		ready_button = new cUIButton;
		ready_button->SetTexture("./UI/ready.png",
			"./UI/ready.png",
			"./UI/ready.png");
		ready_button->setSize(1.0f, 1.0f);
		ready_button->SetPosition(g_pGameInfoManager->getScreenXPosByPer(78) , g_pGameInfoManager->getScreenYPosByPer(83));
		ready_button->SetDelegate(this);
		ready_button->SetTag(E_BUTTON_READY);
		m_pUICharacterSelect->AddChild(ready_button);




		m_pUICharacterSelect->m_isHidden = 1;
	}

	{
		cUIImageView* pImageView = new cUIImageView;
		pImageView->SetPosition(0, 0, 0);
		m_pUILoading = pImageView;

		cUIButton* loading_back = new cUIButton;
		loading_back->SetTexture("./UI/loading_back.TGA",
			"./UI/loading_back.TGA",
			"./UI/loading_back.TGA");
		loading_back->setSize(1.0f, 1.0f);
		loading_back->SetPosition(g_pGameInfoManager->getScreenXPosByPer(0), g_pGameInfoManager->getScreenYPosByPer(0));
		loading_back->SetDelegate(this);
		m_pUILoading->AddChild(loading_back);
		
		cUIButton* loading_bar1 = new cUIButton;
		loading_bar1->SetTexture("./UI/loadingbar1.png",
			"./UI/loadingbar1.png",
			"./UI/loadingbar1.png");
		loading_bar1->setSize(1.0f, 1.0f);
		loading_bar1->SetPosition(g_pGameInfoManager->getScreenXPosByPer(0), g_pGameInfoManager->getScreenYPosByPer(97));
		loading_bar1->SetDelegate(this);
		m_pUILoading->AddChild(loading_bar1);
		
		cUIButton* loading_bar2 = new cUIButton;
		loading_bar2->SetTexture("./UI/loadingbar2.png",
			"./UI/loadingbar2.png",
			"./UI/loadingbar2.png");
		loading_bar2->setSize(1.0f, 1.0f);
		loading_bar2->SetPosition(g_pGameInfoManager->getScreenXPosByPer(0), g_pGameInfoManager->getScreenYPosByPer(97));
		loading_bar2->SetDelegate(this);
		loading_bar2->SetTag(LOADING_BAR);
		m_pUILoading->AddChild(loading_bar2);


		m_pUILoading->m_isHidden = 1;
	}
}

void cSCENE_TITLE::UIrender()
{
	if (m_pUIRoot)
		m_pUIRoot->Render(m_pSprite);

	if (m_pUIShadowRoot)
		m_pUIShadowRoot->Render(m_pSprite);


	if (m_pUICharacterSelect)
		m_pUICharacterSelect->Render(m_pSprite);


	if (m_pUILoading)
		m_pUILoading->Render(m_pSprite);

}

void cSCENE_TITLE::OtherPlayerUpdate()
{
	
	

}

void cSCENE_TITLE::showTime()
{
	int time = 30;
	string remain_time = to_string(time);

	RECT rc;
	SetRect(&rc,
		g_pGameInfoManager->getScreenXPosByPer(44),
		g_pGameInfoManager->getScreenYPosByPer(0),
		g_pGameInfoManager->getScreenXPosByPer(44) + 30,
		g_pGameInfoManager->getScreenYPosByPer(0) + 30);
	if(!m_pUICharacterSelect->m_isHidden)	
		m_pFont->DrawTextA(NULL, remain_time.c_str(), remain_time.length(), &rc, DT_LEFT | DT_TOP | DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));
}

void cSCENE_TITLE::Creat_font()
{
	D3DXFONT_DESC fd;
	ZeroMemory(&fd, sizeof(D3DXFONT_DESC));
	fd.Height = 100;
	fd.Width = 50;
	fd.Weight = FW_MEDIUM;
	fd.Italic = false;
	fd.CharSet = DEFAULT_CHARSET;
	fd.OutputPrecision = OUT_DEFAULT_PRECIS;
	fd.PitchAndFamily = FF_DONTCARE;

	{
		AddFontResource("font/BigNoodleTooOblique.ttf");
		strcpy(fd.FaceName, "BigNoodleTooOblique");
	}
	D3DXCreateFontIndirect(g_pDevice, &fd, &m_pFont);
	fd.Height = 30;
	fd.Width = 15;
	D3DXCreateFontIndirect(g_pDevice, &fd, &m_pFont2);

}



void cSCENE_TITLE::OnClick(cUIButton * pSender)
{
	cUITextView* pTextView =
		(cUITextView*)m_pUIRoot->FindChildByTag(E_TEXT_VIEW);

	if (pSender->GetTag() == E_BUTTON_PLAY)
	{
		m_pUIShadowRoot->m_isHidden = !m_pUIShadowRoot->m_isHidden;
		m_pUIRoot->m_isHidden = 0;
		m_pUIShadowRoot->setable();
	}

	else if (pSender->GetTag() == E_BUTTON_EXIT)
	{
		//네트워크 수정 필요
		//g_pNetworkManager->SendData("disconnect", g_pGameInfoManager->GetMyInfo());
		exit(0);
	}
	else if (pSender->GetTag() == E_BUTTON_OK)
	{
		g_pGameInfoManager->SetMyName();
		//g_pGameInfoManager->sceneChangeTriger = TRUE;
		//g_pGameInfoManager->nextScene = 1;
		g_pGameInfoManager->SetMyCharacter(PC_Soldier);
		//네트워크 수정 필요
		//g_pNetworkManager->SendData("join", g_pGameInfoManager->GetMyInfo());
		m_pUIRoot->m_isHidden = TRUE;
		m_pUIShadowRoot->m_isHidden = TRUE;
		m_pUICharacterSelect->m_isHidden = FALSE;
	}
	else if (pSender->GetTag() == E_BUTTON_CANCLE)
	{
		m_pUIShadowRoot->setUnable();
		m_pUIShadowRoot->m_isHidden = TRUE;
		m_pUIRoot->setable();
	}
	else if (pSender->GetTag() == SELECT_DIVA)
	{
		g_pGameInfoManager->m_strMyCharacter.Character_No = PC_DIVA;
		player1_character->SetPosition(g_pGameInfoManager->getScreenXPosByPer(30), g_pGameInfoManager->getScreenYPosByPer(15));
		player1_character->SetTexture("./UI/diva.png", "./UI/diva.png", "./UI/diva.png");
	}
	else if (pSender->GetTag() == SELECT_REAPER)
	{
		g_pGameInfoManager->m_strMyCharacter.Character_No = PC_Reaper;
		player1_character->SetPosition(g_pGameInfoManager->getScreenXPosByPer(30), g_pGameInfoManager->getScreenYPosByPer(15));
		player1_character->SetTexture("./UI/reaper.png", "./UI/reaper.png", "./UI/reaper.png");
	}
	else if (pSender->GetTag() == SELECT_HANZO)
	{
		g_pGameInfoManager->m_strMyCharacter.Character_No = PC_Hanzo;
		player1_character->SetPosition(g_pGameInfoManager->getScreenXPosByPer(37), g_pGameInfoManager->getScreenYPosByPer(15));
		player1_character->SetTexture("./UI/hanzo.png", "./UI/hanzo.png", "./UI/hanzo.png");
	}
	else if (pSender->GetTag() == SELECT_REIN)
	{
		g_pGameInfoManager->m_strMyCharacter.Character_No = PC_Reinhardt;
		player1_character->SetPosition(g_pGameInfoManager->getScreenXPosByPer(20), g_pGameInfoManager->getScreenYPosByPer(15));
		player1_character->SetTexture("./UI/rein.png", "./UI/rein.png", "./UI/rein.png");
	}
	else if (pSender->GetTag() == SELECT_TRACER)
	{
		g_pGameInfoManager->m_strMyCharacter.Character_No = PC_Tracer;
		player1_character->SetPosition(g_pGameInfoManager->getScreenXPosByPer(40), g_pGameInfoManager->getScreenYPosByPer(15));
		player1_character->SetTexture("./UI/tracer.png", "./UI/tracer.png", "./UI/tracer.png");
	}
	else if (pSender->GetTag() == SELECT_SOLDIER)
	{
		g_pGameInfoManager->m_strMyCharacter.Character_No = PC_Soldier;
		player1_character->SetPosition(g_pGameInfoManager->getScreenXPosByPer(40), g_pGameInfoManager->getScreenYPosByPer(15));
		player1_character->SetTexture("./UI/soldier.png", "./UI/soldier.png", "./UI/soldier.png");
	}
	else if (pSender->GetTag() == E_BUTTON_READY)
	{
		static bool ready = false;
		if (ready)
		{
			ready_button->SetTexture("./UI/ready.png",
				"./UI/ready.png",
				"./UI/ready.png");
			ready = false;
		}
		else if (g_pGameInfoManager->m_strMyCharacter.Character_No != 0)
		{
			ready_button->SetTexture("./UI/ready_set.png",
				"./UI/ready_set.png",
				"./UI/ready_set.png");
			ready = true;

			m_pUICharacterSelect->m_isHidden = 1;
			m_pUILoading->m_isHidden = 0;
		}
	}
}


void cSCENE_TITLE::buttonUpdate(cUIButton * pSender)
{
	
	if (pSender->GetTag() == LOADING_BAR)
	{
		static float load = 0.0f;
		/*if(g_pGameInfoManager->loading == 0 && load>0.5)
			load = 0.5;*/

		load += 0.015f;
		pSender->cutSize(load, 1.0f);
		if (load > 1.0f)
		{
			g_pGameInfoManager->sceneChangeTriger = TRUE;
			g_pGameInfoManager->nextScene = 1;
		}
	}

}
