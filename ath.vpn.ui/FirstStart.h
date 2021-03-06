#pragma once


// FirstStart dialog

class FirstStart : public CDialog
{
	DECLARE_DYNAMIC(FirstStart)

public:
	FirstStart(CWnd* pParent = nullptr);   // standard constructor
	virtual ~FirstStart();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ATHProgress };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CStatic s_status;
	CProgressCtrl ProgressBar;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
