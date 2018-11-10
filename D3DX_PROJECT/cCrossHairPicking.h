#pragma once
class cCrossHairPicking
{
public:
	cCrossHairPicking();
	~cCrossHairPicking();
private:
	RECT rc;
	POINT cp;
	D3DXVECTOR3 m_vMiddlePos;
	SYNTHESIZE(D3DXVECTOR3, m_Direction, Direction);
	SYNTHESIZE(D3DXVECTOR3, m_Origin, Origin);
public:
	void Setup();
	void CalcPosition();
};

