#pragma once
class cCharacter
{
public:
	cCharacter();
	virtual ~cCharacter(void);

protected:
	float m_fRotY;
	D3DXVECTOR3 m_vDirection;
	D3DXVECTOR3 m_vPosition;
	D3DXMATRIXA16 m_matWorld;

public:
	virtual void SetUP();
	virtual void Update();
	virtual void Render();
	virtual D3DXVECTOR3& GetPosition();
};

