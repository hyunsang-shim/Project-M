#pragma once
#include "cUIButton.h"
class cSCENE_TITLE : public iButtonDelegate
{
public:
	cSCENE_TITLE();
	~cSCENE_TITLE();

	void Setup();
	void Update();
	void Render();
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


	// << UI start
	LPD3DXSPRITE		m_pSprite;
	LPDIRECT3DTEXTURE9	m_pTextureUI;
	D3DXIMAGE_INFO		m_stBGImageInfo, m_stImageInfo;
	cUIObject*			m_pUIRoot;
	cUIObject*			m_pUIShadowRoot;
	cUIObject*			m_pUICharacterSelect;
	cUITextView*		m_pNameInput;
	

	void BGSetup();			// 배경화면 셋업
	void BGRender();		// 배경화면 렌더

	void UIsetup();
	void UIrender();
	virtual void OnClick(cUIButton* pSender) override;
	virtual void buttonUpdate(cUIButton* pSender) override;

	bool enterNameState;
	// << UI end


private:
	cCamera* m_pCamera;
	CharacterStatus_PC m_strMyCharacter;
};

