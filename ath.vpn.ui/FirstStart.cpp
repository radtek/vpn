// FirstStart.cpp : implementation file
//

#include "stdafx.h"
#include "ath.vpn.ui.h"
#include "FirstStart.h"
#include "afxdialogex.h"


// FirstStart dialog

IMPLEMENT_DYNAMIC(FirstStart, CDialog)

FirstStart::FirstStart(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_ATHProgress, pParent)
{

}

FirstStart::~FirstStart()
{
}

void FirstStart::DoDataExchange(CDataExchange* pDX)
{
	SetTimer(503, 1000, NULL);
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, ProgressBar);
}


BEGIN_MESSAGE_MAP(FirstStart, CDialog)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// FirstStart message handlers


void FirstStart::OnTimer(UINT_PTR nIDEvent)
{
	ProgressBar.SetPos(ProgressBar.GetPos() + 1);

	CDialog::OnTimer(nIDEvent);
}
