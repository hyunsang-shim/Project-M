
#include "stdafx.h"
#include "D3DX_PROJECT_SERVER.h"

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <vector>
#include <string>
#include <d3dx9.h>
#include <process.h>
#include <time.h>


#pragma comment(lib,"ws2_32.lib")

#define WM_ASYNC WM_USER+2
#define Tryout 200
#define MAX_USER 10

// Server IP Addresses.
// should activated only one at once;
//#define SERVER_ADDR "165.246.163.66"	// 은호씨
//#define SERVER_ADDR "165.246.163.71"		// 심현상
//#define SERVER_ADDR "192.168.0.9"	// 심현상 (노트북/공유기)
#define SERVER_ADDR "192.168.0.3"	// 심현상 (집)

using namespace std;

// global variables.
HWND			g_hWnd;
int				i;




enum character_status {
	Stand,
	Stand_Shoot,
	Run_Front,
	Run_Front_Shoot,
	Run_Left,
	Run_Left_Shoot,
	Run_Right,
	Run_Right_Shoot,
	Run_Back,
	Run_Back_Shoot,
	Dash,
	Reload,
	Hit,
	Down,
	Down_idle,
	Stand_Up,
	Dead,
	NumSize
};

struct CharacterStatus_PC
{
	char			MsgHeader[64];			// 메시지 헤더
	WORD			ID;				// 세션 ID	
	char			PlayerName[16];	// 유저이름
	WORD			Character_No;	// 캐릭터 종류
	WORD			Attack;			// 공력력
	DWORD			MaxHP;			// 최대 체력
	DWORD			CurHP;			// 현재 체력
	WORD			HP_Regen;		// 체력 재생
	DWORD			MoveSpeed;		// 이동 속도
	WORD			Mag_Cnt;		// 장탄 수
	WORD			Mag_Max;			// 최대 장전 수
	DWORD			ShootSpeed;		// 연사속도
	WORD			BulletTime;		// 총알 속도
	D3DXVECTOR3		CurPos;			// 현재 위치값
	float			Dir;				// 캐릭터가 바라보는 방향
	WORD			Status;			// 캐릭터 상태
	int				TargetID;		// 공격 한 대상
	int				FailCnt;		// 접속 여부
	SOCKET			s;				// 소켓
};

struct CharacterStatus_NPC
{
	char			MsgHeader[64];			// 메시지 헤더
	WORD			ID;				// 세션 ID	
	char			CharacterName[16];	// 캐릭터 이름
	WORD			Character_No;	// 캐릭터 종류
	WORD			Attack;			// 공력력
	DWORD			MaxHP;			// 최대 체력
	DWORD			CurHP;			// 현재 체력
	WORD			HP_Regen;		// 체력 재생
	DWORD			MoveSpeed;		// 이동 속도
	WORD			Mag_Cnt;		// 장탄 수
	WORD			Mag_Max;		// 최대 장전 수
	DWORD			ShootSpeed;		// 연사속도
	WORD			BulletTime;		// 총알 속도
	D3DXVECTOR3		CurPos;			// 현재 위치값
	float			Dir;			// 캐릭터가 바라보는 방향
	WORD			Status;			// 캐릭터 상태
	int				TargetID;		// 공격 한 대상
};


#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE					hInst;                              // 현재 인스턴스입니다.
WCHAR						szTitle[MAX_LOADSTRING];            // 제목 표시줄 텍스트입니다.
WCHAR						szWindowClass[MAX_LOADSTRING];      // 기본 창 클래스 이름입니다.
static						HANDLE hThread[MAX_USER];			// handles array for Thread 
HANDLE						hEvent;								// Handle for creating thread
CRITICAL_SECTION			crit;								// Critical Section for threads
vector<CharacterStatus_PC>	g_users;							// 유저 정보 구조체

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
void				threadUpdateUserInfo(void *user);
void CALLBACK		TimerProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_D3DX_PROJECT_SERVER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_D3DX_PROJECT_SERVER));

    MSG msg;

    // 기본 메시지 루프입니다.
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_D3DX_PROJECT_SERVER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName = nullptr;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   g_hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!g_hWnd)
   {
      return FALSE;
   }

   ShowWindow(g_hWnd, nCmdShow);
   UpdateWindow(g_hWnd);

   return TRUE;
}



//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	static WSADATA wsadata;
	static SOCKET p1, p2;
	static SOCKET s;
	static int arr[20][20] = { 0 };
	static TCHAR msg[200];
	static SOCKADDR_IN addr = { 0 }, c_addr;
	static TCHAR str[10];
	int size, msgLen;
	char* buffer;
	static bool turn;
	static WORD playerCnt;
	
	

	static int userNum = 0;
	buffer = (char*)malloc(sizeof(CharacterStatus_NPC) + 1);
	ZeroMemory(buffer, sizeof(CharacterStatus_PC) + 1);

    switch (message)
    {
	case WM_CREATE:
		SetTimer(hWnd, 123, 100, NULL);
		InitializeCriticalSection(&crit);
		playerCnt = 0;
		WSAStartup(MAKEWORD(2, 2), &wsadata);
		s = socket(AF_INET, SOCK_STREAM, 0);
		addr.sin_family = AF_INET;
		addr.sin_port = 20;
		addr.sin_addr.S_un.S_addr = inet_addr(SERVER_ADDR);

		// Network Bind Test
		// Success ==0 / FAIL = -1
		if (bind(s, (LPSOCKADDR)&addr, sizeof(addr)) != 0)
		{
			MessageBox(NULL, _T("Binding Failed!"), _T("Error"), MB_OK);
			return 0;
		}
		
		WSAAsyncSelect(s, hWnd, WM_ASYNC, FD_READ | FD_ACCEPT | FD_CLOSE);
		
		//
		// Check Listening
		if (listen(s, MAX_USER) == SOCKET_ERROR)
		{
			MessageBox(NULL, _T("Listening Failed!"), _T("Error"), MB_OK);
			return 0;
		}


		break;
	case WM_ASYNC:
		switch (lParam)
		{
		case FD_ACCEPT:
		{
			CharacterStatus_PC TmpUser;
			size = sizeof(c_addr);
			g_users.push_back(TmpUser);
			g_users.back().s = accept(s, (LPSOCKADDR)&c_addr, &size);
			g_users.back().ID = playerCnt;
			WSAAsyncSelect(g_users.back().s, hWnd, WM_ASYNC, FD_READ | FD_CLOSE);

			// Add Current User into Total User List
			playerCnt++;

			strcpy(g_users.back().MsgHeader, "welcome");
			send(g_users.back().s, (char*)&g_users.back(), sizeof(CharacterStatus_PC) + 1, 0);
			strcpy(g_users.back().MsgHeader, "TitleScene");

		}
			break;
		case FD_READ:			
		{			
			
				buffer[sizeof(CharacterStatus_PC)] = NULL;
				msgLen = recv(g_users[i].s, buffer, sizeof(CharacterStatus_PC) + 1, 0);
				buffer[msgLen] = NULL;
				CharacterStatus_PC *recieved = (CharacterStatus_PC*)&buffer;

				hThread[i] = (HANDLE)_beginthreadex(NULL, 0, (unsigned int(__stdcall *)(void *))threadUpdateUserInfo, (void *)&recieved, 0, NULL);
		}
			break;
		case FD_CLOSE:
			for (int i = 0; i < g_users.size(); i++)
			{
				if (g_users.at(i).s == (SOCKET)wParam)
				{					
					g_users.push_back(g_users[i]);
					g_users[i] = g_users[g_users.size() - 2];
					g_users.pop_back();

//					userMsgs[i].swap(userMsgs[user.size()-1]);
//					userMsgs.erase(userMsgs.end());
				}
			}
			break;
		}

		break;
	case WM_TIMER:
		for (int i = 0; i<g_users.size(); i++)
		{
			for (int j = 0; j < g_users.size(); j++)
			{			
				CharacterStatus_PC PrepareMsg;
				PrepareMsg = g_users.at(j);

				if (strcmp(g_users[i].MsgHeader, "welcome") == 0)
				{
					strcpy(PrepareMsg.MsgHeader, "welcome");
					PrepareMsg.MsgHeader[strlen(PrepareMsg.MsgHeader)] = NULL;

				}
				else if (strcmp(g_users[i].MsgHeader, "TitleScene") == 0)
				{
					strcpy(PrepareMsg.MsgHeader, "TitleScene");
					PrepareMsg.MsgHeader[strlen(PrepareMsg.MsgHeader)] = NULL;

				}
				else
				{
					strcpy(PrepareMsg.MsgHeader, "userData");
					PrepareMsg.MsgHeader[strlen(PrepareMsg.MsgHeader)] = NULL;

				}


				if (send(g_users.at(i).s, (char*)&PrepareMsg, sizeof(CharacterStatus_PC) + 1, 0) != -1)
				{
					g_users[i].FailCnt = 0;		// 전송 성공하면 접종 카운트를 초기화 한다.
				}
				else
				{
					g_users[i].FailCnt += 1;		// 전송 실패 카운트를 올린다.
				}
				
			}
		}

		for (int i = 0; i < g_users.size(); i++)
		{
			if (g_users.at(i).FailCnt > Tryout)
			{
				
				CharacterStatus_PC disconnectMsg;
				disconnectMsg.ID = g_users.at(i).ID;
				strcpy(disconnectMsg.MsgHeader, "disconnect");
				disconnectMsg.MsgHeader[strlen(disconnectMsg.MsgHeader)] = '\0';

			//	strcpy(buffer, message.c_str());
				for (int j = 0; j < g_users.size(); j++)
				{
					send(g_users.at(j).s, (char*)&disconnectMsg, sizeof(CharacterStatus_PC) + 1, 0);
				}
				g_users.erase(g_users.begin() + i);
				i--;
				playerCnt = g_users.size();
				
			}
		}
		break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다.
			char msg[64];
			sprintf(msg, "Connected %d Clients", playerCnt);
			TextOutA(hdc, 10, 10, msg, strlen(msg));
			if (g_users.size() > 0)
			{
				for (int i = 0; i < g_users.size(); i++)
				{
					string tmp;
					tmp.append("user #");
					char sTmp[12];
					tmp.append(itoa((int)g_users[i].ID, sTmp, 10));
					tmp.append(" id : ");
					tmp.append(g_users[i].PlayerName);
					TextOut(hdc, 10, 10 + (i+1) * 20 , tmp.c_str(), strlen(tmp.c_str()));
				}
			}

            EndPaint(hWnd, &ps);
			DeleteObject(hdc);			
        }
        break;
    case WM_DESTROY:
		PostQuitMessage(0);
		WSACleanup();
		PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Thread
void threadUpdateUserInfo(void *strUser)
{
	static CharacterStatus_PC TmpUser = *((CharacterStatus_PC*)&strUser);

	int idx = TmpUser.ID;
	EnterCriticalSection(&crit);
	// do something here
	if (strcmp(TmpUser.MsgHeader, "userData") == 0)
	{
		g_users[idx].CurHP = TmpUser.CurHP;
		g_users[idx].MaxHP = TmpUser.MaxHP;
		g_users[idx].Dir = TmpUser.Dir;
		g_users[idx].Status = TmpUser.Status;

		printf("recieved : user #%d's userData\n", g_users[idx].ID);
	}
	else if (strcmp(TmpUser.MsgHeader, "join") == 0)
	{
		char tmp[16];
		string s;
		itoa(TmpUser.ID, tmp, 10);
		s = "User # (";
		s.append(tmp);
		s.append(TmpUser.PlayerName);
		s.append(") Joinned the Game");
		MessageBox(NULL, s.c_str(), _T("New User!"), MB_OK);

		g_users[idx].CurHP = TmpUser.CurHP;
		g_users[idx].MaxHP = TmpUser.MaxHP;
		g_users[idx].Dir = TmpUser.Dir;
		g_users[idx].Status = TmpUser.Status;
		strcpy_s(g_users[idx].PlayerName, TmpUser.PlayerName);
		printf("recieved : user #%d(%s) Has joined in the Game.\n", g_users[idx].ID, g_users[idx].PlayerName);
	}
	else if (strcmp(TmpUser.MsgHeader, "disconnect") == 0)
	{
		for (int i = 0; i < g_users.size(); i++)
		{
			if (g_users[i].ID == TmpUser.ID)
			{
				g_users.erase(g_users.begin() + i);
				i--;
				break;
			}
		}

		InvalidateRgn(g_hWnd, NULL, TRUE);
	}
	else if (strcmp(TmpUser.MsgHeader, "TitleScene") == 0)
	{
		strcmp(g_users[i].MsgHeader, "TitleScene");
	}
	LeaveCriticalSection(&crit);

	return;
}

void CALLBACK TimerProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	InvalidateRgn(hWnd, NULL, FALSE);
}