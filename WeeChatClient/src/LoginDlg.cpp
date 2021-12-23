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
	srand(time(0));
	for (int i = 0; i < 3; i++)
	{
		SendDlgItemMessage(hwnd, IDC_SLIDER1 + i, TBM_SETRANGE, TRUE, MAKELPARAM(0, 255));
		rgb[i] = rand() % 256;
		SendDlgItemMessage(hwnd, IDC_SLIDER1 + i, TBM_SETPOS, WPARAM(TRUE), LPARAM(rgb[i]));
	}
	result->color = RGB(rgb[0], rgb[1], rgb[2]);
	SendDlgItemMessage(hwnd, IDC_COLOR, PBM_SETBKCOLOR, 0, LPARAM(result->color));
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
		result->success = true;
		EndDialog(hwnd, 0);
		break;
	case IDC_BUTTON_CANCEL:
		EndDialog(hwnd, 0);
		break;
	}
}

void LoginDlg::Cls_OnHScroll(HWND hwnd, HWND hwndCtl, UINT code, int pos)
{
	int nCurrrentPosition = SendMessage(hwndCtl, TBM_GETPOS, 0, 0);
	switch (GetDlgCtrlID(hwndCtl))
	{
	case IDC_SLIDER1:
		rgb[0] = nCurrrentPosition;
		break;
	case IDC_SLIDER2:
		rgb[1] = nCurrrentPosition;
		break;
	case IDC_SLIDER3:
		rgb[2] = nCurrrentPosition;
		break;
	}
	result->color = RGB(rgb[0], rgb[1], rgb[2]);
	SendDlgItemMessage(hwnd, IDC_COLOR, PBM_SETBKCOLOR, 0, LPARAM(result->color));
}

INT_PTR CALLBACK LoginDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
		HANDLE_MSG(hwnd, WM_HSCROLL, ptr->Cls_OnHScroll);
	}
	return FALSE;
}