// D3DX_PROJECT_SERVER.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
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
	char			MsgHeader[64];			// �޽��� ���
	WORD			ID;				// ���� ID	
	char			PlayerName[16];	// �����̸�
	WORD			Character_No;	// ĳ���� ����
	WORD			Attack;			// ���·�
	DWORD			MaxHP;			// �ִ� ü��
	DWORD			CurHP;			// ���� ü��
	WORD			HP_Regen;		// ü�� ���
	DWORD			MoveSpeed;		// �̵� �ӵ�
	WORD			Mag_Cnt;		// ��ź ��
	WORD			Mag_Max;			// �ִ� ���� ��
	DWORD			ShootSpeed;		// ����ӵ�
	WORD			BulletTime;		// �Ѿ� �ӵ�
	D3DXVECTOR3		CurPos;			// ���� ��ġ��
	float			Dir;				// ĳ���Ͱ� �ٶ󺸴� ����
	WORD			Status;			// ĳ���� ����
	int				TargetID;		// ���� �� ���
	int				FailCnt;		// ���� ����
	SOCKET			s;				// ����
};

struct CharacterStatus_NPC
{
	char			MsgHeader[64];			// �޽��� ���
	WORD			ID;				// ���� ID	
	char			CharacterName[16];	// ĳ���� �̸�
	WORD			Character_No;	// ĳ���� ����
	WORD			Attack;			// ���·�
	DWORD			MaxHP;			// �ִ� ü��
	DWORD			CurHP;			// ���� ü��
	WORD			HP_Regen;		// ü�� ���
	DWORD			MoveSpeed;		// �̵� �ӵ�
	WORD			Mag_Cnt;		// ��ź ��
	WORD			Mag_Max;		// �ִ� ���� ��
	DWORD			ShootSpeed;		// ����ӵ�
	WORD			BulletTime;		// �Ѿ� �ӵ�
	D3DXVECTOR3		CurPos;			// ���� ��ġ��
	float			Dir;			// ĳ���Ͱ� �ٶ󺸴� ����
	WORD			Status;			// ĳ���� ����
	int				TargetID;		// ���� �� ���
};


//bool StartWith(char * FindStr, char * SearchStr)
//{
//	char* temp = strstr(FindStr, SearchStr);
//	if (temp == FindStr)
//		return true;
//	return false;
//}


#define MAX_LOADSTRING 100

// ���� ����:
HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.
WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
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

    // TODO: ���⿡ �ڵ带 �Է��մϴ�.

    // ���� ���ڿ��� �ʱ�ȭ�մϴ�.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_D3DX_PROJECT_SERVER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_D3DX_PROJECT_SERVER));

    MSG msg;

    // �⺻ �޽��� �����Դϴ�.
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
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
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
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

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
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����:  �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND  - ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT    - �� â�� �׸��ϴ�.
//  WM_DESTROY  - ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
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
//	static vector<string> userMsgs;		//���� ���� �޽���

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
		// addr.sin_addr.s_addr = inet_addr("165.246.163.66");	// ��ȣ��
		addr.sin_addr.s_addr = inet_addr("165.246.163.71");		// ������
		// addr.sin_addr.s_addr = inet_addr("192.168.0.7");		// ������ (��)


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

				// ���� : send(cs[0], (char*)&GameMessage, sizeof(GameMessage) + 1, 0);
				if (send(user.at(i).s, (char*)&PrepareMsg, sizeof(CharacterStatus_PC) + 1, 0) != -1)
				{
					user.at(i).FailCnt = 0;		// ���� �����ϸ� ���� ī��Ʈ�� �ʱ�ȭ �Ѵ�.
				}
				else
				{
					user.at(i).FailCnt += 1;		// ���� ���� ī��Ʈ�� �ø���.
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
            // TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�.
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

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
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
