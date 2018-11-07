#pragma once

class cCamera;
class cHeightMap;
class cGrid;
class cNewObject;
class cAseLoader;
class cXModel;
class cFrame;
class cSKY;
class cMyCharacter;

class cSCENE_INGAME
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
	cHeightMap* m_pMap;
	cGrid* m_pGrid;
	cNewObject* m_pObject;
	cAseLoader* loader;
	cXModel* m_pXmodel;
	cFrame *m_pRootFrame;
	cSKY *m_pSKY;
	cMyCharacter *m_pMyCharacter;

private:
	D3DLIGHT9 DirectLight; //¸ÞÀÎ ±¤¿ø

public:
	D3DLIGHT9 InitDirectionalLight(D3DXVECTOR3* direction, D3DXCOLOR* color);
	D3DLIGHT9 InitPointLight(D3DXVECTOR3* position, D3DXCOLOR* color);
	D3DLIGHT9 InitSpotLight(D3DXVECTOR3* position, D3DXVECTOR3* direction, D3DXCOLOR* color);
	
public:
	POINT beforeMousePos;
	POINT nowMousePos;
	  
private:
	LPD3DXFONT m_pFont;

public:
	void setupUI();
	void renderUI();
};

