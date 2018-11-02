#include "stdafx.h"
#include "cUITextView.h"


cUITextView::cUITextView()
	:m_eFontType(cFontManager::E_DEFAULT),
	m_dwDrawTextFormat(DT_LEFT | DT_TOP),
	m_dwTextColor(D3DCOLOR_XRGB(0, 0, 0))
{
	D3DXMatrixIdentity(&m_matWorld);
}


cUITextView::~cUITextView()
{
}

void cUITextView::Render(LPD3DXSPRITE pSprite)
{
	if (m_isHidden)	return;

	LPD3DXFONT pFont = g_pFontManager->GetFont(m_eFontType);
	RECT rc;
	SetRect(&rc,
		(int)m_matWorld._41,
		(int)m_matWorld._42,
		(int)m_matWorld._41 + (int)m_stSize.nWidth,
		(int)m_matWorld._42 + (int)m_stSize.nHeight);
	pFont->DrawText(NULL,
		m_sText.c_str(),
		m_sText.length(),
		&rc,
		m_dwDrawTextFormat,
		m_dwTextColor);

	cUIObject::Render(pSprite);
}
