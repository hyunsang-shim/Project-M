// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <winsock.h>


// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#include <map>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <list>
#include <time.h>
#include <fstream>
#include <stdio.h>
#include <winsock2.h>

using namespace std;


#include <assert.h>

#include <d3dx9.h>
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#define EPSILON 0.00001f
#define PI 3.141592

extern HWND g_hWnd;
#define SAFE_RELEASE(p) {if(p) p->Release() ; p=NULL;}
#define SAFE_DELETE(p) { if (p) delete p; p = NULL; }
#define SAFE_DELETE_ARRAY(p){if(p) delete[] p ; p=NULL;}


#define GRID_SIZE 100

#define SINGLETON(class_name) \
	private:\
		class_name(void);\
		~class_name(void);\
	public:\
		static class_name* GetInstance()\
		{\
			static class_name instance;\
			return &instance;\
		}

#define SYNTHESIZE(varType, varName, funName) \
protected : varType varName; \
public : inline varType Get##funName(void) const {return varName;}\
public : inline void Set##funName(varType var) { varName = var; }

#define SYNTHESIZE_PASS_BY_REF(varType, varName, funName)\
protected : varType varName;\
public : inline varType& Get##funName(void)  {return varName;}\
public : inline void Set##funName(varType& var) { varName = var; }

#define SAFE_ADD_REF(p){if(p) p->AddRef();}

#define SYNTHESIZE_ADD_REF(varType, varName, funName)\
protected: varType varName;\
public: virtual varType Get##funName(void) const {return varName;}\
public: virtual void Set##funName(varType var){\
	if(varName != var){\
	SAFE_ADD_REF(var);\
	SAFE_RELEASE(varName);\
	varName = var;\
	}\
}


#include "cDeviceManager.h"

struct ST_PC_VERTEX
{
	D3DXVECTOR3 p;
	D3DCOLOR c;
	enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };



};

struct ST_PNT_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR3 n;
	D3DXVECTOR2 t;//u,v coord
	enum { FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 };

};

struct ST_PT_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR2 t;//u,v coord
	enum { FVF = D3DFVF_XYZ | D3DFVF_TEX1 };

};

struct ST_POS_SAMPLE
{
	int n;
	D3DXVECTOR3 v;
	ST_POS_SAMPLE()
		:n(0)
		, v(0, 0, 0)
	{

	}
};

struct ST_ROT_SAMPLE
{
	int n;
	D3DXQUATERNION q;
	ST_ROT_SAMPLE()
		: n(0)
	{
		D3DXQuaternionIdentity(&q);

	}
};

struct ST_SIZEN
{
	int		nWidth;
	int		nHeight;
	ST_SIZEN(): nWidth(0), nHeight(0) { }
	ST_SIZEN(int _w, int _h): nWidth(_w), nHeight(_h) {}

};

enum character_list {
	PC_Soldier = 1,
	PC_Reaper,
	PC_Reinhardt,
	PC_Tracer,
	PC_Hanzo,
	PC_DIVA
};


enum character_status {
	CS_IDLE,
	CS_FRONT,
	CS_FRONT_SHIFT,
	CS_FRONT_LEFT,
	CS_FRONT_RIGHT,
	CS_FRONT_LEFT_SHIFT,
	CS_FRONT_RIGHT_SHIFT,
	CS_LEFT,
	CS_RIGHT,
	CS_BACK,
	CS_BACK_LEFT,
	CS_BACK_RIGHT,
	CS_RELOAD,
	CS_DOWN,
	CS_DEAD
};

enum NETWORK_HEADER {
	NH_USER_STATUS,
	NH_SHOT,
	NH_SELECT,
	NH_MY_NAME_IS
};


struct CharacterStatus_PC
{
	char		MsgHeader[64];			// �޽��� ���
	int			ID;				// ���� ID	
	char		PlayerName[16];	// �����̸�
	int			Character_No;	// ĳ���� ����
	int			Attack;			// ���·�
	int			MaxHP;			// �ִ� ü��
	int			CurHP;			// ���� ü��
	int			HP_Regen;		// ü�� ���
	int			MoveSpeed;		// �̵� �ӵ�
	int			Mag_Cnt;		// ��ź ��
	int			Mag_Max;			// �ִ� ���� ��
	int			ShootSpeed;		// ����ӵ�
	int			BulletTime;		// �Ѿ� �ӵ�
	D3DXVECTOR3	CurPos;			// ���� ��ġ��
	float		Dir;				// ĳ���Ͱ� �ٶ󺸴� ����
	int			Status;			// ĳ���� ����
	int			TargetID;		// ���� �� ���
	int			FailCnt;		// ���� ����
	SOCKET		s;				// ����
};

struct CharacterStatus_NPC
{
	char		MsgHeader[64];			// �޽��� ���
	int			ID;				// ���� ID	
	char		CharacterName[16];	// ĳ���� �̸�
	int			Character_No;	// ĳ���� ����
	int			Attack;			// ���·�
	int			MaxHP;			// �ִ� ü��
	int			CurHP;			// ���� ü��
	int			HP_Regen;		// ü�� ���
	int			MoveSpeed;		// �̵� �ӵ�
	int			Mag_Cnt;		// ��ź ��
	int			Mag_Max;		// �ִ� ���� ��
	int			ShootSpeed;		// ����ӵ�
	int			BulletTime;		// �Ѿ� �ӵ�
	D3DXVECTOR3	CurPos;			// ���� ��ġ��
	float		Dir;			// ĳ���Ͱ� �ٶ󺸴� ����
	int			Status;			// ĳ���� ����
	int			TargetID;		// ���� �� ���
};

typedef struct UserInfo
{//(int x, int y, int direction, int action, int actionCount)

	int userNum;
	float x;
	float y;
	float z;
	float direction;
	int action;
	int actionCount;
}UserInfo;


#include "cMainGame.h"
#include "cCamera.h"
#include "cHeightMap.h"

namespace d3d
{
	D3DLIGHT9 InitDirectionalLight(D3DXVECTOR3* direction, D3DXCOLOR* color);
	D3DLIGHT9 InitPointLight(D3DXVECTOR3* position, D3DXCOLOR* color);
	D3DLIGHT9 InitSpotLight(D3DXVECTOR3* position, D3DXVECTOR3* direction, D3DXCOLOR* color);

	const D3DXCOLOR WHITE(D3DCOLOR_XRGB(255, 255, 255));
	const D3DXCOLOR BLACK(D3DCOLOR_XRGB(0, 0, 0));
	const D3DXCOLOR RED(D3DCOLOR_XRGB(255, 0, 0));
	const D3DXCOLOR GREEN(D3DCOLOR_XRGB(0, 255, 0));
	const D3DXCOLOR BLUE(D3DCOLOR_XRGB(0, 0, 255));
	const D3DXCOLOR YELLOW(D3DCOLOR_XRGB(255, 255, 0));
	const D3DXCOLOR CYAN(D3DCOLOR_XRGB(0, 255, 255));
	const D3DXCOLOR MAGENTA(D3DCOLOR_XRGB(255, 0, 255));
}

#include "cDeviceManager.h"
#include "cObjectManager.h"
#include "cTextureManager.h"
#include "cTimeManager.h"
#include "cFontManager.h"
#include "cGameInfoManager.h"
#include "cNetworkManager.h"
#include "cOtherPlayerManager.h"

using namespace std;


//<< :


