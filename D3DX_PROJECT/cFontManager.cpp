#include "stdafx.h"
#include "cFontManager.h"


cFontManager::cFontManager()
{
}


cFontManager::~cFontManager()
{
}

LPD3DXFONT cFontManager::GetFont(eFontType e)
{
	D3DXFONT_DESC fd;
	ZeroMemory(&fd, sizeof(D3DXFONT_DESC));

	if (m_mapFont.find(e) != m_mapFont.end())
	{
		return m_mapFont[e];
	}
	if (e == E_DEFAULT)
	{
		fd.Height = 50;
		fd.Width = 25;
		fd.Weight = FW_MEDIUM;
		fd.Italic = false;
		fd.CharSet = DEFAULT_CHARSET;
		fd.OutputPrecision = OUT_DEFAULT_PRECIS;
		fd.PitchAndFamily = FF_DONTCARE;

		{
			AddFontResource(TEXT("font/umberto.ttf"));
			std::wstring name(L"±¼¸²Ã¼");
			const wchar_t* szName = name.c_str();
			wcscpy(fd.FaceName, szName);
		}
	}
	else if (e == E_QUEST)
	{
		fd.Height = 50;
		fd.Width = 25;
		fd.Weight = FW_MEDIUM;
		fd.Italic = false;
		fd.CharSet = DEFAULT_CHARSET;
		fd.OutputPrecision = OUT_DEFAULT_PRECIS;
		fd.PitchAndFamily = FF_DONTCARE;

		{
			AddFontResource(TEXT("font/umberto.ttf"));
			std::wstring name(L"umberto");
			const wchar_t* szName = name.c_str();
			wcscpy(fd.FaceName, szName);
		}
	}

		D3DXCreateFontIndirect(g_pDevice, &fd, &m_mapFont[e]);

	return m_mapFont[e];
}

void cFontManager::Destroy()
{
	for each(auto it in m_mapFont)
	{
		SAFE_RELEASE(it.second);
	}
}

