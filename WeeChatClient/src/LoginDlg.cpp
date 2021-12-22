#include "LoginDlg.h"
using namespace std;

LoginDlg* LoginDlg::ptr = NULL;

LoginDlg::LoginDlg(LoginResult* result) : result(result)
{
	ptr = this;
}

LoginDlg::~LoginDlg(void) {}

void LoginDlg::Cls_OnClose(HWND hwnd) 
{
	EndDialog(hwnd, 0);
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
	case ID_BUTTON_OK:
		WCHAR text[256];
		GetDlgItemText(hwnd, IDC_EDIT_LOGIN, text, 256);
		if (!wcslen(text))
		{
			MessageBox(NULL, L"¬ведите логин.", L"ќшибка", MB_OK | MB_ICONERROR);
			return;
		}
		result->name = text;
		result->color = 532;
		result->success = true;
		EndDialog(hwnd, 0);
		break;
	case IDC_BUTTON_CANCEL:
		EndDialog(hwnd, 0);
		break;
	}
}

INT_PTR CALLBACK LoginDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
	}
	return FALSE;
}