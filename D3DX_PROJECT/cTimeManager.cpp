#include "stdafx.h"
#include "cTimeManager.h"


cTimeManager::cTimeManager()
{
	m_dwLastUpdateTime = GetTickCount();
}


cTimeManager::~cTimeManager()
{
}

void cTimeManager::Update()
{
	DWORD dwCurrentTIme = GetTickCount();
	m_fElapsedTime = (dwCurrentTIme - m_dwLastUpdateTime) / 1000.0f;
	m_dwLastUpdateTime = dwCurrentTIme;
}

float cTimeManager::GetElapsedTime()
{
	return m_fElapsedTime;
}

float cTimeManager::GetLastUpdateTime()
{
	return m_dwLastUpdateTime;
}
