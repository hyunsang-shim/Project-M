#include "stdafx.h"
#include "ObjGroup.h"

ObjGroup::ObjGroup()
{
}


ObjGroup::~ObjGroup()
{
}

LPWSTR ConvertToLPWSTR1(const std::string& s)
{
	LPWSTR ws = new wchar_t[s.size() + 1]; // +1 for zero at the end
	copy(s.begin(), s.end(), ws);
	ws[s.size()] = 0; // zero at the end
	return ws;
}

void ObjGroup::setup()
{
	HRESULT tmp;
	LPWSTR ws = ConvertToLPWSTR1(textureName);
	tmp = D3DXCreateTextureFromFileW(g_pDevice, ws, &m_pTexture);

}

void ObjGroup::render(D3DXMATRIXA16& m_matTotal)
{
	
	g_pDevice->SetTransform(D3DTS_WORLD, &m_matTotal);
	g_pDevice->SetTexture(0, m_pTexture);
	g_pDevice->SetFVF(ST_PNT_VERTEX::FVF);
	g_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_vecPoint.size() / 3, &m_vecPoint[0], sizeof(ST_PNT_VERTEX));



	g_pDevice->SetTexture(0, NULL);
}
