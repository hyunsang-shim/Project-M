#include "stdafx.h"
#include "cSCENE_TITLE.h"


cSCENE_TITLE::cSCENE_TITLE() :
	m_pCamera(NULL)
{
}


cSCENE_TITLE::~cSCENE_TITLE()
{
	SAFE_DELETE(m_pCamera);
}

void cSCENE_TITLE::Setup()
{
	//朝五虞 実特
	m_pCamera = new cCamera();
	m_pCamera->Setup();
}

void cSCENE_TITLE::Update()
{
}

void cSCENE_TITLE::Render()
{
}

void cSCENE_TITLE::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
}
