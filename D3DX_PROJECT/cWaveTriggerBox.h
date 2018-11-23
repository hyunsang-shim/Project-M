#pragma once

class cOBB;

struct TriggerBox
{
	string Name;
	SYNTHESIZE(D3DXVECTOR3, m_vMin, Min);
	SYNTHESIZE(D3DXVECTOR3, m_vMax, Max);
	bool m_DisplayOrNot;
};

class cWaveTriggerBox
{
public:
	cWaveTriggerBox();
	~cWaveTriggerBox();
private:
	vector<TriggerBox> m_vecTrigger;
	D3DXMATRIXA16 m_matWorld;
	SYNTHESIZE(int, m_WaveCount, WaveCount);
	SYNTHESIZE(bool, m_NextWave, NextWave);
	cOBB* m_pOBB;
	TriggerBox tb;
public:
	void Setup();
	void Update();
	void Render();
	vector<TriggerBox> GetTriggerBox();
};

