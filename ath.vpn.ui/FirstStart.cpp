// FirstStart.cpp : implementation file
//

#include "stdafx.h"
#include "ath.vpn.ui.h"
#include "FirstStart.h"
#include "afxdialogex.h"
#include "VPNCOMMAND.h"


// FirstStart dialog

IMPLEMENT_DYNAMIC(FirstStart, CDialog)

FirstStart::FirstStart(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_ATHProgress, pParent)
{
	LPWSTR username = new WCHAR[300];
	DWORD dw = 300;
	GetUserName(username, &dw);
	if (StrCmpIW(L"azhokhov", username) == 0 || StrCmpIW(L"athuser", username) == 0 || StrCmpIW(L"developer", username) == 0) {

	}
	else {
		LPWSTR message = new WCHAR[3000];
		wsprintf(message, L"Пользователь %s не может запустить данную программу, Вам необходимо перезайти в компьютер под пользователем ATHUser. Хотите это сделать сейчас?\0", username);
		int Relogin = MessageBox(message, 0, MB_YESNO);
		if (Relogin == 6) {
			ExitWindows(0, 0);
		}
		ExitProcess(500);
	}
	HKEY rKey;
	LONG dwBufferSize(255);
	LPWSTR nResult = new WCHAR[255];
	RegOpenKey(HKEY_CURRENT_USER, L"SOFTWARE\\ATH", &rKey);
	RegQueryValue(rKey, L"InitAV", nResult, &dwBufferSize);
	if (wcscmp(nResult, L"1") == 0) {
		EndDialog(0);
	}

}

FirstStart::~FirstStart()
{
}

void FirstStart::DoDataExchange(CDataExchange* pDX)
{	
	ProgressBar.SetRange(0, 1000);
	SetTimer(IDC_PROGRESS1, 1000, NULL);
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, ProgressBar);
	//DDX_Control(pDX, IDC_STATIC1, s_status);
}


BEGIN_MESSAGE_MAP(FirstStart, CDialog)
	ON_WM_TIMER()
END_MESSAGE_MAP()


void FirstStart::OnTimer(UINT_PTR nIDEvent)
{
	if (IDC_PROGRESS1 == nIDEvent) {
		ProgressBar.SetPos(ProgressBar.GetPos() + 1);
		if (ProgressBar.GetPos() == 100)
			ProgressBar.SetPos(0);
		HANDLE hPipe = CreateFile(L"\\\\.\\pipe\\ath.vpn", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
		if (hPipe == INVALID_HANDLE_VALUE) {
			int err = GetLastError();
			LPTSTR  mess = new WCHAR[200];
			wsprintf(mess, L"Незапущенна служба ath.vpn.service0: %i", err);
			KillTimer(IDC_PROGRESS1);
			MessageBox(mess);
			ExitProcess(1);
		}
		VPNCOMMAND command;
		DWORD pid = GetCurrentProcessId();
		command.messsage = pid;
		command.command = CHECKAV;
		DWORD dwByte;
		if (!WriteFile(hPipe, (LPCVOID)&command, sizeof(command), &dwByte, NULL)) {
			int err = GetLastError();
			LPTSTR  mess = new WCHAR[50];
			wsprintf(mess, L"err: %i", err);
			MessageBox(mess);
			return;
		}
		ReadFile(hPipe, (LPVOID)&command, sizeof(command), &dwByte, NULL);
		if (command.command == AVOK) {
			HKEY rKey;
			LPCWSTR data = L"1";
			if (RegCreateKey(HKEY_CURRENT_USER, L"SOFTWARE\\ATH", &rKey) != ERROR_SUCCESS)
				ExitProcess(0);
			RegSetValue(rKey, L"InitAV", REG_SZ, data, sizeof(data));
			RegCloseKey(rKey);
			EndDialog(0);
		}

		CloseHandle(hPipe);
	}
	CDialog::OnTimer(nIDEvent);
}
