#pragma once
#include "cUIObject.h"
class cFontManager;

class cUITextView : public cUIObject
{
public:
	cUITextView();
	~cUITextView();
protected:
	SYNTHESIZE(cFontManager::eFontType, m_eFontType, FontType);
	SYNTHESIZE(string, m_sText, text);
	SYNTHESIZE(DWORD, m_dwDrawTextFormat, DrawTextFormat);
	SYNTHESIZE(D3DXCOLOR, m_dwTextColor, TextColor);
public:
	virtual void Render(LPD3DXSPRITE pSprite) override;
};


