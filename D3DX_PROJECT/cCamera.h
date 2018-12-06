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

	BOOL m_isLButtonDown;

	D3DXVECTOR3 m_vCamRotAngle;
	D3DXVECTOR3 m_vCamDirection;
	float m_fCameraDistance;
	float fDeltaX;
	float fDeltaY;
	BOOL SpringArmHit;
	float SpringArmDist;
	D3DXVECTOR3 HeadToCameraDir;
public:
	void Setup();
	void Update(D3DXVECTOR3 cube, D3DXVECTOR3 HeadPos);
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	D3DXVECTOR3 getDirection();

};