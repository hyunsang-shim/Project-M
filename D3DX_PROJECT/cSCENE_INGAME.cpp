#include "stdafx.h"
#include "cSCENE_INGAME.h"
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
#include "cFrame.h"
#include "cUIObject.h"
#include "cUIImageView.h"
#include "cCrossHairPicking.h"
#include "cAI.h"
#include "cWaveTriggerBox.h"
#include "cUITextView.h"
#include "cXModelSurface.h"


enum
{
	HP_BAR = 11,
	CROSS_UP,
	CROSS_RIGHT,
	CROSS_DOWN,
	CROSS_LEFT,
	EXIT_OK,
	EXIT_CANCLE
};


cSCENE_INGAME::cSCENE_INGAME()
	: m_pCamera(NULL),
	m_pGrid(NULL),
	m_pMap(NULL),
	m_pObject(NULL),
	m_pXmodel(NULL),
	m_pSKY(NULL),
	m_pMyCharacter(NULL),
	BulletCreateCount(0),
	BulletCreateTime(MAXBulletCreateCount),
	load(FALSE),
	m_pTriggerBox(NULL),
	m_pUIShadowRoot(NULL),
	m_pUIBase(NULL),
	WaveStartOrEnd(false),
	SpawnMoster(200),
	SpawnCount(0),
	m_pRootFrame(NULL)
{
	GetClientRect(g_hWnd, &m_Worldrc);
	m_Bullet.resize(30);
}


cSCENE_INGAME::~cSCENE_INGAME()
{
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pGrid);
	SAFE_DELETE(m_pMap);
	SAFE_DELETE(m_pObject);
	SAFE_DELETE(m_pXmodel);
	//SAFE_DELETE(m_pSKY);
	SAFE_DELETE(m_pMyCharacter);
	SAFE_DELETE(m_pTriggerBox);
	SAFE_DELETE(m_pUIBase);
	SAFE_DELETE(m_pUIShadowRoot);
}

void cSCENE_INGAME::Setup()
{
	//g_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pCrossHairPicking = new cCrossHairPicking;

	m_pTriggerBox = new cWaveTriggerBox;
	m_pTriggerBox->Setup();

	//ī�޶� ����
	m_pCamera = new cCamera();
	m_pCamera->Setup();

	// �׸��� ����
	m_pGrid = new cGrid();
	m_pGrid->Setup();

	// ���� ���� ����
	D3DXVECTOR3 dir(0, -1, 0);
	D3DXVECTOR3 Pos(100, 100, 100);
	D3DXCOLOR c(1.0f, 1.0f, 1.0f, 1.0f);
	ZeroMemory(&DirectLight, sizeof(D3DLIGHT9));
	ZeroMemory(&subLight, sizeof(D3DLIGHT9));

	DirectLight = InitDirectionalLight(&dir, &c);
	subLight = InitPointLight(&Pos, &c);
	g_pDevice->SetLight(0, &DirectLight);
	//g_pDevice->SetLight(1, &DirectLight);
	//g_pDevice->SetLight(2, &DirectLight);
	//g_pDevice->SetLight(3, &DirectLight);

	g_pDevice->LightEnable(0, TRUE);
	//g_pDevice->LightEnable(1, TRUE);
	//g_pDevice->LightEnable(2, TRUE);
	//g_pDevice->LightEnable(3, TRUE);
	g_pDevice->SetRenderState(D3DRS_LIGHTING, true);

	// ����Ʈ�� ����
	//m_pMap = new cHeightMap();
	//m_pMap->Setup("map/", "HeightMap.raw", "terrain.jpg", 1);


	//�׽�Ʈ ������Ʈ ����
	/*m_pObject = new cNewObject;
	m_pObject->Setup("map", "box.obj");*/
	//m_pObject->SetSRT(D3DXVECTOR3(5.0f, 5.0f, 5.0f), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(-10, 0, 10));
	//

	//OBJŸ���� �� �ε�
	//g_pGameInfoManager->setup_Map("map/rialto_obj_2", "Rialto_8B4.obj");
	//g_pGameInfoManager->setup_Map("map", "box.obj");

	/*loader = new cAseLoader();
	m_pRootFrame = loader->Load("woman/woman_01_all.ASE");
	m_pRootFrame->SetSRT(D3DXVECTOR3(5.0f, 5.0f, 5.0f), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(10, 0, 10));*/

	//xfile Map load
	g_pGameInfoManager->setup_XMap("map/rialto_map_new3_text.X"); 

	g_pGameInfoManager->setup_SXMap("map/floorBox.X");

	//�ϴ� ����
	m_pSKY = new cSKY();
	m_pSKY->Setup();

	//���� ����
	m_pMyCharacter = new cMyCharacter;
	m_pMyCharacter->Setup("Xfile", "Soldier76_with_gun.x");
	cCharacter* pCharacter = new cCharacter;
	m_pMyCharacter->SetCharacterController(pCharacter);

	//���콺 ��ǥ ����
	beforeMousePos.x = 1920 / 2;
	beforeMousePos.y = 1080 / 2;
	nowMousePos.x = 1920 / 2;
	nowMousePos.y = 1080 / 2;
	//SetCursorPos(1920 / 2, 1080 / 2);

	//cAI_Controller* m_pVecAI_Controller;

	m_pVecAI.resize(m_pTriggerBox->tb.MakeMonster);

	for (int i = 0; i < m_pVecAI.size(); i++)
	{
		m_pVecAI[i] = new cAI;
		m_pVecAI[i]->Setup("NPCS", "slicer.X");
		cAI_Controller* m_pVecAI_Controller = new cAI_Controller;
		m_pVecAI[i]->SetAIController(m_pVecAI_Controller);
		m_pVecAI[i]->SetPosition(D3DXVECTOR3(m_pTriggerBox->GetSpawnXPos() , m_pTriggerBox->GetSpawnYPos(), m_pTriggerBox->GetSpawnZPos() + ((i - (5 * (i/5)))  *5)));
		SAFE_DELETE(m_pVecAI_Controller);
	}

	WaveStartOrEnd = false;

	setupUI();

	load = TRUE;

	//��Ʈ ����
	Creat_Font();
	g_pGameInfoManager->loading = 1;
}

void cSCENE_INGAME::Update()
{
	if (SpawnMoster == 0 && m_pVecAI.size() != SpawnCount * 5)
	{
		SpawnMoster = 200;
		SpawnCount++;
	}

	if (m_pTriggerBox->GetNextWave())
	{
		m_pTriggerBox->Setup();
		WaveStartOrEnd = false;
		SpawnMoster = 200;
		SpawnCount = 0;
		cAI_Controller* m_pVecAI_Controller;

		m_pVecAI.resize(m_pTriggerBox->tb.MakeMonster);

		for (int i = 0; i < m_pVecAI.size(); i++)
		{
			m_pVecAI[i] = new cAI;
			m_pVecAI[i]->Setup("NPCS", "slicer.X");
			m_pVecAI_Controller = new cAI_Controller;
			m_pVecAI[i]->SetAIController(m_pVecAI_Controller);
			m_pVecAI[i]->SetPosition(D3DXVECTOR3(m_pTriggerBox->GetSpawnXPos(), m_pTriggerBox->GetSpawnYPos(), m_pTriggerBox->GetSpawnZPos() + ((i - (5 * (i / 5))) * 10)));
			SAFE_RELEASE(m_pVecAI_Controller);
		}
	}

	BOOL static mouseMove = 0;
	g_pTimeManager->Update();

	if (mouseMove == 0)
	{
		GetCursorPos(&nowMousePos);

		g_pGameInfoManager->mouseMoveX = nowMousePos.x - beforeMousePos.x;
		g_pGameInfoManager->mouseMoveY = nowMousePos.y - beforeMousePos.y;

		beforeMousePos = nowMousePos;
		if (nowMousePos.x == 0 || nowMousePos.y == 0 || nowMousePos.x >= 1920 || nowMousePos.y >= 1080)
		{
			SetCursorPos(1920 / 2, 1080 / 2);
			mouseMove = 1;
		}

	}
	else
	{
		g_pGameInfoManager->mouseMoveX = 0;
		g_pGameInfoManager->mouseMoveY = 0;
		GetCursorPos(&nowMousePos);
		GetCursorPos(&beforeMousePos);
		mouseMove = 0;
	}

	m_pCamera->Update(m_pMyCharacter->GetPosition(), m_pMyCharacter->GetMyHeadPos());
	if (m_pRootFrame)
		m_pRootFrame->Update(m_pRootFrame->GetKeyFrame(), NULL);

	if (m_pObject)
		m_pObject->Updata();



	float y = m_pMyCharacter->GetPosition().y;
	/*if (m_pMap && m_pMap->GetY(m_pMyCharacter->GetPosition().x, y, m_pMyCharacter->GetPosition().z, m_pMyCharacter->GetMyHeadPos()))
	{
		g_pGameInfoManager->canGo = 1;
		m_pMyCharacter->GetCharacterController()->SetPositionY(y);
	}*/

	if (m_pMyCharacter)
		m_pMyCharacter->Update(m_pCamera->getDirection());
	
	if (cOBB::isCollision(m_pMyCharacter->GetOBB(), m_pTriggerBox->m_pOBB))
	{
		WaveStartOrEnd = true;
	}

	if (WaveStartOrEnd)
	{
		SpawnMoster--;
		for (int i = 0; i < SpawnCount * 5; i++)
		{
			if (m_pVecAI[i])
				m_pVecAI[i]->Update(true, m_pMyCharacter->GetPosition() - m_pVecAI[i]->GetPosition());
		}
	}
	/*else
	{
		for (int i = 0; i < m_pVecAI.size(); i++)
		{
			if (m_pVecAI[i])
				m_pVecAI[i]->Update(false, D3DXVECTOR3(0, 0, 0));
		}
	}*/

	
	if (GetKeyState(VK_LBUTTON) & 0x8000)
	{ 
		if (g_pGameInfoManager->MaxBulletCount != 0)
		{
			m_pCrossHairPicking->CalcPosition();

			float dist;
			D3DXIntersect(g_pGameInfoManager->m_pXMap->GetXMESH(), &m_pCrossHairPicking->GetOrigin(), &m_pCrossHairPicking->GetDirection(), &pHit, NULL, NULL, NULL, &dist, NULL, NULL);
			
			if (pHit && BulletCreateTime == MAXBulletCreateCount)
			{
				m_vCrossHairHitPos = m_pCrossHairPicking->GetOrigin() + dist * m_pCrossHairPicking->GetDirection();

				m_Bullet[BulletCreateCount].BulletDirection = m_vCrossHairHitPos - m_pMyCharacter->GetBulletPos();

				D3DXCreateSphere(g_pDevice, m_Bullet[BulletCreateCount].Radius, 20, 10, &m_Bullet[BulletCreateCount].m_pBulletMesh, NULL);
				m_Bullet[BulletCreateCount].m_vBulletCreatePos = m_pMyCharacter->GetBulletPos();
				m_Bullet[BulletCreateCount].m_stMtlCircle.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				m_Bullet[BulletCreateCount].m_stMtlCircle.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				m_Bullet[BulletCreateCount].m_stMtlCircle.Specular = D3DXCOLOR(0.0f, 0.7f, 0.7f, 1.0f);
				m_Bullet[BulletCreateCount].BulletLiveTime = 10;

				BulletCreateCount++;

				BulletCreateTime = 0;

				g_pGameInfoManager->MaxBulletCount--;
			}
			else
			{
				if (BulletCreateTime < MAXBulletCreateCount)
				{
					BulletCreateTime++;
				}
			}
		}
		else
		{
			BulletCreateCount = 0;
			m_Bullet.clear();
			m_Bullet.resize(30);
		}
	}

	if (g_pNetworkManager->GetNetStatus())
	{
		for (int i = 0; i < g_pOtherPlayerManager->otherPlayerInfo.size(); i++)
		{
			g_pOtherPlayerManager->otherPlayerInfo.at(i)->Update();
		}
	}

	if(m_pTriggerBox)
		m_pTriggerBox->Update();
	updateUI();
}

void cSCENE_INGAME::Render()
{
	SetCursor(NULL);

	if (g_pGameInfoManager->isESCPushed)
	{
		SetCursor(LoadCursor(NULL, IDC_ARROW));
	}

	m_pSKY->Render();
	//m_pGrid->Render();
	if(g_pGameInfoManager->m_pMap)
		g_pGameInfoManager->m_pMap->Render();
	//m_pObject->Render();
	if(g_pGameInfoManager->m_pXMap)
		g_pGameInfoManager->m_pXMap->Render();

	//if(g_pGameInfoManager->m_pSXMap)
	//	g_pGameInfoManager->m_pSXMap->Render();
	if(m_pTriggerBox)
		m_pTriggerBox->Render();

	// �� ���¿� ���� ��ŵ �����ϵ��� ����
	if (g_pNetworkManager->GetNetStatus())
	{
		g_pOtherPlayerManager->render();
	}

	/*if (m_pRootFrame)
		m_pRootFrame->Render();*/
	{
		if (m_pMyCharacter)
			m_pMyCharacter->Render(NULL);
	}

	renderUI();

	if (pHit)
	{
		Mesh_Render();
	}

	if (WaveStartOrEnd)
	{
		for (int i = 0; i < SpawnCount * 5; i++)
		{
			if (m_pVecAI[i])
				m_pVecAI[i]->Render(NULL);
		}
	}
	//?????????????????????????
	for (int i = 0; i < g_pGameInfoManager->GetNumTotalUser(); i++)
	{
		g_pGameInfoManager->GetOthersInfo();
	}

	Render_Text();
}

void cSCENE_INGAME::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_LBUTTONDOWN:
	{
		static int n = 0;
		//m_pSkinnedMesh->SetAnimationIndex(n++);
		//m_pMyCharacter->SetAnimationIndexBlend(n++);
		//m_pAI->SetAnimationIndexBlend(n++);
		/*for (int i = 0; i < m_pVecAI.size(); i++)
		{
			m_pVecAI[i]->SetAnimationIndexBlend(n);
		}
		n++;*/
		break;
	}

	case WM_KEYUP:
		switch (wParam)
		{
		case VK_ESCAPE:
		{
			g_pGameInfoManager->isESCPushed = ~g_pGameInfoManager->isESCPushed;
			m_pUIShadowRoot->m_isHidden = !m_pUIShadowRoot->m_isHidden;
			break;
		}
		break;
		}
	case WM_RBUTTONUP:
		//g_pNetworkManager->SendData();
		break;
	}
	if (m_pCamera)
	{
		m_pCamera->WndProc(hWnd, message, wParam, lParam);
	}
	if (m_pMyCharacter)
	{
		m_pMyCharacter->WndProc(hWnd, message, wParam, lParam);
	}

	if (GetKeyState('N') & 0x8000)
	{
		m_pVecAI.clear();
		WaveStartOrEnd = false;
	}
}
void cSCENE_INGAME::Setup_HeightMap()
{
	cNewObject* pMap = new cNewObject;
	pMap->Setup("test_map_obj.obj");
	m_pMap = pMap;
}

D3DLIGHT9 cSCENE_INGAME::InitDirectionalLight(D3DXVECTOR3 * direction, D3DXCOLOR * color)
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

D3DLIGHT9 cSCENE_INGAME::InitPointLight(D3DXVECTOR3 * position, D3DXCOLOR * color)
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

D3DLIGHT9 cSCENE_INGAME::InitSpotLight(D3DXVECTOR3 * position, D3DXVECTOR3 * direction, D3DXCOLOR * color)
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

void cSCENE_INGAME::setupUI()
{
	{
		D3DXCreateSprite(g_pDevice, &m_pSprite);
		cUIImageView* pImageView = new cUIImageView;
		pImageView->SetPosition(0, 0, 0);
		m_pUIBase = pImageView;


		cUIButton* pCrossUp = new cUIButton;
		pCrossUp->SetTexture("./UI/crosshair_p.png",
			"./UI/crosshair_p.png",
			"./UI/crosshair_p.png");
		pCrossUp->setSize(1.0f, 1.0f);
		pCrossUp->SetPosition(g_pGameInfoManager->getScreenXPosByPer(50), g_pGameInfoManager->getScreenYPosByPer(50) - 10);
		pCrossUp->SetDelegate(this);
		pCrossUp->SetTag(CROSS_UP);
		m_pUIBase->AddChild(pCrossUp);

		cUIButton* pCrossRight = new cUIButton;
		pCrossRight->SetTexture("./UI/crosshair_p_right.png",
			"./UI/crosshair_p_right.png",
			"./UI/crosshair_p_right.png");
		pCrossRight->setSize(1.0f, 1.0f);
		pCrossRight->SetPosition(g_pGameInfoManager->getScreenXPosByPer(50) + 5, g_pGameInfoManager->getScreenYPosByPer(50));
		pCrossRight->SetDelegate(this);
		pCrossRight->SetTag(CROSS_RIGHT);
		m_pUIBase->AddChild(pCrossRight);

		cUIButton* pCrossDown = new cUIButton;
		pCrossDown->SetTexture("./UI/crosshair_p.png",
			"./UI/crosshair_p.png",
			"./UI/crosshair_p.png");
		pCrossDown->setSize(1.0f, 1.0f);
		pCrossDown->SetPosition(g_pGameInfoManager->getScreenXPosByPer(50), g_pGameInfoManager->getScreenYPosByPer(50) + 5);
		pCrossDown->SetDelegate(this);
		pCrossDown->SetTag(CROSS_DOWN);
		m_pUIBase->AddChild(pCrossDown);

		cUIButton* pCrossLeft = new cUIButton;
		pCrossLeft->SetTexture("./UI/crosshair_p_right.png",
			"./UI/crosshair_p_right.png",
			"./UI/crosshair_p_right.png");
		pCrossLeft->setSize(1.0f, 1.0f);
		pCrossLeft->SetPosition(g_pGameInfoManager->getScreenXPosByPer(50) - 11, g_pGameInfoManager->getScreenYPosByPer(50));
		pCrossLeft->SetDelegate(this);
		pCrossLeft->SetTag(CROSS_LEFT);
		m_pUIBase->AddChild(pCrossLeft);





		cUIButton* pHPBar = new cUIButton;
		pHPBar->SetTexture("./UI/PlayerHealthBar.png",
			"./UI/PlayerHealthBar.png",
			"./UI/PlayerHealthBar.png");
		pHPBar->setSize(1.0f, 2.0f);
		pHPBar->SetPosition(g_pGameInfoManager->getScreenXPosByPer(12), g_pGameInfoManager->getScreenYPosByPer(80));
		pHPBar->SetDelegate(this);
		pHPBar->SetTag(HP_BAR);
		m_pUIBase->AddChild(pHPBar);

		cUIButton* pHPBarLoss = new cUIButton;
		pHPBarLoss->SetTexture("./UI/PlayerHealthBarLoss.png",
			"./UI/PlayerHealthBarLoss.png",
			"./UI/PlayerHealthBarLoss.png");
		pHPBarLoss->setSize(1.0f, 2.0f);
		pHPBarLoss->SetPosition(g_pGameInfoManager->getScreenXPosByPer(12), g_pGameInfoManager->getScreenYPosByPer(80));
		pHPBarLoss->SetDelegate(this);
		m_pUIBase->AddChild(pHPBarLoss);

		cUIButton* pFaceLine = new cUIButton;
		pFaceLine->SetTexture("./UI/chaback.png",
			"./UI/chaback.png",
			"./UI/chaback.png");
		pFaceLine->setSize(1.0f, 1.0f);
		pFaceLine->SetPosition(g_pGameInfoManager->getScreenXPosByPer(7), g_pGameInfoManager->getScreenYPosByPer(77));
		pFaceLine->SetDelegate(this);
		m_pUIBase->AddChild(pFaceLine);

		cUIButton* pFACE = new cUIButton;
		pFACE->SetTexture("./UI/0000000040C8.01C.dds",
			"./UI/0000000040C8.01C.dds",
			"./UI/0000000040C8.01C.dds");
		pFACE->setSize(1.0f, 2.0f);
		pFACE->SetPosition(g_pGameInfoManager->getScreenXPosByPer(7), g_pGameInfoManager->getScreenYPosByPer(77));
		pFACE->SetDelegate(this);
		m_pUIBase->AddChild(pFACE);

		cUIButton* pWeapon = new cUIButton;
		pWeapon->SetTexture("./UI/Weapon.png",
			"./UI/Weapon.png",
			"./UI/Weapon.png");
		pWeapon->setSize(1.0f, 1.0f);
		pWeapon->SetPosition(g_pGameInfoManager->getScreenXPosByPer(85), g_pGameInfoManager->getScreenYPosByPer(81));
		pWeapon->SetDelegate(this);
		m_pUIBase->AddChild(pWeapon);

		cUIButton* pSkill1 = new cUIButton;
		pSkill1->SetTexture("./UI/E_skill1.png",
			"./UI/E_skill1.png",
			"./UI/E_skill1.png");
		pSkill1->setSize(1.0f, 1.0f);
		pSkill1->SetPosition(g_pGameInfoManager->getScreenXPosByPer(78), g_pGameInfoManager->getScreenYPosByPer(81));
		pSkill1->SetDelegate(this);
		m_pUIBase->AddChild(pSkill1);
	}
	{
		D3DXCreateSprite(g_pDevice, &m_pSprite);
		cUIImageView* pImageView = new cUIImageView;
		pImageView->SetPosition(0, 0, 0);
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
		pTextView->Settext("������ �����Ͻðڽ��ϱ�?");
		pTextView->SetSize(ST_SIZEN(700, 200));
		pTextView->SetPosition(g_pGameInfoManager->getScreenXPosByPer(30), g_pGameInfoManager->getScreenYPosByPer(35));
		pTextView->SetDrawTextFormat(DT_CENTER | DT_VCENTER);
		pTextView->SetTextColor(D3DCOLOR_XRGB(255, 255, 0));

		m_pUIShadowRoot->AddChild(pTextView);



		cUIButton* OK_button = new cUIButton;
		OK_button->SetTexture("./UI/OK.png",
			"./UI/OK.png",
			"./UI/OK.png");
		OK_button->setSize(1.0f, 1.0f);
		OK_button->SetPosition(g_pGameInfoManager->getScreenXPosByPer(35), g_pGameInfoManager->getScreenYPosByPer(50));
		OK_button->SetDelegate(this);
		OK_button->SetTag(EXIT_OK);
		m_pUIShadowRoot->AddChild(OK_button);

		cUIButton* cancle_button = new cUIButton;
		cancle_button->SetTexture("./UI/cancle.png",
			"./UI/cancle.png",
			"./UI/cancle.png");
		cancle_button->setSize(0.5f, 1.0f);
		cancle_button->SetPosition(g_pGameInfoManager->getScreenXPosByPer(48), g_pGameInfoManager->getScreenYPosByPer(50));
		cancle_button->SetDelegate(this);
		cancle_button->SetTag(EXIT_CANCLE);
		m_pUIShadowRoot->AddChild(cancle_button);


		m_pUIShadowRoot->m_isHidden = 1;
	}
}

void cSCENE_INGAME::renderUI()
{
	if (m_pUIBase)
		m_pUIBase->Render(m_pSprite);

	if (m_pUIShadowRoot)
		m_pUIShadowRoot->Render(m_pSprite);
}

void cSCENE_INGAME::updateUI()
{
	if (m_pUIBase)
		m_pUIBase->Update();


	if (m_pUIShadowRoot)
		m_pUIShadowRoot->Update();
}


void cSCENE_INGAME::OnClick(cUIButton * pSender)
{
	if (pSender->GetTag() == EXIT_OK)
	{
		exit(0);
	}
	else if (pSender->GetTag() == EXIT_CANCLE)
	{
		m_pUIShadowRoot->m_isHidden = 1;
		g_pGameInfoManager->isESCPushed = ~g_pGameInfoManager->isESCPushed;
	}
}

void cSCENE_INGAME::buttonUpdate(cUIButton * pSender)
{
	float aimSize = 0.0f;
	//aimSize = g_pGameInfoManager->aimSize;

	if (pSender->GetTag() == HP_BAR)
	{
		pSender->cutSize(0.65f, 1.0f);
	}
	else if (pSender->GetTag() == CROSS_UP)
	{
		pSender->SetPosition(g_pGameInfoManager->getScreenXPosByPer(50), g_pGameInfoManager->getScreenYPosByPer(50) - 10 - aimSize);
	}
	else if (pSender->GetTag() == CROSS_RIGHT)
	{

		pSender->SetPosition(g_pGameInfoManager->getScreenXPosByPer(50) + 5 + aimSize, g_pGameInfoManager->getScreenYPosByPer(50));
	}
	else if (pSender->GetTag() == CROSS_DOWN)
	{

		pSender->SetPosition(g_pGameInfoManager->getScreenXPosByPer(50), g_pGameInfoManager->getScreenYPosByPer(50) + 5 + aimSize);
	}
	else if (pSender->GetTag() == CROSS_LEFT)
	{

		pSender->SetPosition(g_pGameInfoManager->getScreenXPosByPer(50) - 11 - aimSize, g_pGameInfoManager->getScreenYPosByPer(50));
	}
	
}

void cSCENE_INGAME::Creat_Font()
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
		AddFontResource("font/BigNoodleTooOblique.ttf");
		strcpy(fd.FaceName, "BigNoodleTooOblique");
	}
	D3DXCreateFontIndirect(g_pDevice, &fd, &m_pFont);
	fd.Height = 30;
	fd.Width = 15;
	D3DXCreateFontIndirect(g_pDevice, &fd, &m_pFont2);


}

void cSCENE_INGAME::Render_Text()
{
	int maxHP = 200;
	int nowHP = 150;

	int maxBullet = 30;
	int nowBullet = 30;

	string str, str2, str3;
	if (nowHP < 100)
		str += ' ';
	if (nowHP < 10)
		str += ' ';
	str += to_string(nowHP) + '/';
	str2 += to_string(maxHP);
	str3 = to_string(nowBullet) + '/' + to_string(maxBullet);
	RECT rc;
	SetRect(&rc,
		g_pGameInfoManager->getScreenXPosByPer(13),
		g_pGameInfoManager->getScreenYPosByPer(77),
		g_pGameInfoManager->getScreenXPosByPer(13) + 40,
		g_pGameInfoManager->getScreenYPosByPer(77) + 5);
	m_pFont->DrawTextA(NULL, str.c_str(), str.length(), &rc, DT_LEFT | DT_TOP | DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));
	RECT rc2;
	SetRect(&rc2,
		g_pGameInfoManager->getScreenXPosByPer(13) + 110,
		g_pGameInfoManager->getScreenYPosByPer(77) + 5,
		g_pGameInfoManager->getScreenXPosByPer(13) + 150,
		g_pGameInfoManager->getScreenYPosByPer(77) + 20);
	m_pFont2->DrawTextA(NULL, str2.c_str(), str2.length(), &rc2, DT_LEFT | DT_TOP | DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));
	RECT rc3;
	SetRect(&rc3,
		1920 * 0.85f,
		1080 * 0.85f + 10,
		1920 * 0.85f + 50,
		1080 * 0.85f + 15);
	m_pFont->DrawTextA(NULL, str3.c_str(), str3.length(), &rc3, DT_LEFT | DT_TOP | DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));


}
/*
void cSCENE_INGAME::Set_Billboard(D3DXMATRIXA16 * pmatWorld)
{
	D3DXMATRIXA16	matBillBoard;
	D3DXMatrixIdentity(&matBillBoard);
	g_pDevice->GetTransform(D3DTS_VIEW, &matBillBoard);
	D3DXMatrixInverse(&matBillBoard, NULL, &matBillBoard);
	matBillBoard._41 = 0;
	matBillBoard._42 = 0;
	matBillBoard._43 = 0;
	*pmatWorld = matBillBoard;
}

void cSCENE_INGAME::Setup_Particle()
{
	m_vecVertexParticle.resize(1000);
	for (int i = 0; i < m_vecVertexParticle.size(); ++i)
	{
		float fRadius = rand() % 100 / 10.0f;
		m_vecVertexParticle[i].p = D3DXVECTOR3(0, 0, fRadius);

		D3DXVECTOR3 vAngle = D3DXVECTOR3(
			D3DXToRadian(rand() % 3600 / 10.0f),
			D3DXToRadian(rand() % 3600 / 10.0f),
			D3DXToRadian(rand() % 3600 / 10.0f)
		);
		D3DXMATRIX matRX, matRY, matRZ, matWorld;
		D3DXMatrixRotationX(&matRX, vAngle.x);
		D3DXMatrixRotationY(&matRY, vAngle.y);
		D3DXMatrixRotationZ(&matRZ, vAngle.z);
		matWorld = matRX* matRY* matRZ;

		D3DXVec3TransformCoord(
			&m_vecVertexParticle[i].p,
			&m_vecVertexParticle[i].p,
			&matWorld);
		m_vecVertexParticle[i].c = D3DCOLOR_ARGB(255, 180, 70, 20);
	}

	g_pDevice->SetRenderState(D3DRS_POINTSCALEENABLE, true);
	g_pDevice->SetRenderState(D3DRS_POINTSIZE, FtoDW(5.0f));

	g_pDevice->SetRenderState(D3DRS_POINTSCALE_A, FtoDW(0.0f));
	g_pDevice->SetRenderState(D3DRS_POINTSCALE_B, FtoDW(0.0f));
	g_pDevice->SetRenderState(D3DRS_POINTSCALE_C, FtoDW(1.0f));

	g_pDevice->SetRenderState(D3DRS_POINTSCALEENABLE, true);
	g_pDevice->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDW(0.0f));
	g_pDevice->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDW(100.0f));

	g_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	g_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	g_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
}

void cSCENE_INGAME::Update_Particle()
{
	static int nAlpha = 0;
	static int nDelta = 4;
	nAlpha += nDelta;

	if (nAlpha > 255)
	{
		nAlpha = 255;
		nDelta *= -1;
	}
	if (nAlpha < 0)
	{
		nAlpha = 0;
		nDelta *= -1;
	}
	for (int i = 0; i < m_vecVertexParticle.size(); i++)
	{
		if (i % 2) continue;

		m_vecVertexParticle[i].c = D3DCOLOR_ARGB(nAlpha, 180, 70, 20);

	}

}

void cSCENE_INGAME::Render_Particle()
{
	D3DXMATRIXA16	matWorld;
	D3DXMatrixIdentity(&matWorld);
	g_pDevice->SetTransform(D3DTS_WORLD, &matWorld);

	g_pDevice->SetRenderState(D3DRS_LIGHTING, false);
	g_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	g_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);

	g_pDevice->SetFVF(ST_PC_VERTEX::FVF);
	g_pDevice->SetTexture(0, g_pTextureManager->GetTexture("./images/alpha_tex.tga"));
	g_pDevice->DrawPrimitiveUP(D3DPT_POINTLIST,
		m_vecVertexParticle.size(),
		&m_vecVertexParticle[0],
		sizeof(ST_PC_VERTEX));


	g_pDevice->SetRenderState(D3DRS_LIGHTING, true);
	g_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	g_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

*/


void cSCENE_INGAME::Mesh_Render()
{
	for (int i = 0; i < BulletCreateCount; i++)
	{
		D3DXMatrixTranslation(&m_Bullet[i].matT, m_Bullet[i].m_vBulletCreatePos.x, m_Bullet[i].m_vBulletCreatePos.y, m_Bullet[i].m_vBulletCreatePos.z);
		g_pDevice->SetTransform(D3DTS_WORLD, &m_Bullet[i].matT);
		g_pDevice->SetMaterial(&m_Bullet[i].m_stMtlCircle);
		m_Bullet[i].m_pBulletMesh->DrawSubset(0);

		m_Bullet[i].m_vBulletCreatePos += m_Bullet[i].BulletDirection/2;

		m_Bullet[i].BulletLiveTime--;

		if (m_Bullet[i].BulletLiveTime == 0)
		{
			m_Bullet.erase(m_Bullet.begin() + i);
			BulletCreateCount--;
		}
	}
}
