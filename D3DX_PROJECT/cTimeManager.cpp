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
	m_fElapsedTime = (dwCurrentTIme - m_dwLastUpdateTime) / 2000.0f;
	m_dwLastUpdateTime = dwCurrentTIme;
}

float cTimeManager::GetElapsedTime()
{
	return m_fElapsedTime * 2.0f;
}

float cTimeManager::GetLastUpdateTime()
{
	return m_dwLastUpdateTime;
}
