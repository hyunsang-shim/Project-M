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

enum
{
	HP_BAR = 11,
	CROSS_UP,
	CROSS_RIGHT,
	CROSS_DOWN,
	CROSS_LEFT,

};

cSCENE_INGAME::cSCENE_INGAME()
	: m_pCamera(NULL),
	m_pGrid(NULL),
	m_pMap(NULL),
	m_pObject(NULL),
	m_pXmodel(NULL),
	m_pSKY(NULL),
	m_pMyCharacter(NULL),
	m_pAI(NULL),
	BulletCreateCount(0),
	BulletCreateTime(MAXBulletCreateCount),
	load(FALSE)
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
	SAFE_DELETE(m_pAI);
}

void cSCENE_INGAME::Setup()
{
	m_pCrossHairPicking = new cCrossHairPicking;

	

	//카메라 셋팅
	m_pCamera = new cCamera();
	m_pCamera->Setup();

	// 그리드 세팅
	m_pGrid = new cGrid();
	m_pGrid->Setup();

	// 메인 광원 셋팅
	D3DXVECTOR3 dir(0, -1, 0);
	D3DXCOLOR c(1.0f, 1.0f, 1.0f, 1.0f);
	ZeroMemory(&DirectLight, sizeof(D3DLIGHT9));
	DirectLight = InitDirectionalLight(&dir, &c);
	g_pDevice->SetLight(0, &DirectLight);
	//g_pDevice->SetLight(1, &DirectLight);
	//g_pDevice->SetLight(2, &DirectLight);
	//g_pDevice->SetLight(3, &DirectLight);
	g_pDevice->LightEnable(0, TRUE);
	//g_pDevice->LightEnable(1, TRUE);
	//g_pDevice->LightEnable(2, TRUE);
	//g_pDevice->LightEnable(3, TRUE);
	g_pDevice->SetRenderState(D3DRS_LIGHTING, true);

	// 하이트맵 셋팅
	//m_pMap = new cHeightMap();
	//m_pMap->Setup("map/", "HeightMap.raw", "terrain.jpg", 1);


	//테스트 오브젝트 셋팅
	m_pObject = new cNewObject;
	m_pObject->Setup("map", "box.obj");
	//m_pObject->SetSRT(D3DXVECTOR3(5.0f, 5.0f, 5.0f), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(-10, 0, 10));
	//

	g_pGameInfoManager->setup_Map("map", "rialto_small.obj");

	loader = new cAseLoader();
	m_pRootFrame = loader->Load("woman/woman_01_all.ASE");
	m_pRootFrame->SetSRT(D3DXVECTOR3(5.0f, 5.0f, 5.0f), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(10, 0, 10));

	//테스트 엑스모델 셋팅
	m_pXmodel = new cXModel("Xfile/bigship1.x");
	m_pXmodel->SetSRT(D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(-15, 0, -15));

	//하늘 셋팅
	m_pSKY = new cSKY();
	m_pSKY->Setup();

	//질럿 셋팅
	m_pMyCharacter = new cMyCharacter;
	m_pMyCharacter->Setup("Xfile", "Soldier76_with_gun.x");
	cCharacter* pCharacter = new cCharacter;
	m_pMyCharacter->SetCharacterController(pCharacter);

	//마우스 좌표 셋팅
	beforeMousePos.x = 1920 / 2;
	beforeMousePos.y = 1080 / 2;
	nowMousePos.x = 1920 / 2;
	nowMousePos.y = 1080 / 2;
	SetCursorPos(1920 / 2, 1080 / 2);
	
	m_pAI = new cAI;
	m_pAI->Setup("NPCS", "slicer.X");
	cAI_Controller* pAI_Controller = new cAI_Controller;
	m_pAI->SetAIController(pAI_Controller);

	setupUI();

	load = TRUE;
}

void cSCENE_INGAME::Update()
{
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

	m_pCamera->Update(m_pMyCharacter->GetPosition());
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

	if (cOBB::isCollision(m_pMyCharacter->GetOBB(), m_pObject->GetOBB()))
	{
		if (m_pAI)
			m_pAI->Update(true, m_pMyCharacter->GetPosition() - m_pAI->GetPosition());
	}
	else
	{
		if (m_pAI)
			m_pAI->Update(false, D3DXVECTOR3(0,0,0));
	}


	if (GetKeyState(VK_LBUTTON) & 0x8000)
	{
		if (g_pGameInfoManager->MaxBulletCount != 0)
		{
			m_pCrossHairPicking->CalcPosition();

			float dist;
			D3DXIntersect(m_pObject->GetMESH(), &m_pCrossHairPicking->GetOrigin(), &m_pCrossHairPicking->GetDirection(), &pHit, NULL, NULL, NULL, &dist, NULL, NULL);


			if (pHit && BulletCreateTime == MAXBulletCreateCount)
			{
				m_vCrossHairHitPos = m_pCrossHairPicking->GetOrigin() + dist * m_pCrossHairPicking->GetDirection();

				m_Bullet[BulletCreateCount].BulletDirection = m_vCrossHairHitPos - m_pMyCharacter->GetBulletPos();

				D3DXCreateSphere(g_pDevice, m_Bullet[BulletCreateCount].Radius, 20, 10, &m_Bullet[BulletCreateCount].m_pBulletMesh, NULL);
				m_Bullet[BulletCreateCount].m_vBulletCreatePos = m_pMyCharacter->GetBulletPos();
				m_Bullet[BulletCreateCount].m_stMtlCircle.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				m_Bullet[BulletCreateCount].m_stMtlCircle.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				m_Bullet[BulletCreateCount].m_stMtlCircle.Specular = D3DXCOLOR(0.0f, 0.7f, 0.7f, 1.0f);
				m_Bullet[BulletCreateCount].BulletLiveTime = 500;

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
		g_pNetworkManager->SendData(m_pMyCharacter->sendData());

	if(m_pUIBase)
		m_pUIBase->Update();
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
	g_pGameInfoManager->m_pMap->Render();
	m_pObject->Render();
	m_pXmodel->Render();

	// 넷 상태에 따라 스킵 가능하도록 수정
	if (g_pNetworkManager->GetNetStatus())
	{
		g_pOtherPlayerManager->render();
	}

	if (m_pRootFrame)
		m_pRootFrame->Render();
	{
		if (m_pMyCharacter)
			m_pMyCharacter->Render(NULL);
	}
	
	renderUI();

	if (pHit)
	{
		Mesh_Render();
	}

	if(m_pAI)
		m_pAI->Render(NULL);
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
		m_pAI->SetAnimationIndexBlend(n++);
		break;
		}
	 
	case WM_KEYUP:
		switch (wParam) 
		{
		case VK_ESCAPE:
			{
				g_pGameInfoManager->isESCPushed = ~g_pGameInfoManager->isESCPushed;
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
	D3DXCreateSprite(g_pDevice, &m_pSprite);
	//m_pTextureUI = g_pTextureManager->GetTexture("UI/김태희.jpg");

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

void cSCENE_INGAME::renderUI()
{
	if (m_pUIBase)
		m_pUIBase->Render(m_pSprite);
}

void cSCENE_INGAME::updateUI()
{
	if (m_pUIBase)
		m_pUIBase->Update();
}

void cSCENE_INGAME::OnClick(cUIButton * pSender)
{
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
}

void cSCENE_INGAME::Render_Text()
{
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

		m_Bullet[i].m_vBulletCreatePos += m_Bullet[i].BulletDirection;

		m_Bullet[i].BulletLiveTime--;

		if (m_Bullet[i].BulletLiveTime == 0)
		{
			m_Bullet.erase(m_Bullet.begin() + i);
			BulletCreateCount--;
		}
	}
}
