#pragma once
#include "cUIButton.h"

class cCamera;
class cHeightMap;
class cGrid;
class cNewObject;
class cAseLoader;
class cXModel;
class cFrame;
class cSKY;
class cMyCharacter;
class cUIObject;
class cCrossHairPicking;
class cAI;
class cWaveTriggerBox;

#define MAXBulletCreateCount 5

struct Bullet
{
	ID3DXMesh *m_pBulletMesh;
	D3DXVECTOR3 m_vBulletCreatePos;
	float Radius = 0.2f;
	D3DMATERIAL9 m_stMtlCircle;
	D3DXVECTOR3 BulletDirection;
	D3DXMATRIXA16 matT;
	int BulletLiveTime;
};

class cSCENE_INGAME: public iButtonDelegate
{
public:
	cSCENE_INGAME();
	~cSCENE_INGAME();

	void Setup();
	void Update();
	void Render();
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void Setup_HeightMap();
private:
	cCamera* m_pCamera;	
	//cNewObject* m_pMap;
	cNewObject* m_pMap;
	cGrid* m_pGrid;
	cNewObject* m_pObject;
	cAseLoader* loader;
	cXModel* m_pXmodel;
	cFrame *m_pRootFrame;
	cSKY *m_pSKY;
	cMyCharacter *m_pMyCharacter;
private:
	D3DLIGHT9 DirectLight; //¸ÞÀÎ ±¤¿ø
	D3DLIGHT9 subLight;

public:
	D3DLIGHT9 InitDirectionalLight(D3DXVECTOR3* direction, D3DXCOLOR* color);
	D3DLIGHT9 InitPointLight(D3DXVECTOR3* position, D3DXCOLOR* color);
	D3DLIGHT9 InitSpotLight(D3DXVECTOR3* position, D3DXVECTOR3* direction, D3DXCOLOR* color);
	
public:
	POINT beforeMousePos;
	POINT nowMousePos;
	  
private:
	LPD3DXSPRITE		m_pSprite;
	LPDIRECT3DTEXTURE9	m_pTextureUI;
	D3DXIMAGE_INFO		m_stImageInfo;
	cUIObject*			m_pUIBase;
	cUIObject*			m_pUIShadowRoot;

	RECT				m_Worldrc;
public:
	void setupUI();
	void renderUI();
	void updateUI();


	virtual void OnClick(cUIButton* pSender) override;
	virtual void buttonUpdate(cUIButton* pSender) override;


private:
	LPD3DXFONT m_pFont; 

	LPD3DXFONT m_pFont2; 

public:
	void Creat_Font();
	void Render_Text();

	/*
	// : billborard
	void Set_Billboard(D3DXMATRIXA16* pmatWorld);

	// : particle
private:
	vector<ST_PC_VERTEX>		m_vecVertexParticle;
public:
	void Setup_Particle();
	void Update_Particle();
	void Render_Particle();

	*/

// >> : CrossHair
private:
	cCrossHairPicking *m_pCrossHairPicking;
	D3DXVECTOR3 m_vCrossHairHitPos;
	BOOL pHit = false;
	vector<Bullet> m_Bullet;
	int BulletCreateTime;
	int BulletCreateCount;
public:
	void Mesh_Render();
// << :
	BOOL load;

// >> : TriggerBox
private:
	cWaveTriggerBox* m_pTriggerBox;
	bool WaveStartOrEnd;
// << :

//>> : temp
private:
	BOOL AIMeshHit;
	float AIMeshDist;
};

