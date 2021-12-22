#include "LoginDlg.h"
using namespace std;

LoginDlg* LoginDlg::ptr = NULL;

LoginDlg::LoginDlg(void)
{
	ptr = this;
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR)
	{
		ShowError();
		WSACleanup();
		exit(10);
	}
}

LoginDlg::~LoginDlg(void) {}

void LoginDlg::Cls_OnClose(HWND hwnd)
{

}

BOOL LoginDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	hWnd = hwnd;
	return TRUE;
}

void LoginDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch (id)
	{
		break;
	}
}

BOOL CALLBACK LoginDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
	}
	return FALSE;
}