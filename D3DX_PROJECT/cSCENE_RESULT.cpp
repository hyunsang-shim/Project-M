#include "stdafx.h"
#include "cSCENE_RESULT.h"


cSCENE_RESULT::cSCENE_RESULT() :
	m_pCamera(NULL)
{
}


cSCENE_RESULT::~cSCENE_RESULT()
{
}

void cSCENE_RESULT::Setup()
{
	//ī�޶� ����
	m_pCamera = new cCamera();
	m_pCamera->Setup();
	
}

void cSCENE_RESULT::Update()
{
}

void cSCENE_RESULT::Render()
{
}

void cSCENE_RESULT::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
}
