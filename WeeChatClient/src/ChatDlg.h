#pragma once
#define WIN32_LEAN_AND_MEAN
#include "..\rsc\resource.h"
#include <Windows.h>
#include <windowsX.h>
#include <WinSock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

#include <string>
#include <thread>
#include "..\..\shared_src\Socket.h"

class ChatDlg
{
	HWND hWnd;
	static ChatDlg* ptr;
	Socket socket;

	void Cls_OnClose(HWND hwnd);
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);

	void Connect(const char* ip, USHORT port);
public:
	ChatDlg();
	~ChatDlg();
	static INT_PTR CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
};
