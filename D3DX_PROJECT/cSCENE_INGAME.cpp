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

DWORD FtoDW(float f)
{
	return *((DWORD*)&f);
}


cSCENE_INGAME::cSCENE_INGAME()
	: m_pCamera(NULL),
	m_pGrid(NULL),
	m_pMap(NULL),
	m_pObject(NULL),
	m_pXmodel(NULL),
	m_pSKY(NULL),
	m_pMyCharacter(NULL),
	m_pFont(NULL)
{
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
	SAFE_RELEASE(m_pFont);
}

void cSCENE_INGAME::Setup()
{
	m_pCrossHairPicking = new cCrossHairPicking;

	GetClientRect(g_hWnd, &m_Worldrc);

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
	g_pDevice->SetLight(1, &DirectLight);
	g_pDevice->SetLight(2, &DirectLight);
	g_pDevice->SetLight(3, &DirectLight);
	g_pDevice->LightEnable(0, TRUE);
	g_pDevice->LightEnable(1, TRUE);
	g_pDevice->LightEnable(2, TRUE);
	g_pDevice->LightEnable(3, TRUE);
	g_pDevice->SetRenderState(D3DRS_LIGHTING, true);

	// 하이트맵 셋팅
	m_pMap = new cHeightMap();
	m_pMap->Setup("map/", "HeightMap.raw", "terrain.jpg", 1);

	//테스트 오브젝트 셋팅
	m_pObject = new cNewObject;
	m_pObject->Setup("box.obj");
	//m_pObject->SetSRT(D3DXVECTOR3(5.0f, 5.0f, 5.0f), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(-10, 0, 10));
	//
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

	//임시 셋팅
	setupUI();

}

void cSCENE_INGAME::Update()
{
	BOOL static mouseMove = 0;
	g_pTimeManager->Update();


	//마우스 이동값 계산


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

	if (m_pMyCharacter)
		m_pMyCharacter->Update(m_pCamera->getDirection());


	float y = m_pMyCharacter->GetPosition().y;
	if (m_pMap)
		m_pMap->GetHeight(m_pMyCharacter->GetPosition().x, y, m_pMyCharacter->GetPosition().z);

	m_pMyCharacter->GetCharacterController()->SetPositionY(y);


	if (GetKeyState(VK_LBUTTON) & 0x8000)
	{
		m_pCrossHairPicking->Setup();

		float dist;
		D3DXIntersect(m_pObject->GetMESH(), &m_pCrossHairPicking->GetOrigin(), &m_pCrossHairPicking->GetDirection(), &pHit, NULL, NULL, NULL, &dist, NULL, NULL);

		if (pHit)
		{

			m_vCrossHairHitPos = m_pCrossHairPicking->GetOrigin() + dist * m_pCrossHairPicking->GetDirection();

			BulletDirection = m_vCrossHairHitPos - m_pMyCharacter->GetBulletPos();

			D3DXCreateSphere(g_pDevice, m_Bullet.Radius, 20, 10, &m_Bullet.m_pBulletMesh, NULL);
			m_Bullet.m_vBulletCreatePos = m_pMyCharacter->GetBulletPos();
			m_Bullet.m_stMtlCircle.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			m_Bullet.m_stMtlCircle.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			m_Bullet.m_stMtlCircle.Specular = D3DXCOLOR(0.0f, 0.7f, 0.7f, 1.0f);
		}
	}


	g_pNetworkManager->SendData(m_pMyCharacter->sendData());

}

void cSCENE_INGAME::Render()
{
	SetCursor(NULL);
	if (g_pGameInfoManager->isESCPushed)
	{
		SetCursor(LoadCursor(NULL, IDC_ARROW));
	}
	

	m_pSKY->Render();
	m_pGrid->Render();
	m_pMap->Render();
	m_pObject->Render();
	m_pXmodel->Render();

	g_pOtherPlayerManager->render();

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

}

void cSCENE_INGAME::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_LBUTTONDOWN:
		{
		static int n = 0;
		//m_pSkinnedMesh->SetAnimationIndex(n++);
		m_pMyCharacter->SetAnimationIndexBlend(n++);
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

void cSCENE_INGAME::RenderOtherPlayer()
{

}

void cSCENE_INGAME::Setup_HeightMap()
{
	cHeightMap* pMap = new cHeightMap;
	pMap->Setup("HeightMapData/", "HeightMap.raw", "terrain.jpg");
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

	D3DXCreateTextureFromFileEx(
		g_pDevice,
		"UI/crosshair_p.png",
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

	cUIImageView* pImageView = new cUIImageView;
	pImageView->SetPosition(0, 0, 0);
	pImageView->SetTexture("UI/crosshair_p.png");
	m_pUIRoot = pImageView;
}

void cSCENE_INGAME::renderUI()
{
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

	D3DXMATRIXA16 matT, matR, matS, mat;
	D3DXMatrixTranslation(&matT, m_Worldrc.right / 2 - 10, m_Worldrc.bottom / 2 - 10, 0);

	mat = matT;

	m_pSprite->SetTransform(&mat);

	RECT rc;
	SetRect(&rc, 0, 0, m_stImageInfo.Width, m_stImageInfo.Height);
	m_pSprite->Draw(m_pTextureUI,
		&rc,
		&D3DXVECTOR3(0, 0, 0),
		&D3DXVECTOR3(0, 0, 0),
		D3DCOLOR_ARGB(255, 255, 255, 255));
	m_pSprite->End();


	if (m_pUIRoot)
		m_pUIRoot->Render(m_pSprite);
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
	D3DXMATRIXA16 matWorld, matS, matR, matT;
	D3DXMatrixIdentity(&matS);
	D3DXMatrixIdentity(&matR);
	D3DXMatrixIdentity(&matT);

	matWorld = matS * matR;

	D3DXMatrixTranslation(&matT, m_Bullet.m_vBulletCreatePos.x, m_Bullet.m_vBulletCreatePos.y, m_Bullet.m_vBulletCreatePos.z);
	matWorld *= matT;
	g_pDevice->SetTransform(D3DTS_WORLD, &matWorld);
	g_pDevice->SetMaterial(&m_Bullet.m_stMtlCircle);
	m_Bullet.m_pBulletMesh->DrawSubset(0);

	m_Bullet.m_vBulletCreatePos += BulletDirection / 200;
}