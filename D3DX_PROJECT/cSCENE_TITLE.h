#pragma once
class cSCENE_TITLE
{
public:
	cSCENE_TITLE();
	~cSCENE_TITLE();

	void Setup();
	void Update();
	void Render();
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	cCamera* m_pCamera;
};

