
#include "stdafx.h"
#include "D3DX_PROJECT_SERVER.h"
#include <list>

// Server Addresses
// should activated just one.
#define SERVER_ADDR "165.246.163.66"	// ����ȣ.
//#define SERVER_ADDR "165.246.163.71"	// Shim Hyunsang
//#define SERVER_ADDR "192.168.0.7"		// ������(��/������1)
//#define SERVER_ADDR "192.168.0.7"		// ������(��/������2)


#define MAX_LOADSTRING 100
#define MAX_USERS 10

// global variables
HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.
WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.
static vector<CharacterStatus_PC> g_vUsers;
CRITICAL_SECTION	crit;
HANDLE	g_thrThreads[MAX_USERS];
int	g_isAliveThread[MAX_USERS] = { 0 };
HWND hWnd;
bool isSent = false;
vector<string> ServerStatus;


// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
int					threadProcessRecv(void* idx);
void CALLBACK		TimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime);
void				ThreadSend();

#pragma commnet

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
	static SOCKET ServerSocket, ClientSocket;
	static SOCKADDR_IN sockInfo = { 0 }, c_sockInfo;
	char* buffer;
	int srvSize, clnSize = sizeof(c_sockInfo);
	static int playerCnt;
	static string errmsg;
//	static vector<string> userMsgs;		//���� ���� �޽���

	static int userNum = 0;
	buffer = (char*)malloc(sizeof(CharacterStatus_NPC) + 1);
	ZeroMemory(buffer, sizeof(CharacterStatus_PC) + 1);

    switch (message)
    {
	case WM_CREATE:
	{
		MoveWindow(hWnd,
			GetSystemMetrics(SM_CXSCREEN) - 420, GetSystemMetrics(SM_CYSCREEN) - 230,
			360, 220,
			TRUE);
		ServerStatus.push_back("0 Players Online");
		InitializeCriticalSection(&crit);
		SetTimer(hWnd, 123, 25, (TIMERPROC)TimerProc);

		WSAStartup(MAKEWORD(2, 2), &wsadata);
		ServerSocket = socket(AF_INET, SOCK_STREAM, 0);
		sockInfo.sin_family = AF_INET;
		sockInfo.sin_port = 20;
		sockInfo.sin_addr.S_un.S_addr = inet_addr(SERVER_ADDR);

		//
		// check Binding
		if (bind(ServerSocket, (LPSOCKADDR)&sockInfo, sizeof(sockInfo)))
		{
			MessageBox(NULL, _T("Binding Failed!"), _T("Error"), MB_OK);
			return 0;
		}

		WSAAsyncSelect(ServerSocket, hWnd, WM_ASYNC, FD_ACCEPT | FD_CLOSE);

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
				CharacterStatus_PC TmpUser;

				g_vUsers.push_back(TmpUser);
				g_vUsers.back().s = accept(ServerSocket, (LPSOCKADDR)&c_sockInfo, &clnSize);
				WSAAsyncSelect(g_vUsers.back().s, hWnd, WM_ASYNC, FD_READ | FD_CLOSE);
				g_vUsers.back().ID = playerCnt++;
				g_vUsers.back().FailCnt = 0;
				strcpy(g_vUsers.back().MsgHeader, "welcome");
				send(g_vUsers.back().s, (char*)&g_vUsers.back(), sizeof(CharacterStatus_PC) + 1, 0);

				{
					string tmpString;
					char tmpChar[32] = { 0 };

					tmpString.append("ID: ");
					tmpString.append(_itoa(g_vUsers.back().ID, tmpChar, 10));
					memset(tmpChar, 0, 32);
					tmpString.append(" Socket: ");
					tmpString.append(_itoa(g_vUsers.back().s, tmpChar, 10));
					memset(tmpChar, 0, 32);
					tmpString.append(" | Fails : ");
					tmpString.append(_itoa(g_vUsers.back().FailCnt, tmpChar, 10));
					tmpString.append(" | Action : ");
					tmpString.append(g_vUsers.back().MsgHeader);
					ServerStatus.push_back(tmpString);
				}

				{
					string tmpString;
					char tmpChar[32] = { 0 };

					ServerStatus[0].clear();
					tmpString.append(_itoa(playerCnt, tmpChar, 10));
					tmpString.append(" Players Online");
					ServerStatus[0] = tmpString;
				}
				
				/*ClientSocket = accept(ServerSocket, (SOCKADDR*)&c_sockInfo, &clnSize);
				if (ClientSocket == INVALID_SOCKET)
				{
					int errorNumber = WSAGetLastError();
					MessageBox(NULL, _T("Accept Error!"), _T("Error"), MB_OK);
				}

				CharacterStatus_PC tmpPlayer;
				tmpPlayer.s = ClientSocket;
				g_vUsers.push_back(tmpPlayer);
				WSAAsyncSelect(g_vUsers.back().s, hWnd, WM_ASYNC, FD_CLOSE | FD_WRITE);

				string tmpString;

				tmpString += "ID : " + tmpPlayer.ID;
				tmpString += " | SocketID : " + tmpPlayer.s;
				tmpString += " | FailCount : " + tmpPlayer.FailCnt;
				ServerStatus.push_back(tmpString);
				tmpString.clear();
				tmpString += playerCnt;
				ServerStatus[0].replace(0, 1, tmpString);*/
			}
			break;
			case FD_READ:
			{
				HANDLE hThread = new HANDLE;
				int idx = -1;

				for (int i = 0; i < g_vUsers.size(); i++)
				{
					if (g_vUsers[i].s == wParam)
					{
						idx = i;
						break;
					}
				}

				if (idx == -1)
					break;

				//
				// initiate buffer
				char* buffer;
				buffer = (char*)malloc(sizeof(CharacterStatus_PC) + 1);
				memset(buffer, 0, sizeof(CharacterStatus_PC) + 1);

				//
				//recieve data
				int bufferLen = recv(g_vUsers[idx].s, buffer, sizeof(CharacterStatus_PC) + 1, 0);
				buffer[bufferLen] = NULL;		// ensure data's end

				hThread = (HANDLE)_beginthreadex(NULL, 0, (unsigned int(__stdcall*)(void *))threadProcessRecv, (void *)buffer, 0, NULL);
				CloseHandle(hThread);
			}
				break;
			case FD_CLOSE:
			{	
				int idx = -1;

				for (int i = 0; i < g_vUsers.size(); i++)
				{
					if (strcmp(g_vUsers[i].MsgHeader, "disconnect") == 0)
					{
						idx = i;
						break;
					}
				}

				if (idx == -1)
				{					
					char tmp2[16];
					errmsg.append("Couldn't Find User. Sock id ");
					errmsg.append(_itoa(wParam, tmp2, 10));
					errmsg.append( " is Unknown.");

					MessageBox(hWnd, _T(errmsg.c_str()), _T("Error"), MB_OK);

					break;
				}
				else
				{

					playerCnt--;
					g_vUsers.erase(g_vUsers.begin() + idx);
					g_isAliveThread[idx] = -1;
					ServerStatus.erase(ServerStatus.begin() + idx);

					string tmpString;
					char tmpChar[32] = { 0 };
					ServerStatus[0].clear();
					tmpString.append(_itoa(g_vUsers.size(), tmpChar, 10));
					tmpString.append(" Players Online");
					ServerStatus[0] = tmpString;
					errmsg.clear();
					errmsg.append("Player #");
					errmsg.append(_itoa(idx, tmpChar, 10));
					errmsg.append(" Socket #");
					memset(tmpChar, 0, 32);
					errmsg.append(_itoa(wParam, tmpChar, 10));
					errmsg.append(" Has left the game!");
					closesocket((g_vUsers[idx].s));
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
			
			for (int i = 0; i < g_vUsers.size()+1; i++)
			{				
				TextOut(hdc, 10, 10 + 20 * (i), ServerStatus[i].c_str(), strlen(ServerStatus[i].c_str()));
			}
			TextOut(hdc, 10, 10 + 20 * (g_vUsers.size() + 1), errmsg.c_str(), strlen(errmsg.c_str()));
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


int threadProcessRecv(void* buffer)
{
	CharacterStatus_PC *tmp = (CharacterStatus_PC*)&buffer;
	int Myidx = tmp->ID;


	EnterCriticalSection(&crit);

	//
	// update Server Status
	string tmpString;
	char tmpChar[32] = { 0 };
	tmpString.append("ID: ");
	tmpString.append(_itoa(g_vUsers[Myidx].ID, tmpChar, 10));
	memset(tmpChar, 0, 32);
	tmpString.append(" Socket: ");
	tmpString.append(_itoa(g_vUsers[Myidx].s, tmpChar, 10));
	memset(tmpChar, 0, 32);
	tmpString.append(" | Fails : ");
	tmpString.append(_itoa(g_vUsers[Myidx].FailCnt, tmpChar, 10));
	tmpString.append(" | Action : ");
	tmpString.append(g_vUsers[Myidx].MsgHeader);
	ServerStatus[Myidx + 1] = tmpString;

	//
	// Update recieved Data 
	g_vUsers[Myidx] = *tmp;

	//
	// duplicate Current Users data to minimize criticalSection Time.
	

	LeaveCriticalSection(&crit);

	for (int i = 0; i < g_vUsers.size(); i++)
	{
		int res = send(g_vUsers.at(i).s, (char*)tmp, sizeof(CharacterStatus_PC) + 1, 0);
		if (res == -1)
		{
			int x = errno;
			EnterCriticalSection(&crit);
			ServerStatus[g_vUsers.at(i).ID + 1].append(" Send Failed! Socket ID : ");
			char tmpChar[32] = { 0 };
			ServerStatus[g_vUsers.at(i).ID+1].append(_itoa(x, tmpChar, 10));
			ServerStatus[Myidx + 1] = tmpString;
			g_vUsers.at(i).FailCnt++;

			if (g_vUsers.at(i).FailCnt > TRYOUT_CNT)
			{				
				closesocket((g_vUsers[i].s));
				g_vUsers.erase(g_vUsers.begin() + i);
				ServerStatus.erase(ServerStatus.begin() + i);

				string tmpString;
				char tmpChar[32] = { 0 };
				ServerStatus[0].clear();
				tmpString.append(_itoa(g_vUsers.size(), tmpChar, 10));
				tmpString.append(" Players Online");
				ServerStatus[0] = tmpString;
			}
		}
	}
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
