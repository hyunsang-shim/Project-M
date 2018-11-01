#include "stdafx.h"
#include "cCamera.h"


cCamera::cCamera() : m_vEye(0, 0, -5), m_vLookAt(0, 0, 0), m_vUp(0, 1, 0)
{
	// : 카메라 앵글, 거리, 버튼, 마우스 위치
	m_vCamRotAngle.x = 0.0f;
	m_vCamRotAngle.y = 0.0f;
	m_isLButtonDown = false;
	m_fCameraDistance = 5.0;
	m_ptPrevMouse.x = 9999;
	m_ptPrevMouse.y = 9999;
	m_vCamDirection = D3DXVECTOR3(0, 0, 0);
}


cCamera::~cCamera()
{
}

void cCamera::Setup()
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);

	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4.0f, rc.right / (float)rc.bottom, 1.0f, 1000.0f);
	g_pDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}


void cCamera::Update(D3DXVECTOR3 cube)
{
	/*cube = D3DXVECTOR3(0, 0, 0);*/
	
	D3DXMATRIXA16 matR, matRX, matRY, matTY;
	D3DXMATRIXA16 m_matTrans;
	D3DXMatrixTranslation(&m_matTrans, cube.x, cube.y, cube.z);

	m_vEye = D3DXVECTOR3(0, m_fCameraDistance, -m_fCameraDistance);

	D3DXMatrixRotationX(&matRX, m_vCamRotAngle.x);
	D3DXMatrixRotationY(&matRY, m_vCamRotAngle.y);
	matR = matRX * matRY;

	D3DXVec3TransformCoord(&m_vEye, &m_vEye, &matR);
	D3DXVec3TransformCoord(&m_vEye, &m_vEye, &m_matTrans);
	D3DXMATRIXA16 matView;
	D3DXVECTOR3 aimSet = cube + (cube - m_vEye)*10;
	D3DXMatrixLookAtLH(&matView, &m_vEye, &cube , &m_vUp);
	g_pDevice->SetTransform(D3DTS_VIEW, &matView);


	m_vCamDirection = cube - m_vEye;
}

void cCamera::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_LBUTTONDOWN:
		m_ptPrevMouse.x = LOWORD(lParam);
		m_ptPrevMouse.y = HIWORD(lParam);
		m_isLButtonDown = true;
		break;

	case WM_LBUTTONUP:
		m_isLButtonDown = false;
		break;

	case WM_MOUSEMOVE:
		if (1)
		{
			POINT ptCurrMouse;
			ptCurrMouse.x = LOWORD(lParam);
			ptCurrMouse.y = HIWORD(lParam);

			float fDeltaX = (float)ptCurrMouse.x - m_ptPrevMouse.x;
			float fDeltaY = (float)ptCurrMouse.y - m_ptPrevMouse.y;

			m_vCamRotAngle.y += (fDeltaX / 100.f);
			m_vCamRotAngle.x += (fDeltaY / 100.f);

			m_ptPrevMouse = ptCurrMouse;
		}
		break;

	case WM_MOUSEWHEEL:
		m_fCameraDistance -= (GET_WHEEL_DELTA_WPARAM(wParam) / 3.0f);
		if (m_fCameraDistance < EPSILON)
			m_fCameraDistance = EPSILON;
		if (m_fCameraDistance > 30.0)
			m_fCameraDistance = 30.0;
		if (m_fCameraDistance < 2.0)
			m_fCameraDistance = 2.0;

		break;
	}
}

D3DXVECTOR3 cCamera::getDirection()
{
	return m_vCamDirection;
}

