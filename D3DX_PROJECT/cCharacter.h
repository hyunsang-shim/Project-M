#pragma once
class cCharacter:public cObject
{
public:
	cCharacter();
	virtual ~cCharacter(void);

protected:
	float m_fRotY;
	float m_fRotX;
	D3DXVECTOR3 m_vDirection;
	D3DXVECTOR3 m_vPosition;
	
	D3DXMATRIXA16 m_matWorld;

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

};

