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
	LOADING_BAR,
	OTHER_PORTRAIT1,
	OTHER_PORTRAIT2,
	OTHER_PORTRAIT3,
	OTHER_NAME1,
	OTHER_NAME2,
	OTHER_NAME3,
	OTHER_READY_BUTTON1,
	OTHER_READY_BUTTON2,
	OTHER_READY_BUTTON3
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
	t1.join();
	t2.join();
}

void cSCENE_TITLE::Setup()
{
	//camera setting
	m_pCamera = new cCamera();
	m_pCamera->Setup();
	//UI setting
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

	if (playerName1)
	{
		if (g_pOtherPlayerManager->otherPlayerInfo.size() == 0)
		{
			playerName1->Settext(" ");
		}
		else
		{
			playerName1->Settext(g_pOtherPlayerManager->otherPlayerInfo.at(0)->info.PlayerName);
		}
	}
	if (playerName2)
	{
		if ( g_pOtherPlayerManager->otherPlayerInfo.size()  <  2)
		{
			playerName2->Settext(" ");
		}
		else
		{
			playerName2->Settext(g_pOtherPlayerManager->otherPlayerInfo.at(1)->info.PlayerName);
		}
	}
	if (playerName3)
	{
		if ( g_pOtherPlayerManager->otherPlayerInfo.size() < 3)
		{
			playerName3->Settext(" ");
		}
		else
		{
			playerName3->Settext(g_pOtherPlayerManager->otherPlayerInfo.at(2)->info.PlayerName);
		}
	}
	
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
	g_pGameInfoManager->Play(1, 0);
	g_pGameInfoManager->channel_volum_set(0, 0.03f);
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
		pTextView->Settext("Enter your name");
		pTextView->SetFontType(g_pFontManager->E_MAX);
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
		m_pNameInput->SetSize(ST_SIZEN(300, 60));
		m_pNameInput->SetPosition(g_pGameInfoManager->getScreenXPosByPer(40), g_pGameInfoManager->getScreenYPosByPer(55));
		m_pNameInput->SetDrawTextFormat(DT_CENTER | DT_VCENTER | DT_WORDBREAK);
		m_pNameInput->SetTextColor(D3DCOLOR_XRGB(230, 230, 230));
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
		selectCharacterBack->SetPosition(g_pGameInfoManager->getScreenXPosByPer(-15)-50, g_pGameInfoManager->getScreenYPosByPer(27));
		selectCharacterBack->SetDelegate(this);
		m_pUICharacterSelect->AddChild(selectCharacterBack);

		cUIButton* soldier_select = new cUIButton;
		soldier_select->SetTexture("./UI/soldier_select.png",
			"./UI/soldier_select.png",
			"./UI/soldier_select.png");
		soldier_select->setSize(0.95f, 0.95f);
		soldier_select->SetPosition(g_pGameInfoManager->getScreenXPosByPer(2)+2 - 30, g_pGameInfoManager->getScreenYPosByPer(45));
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



		playerName1 = new cUITextView;
		playerName1->Settext("  ");
		playerName1->SetFontType(g_pFontManager->E_DEFAULT);
		playerName1->SetSize(ST_SIZEN(200, 50));
		playerName1->SetPosition(g_pGameInfoManager->getScreenXPosByPer(91), g_pGameInfoManager->getScreenYPosByPer(34) + 12);
		playerName1->SetDrawTextFormat(DT_LEFT | DT_VCENTER);
		playerName1->SetTag(OTHER_NAME1);
		playerName1->SetTextColor(D3DCOLOR_XRGB(20, 20, 20));
		m_pUICharacterSelect->AddChild(playerName1);

		playerName2 = new cUITextView;
		playerName2->Settext("  ");
		playerName2->SetFontType(g_pFontManager->E_DEFAULT);
		playerName2->SetSize(ST_SIZEN(200, 50));
		playerName2->SetPosition(g_pGameInfoManager->getScreenXPosByPer(91), g_pGameInfoManager->getScreenYPosByPer(50) - 5);
		playerName2->SetDrawTextFormat(DT_LEFT | DT_VCENTER);
		playerName1->SetTag(OTHER_NAME2);
		playerName2->SetTextColor(D3DCOLOR_XRGB(20, 20, 20));
		m_pUICharacterSelect->AddChild(playerName2);

		playerName3 = new cUITextView;
		playerName3->Settext("  ");
		playerName3->SetFontType(g_pFontManager->E_DEFAULT);
		playerName3->SetSize(ST_SIZEN(200, 50));
		playerName3->SetPosition(g_pGameInfoManager->getScreenXPosByPer(91), g_pGameInfoManager->getScreenYPosByPer(63) + 5);
		playerName3->SetDrawTextFormat(DT_LEFT | DT_VCENTER);
		playerName1->SetTag(OTHER_NAME3);
		playerName3->SetTextColor(D3DCOLOR_XRGB(20, 20, 20));
		m_pUICharacterSelect->AddChild(playerName3);




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



		cUIButton* player2_ready = new cUIButton;
		player2_ready->SetTexture("./UI/ready_set.png",
			"./UI/ready_set.png",
			"./UI/ready_set.png");
		player2_ready->setSize(0.4f, 0.4f);
		player2_ready->SetPosition(g_pGameInfoManager->getScreenXPosByPer(91), g_pGameInfoManager->getScreenYPosByPer(41) + 12);
		player2_ready->SetDelegate(this);
		player2_ready->m_isHidden = true;
		player2_ready->SetTag(OTHER_READY_BUTTON1);
		m_pUICharacterSelect->AddChild(player2_ready);

		cUIButton* player3_ready = new cUIButton;
		player3_ready->SetTexture("./UI/ready_set.png",
			"./UI/ready_set.png",
			"./UI/ready_set.png");
		player3_ready->setSize(0.4f, 0.4f);
		player3_ready->SetPosition(g_pGameInfoManager->getScreenXPosByPer(91), g_pGameInfoManager->getScreenYPosByPer(56) + 5);
		player3_ready->SetDelegate(this);
		player3_ready->m_isHidden = true;
		player3_ready->SetTag(OTHER_READY_BUTTON2);
		m_pUICharacterSelect->AddChild(player3_ready);

		cUIButton* player4_ready = new cUIButton;
		player4_ready->SetTexture("./UI/ready_set.png",
			"./UI/ready_set.png",
			"./UI/ready_set.png");
		player4_ready->setSize(0.4f, 0.4f);
		player4_ready->m_isHidden = true;
		player4_ready->SetPosition(g_pGameInfoManager->getScreenXPosByPer(91), g_pGameInfoManager->getScreenYPosByPer(70) + 5);
		player4_ready->SetDelegate(this);
		player4_ready->SetTag(OTHER_READY_BUTTON3);
		m_pUICharacterSelect->AddChild(player4_ready);



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
			other_player_character.back()->SetTag(OTHER_PORTRAIT1+i);
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

		cUIButton* loads = new cUIButton;
		loads->SetTexture("./UI/loads.png",
			"./UI/loads.png",
			"./UI/loads.png");
		loads->setSize(1.0f, 1.0f);
		loads->SetPosition(g_pGameInfoManager->getScreenXPosByPer(70), g_pGameInfoManager->getScreenYPosByPer(80));
		loads->SetDelegate(this);
		m_pUILoading->AddChild(loads);

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
	string remain_time = to_string(g_pGameInfoManager->timer);

	RECT rc;
	SetRect(&rc,
		g_pGameInfoManager->getScreenXPosByPer(45),
		g_pGameInfoManager->getScreenYPosByPer(0),
		g_pGameInfoManager->getScreenXPosByPer(49) ,
		g_pGameInfoManager->getScreenYPosByPer(0) + 30);
	if(!m_pUICharacterSelect->m_isHidden)	
		g_pFontManager->GetFont(g_pFontManager->E_TIMER_FONT)->DrawTextA(NULL, remain_time.c_str(), remain_time.length(), &rc, DT_CENTER | DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));
}

void cSCENE_TITLE::Creat_font()
{
	

}



void cSCENE_TITLE::OnClick(cUIButton * pSender)
{
	cUITextView* pTextView =
		(cUITextView*)m_pUIRoot->FindChildByTag(E_TEXT_VIEW);

	if (pSender->GetTag() == E_BUTTON_PLAY)
	{
		g_pGameInfoManager->Play(19, 1);
		m_pUIShadowRoot->m_isHidden = !m_pUIShadowRoot->m_isHidden;
		m_pUIRoot->m_isHidden = 0;
		m_pUIShadowRoot->setable();
	}

	else if (pSender->GetTag() == E_BUTTON_EXIT)
	{
		g_pGameInfoManager->Play(19, 1);
		//네트워크 수정 필요
		//g_pNetworkManager->SendData(, g_pGameInfoManager->GetMyInfo());
		exit(0);
	}
	else if (pSender->GetTag() == E_BUTTON_OK)
	{
		g_pGameInfoManager->Play(19, 1);
		g_pGameInfoManager->SetMyName();
		//g_pGameInfoManager->sceneChangeTriger = TRUE;
		//g_pGameInfoManager->nextScene = 1;
		g_pGameInfoManager->SetMyCharacter(PC_Soldier);
		//네트워크 수정 필요
		g_pNetworkManager->SendData(NH_MY_NAME_IS, g_pGameInfoManager->GetMyInfo());
		g_pGameInfoManager->timer = 30;
		m_pUIRoot->m_isHidden = TRUE;
		m_pUIShadowRoot->m_isHidden = TRUE;
		m_pUICharacterSelect->m_isHidden = FALSE;
	}
	else if (pSender->GetTag() == E_BUTTON_CANCLE)
	{
		g_pGameInfoManager->Play(19, 1);
		m_pUIShadowRoot->setUnable();
		m_pUIShadowRoot->m_isHidden = TRUE;
		m_pUIRoot->setable();
	}
	else if (pSender->GetTag() == SELECT_DIVA)
	{
		g_pGameInfoManager->Play(11, 1);
		g_pGameInfoManager->m_strMyCharacter.Character_No = PC_DIVA;
		player1_character->SetPosition(g_pGameInfoManager->getScreenXPosByPer(30), g_pGameInfoManager->getScreenYPosByPer(15));
		player1_character->SetTexture("./UI/diva.png", "./UI/diva.png", "./UI/diva.png");
		g_pNetworkManager->SendData(NH_SELECT, g_pGameInfoManager->GetMyInfo());
	}
	else if (pSender->GetTag() == SELECT_REAPER)
	{
		g_pGameInfoManager->Play(13, 1);
		g_pGameInfoManager->m_strMyCharacter.Character_No = PC_Reaper;
		player1_character->SetPosition(g_pGameInfoManager->getScreenXPosByPer(30), g_pGameInfoManager->getScreenYPosByPer(15));
		player1_character->SetTexture("./UI/reaper.png", "./UI/reaper.png", "./UI/reaper.png");
		g_pNetworkManager->SendData(NH_SELECT, g_pGameInfoManager->GetMyInfo());
	}
	else if (pSender->GetTag() == SELECT_HANZO)
	{
		g_pGameInfoManager->m_strMyCharacter.Character_No = PC_Hanzo;
		player1_character->SetPosition(g_pGameInfoManager->getScreenXPosByPer(37), g_pGameInfoManager->getScreenYPosByPer(15));
		player1_character->SetTexture("./UI/hanzo.png", "./UI/hanzo.png", "./UI/hanzo.png");
		g_pNetworkManager->SendData(NH_SELECT, g_pGameInfoManager->GetMyInfo());
	}
	else if (pSender->GetTag() == SELECT_REIN)
	{
		g_pGameInfoManager->Play(12, 1);
		g_pGameInfoManager->m_strMyCharacter.Character_No = PC_Reinhardt;
		player1_character->SetPosition(g_pGameInfoManager->getScreenXPosByPer(20), g_pGameInfoManager->getScreenYPosByPer(15));
		player1_character->SetTexture("./UI/rein.png", "./UI/rein.png", "./UI/rein.png");
		g_pNetworkManager->SendData(NH_SELECT, g_pGameInfoManager->GetMyInfo());
	}
	else if (pSender->GetTag() == SELECT_TRACER)
	{
		g_pGameInfoManager->m_strMyCharacter.Character_No = PC_Tracer;
		player1_character->SetPosition(g_pGameInfoManager->getScreenXPosByPer(40), g_pGameInfoManager->getScreenYPosByPer(15));
		player1_character->SetTexture("./UI/tracer.png", "./UI/tracer.png", "./UI/tracer.png");
		g_pNetworkManager->SendData(NH_SELECT, g_pGameInfoManager->GetMyInfo());
	}
	else if (pSender->GetTag() == SELECT_SOLDIER)
	{
		g_pGameInfoManager->Stop(1);
		g_pGameInfoManager->Play(10, 1);
		g_pGameInfoManager->m_strMyCharacter.Character_No = PC_Soldier;
		player1_character->SetPosition(g_pGameInfoManager->getScreenXPosByPer(40), g_pGameInfoManager->getScreenYPosByPer(15));
		player1_character->SetTexture("./UI/soldier.png", "./UI/soldier.png", "./UI/soldier.png");
		g_pNetworkManager->SendData(NH_SELECT, g_pGameInfoManager->GetMyInfo());
	}
	else if (pSender->GetTag() == E_BUTTON_READY)
	{
		static bool ready = false;
		if (g_pGameInfoManager->m_strMyCharacter.Character_No != 0)
		{
			ready_button->SetTexture("./UI/ready_set.png",
				"./UI/ready_set.png",
				"./UI/ready_set.png");
			ready = true;

			g_pNetworkManager->SendData(NH_READY_BUTTON, g_pGameInfoManager->GetMyInfo());

			m_pUICharacterSelect->m_isHidden = 1;
			m_pUILoading->m_isHidden = 0;
		}
		t1 = thread([]() { g_pGameInfoManager->setup_XMap("map/Rialto_map.X"); });
		t2 = thread([]() { g_pGameInfoManager->setup_SXMap("map/Rialto_floor.X"); });
		if (m_pUILoading->m_isHidden == 0)
		{

			g_pGameInfoManager->Stop(1);
			g_pGameInfoManager->Stop(0);
			//// load sound start
			g_pGameInfoManager->Play(0, 0);
			g_pGameInfoManager->channel_volum_set(0, 0.07f);
			////
		}
		g_pGameInfoManager->Play(19, 1);
	}
	g_pGameInfoManager->channel_volum_set(1, 0.175f);
}


void cSCENE_TITLE::buttonUpdate(cUIButton * pSender)
{
	static int otherselect1 = PC_DEFAULT;
	static int otherselect2 = PC_DEFAULT;
	static int otherselect3 = PC_DEFAULT;
	
	if (pSender->GetTag() == OTHER_PORTRAIT1 && g_pOtherPlayerManager->otherPlayerInfo.size() == 0)
	{
		if (g_pOtherPlayerManager->otherPlayerInfo.size() == 0 && otherselect1 != PC_DEFAULT)
		{
			pSender->SetTexture("./UI/default_portrait.png", "./UI/default_portrait.png", "./UI/default_portrait.png");
			otherselect1 = PC_DEFAULT;
		}
	}
	else if(pSender->GetTag() == OTHER_PORTRAIT2 && g_pOtherPlayerManager->otherPlayerInfo.size() < 2)
	{
		if (g_pOtherPlayerManager->otherPlayerInfo.size() == 0 && otherselect2 != PC_DEFAULT)
		{
			pSender->SetTexture("./UI/default_portrait.png", "./UI/default_portrait.png", "./UI/default_portrait.png");
			otherselect2 = PC_DEFAULT;
		}
	}
	else if (pSender->GetTag() == OTHER_PORTRAIT3 && g_pOtherPlayerManager->otherPlayerInfo.size() < 3)
	{
		if (g_pOtherPlayerManager->otherPlayerInfo.size() == 0 && otherselect3 != PC_DEFAULT)
		{
			pSender->SetTexture("./UI/default_portrait.png", "./UI/default_portrait.png", "./UI/default_portrait.png");
			otherselect3 = PC_DEFAULT;
		}
	}


	if (pSender->GetTag() == OTHER_PORTRAIT1 && g_pOtherPlayerManager->otherPlayerInfo.size()>0)
	{
		if (otherselect1 != g_pOtherPlayerManager->otherPlayerInfo.at(0)->info.Character_No)
		{
			pSender->SetTexture("./UI/soldier_portrait.png", "./UI/soldier_portrait.png", "./UI/soldier_portrait.png");
			otherselect1 = PC_Soldier;
		}
	}
	else if (pSender->GetTag() == OTHER_PORTRAIT2&& g_pOtherPlayerManager->otherPlayerInfo.size()>1)
	{
		if (otherselect2 != g_pOtherPlayerManager->otherPlayerInfo.at(1)->info.Character_No)
		{
			pSender->SetTexture("./UI/soldier_portrait.png", "./UI/soldier_portrait.png", "./UI/soldier_portrait.png");
			otherselect2 = PC_Soldier;
		}
	}
	else if (pSender->GetTag() == OTHER_PORTRAIT3&& g_pOtherPlayerManager->otherPlayerInfo.size()>2)
	{
		if (otherselect3 != g_pOtherPlayerManager->otherPlayerInfo.at(2)->info.Character_No)
		{
			pSender->SetTexture("./UI/soldier_portrait.png", "./UI/soldier_portrait.png", "./UI/soldier_portrait.png");
			otherselect3 = PC_Soldier;
		}
	}
	
	if (pSender->GetTag() == OTHER_READY_BUTTON1)
	{
		if (g_pOtherPlayerManager->otherPlayerInfo.size() > 0)
		{
			if (g_pOtherPlayerManager->otherPlayerInfo.at(0)->info.readyButton == true)
				pSender->m_isHidden = false;
			else
				pSender->m_isHidden = true;
		}
		else
			pSender->m_isHidden = true;
	}
	else if (pSender->GetTag() == OTHER_READY_BUTTON2)
	{
		if (g_pOtherPlayerManager->otherPlayerInfo.size() > 1)
		{
			if (g_pOtherPlayerManager->otherPlayerInfo.at(1)->info.readyButton == true)
				pSender->m_isHidden = false;
			else
				pSender->m_isHidden = true;
		}
		else
			pSender->m_isHidden = true;
	}
	else if (pSender->GetTag() == OTHER_READY_BUTTON3)
	{
		if (g_pOtherPlayerManager->otherPlayerInfo.size() > 2)
		{
			if (g_pOtherPlayerManager->otherPlayerInfo.at(2)->info.readyButton == true)
				pSender->m_isHidden = false;
			else
				pSender->m_isHidden = true;
		}
		else
			pSender->m_isHidden = true;
	}


	if (pSender->GetTag() == LOADING_BAR)
	{
		//t1 = thread([]() {g_pGameInfoManager->setup_XMap("map/rialto_map_new3_text.X");});

		static float load = 0.0f;
		/*if(g_pGameInfoManager->loading == 0 && load>0.5)
		load = 0.5;*/

		load += 0.33f;
		/*if (g_pGameInfoManager->loading == 0 && load > 1)
		{
			load = 0.0;
		}*/
		pSender->cutSize(load, 1.0f);
		if (load > 1.0f)
		{
			g_pGameInfoManager->sceneChangeTriger = TRUE;
			g_pGameInfoManager->nextScene = 1;
		}
		else
		{
			Sleep(15000);
		}
	}

}
