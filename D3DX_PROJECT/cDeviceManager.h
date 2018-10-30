#pragma once
#define g_pDeviceManager cDeviceManager::GetInstance()
#define g_pDevice g_pDeviceManager->GetDevice()


class cDeviceManager
{
public:
	SINGLETON(cDeviceManager)

private:
	LPDIRECT3D9 m_pD3D;
	PDIRECT3DDEVICE9 m_pD3DDevice;
	// ΩÃ±€≈Ê

	//
public:
	LPDIRECT3DDEVICE9 GetDevice();
	void Destroy();


};

