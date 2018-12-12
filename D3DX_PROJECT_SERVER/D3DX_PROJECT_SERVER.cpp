
#include "stdafx.h"
#include "D3DX_PROJECT_SERVER.h"
#include <list>
#include <queue>
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") 
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
static vector<CharacterStatus_PC*> g_vUsers;
CRITICAL_SECTION	crit;
CRITICAL_SECTION	messageGet;

HANDLE	g_thrThreads[MAX_USERS];
int	g_isAliveThread[MAX_USERS] = { 0 };
HWND hWnd;
bool isSent = false;
vector<string> ServerStatus;
queue<string> messageQueue;
int triggerBoxNum = 0;
void monsterSet(int i);
vector<dogMonster> dogVec;

bool StartWith(char * FindStr, char * SearchStr)
{
	char* temp = strstr(FindStr, SearchStr);
	if (temp == FindStr)
		return true;
	return false;
}


// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
void				threadProcessRecv(void* str);
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
	if (!InitInstance(hInstance, nCmdShow))
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

	return (int)msg.wParam;
}




ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_D3DX_PROJECT_SERVER));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
	//char buffer[64];
	int srvSize, clnSize = sizeof(c_sockInfo);
	static int playerCnt;
	static string errmsg;
	//	static vector<string> userMsgs;		//���� ���� �޽���

	static int userNum = 0;
	static char* ping;
	//ZeroMemory(buffer, sizeof(CharacterStatus_PC) + 1);

	switch (message)
	{
	case WM_CREATE:
	{
		HANDLE hThread = new HANDLE;
		hThread = (HANDLE)_beginthreadex(NULL, 0, (unsigned int(__stdcall*)(void *))threadProcessRecv, NULL, 0, NULL);

		MoveWindow(hWnd,
			GetSystemMetrics(SM_CXSCREEN) - 420, GetSystemMetrics(SM_CYSCREEN) - 230,
			360, 220,
			TRUE);
		ServerStatus.push_back("0 Players Online");
		InitializeCriticalSection(&crit);
		InitializeCriticalSection(&messageGet);
		SetTimer(hWnd, 123, 500,NULL);

		WSAStartup(MAKEWORD(2, 2), &wsadata);
		ServerSocket = socket(AF_INET, SOCK_STREAM, 0);
		sockInfo.sin_family = AF_INET;
		sockInfo.sin_port = 20;
		sockInfo.sin_addr.S_un.S_addr = inet_addr(SERVER_ADDR);

		//
		string strping = "ping";
		ping = new char[strping.size() + 1];
		copy(strping.begin(), strping.end(), ping);
		ping[strping.size()] = '\0';

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
			CharacterStatus_PC* TmpUser = new CharacterStatus_PC;

			g_vUsers.push_back(TmpUser);
			g_vUsers.back()->s = accept(ServerSocket, (LPSOCKADDR)&c_sockInfo, &clnSize);
			WSAAsyncSelect(g_vUsers.back()->s, hWnd, WM_ASYNC, FD_READ | FD_CLOSE);
			g_vUsers.back()->ID = playerCnt++;
			g_vUsers.back()->FailCnt = 0;



			string tmp;
			char* givenMessage;

			tmp += "SetID";
			tmp += ' ';
			tmp += to_string(g_vUsers.back()->ID);

			givenMessage = new char[tmp.size() + 1];
			std::copy(tmp.begin(), tmp.end(), givenMessage);
			givenMessage[tmp.size()] = '\0';

			send(g_vUsers.back()->s, givenMessage, tmp.size() + 1, 0);

		}
		break;
		case FD_READ:
		{
			int idx = -1;

			for (int i = 0; i < g_vUsers.size(); i++)
			{
				if (g_vUsers[i]->s == wParam)
				{
					idx = i;
					break;
				}
			}

			if (idx == -1)
				break;

			//
			// initiate buffer
			char buffer[128];
			//
			//recieve data
			int bufferLen = recv(g_vUsers[idx]->s, buffer, 128, 0);
			buffer[bufferLen-1] = '\0';		// ensure data's end

			string tmp = string(buffer);

			EnterCriticalSection(&crit);
			messageQueue.push(tmp);
			LeaveCriticalSection(&crit);

		}
		break;
		case FD_CLOSE:
		{
			int idx = -1;

			for (int i = 0; i < g_vUsers.size(); i++)
			{
				if (strcmp(g_vUsers[i]->MsgHeader, "disconnect") == 0)
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
				errmsg.append(" is Unknown.");

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
				closesocket((g_vUsers[idx]->s));
			}
		}
		break;
		}
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		/*	for (int i = 0; i < g_vUsers.size()+1; i++)
		{
		TextOut(hdc, 10, 10 + 20 * (i), ServerStatus[i].c_str(), strlen(ServerStatus[i].c_str()));
		}
		TextOut(hdc, 10, 10 + 20 * (g_vUsers.size() + 1), errmsg.c_str(), strlen(errmsg.c_str()));*/
		DeleteObject(hdc);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		for (int i = 0; i < g_vUsers.size(); i++)
		{
			EnterCriticalSection(&crit);
			g_isAliveThread[i] = -1;
			closesocket(g_vUsers[i]->s);
			LeaveCriticalSection(&crit);
		}
		WSACleanup();
		PostQuitMessage(0);
		break;
	case WM_TIMER:

	/*	EnterCriticalSection(&crit);
		messageQueue.push("ping");
		LeaveCriticalSection(&crit);
*/

		//for (int i = 0; i < g_vUsers.size(); i++)
		//{
		//	//int result = send(g_vUsers.at(i)->s, ping, 5, 0);
		//	int result = 0;
		//	if (result < 0)
		//	{
		//		g_vUsers.at(i)->FailCnt++;

		//		if (g_vUsers.at(i)->FailCnt > 20)
		//		{
		//			string disconnect;
		//			disconnect += "disconnect";
		//			disconnect += ' ';
		//			disconnect += to_string(i);

		//			char * sendMessage = new char[disconnect.size() + 1];
		//			copy(disconnect.begin(), disconnect.end(), sendMessage);
		//			sendMessage[disconnect.size()] = '\0';

		//			for (int j = 0; j < g_vUsers.size(); j++)
		//			{
		//				send(g_vUsers.at(j)->s, sendMessage, disconnect.size() + 1, 0);
		//			}
		//			closesocket(g_vUsers.at(i)->s);
		//			if(g_vUsers.at(i))
		//				delete(g_vUsers.at(i));
		//			g_vUsers.erase(g_vUsers.begin() + i);

		//			printf("%d user disconnected", i);
		//		}
		//	}
		//	else
		//	{
		//		g_vUsers.at(i)->FailCnt = 0;
		//	}
		//}

		


		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


void threadProcessRecv(void * str)
{
	int isReady = 0;
	while (1)
	{
		Sleep(1);
		string tmp;
		char* givenMessage;
		static int sleepCount = 0;
		if (messageQueue.size() == 0)
		{
			sleepCount++;
			if (sleepCount > 500)
			{
				sleepCount = 0;
				messageQueue.push("ping");
			}

			continue;
		}

		EnterCriticalSection(&crit);
		tmp = messageQueue.front();
		messageQueue.pop();
		LeaveCriticalSection(&crit);
		

		givenMessage = new char[tmp.size() + 1];
		std::copy(tmp.begin(), tmp.end(), givenMessage);
		givenMessage[tmp.size()] = '\0';

		int result = 10;

		if (StartWith(givenMessage, "userStatus"))
		{
			int ID;
			D3DXVECTOR3 Pos;
			float Dir;
			int Status;
			int CurHP;

			sscanf_s(givenMessage, "%*s %d %f %f %f %f %d %d", &ID, &Pos.x, &Pos.y, &Pos.z, &Dir, &Status, &CurHP);

			for (int i = 0; i < g_vUsers.size(); i++)
			{
				if (g_vUsers.at(i)->ID == ID)
				{
					g_vUsers.at(i)->CurPos = Pos;
					g_vUsers.at(i)->Dir = Dir;
					g_vUsers.at(i)->Status = Status;
					g_vUsers.at(i)->CurHP = CurHP;
				}
			}
		}
		else if (StartWith(givenMessage, "shot"))
		{
			int ID;
			int monsterNum;
			int damage;

			sscanf_s(givenMessage, "%*s %d %d %d", &ID, &damage, &ID);

		}
		else if (StartWith(givenMessage, "select"))
		{
			int ID;
			int character;

			sscanf_s(givenMessage, "%*s %d %d", &ID, &character);
			for (int i = 0; i < g_vUsers.size(); i++)
			{
				if (g_vUsers.at(i)->ID == ID)
				{
					g_vUsers.at(i)->Character_No = character;
				}
			}

		}
		else if (StartWith(givenMessage, "myNameIs"))
		{
			int ID;
			char name[16];

			sscanf_s(givenMessage, "%*s %d %s", &ID, &name, 16);

			for (int i = 0; i < g_vUsers.size(); i++)
			{
				if (g_vUsers.at(i)->ID == ID)
					strcpy(g_vUsers.at(i)->PlayerName, name);
			}

			for (int i = 0; i < g_vUsers.size(); i++)
			{
				string tmp;
				char* givenMessage;

				tmp += "myNameIs";
				tmp += ' ';
				tmp += to_string(g_vUsers.at(i)->ID);
				tmp += ' ';
				tmp += string(g_vUsers.at(i)->PlayerName);

				givenMessage = new char[tmp.size() + 1];
				std::copy(tmp.begin(), tmp.end(), givenMessage);
				givenMessage[tmp.size()] = '\0';

				result = send(g_vUsers.back()->s, givenMessage, tmp.size() + 1, 0);
			}
		}
		else if (StartWith(givenMessage, "IsReady"))
		{
			int ID;

			sscanf_s(givenMessage, "%*s %d %d", &ID);
			isReady++;

			if (isReady != g_vUsers.size())
			{
				continue;
			}
		}
		else if (StartWith(givenMessage, "triggerBoxNum"))
		{
			static int tmpBoxNum = -1;
			sscanf_s(givenMessage, "%*s %d %d", &tmpBoxNum);
			if (tmpBoxNum > triggerBoxNum)
			{
				triggerBoxNum = tmpBoxNum;
				HANDLE hThread = new HANDLE;
				hThread = (HANDLE)_beginthreadex(NULL, 0, (unsigned int(__stdcall*)(void *))monsterSet, &tmpBoxNum, 0, NULL);
				
			}
		}
		for (int i = 0; i < g_vUsers.size(); i++)
		{
			result = send(g_vUsers.at(i)->s, givenMessage, 128, 0);
			if (result < 0)
				g_vUsers.at(i)->FailCnt++;
			else
				g_vUsers.at(i)->FailCnt = 0;

			if (g_vUsers.at(i)->FailCnt > 30)
			{
				string disconnect;
				disconnect += "disconnect";
				disconnect += ' ';
				disconnect += to_string(i);
				messageQueue.push(disconnect);

				closesocket(g_vUsers.at(i)->s);
				if (g_vUsers.at(i))
					delete(g_vUsers.at(i));
				g_vUsers.erase(g_vUsers.begin() + i);
			}
		}

		if (!StartWith(givenMessage, "ping"))
			printf("%s\n", givenMessage);



	}
}




void ThreadSend()
{
	EnterCriticalSection(&crit);
	for (int n = 0; n < g_vUsers.size(); n++)
	{
		int result = send(g_vUsers[n]->s, (char*)&g_vUsers[n], sizeof(CharacterStatus_PC) + 1, 0);

		if (result != -1 && result != 0)
		{
			g_vUsers[n]->FailCnt = 0;
		}
		else
		{
			g_vUsers[n]->FailCnt += 1;
		}

		if (g_vUsers[n]->FailCnt > TRYOUT_CNT)
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


void monsterSet(int i)
{
	static int MonID = 0;
	if (i == 0)
	{
		
		int m_SpawnXPos = 200;
		int m_SpawnYPos = 5;
		int m_SpawnZPos = -395;
		for (int j = 0; j < 2; j++)
		{
			for (int i = 0; i < 5; i++)
			{
				string tmpMessage;
				dogMonster tmpDog;
				tmpDog.monsterNum = MonID;
				tmpDog.maxHealth = 200;
				tmpDog.nowHealth = 200;


				tmpMessage += "setMonster";
				tmpMessage += " ";
				tmpMessage += to_string(MonID);
				tmpMessage += " ";
				tmpMessage += to_string(g_vUsers.at(rand() % g_vUsers.size())->ID);
				tmpMessage += m_SpawnXPos;
				tmpMessage += " ";
				tmpMessage += m_SpawnYPos;
				tmpMessage += " ";
				tmpMessage += m_SpawnZPos + i * 10;

				messageQueue.push(tmpMessage);

				MonID++;
			}
			Sleep(5000);
		}
	}
}