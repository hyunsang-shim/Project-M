#pragma once
class cCamera;

#include "cHeightMap.h"
#include "cFrame.h"
class cSkinnedMesh;
class cGrid;
class cNewObject;
class cXModel;
class cAseLoader;
class cSKY;
class cMyCharacter;
class cMainGame
{
public:
	cMainGame();
	~cMainGame();


public:
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

public:	//광원을 초기화 하는 함수
	D3DLIGHT9 InitDirectionalLight(D3DXVECTOR3* direction, D3DXCOLOR* color);
	D3DLIGHT9 InitPointLight(D3DXVECTOR3* position, D3DXCOLOR* color);
	D3DLIGHT9 InitSpotLight(D3DXVECTOR3* position, D3DXVECTOR3* direction, D3DXCOLOR* color);
private:
	D3DLIGHT9 DirectLight;	//메인 광원

private :
	LPD3DXFONT m_pFont;
	ID3DXMesh* m_p3DText;
public:
	void Creat_Font();
	void Render_Text();
};

