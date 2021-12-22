#include "ChatDlg.h"
using namespace std;

ChatDlg* ChatDlg::ptr = NULL;

ChatDlg::ChatDlg()
{
	ptr = this;
}

ChatDlg::~ChatDlg(void) 
{
	if (listenThread.joinable())
		listenThread.detach();
}

void ChatDlg::Cls_OnClose(HWND hwnd)
{
	EndDialog(hwnd, 0);
}

BOOL ChatDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	LoginResult result{ false };
	LoginDlg dlg(&result);
	DialogBox(0, MAKEINTRESOURCE(IDD_DIALOG_LOGIN), NULL, LoginDlg::DlgProc);
	if (!result.success) EndDialog(hwnd, 0);
	name = result.name;
	color = result.color;
	hWnd = hwnd;

	listenThread = std::thread(&ChatDlg::Listen, this, std::thread(&ChatDlg::Connect, this));
	return TRUE;
}

void ChatDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch (id)
	{
	case IDC_BUTTON_SEND:
		std::wstringstream msg;
		msg << (wchar_t)color << (wchar_t)*((wchar_t*)&color + 1);
		WCHAR text[1024];
		GetDlgItemText(hwnd, IDC_INPUTBOX, text, 1024);
		if (!*text) return; // if first wchar == 0
		msg << name << L": " << text << L"\n";
		socket.Send(msg.str().c_str(), msg.str().size());
		break;
	}
}

void ChatDlg::Listen(std::thread firstConnection)
{
	if (firstConnection.joinable())
		firstConnection.join();
	std::wstring message;
	while (true)
	{
		while (socket.Recv(message))
		{
			ULONG color = *(ULONG*)message.c_str();
			CHARRANGE cr{ -1,-1 };
			SendDlgItemMessage(hWnd,IDC_CHATBOX, EM_EXSETSEL, 0, LPARAM(&cr));
			// coloring
			CHARFORMAT cf;
			cf.cbSize = sizeof(cf);
			cf.dwMask = CFM_COLOR;
			cf.crTextColor = color;
			cf.dwEffects = 0;
			SendDlgItemMessage(hWnd, IDC_CHATBOX, EM_SETCHARFORMAT, WPARAM(SCF_SELECTION), (LPARAM)&cf);
			// appending
			SendDlgItemMessage(hWnd,IDC_CHATBOX, EM_REPLACESEL, 0, LPARAM(message.substr(2).c_str()));
		}
		Connect();
	}
}

void ChatDlg::Connect()
{
	socket.CloseConnection();
	SetDlgItemText(hWnd, IDC_TEXT_CONNECTION, L"Подключение к серверу...");
	while (!socket.Connect(ip.c_str(), port));
	SetDlgItemText(hWnd, IDC_TEXT_CONNECTION, L"Подключено.");
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