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
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); \


	void SetSceneChangeTriger(int sceneNum);
	int getSceneNum();

private:
	void SceneChangeNum();

private:
	int SCENENUM;

public:


};

