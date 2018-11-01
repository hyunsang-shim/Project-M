#pragma once
class cCharacter:public cObject
{
public:
	cCharacter();
	virtual ~cCharacter(void);

protected:
	float m_fRotY;
<<<<<<< HEAD
	float m_fRotX;
=======
	float fDeltaX;
	float fDeltaY;
>>>>>>> 4126a3d624bc20d3ca90c49fd4d7d019b4664780
	D3DXVECTOR3 m_vDirection;
	D3DXVECTOR3 m_vPosition;
	
	D3DXMATRIXA16 m_matWorld;

	POINT m_ptPrevMouse;



public:
	virtual void SetUP();
	virtual void Update();
	virtual void Update(float ROTY, D3DXVECTOR3 POSITION);
	virtual void Render();
	virtual D3DXVECTOR3& GetPosition();
	void SetPositionY(float y);

	POINT m_ptPrevMouse;
	virtual D3DXMATRIXA16* GetTransform()
	{
		return &m_matWorld;
	}
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

