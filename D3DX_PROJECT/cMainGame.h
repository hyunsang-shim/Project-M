#pragma once

#include "cSCENE_INGAME.h"
#include "cSCENE_RESULT.h"
#include "cSCENE_TITLE.h"

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

	void SetSceneChangeTriger(int sceneNum);
	int getSceneNum();

private:
	void SceneChangeNum();

private:
	int SCENENUM;
	int Next_SCENEN;
	BOOL sceneChangeTriger;

private:
	cSCENE_INGAME* m_pScene_Ingame;
	cSCENE_RESULT* m_pScene_Result;
	cSCENE_TITLE* m_pScene_Title;


};

