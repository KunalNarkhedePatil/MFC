// ShowServicesDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CShowServicesDlg dialog
class CShowServicesDlg : public CDialog
{
// Construction
public:
	CShowServicesDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SHOWSERVICES_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listServices;
	void RefreshServiceList();
	CButton m_btnStart;
	CButton m_btnStop;
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnStnClickedStaticAllServices();
	CStatic m_staticTotalSer;
	CStatic m_staticRunningSer;
	CStatic m_WinSerExplorer;
};
