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
	cUIObject*			m_pUILoading;
	cUITextView*		m_pNameInput;
	

	void BGSetup();			// 배경화면 셋업
	void BGRender();		// 배경화면 렌더

	void UIsetup();
	void UIrender();
	void OtherPlayerUpdate();
	void showTime();

	void Creat_font();
	virtual void OnClick(cUIButton* pSender) override;
	virtual void buttonUpdate(cUIButton* pSender) override;

	bool enterNameState;
	// << UI end

	cUIButton* player1_character; // 내 캐릭

	cUIButton* ready_button;
	vector<cUIButton*> other_player_character;
	thread t1, t2;

	thread t1;
	thread t2;

private:
	cCamera* m_pCamera;
	CharacterStatus_PC m_strMyCharacter;

	LPD3DXFONT m_pFont;
	LPD3DXFONT m_pFont2;
};

