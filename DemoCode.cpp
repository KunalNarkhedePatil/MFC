// DwonloadMonitorDlg.h : header file
//

#pragma once
#include "afxwin.h"


// CDwonloadMonitorDlg dialog
class CDwonloadMonitorDlg : public CDialog
{
// Construction
public:
	CDwonloadMonitorDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_DWONLOADMONITOR_DIALOG };

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
	CButton m_AddExtensionBtn;
	CListBox m_listBoxExtension;
	CButton m_saveBtn;
	afx_msg void OnBnClickedButtonAddExtension();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnListBoxRightClick(CWnd* pWnd, CPoint point);
   


	void SetIntoIni();
	void ParseFromIniAndSaveIntoListBox();
	bool IsExtensionExists(const CString& extension);
	CEdit m_editExtension;
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnMenuDelete();
	afx_msg void OnMenuDeleteall();
};


---------------------------------------------


// DwonloadMonitorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DwonloadMonitor.h"
#include "DwonloadMonitorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

TCHAR iniPath[MAX_PATH]=TEXT("C:\\Users\\User\\Desktop\\sample.ini");
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	
	afx_msg void OnMenuDelete();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)

END_MESSAGE_MAP()


// CDwonloadMonitorDlg dialog




CDwonloadMonitorDlg::CDwonloadMonitorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDwonloadMonitorDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDwonloadMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_ADD_EXTENSION, m_AddExtensionBtn);
	DDX_Control(pDX, IDC_LIST_EXTENSION, m_listBoxExtension);
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_saveBtn);
	DDX_Control(pDX, IDC_EDIT_EXTENSION, m_editExtension);
}

BEGIN_MESSAGE_MAP(CDwonloadMonitorDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_ADD_EXTENSION, &CDwonloadMonitorDlg::OnBnClickedButtonAddExtension)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CDwonloadMonitorDlg::OnBnClickedButtonSave)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_MENU_DELETE, &CDwonloadMonitorDlg::OnMenuDelete)
	ON_COMMAND(ID_MENU_DELETEALL, &CDwonloadMonitorDlg::OnMenuDeleteall)
END_MESSAGE_MAP()


// CDwonloadMonitorDlg message handlers

BOOL CDwonloadMonitorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
    ParseFromIniAndSaveIntoListBox();
	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDwonloadMonitorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDwonloadMonitorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDwonloadMonitorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


bool CDwonloadMonitorDlg::IsExtensionExists(const CString& extension)
{
    int count = m_listBoxExtension.GetCount();
    CString existingItem;

    for (int i = 0; i < count; ++i)
    {
        m_listBoxExtension.GetText(i, existingItem);
        if (existingItem.CompareNoCase(extension) == 0) // Case insensitive comparison
        {
            return true; // Extension found
        }
    }
    return false; // Extension not found
}

void CDwonloadMonitorDlg::OnBnClickedButtonAddExtension()
{
    CString strItem;
    m_editExtension.GetWindowText(strItem);

    if (!strItem.IsEmpty())
    {
        strItem.MakeUpper(); // Convert to uppercase

        if (IsExtensionExists(strItem))
        {
            AfxMessageBox(_T("This extension already exists in the list!"));
            return;
        }

        // If not found, add to the list box
        m_listBoxExtension.AddString(strItem);
        m_editExtension.SetWindowText(_T("")); // Clear input field
    }
}

void CDwonloadMonitorDlg::SetIntoIni()
{
    CString strExtensions;
    CString strItem;
    int count = m_listBoxExtension.GetCount();

    for (int i = 0; i < count; ++i)
    {
        m_listBoxExtension.GetText(i, strItem);
        strExtensions += strItem + _T("|");
    }

    if (!strExtensions.IsEmpty() && strExtensions.GetLength() > 0)
        strExtensions.Delete(strExtensions.GetLength() - 1, 1); // Remove trailing '|'

    WritePrivateProfileString(TEXT("Demo"), TEXT("Extension"), strExtensions, iniPath);
}

void CDwonloadMonitorDlg::ParseFromIniAndSaveIntoListBox()
{
    TCHAR buffer[2048] = { 0 };  // Increased buffer size
    GetPrivateProfileString(TEXT("Demo"), TEXT("Extension"), _T(""), buffer, 2048, iniPath);
    
    CString strExtensions(buffer);
    m_listBoxExtension.ResetContent(); // Clear existing list

    int start = 0;
    CString token = strExtensions.Tokenize(_T("|"), start);
    while (!token.IsEmpty())
    {
        m_listBoxExtension.AddString(token);
        token = strExtensions.Tokenize(_T("|"), start);
    }
}

void CDwonloadMonitorDlg::OnBnClickedButtonSave()
{
    SetIntoIni();
}
void CDwonloadMonitorDlg::OnContextMenu(CWnd* pWnd, CPoint point)
{
    // Check if the right-clicked control is the ListBox
    if (pWnd == &m_listBoxExtension)
    {
        // Create a context menu
        CMenu menu;
        menu.CreatePopupMenu();
        
        // Add "Delete" and "Delete All" items to the context menu
        menu.AppendMenu(MF_STRING, ID_MENU_DELETE, _T("Delete"));
        menu.AppendMenu(MF_STRING, ID_MENU_DELETEALL, _T("Delete All"));

        // Track the popup menu at the right-click location (point)
        menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
    }
}
void CDwonloadMonitorDlg::OnMenuDelete()
{
    int selectedIndex = m_listBoxExtension.GetCurSel();  // Get the selected item index
    
    // If an item is selected
    if (selectedIndex != LB_ERR)
    {
        m_listBoxExtension.DeleteString(selectedIndex);  // Delete the selected item from the list box
    }
    else
    {
        AfxMessageBox(_T("No item selected for deletion!"));
    }
}


void CDwonloadMonitorDlg::OnMenuDeleteall()
{
	int itemCount = m_listBoxExtension.GetCount();  // Get the total number of items in the list box
    
    // If there are items in the list box
    if (itemCount > 0)
    {
        m_listBoxExtension.ResetContent();  // Remove all items from the list box
    }
    else
    {
        AfxMessageBox(_T("The list box is empty!"));
    }
}
