#define WIN32_LEAN_AND_MEAN
#include "ChatDlg.h"
#include <commctrl.h>
#pragma comment(lib,"comctl32")

void RichEditInit()
{
	INITCOMMONCONTROLSEX initCmnCtrls;
	initCmnCtrls.dwSize = sizeof(initCmnCtrls);
	initCmnCtrls.dwICC = 0x00004000;
	InitCommonControlsEx(&initCmnCtrls);
	HMODULE hModule = LoadLibraryA("RichEd20.dll");
	if (hModule == NULL)
	{
		MessageBox(NULL, L"Не найден модуль RichEd20.dll", L"Ошибка", MB_OK | MB_ICONERROR);
		exit(-1);
	}
}

void WSAInit()
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR)
	{
		MessageBox(NULL, L"Ошибка инициализации WSAStartup", L"Ошибка", MB_OK | MB_ICONERROR);
		WSACleanup();
		exit(10);
	}
}

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrev, LPTSTR lpszCmdLine, int nCmdShow)
{

	RichEditInit();
	WSAInit();
	ChatDlg chat;
	DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_CHAT), NULL, ChatDlg::DlgProc);
	return 0;
}