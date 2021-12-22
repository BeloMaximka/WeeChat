#define WIN32_LEAN_AND_MEAN
#include "LoginDlg.h"
#include <commctrl.h>
#pragma comment(lib,"comctl32")

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrev, LPTSTR lpszCmdLine, int nCmdShow)
{
	LoginResult result{ false };
	LoginDlg dlg(&result);
	DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_LOGIN), NULL, LoginDlg::DlgProc);
	return 0;
}