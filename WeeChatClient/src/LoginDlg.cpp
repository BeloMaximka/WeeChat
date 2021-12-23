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
	HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
	SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
	SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);

	hWnd = hwnd;
	srand(time(0));
	for (int i = 0; i < 3; i++)
	{
		SendDlgItemMessage(hwnd, IDC_SLIDER1 + i, TBM_SETRANGE, TRUE, MAKELPARAM(0, 128));
		rgb[i] = rand() % 128;
		SendDlgItemMessage(hwnd, IDC_SLIDER1 + i, TBM_SETPOS, WPARAM(TRUE), LPARAM(rgb[i]));
	}
	result->color = RGB(rgb[0], rgb[1], rgb[2]);
	SendDlgItemMessage(hwnd, IDC_COLOR, PBM_SETBKCOLOR, 0, LPARAM(result->color));

	wifstream file("config.cfg");
	file.imbue(locale(locale(), new codecvt_utf8<wchar_t, 0x10ffff, consume_header>{}));
	wstring line;
	if (file.is_open())
	{
		file >> line;
		SetDlgItemText(hwnd, IDC_EDIT_LOGIN, line.c_str());
		file >> line;
		SetDlgItemText(hwnd, IDC_EDIT_IP, line.c_str());
		file.close();
	}
	return TRUE;
}

void LoginDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch (id)
	{
	case ID_BUTTON_OK:
	{
		WCHAR text[256];
		GetDlgItemText(hwnd, IDC_EDIT_LOGIN, text, 256);
		if (!wcslen(text))
		{
			MessageBox(NULL, L"¬ведите логин.", L"ќшибка", MB_OK | MB_ICONERROR);
			return;
		}
		result->name = text;
		GetDlgItemText(hwnd, IDC_EDIT_IP, text, 256);
		if (!wcslen(text))
		{
			MessageBox(NULL, L"¬ведите IP.", L"ќшибка", MB_OK | MB_ICONERROR);
			return;
		}
		wofstream file("config.cfg");
		file.imbue(locale(std::locale(), new codecvt_utf8<wchar_t, 0x10ffff, generate_header>{}));
		result->ip = text;
		if (file.is_open())
		{
			file << result->name << endl << result->ip;
			file.close();
		}
		result->success = true;
		EndDialog(hwnd, 0);
		break;
	}
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