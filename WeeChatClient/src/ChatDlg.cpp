#include "ChatDlg.h"
using namespace std;

ChatDlg* ChatDlg::ptr = NULL;

ChatDlg::ChatDlg()
{
	ptr = this;
}

ChatDlg::~ChatDlg(void) {}

void ChatDlg::Cls_OnClose(HWND hwnd)
{
	EndDialog(hwnd, 0);
}

BOOL ChatDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	hWnd = hwnd;
	return TRUE;
}

void ChatDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch (id)
	{

	}
}

void ChatDlg::Connect(const char* ip, USHORT port)
{
	socket.Connect(ip, port);
}

INT_PTR CALLBACK ChatDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
	}
	return FALSE;
}