// D3DX_PROJECT_SERVER.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "D3DX_PROJECT_SERVER.h"

#include <stdio.h>
#include <winsock.h>
#include <vector>
#include <string>
#include <d3dx9.h>

using namespace std;
#pragma comment(lib,"ws2_32.lib")
#define WM_ASYNC WM_USER+2
using namespace std;
#define Tryout 200

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


//bool StartWith(char * FindStr, char * SearchStr)
//{
//	char* temp = strstr(FindStr, SearchStr);
//	if (temp == FindStr)
//		return true;
//	return false;
//}


#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

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
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_D3DX_PROJECT_SERVER);
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

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

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
	static vector<CharacterStatus_PC> user;
	static SOCKET s;
	static int arr[20][20] = { 0 };
	static TCHAR msg[200];
	static SOCKADDR_IN addr = { 0 }, c_addr;
	static TCHAR str[10];
	int size, msgLen;
	char* buffer;
	static bool turn;
	static WORD playerCnt;
//	static vector<string> userMsgs;		//유저 상태 메시지

	CharacterStatus_PC TmpUser;

	static int userNum;


    switch (message)
    {
	case WM_CREATE:
		SetTimer(hWnd, 123, 25, NULL);

		playerCnt = 0;
		WSAStartup(MAKEWORD(2, 2), &wsadata);
		s = socket(AF_INET, SOCK_STREAM, 0);
		addr.sin_family = AF_INET;
		addr.sin_port = 20;
		// addr.sin_addr.s_addr = inet_addr("165.246.163.66");	// 은호씨
		addr.sin_addr.s_addr = inet_addr("165.246.163.71");		// 심현상
		// addr.sin_addr.s_addr = inet_addr("192.168.0.7");		// 심현상 (집)


		// Network Bind Test
		if (bind(s, (LPSOCKADDR)&addr, sizeof(addr)))
		{
			MessageBox(NULL, _T("Binding Failed!"), _T("Error"), MB_OK);
			return 0;
		}


		WSAAsyncSelect(s, hWnd, WM_ASYNC, FD_ACCEPT | FD_CLOSE);


		//
		// Check Listening
		if (listen(s, 5) == SOCKET_ERROR)
		{
			MessageBox(NULL, _T("Listening Failed!"), _T("Error"), MB_OK);
			return 0;
		}

		break;

	case WM_ASYNC:
		switch (lParam)
		{
		case FD_ACCEPT:
			size = sizeof(c_addr);
			user.push_back(TmpUser);
			user.back().s = accept(s, (LPSOCKADDR)&c_addr, &size);
			user.back().ID = playerCnt;
			WSAAsyncSelect(user.back().s, hWnd, WM_ASYNC, FD_READ | FD_CLOSE);
			
			// Add Current User into Total User List
			playerCnt++;

			strcpy(user.back().MsgHeader, "welcome");
			send(user.back().s, (char*)&user.back(), sizeof(CharacterStatus_PC) + 1, 0);
//			userMsgs.resize(user.size());			//
			InvalidateRgn(hWnd, NULL, FALSE);		//
			
			break;
		case FD_READ:
			for (int i = 0; i < user.size(); i++)			
			{
				memset(buffer, 0, sizeof(CharacterStatus_PC)+1); // reset buffer to null;
				CharacterStatus_PC *recieved;
				if (user[i].s == (SOCKET)wParam)
				{
					msgLen = recv((SOCKET)wParam, buffer, sizeof(CharacterStatus_PC) + 1, 0);
					buffer[msgLen] = NULL;

					if (strcmp(recieved->MsgHeader, "userData") == 0)
					{
						user[i].CurHP = recieved->CurHP;
						user[i].MaxHP = recieved->MaxHP;
						user[i].Dir = recieved->Dir;
						user[i].Status = recieved->Status;

						printf("recieved : user #%d's userData\n", user[i].ID);
					}
					else if (strcmp(recieved->MsgHeader, "join") == 0)
					{
						user[i].CurHP = recieved->CurHP;
						user[i].MaxHP = recieved->MaxHP;
						user[i].Dir = recieved->Dir;
						user[i].Status = recieved->Status;
						strcpy_s(user[i].PlayerName, recieved->PlayerName);
						printf("recieved : user #%d(%s) Has joined in the Game.\n", user[i].ID, user[i].PlayerName);
					}
				}
			}			
			break;
		case FD_CLOSE:
			for (int i = 0; i < user.size(); i++)
			{
				if (user.at(i).s == (SOCKET)wParam)
				{
					user.erase(user.begin() + i);
//					userMsgs[i].swap(userMsgs[user.size()-1]);
//					userMsgs.erase(userMsgs.end());
				}
			}
			break;
		}

		break;
	case WM_TIMER:
		for (int i = 0; i<user.size(); i++)
		{
			for (int j = 0; j < user.size(); j++)
			{			
				CharacterStatus_PC PrepareMsg;
				PrepareMsg = user.at(j);

				// 참고 : send(cs[0], (char*)&GameMessage, sizeof(GameMessage) + 1, 0);
				if (send(user.at(i).s, (char*)&PrepareMsg, sizeof(CharacterStatus_PC) + 1, 0) != -1)
				{
					user.at(i).FailCnt = 0;		// 전송 성공하면 접종 카운트를 초기화 한다.
				}
				else
				{
					user.at(i).FailCnt += 1;		// 전송 실패 카운트를 올린다.
				}
				
			}
		}

		for (int i = 0; i < user.size(); i++)
		{
			if (user.at(i).FailCnt > Tryout)
			{
				/*
				string message;
				message += "disconnect ";
				message += to_string(user.at(i).ID);
				*/
				CharacterStatus_PC disconnectMsg;
				disconnectMsg.ID = user.at(i).ID;
				strcpy(disconnectMsg.MsgHeader, "disconnect");
				disconnectMsg.MsgHeader[strlen(disconnectMsg.MsgHeader)] = '\0';

			//	strcpy(buffer, message.c_str());
				for (int j = 0; j < user.size(); j++)
				{
					send(user.at(j).s, (char*)&disconnectMsg, sizeof(CharacterStatus_PC) + 1, 0);
				}
				user.erase(user.begin() + i);
				i--;
				playerCnt = user.size();
				
			}
		}
		InvalidateRgn(hWnd, NULL, FALSE);
		break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다.
			char msg[64];
			sprintf(msg, "Connected %d Clients", playerCnt);
			TextOutA(hdc, 10, 10, msg, strlen(msg));
			if (user.size() > 0)
			{
				for (int i = 0; i < user.size(); i++)
				{
					char tmp[128];
					sprintf(tmp, "user #%d (%s) is alive", user[i].ID, user[i].PlayerName);
					tmp[strlen(tmp)] = NULL;
					TextOutA(hdc, 10, 10 + (i+1) * 20 , tmp, strlen(tmp));
				}
			}

            EndPaint(hWnd, &ps);
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

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
