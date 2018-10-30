#pragma once
class cCamera
{
public:
	cCamera();
	~cCamera();
private:
	D3DXVECTOR3 m_vEye;
	D3DXVECTOR3 m_vLookAt;
	D3DXVECTOR3 m_vUp;

	POINT m_ptPrevMouse;
	BOOL m_isLButtonDown;

	D3DXVECTOR3 m_vCamRotAngle;
	float m_fCameraDistance;



public:
	void Setup();
	void Update(D3DXVECTOR3 cube);
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

};