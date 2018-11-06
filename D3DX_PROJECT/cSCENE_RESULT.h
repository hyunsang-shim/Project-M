#pragma once
class cSCENE_RESULT
{
public:
	cSCENE_RESULT();
	~cSCENE_RESULT();

	void Setup();
	void Update();
	void Render();
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	cCamera* m_pCamera;
};

