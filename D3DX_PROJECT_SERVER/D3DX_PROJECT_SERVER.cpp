
#include "stdafx.h"
#include "D3DX_PROJECT_SERVER.h"

// Server Addresses
// should activated just one.
//#define SERVER_ADDR "165.246.163.66"	// ����ȣ.
#define SERVER_ADDR "165.246.163.71"	// ������.
//#define SERVER_ADDR "192.168.0.9"		// ������(��/������1)
//#define SERVER_ADDR "192.168.0.7"		// ������(��/������2)


#define MAX_LOADSTRING 100
#define MAX_USERS 10

// ���� ����:
HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.
WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.
static vector<CharacterStatus_PC> g_vUsers;
CRITICAL_SECTION	crit;
HANDLE	g_thrThreads[MAX_USERS];
int	g_isAliveThread[MAX_USERS] = { 0 };
HWND hWnd;
bool isSent = false;


// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
int					ThreadRecieveAndUpdate(void* idx);
void CALLBACK		TimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime);
void				ThreadSend();



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

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.
   
   hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 600, 400, nullptr, nullptr, hInstance, nullptr);

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
	static SOCKADDR_IN sockInfo = { 0 };
	char* buffer;
	int size;
	static int playerCnt;
//	static vector<string> userMsgs;		//���� ���� �޽���

	static int userNum = 0;
	buffer = (char*)malloc(sizeof(CharacterStatus_NPC) + 1);
	ZeroMemory(buffer, sizeof(CharacterStatus_PC) + 1);

    switch (message)
    {
	case WM_CREATE:
	{
		MoveWindow(hWnd,
			GetSystemMetrics(SM_CXSCREEN) - 370, GetSystemMetrics(SM_CYSCREEN) - 230,
			300, 220,
			TRUE);

		InitializeCriticalSection(&crit);
		SetTimer(hWnd, 123, 25, (TIMERPROC)TimerProc);

		playerCnt = 0;
		WSAStartup(MAKEWORD(2, 2), &wsadata);
		ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (ServerSocket == INVALID_SOCKET)
		{
			MessageBox(NULL, _T("Socket Initialization Failed!! 퉷!"), _T("Fail!"), MB_OK);
		}
		else
		{
			MessageBoxA(NULL, "Socket Initialization Successful! 퉤엣!", "Success!", MB_OK);
		}
		sockInfo.sin_family = AF_INET;
		sockInfo.sin_port = htons(2);
		sockInfo.sin_addr.S_un.S_addr = htonl(INADDR_ANY);


		// Network Bind Test
		if (bind(ServerSocket, (LPSOCKADDR)&sockInfo, sizeof(sockInfo)))
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
	}
		break;
	case WM_ASYNC:
		switch (lParam)
		{
			case FD_ACCEPT:
			{
				/*HANDLE hThread = new HANDLE;
				size = sizeof(c_addr);
				CharacterStatus_PC TmpUser;


				g_vUsers.push_back(TmpUser);
				g_vUsers.back().s = accept(ServerSocket, (LPSOCKADDR)&c_addr, &size);
				g_vUsers.back().ID = playerCnt;
				g_vUsers.back().FailCnt = 0;
				WSAAsyncSelect(g_vUsers.back().s, hWnd, WM_ASYNC, FD_CLOSE | FD_WRITE);

				playerCnt++;

				strcpy(g_vUsers.back().MsgHeader, "welcome");
				send(g_vUsers.back().s, (char*)&g_vUsers.back(), sizeof(CharacterStatus_PC) + 1, 0);
				strcpy(g_vUsers.back().MsgHeader, "TitleScene");

				g_isAliveThread[(int)g_vUsers.back().ID] = 0;
				hThread = (HANDLE)_beginthreadex(NULL, 0, (unsigned int(__stdcall*)(void *))ThreadRecieveAndUpdate, (void *)g_vUsers.back().ID, 0, NULL);
				g_thrThreads[(int)g_vUsers.back().ID] = hThread;

				CloseHandle(hThread);*/


			}
			break;
			case FD_CLOSE:
			{
				int idx = -1;
				for (int i = 0; i < g_vUsers.size(); i++)
				{
					char* buffer;
					buffer = (char*)malloc(sizeof(CharacterStatus_PC) + 1);
					CharacterStatus_PC* userData = (CharacterStatus_PC*)&buffer;

					if (g_vUsers[i].s == userData->s)
					{					
						closesocket((g_vUsers[i].s));
						g_isAliveThread[i] = -1;
						break;
					}
				}
				
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
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
			char *tmp;
			tmp = (char*)calloc(32, sizeof(char));
			if (g_vUsers.size() == 0)
				TextOut(hdc, 10, 10, "No Player is online", strlen("No Player is online"));
			else
			{				
				TextOut(hdc, 10, 10, _itoa((g_vUsers.size()), tmp, 10), strlen(tmp));
				TextOut(hdc, 50, 10, "Player(s) online", strlen("Player(s) online"));
			}			
			DeleteObject(hdc);
			EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
		for (int i = 0; i < g_vUsers.size(); i++)
		{
			EnterCriticalSection(&crit);
			g_isAliveThread[i] = -1;
			closesocket(g_vUsers[i].s);
			LeaveCriticalSection(&crit);
		}
		WSACleanup();
		PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


int ThreadRecieveAndUpdate(void* idx)
{
	static int Myidx = (int)idx;

	EnterCriticalSection(&crit);
	

	LeaveCriticalSection(&crit);

	return 0;
}




void ThreadSend()
{
	EnterCriticalSection(&crit);		
	for (int n = 0; n < g_vUsers.size(); n++)
	{
		int result = send(g_vUsers[n].s, (char*)&g_vUsers[n], sizeof(CharacterStatus_PC) + 1, 0);

		if (result != -1 && result != 0)
		{
			g_vUsers[n].FailCnt = 0;
		}
		else
		{
			g_vUsers[n].FailCnt += 1;
		}

		if (g_vUsers[n].FailCnt > TRYOUT_CNT)
		{
			g_isAliveThread[n] = -1;
			break;
		}
	}
	LeaveCriticalSection(&crit);

}

void CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	InvalidateRgn(hwnd, NULL, TRUE);
}
