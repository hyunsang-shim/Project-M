#pragma once
#include "cUIObject.h"
class cUIImageView:public cUIObject
{
public:
	cUIImageView();
	virtual ~cUIImageView();

protected:
	LPDIRECT3DTEXTURE9 m_pTexture;
public:
	virtual void SetTexture(char* szFullPath);
	virtual void Render(LPD3DXSPRITE pSprite) override;


};

