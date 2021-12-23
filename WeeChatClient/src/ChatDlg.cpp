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
	SetDlgItemText(hwnd, IDC_TEXT_NICKNAME, (L"Имя: " + name).c_str());
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
		WCHAR text[1024];
		GetDlgItemText(hwnd, IDC_INPUTBOX, text, 1024);
		if (*text) socket.Send(text);
		SetDlgItemText(hwnd, IDC_INPUTBOX, L" ");
		SetFocus(GetDlgItem(hwnd, IDC_INPUTBOX));
		break;
	}
}

void ChatDlg::Listen(std::thread firstConnection)
{
	if (firstConnection.joinable())
		firstConnection.join();
	
	std::wstringstream msg;
	msg << L"SET_DATA";
	msg << (wchar_t)*((wchar_t*)&color) << (wchar_t)*((wchar_t*)&color + 1);
	msg << name;
	socket.Send(msg.str().c_str(), msg.str().size());
	msg.clear();

	std::wstring message;
	while (true)
	{
		while (socket.Recv(message))
		{
			while (message.size())
			{
				USHORT size = message[0];
				std::wstring sub = message.substr(1, size);
				ULONG color = *(ULONG*)sub.c_str();
				CHARRANGE cr{ -1,-1 };
				SendDlgItemMessage(hWnd, IDC_CHATBOX, EM_EXSETSEL, 0, LPARAM(&cr));
				// coloring
				CHARFORMAT cf;
				cf.cbSize = sizeof(cf);
				cf.dwMask = CFM_COLOR;
				cf.crTextColor = color;
				cf.dwEffects = 0;
				SendDlgItemMessage(hWnd, IDC_CHATBOX, EM_SETCHARFORMAT, WPARAM(SCF_SELECTION), (LPARAM)&cf);
				// appending
				SendDlgItemMessage(hWnd, IDC_CHATBOX, EM_REPLACESEL, 0, LPARAM(sub.substr(2).c_str()));
				message = message.substr(size + 1);
			}
			SendDlgItemMessage(hWnd, IDC_CHATBOX, WM_VSCROLL, SB_BOTTOM, 0);
		}
		Connect();
	}
}

void ChatDlg::Connect()
{
	socket.CloseConnection();
	SetDlgItemText(hWnd, IDC_TEXT_CONNECTION, L"Статус: подключение к серверу...");
	while (!socket.Connect(ip.c_str(), port));
	SetDlgItemText(hWnd, IDC_TEXT_CONNECTION, L"Статус: подключено.");
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