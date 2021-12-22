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
	hWnd = hwnd;

	listenThread = std::thread(&ChatDlg::Listen, this, std::thread(&ChatDlg::Connect, this));
	return TRUE;
}

void ChatDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch (id)
	{

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
			socket.Send(message.c_str());
		Connect();
	}
}

void ChatDlg::Connect()
{
	socket.CloseConnection();
	SetDlgItemText(hWnd, IDC_TEXT_CONNECTION, L"����������� � �������...");
	while (!socket.Connect(ip.c_str(), port));
	SetDlgItemText(hWnd, IDC_TEXT_CONNECTION, L"����������.");
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