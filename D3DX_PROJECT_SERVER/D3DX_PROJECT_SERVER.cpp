// D3DX_PROJECT_SERVER.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "D3DX_PROJECT_SERVER.h"

// Server Addresses
// should activated just one.
//#define SERVER_ADDR "165.246.163.66"	// 고은호.
#define SERVER_ADDR "165.246.163.71"	// 심현상.
//#define SERVER_ADDR "192.168.0.9"		// 심현상(집/공유기1)
//#define SERVER_ADDR "192.168.0.7"		// 심현상(집/공유기2)


#define MAX_LOADSTRING 100
#define MAX_USERS 10

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
static vector<CharacterStatus_PC> g_vUsers;
CRITICAL_SECTION	crit;
HANDLE	g_thrThreads[MAX_USERS];
int	g_isAliveThread[MAX_USERS] = { 0 };

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
int					UpdateAndSend(void* idx);


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

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	static WSADATA wsadata;
	static SOCKET ServerSocket;
	static SOCKADDR_IN addr = { 0 }, c_addr;	
	char* buffer;
	int size;
	static int playerCnt;
//	static vector<string> userMsgs;		//유저 상태 메시지

	CharacterStatus_PC TmpUser;

	static int userNum;
	buffer = (char*)malloc(sizeof(CharacterStatus_NPC) + 1);
	ZeroMemory(buffer, 0, sizeof(CharacterStatus_PC) + 1);

    switch (message)
    {
	case WM_CREATE:
		InitializeCriticalSection(&crit);
		SetTimer(hWnd, 123, 25, NULL);		
		playerCnt = 0;
		WSAStartup(MAKEWORD(2, 2), &wsadata);
		ServerSocket = socket(AF_INET, SOCK_STREAM, 0);
		addr.sin_family = AF_INET;
		addr.sin_port = 20;
		addr.sin_addr.S_un.S_addr = inet_addr(SERVER_ADDR);

		// Network Bind Test
		if (bind(ServerSocket, (LPSOCKADDR)&addr, sizeof(addr)))
		{
			MessageBox(NULL, _T("Binding Failed!"), _T("Error"), MB_OK);
			return 0;
		}

		WSAAsyncSelect(ServerSocket, hWnd, WM_ASYNC, FD_ACCEPT);
		
		//
		// Check Listening
		if (listen(ServerSocket, 5) == SOCKET_ERROR)
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
				HANDLE hThread;
				size = sizeof(c_addr);
				g_vUsers.push_back(TmpUser);
				g_vUsers.back().s = accept(ServerSocket, (LPSOCKADDR)&c_addr, &size);
				g_vUsers.back().ID = playerCnt;
				WSAAsyncSelect(g_vUsers.back().s, hWnd, WM_ASYNC, FD_READ);

				playerCnt++;

				strcpy(g_vUsers.back().MsgHeader, "welcome");
				send(g_vUsers.back().s, (char*)&g_vUsers.back(), sizeof(CharacterStatus_PC) + 1, 0);
				strcpy(g_vUsers.back().MsgHeader, "TitleScene");

				g_isAliveThread[(int)g_vUsers.back().ID] = 0;	// 스레드 통싱 실패 횟수를 초기화 한다.
				hThread = (HANDLE)_beginthreadex(NULL, 0, (unsigned int(__stdcall*)(void *))UpdateAndSend, (void *)g_vUsers.back().ID, 0, NULL);
				g_thrThreads[(int)g_vUsers.back().ID] = hThread;

				CloseHandle(hThread);
			}
			break;
//		case FD_READ:			
//		{					
//			for (int i = 0; i < user.size(); i++)
//			{
//				buffer[sizeof(CharacterStatus_PC)] = NULL;
//				int bufferLen = recv(user[i].s, buffer, sizeof(CharacterStatus_PC) + 1, 0);
//				buffer[bufferLen] = NULL;
//				CharacterStatus_PC *recieved = (CharacterStatus_PC*)&buffer;
//
//			//	if (user[i].s == recieved->s)
//				{
//					if (strcmp(recieved->MsgHeader, "userData") == 0)
//					{
//						user[i].CurHP = recieved->CurHP;
//						user[i].MaxHP = recieved->MaxHP;
//						user[i].Dir = recieved->Dir;
//						user[i].Status = recieved->Status;
//
//						printf("recieved : user #%d's userData\n", user[i].ID);
//					}
//					else if (strcmp(recieved->MsgHeader, "join") == 0)
//					{
//						char tmp[16];
//						string s;
//						itoa(recieved->ID, tmp, 10);
//						s = "User # (";
//						s.append(tmp);
//						s.append(recieved->PlayerName);
//						s.append(") Joinned the Game");
//						MessageBox(NULL, s.c_str(), _T("New User!"), MB_OK);
//
//						user[i].CurHP = recieved->CurHP;
//						user[i].MaxHP = recieved->MaxHP;
//						user[i].Dir = recieved->Dir;
//						user[i].Status = recieved->Status;
//						strcpy_s(user[i].PlayerName, recieved->PlayerName);
//						printf("recieved : user #%d(%s) Has joined in the Game.\n", user[i].ID, user[i].PlayerName);
//					}
//					else if (strcmp(recieved->MsgHeader, "disconnect") == 0)
//					{
//						for (int i = 0; i < user.size(); i++)
//						{
//							if (user[i].ID == recieved->ID)
//							{
//								user.erase(user.begin() + i);
//								i--;
//								break;
//							}
//						}
//
//						InvalidateRgn(hWnd, NULL, TRUE);
//					}
//					else if (strcmp(recieved->MsgHeader, "TitleScene") == 0)
//					{
//						strcmp(user[i].MsgHeader, "TitleScene");
//					}
//				}
//			}
//		}
//			break;
//		case FD_CLOSE:
//			for (int i = 0; i < user.size(); i++)
//			{
//				if (user.at(i).s == (SOCKET)wParam)
//				{					
//					user.push_back(user[i]);
//					user[i] = user[user.size() - 2];
//					user.pop_back();
//
////					userMsgs[i].swap(userMsgs[user.size()-1]);
////					userMsgs.erase(userMsgs.end());
//				}
//			}
//			break;
		}
		break;
	case WM_TIMER:
		InvalidateRgn(hWnd, NULL, TRUE);
		break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다.
			if (g_vUsers.size() == 0)
				TextOut(hdc, 10, 10, "No Player is online", strlen("No Player is online"));
			else
			{
				TextOut(hdc, 10, 10, to_string(g_vUsers.size()).c_str(), strlen(to_string(g_vUsers.size()).c_str()));
				TextOut(hdc, 50, 10, "Player(s) online", strlen("Player(s) online"));
			}

			for (int i = 0; i < g_vUsers.size(); i++)
			{
				TextOut(hdc, 10, 30 + 20 * g_vUsers[i].ID, to_string((int)g_vUsers[i].FailCnt).c_str(), strlen(to_string((int)g_vUsers[i].FailCnt).c_str()));
				TextOut(hdc, 50, 30 + 20 * g_vUsers[i].ID, to_string(g_vUsers[i].ID).c_str(), strlen(to_string(g_vUsers[i].ID).c_str()));
				TextOut(hdc, 90, 30 + 20 * g_vUsers[i].ID, to_string(g_vUsers[i].s).c_str(), strlen(to_string(g_vUsers[i].s).c_str()));
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


int UpdateAndSend(void* idx)
{
	int i = (int)idx;
	int buffLen = 0;
	char* buffer;
	buffer = (char*)malloc(sizeof(CharacterStatus_PC) + 1);
	static int tryout = 0;
	bool isExit = false;

	buffLen = recv(g_vUsers[i].s, buffer, sizeof(CharacterStatus_PC) + 1, 0);
	while (buffLen && !isExit)
	{
		CharacterStatus_PC* userData = (CharacterStatus_PC*)&buffer;

		EnterCriticalSection(&crit);		

		g_vUsers[i] = *userData;
		
		int result;
		for (int n = 0; n < g_vUsers.size(); n++)
		{			
				result = send(g_vUsers[n].s, (char*)userData, sizeof(CharacterStatus_PC) + 1, 0);

				if (result != -1)
					g_vUsers[n].FailCnt = 0;
				else
					g_vUsers[n].FailCnt += 1;

				if (g_isAliveThread[n] > TRYOUT_CNT)
					g_isAliveThread[n] = false;			
		}

		LeaveCriticalSection(&crit);

		if (g_isAliveThread[i] == -1)
			isExit = true;
				
	}

	return 0;
}
