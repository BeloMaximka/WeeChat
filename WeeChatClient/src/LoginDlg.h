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
#include <commctrl.h>
#include <fstream>
#include <codecvt>


struct LoginResult
{
	bool success;
	std::wstring name;
	std::wstring ip;
	ULONG color;
};
class LoginDlg
{
	static LoginDlg* ptr;
	LoginResult* result;
	HWND hWnd;
	BYTE rgb[3];
	
	void Cls_OnClose(HWND hwnd);
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
	void Cls_OnHScroll(HWND hwnd, HWND hwndCtl, UINT code, int pos);
public:
	LoginDlg() = delete;
	LoginDlg(LoginResult* result);
	~LoginDlg();
	
	static INT_PTR CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
};
