// ShowServicesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ShowServices.h"
#include "ShowServicesDlg.h"
#include <Winsvc.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CShowServicesDlg dialog




CShowServicesDlg::CShowServicesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShowServicesDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CShowServicesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SERVICES, m_listServices);
	//DDX_Control(pDX, IDC_STATIC_ALL_SER, m_staticTotalServices);
	//DDX_Control(pDX, IDC_STATIC_RUNNING_SER, m_staticRunningServices);

	DDX_Control(pDX, IDC_STATIC_TOTAL_SERVICES, m_staticTotalSer);
	DDX_Control(pDX, IDC_STATIC_ALL_SERVICES, m_staticRunningSer);
	DDX_Control(pDX, IDC_STATIC_WINSERVICEEXPLORER, m_WinSerExplorer);
}

BEGIN_MESSAGE_MAP(CShowServicesDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_STATIC_ALL_SERVICES, &CShowServicesDlg::OnStnClickedStaticAllServices)
END_MESSAGE_MAP()


// CShowServicesDlg message handlers

BOOL CShowServicesDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CFont font;
    font.CreatePointFont(200, _T("Arial"));  // Font size = 200, Font family = Arial

    // Step 2: Apply the bold style to the font
    LOGFONT logFont;
    font.GetLogFont(&logFont);
    logFont.lfWeight = FW_BOLD;  // Set the font weight to bold

    // Step 3: Create the bold font
    CFont boldFont;
    boldFont.CreateFontIndirect(&logFont);

    // Step 4: Apply the bold font to the static control
    m_WinSerExplorer.SetFont(&boldFont);

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

	// Set the List Control to "Details" view
m_listServices.ModifyStyle(0, LVS_REPORT);

// Set extended styles for the list control
m_listServices.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

// Insert columns into the List Control
m_listServices.InsertColumn(0, _T("Service Name"), LVCFMT_LEFT, 150);
m_listServices.InsertColumn(1, _T("Display Name"), LVCFMT_LEFT, 180);
m_listServices.InsertColumn(2, _T("Description"), LVCFMT_LEFT, 200);
m_listServices.InsertColumn(3, _T("Startup Type"), LVCFMT_LEFT, 100);  // Insert the new "Startup Type" column at index 3
m_listServices.InsertColumn(4, _T("Path"), LVCFMT_LEFT, 300);
m_listServices.InsertColumn(5, _T("Status"), LVCFMT_LEFT, 150);

	RefreshServiceList();

	
	return TRUE;  // return TRUE  unless you set the focus to a control
}
void CShowServicesDlg::RefreshServiceList()
{
    m_listServices.DeleteAllItems();

    SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE);
    if (!hSCManager) return;

    DWORD bytesNeeded = 0, servicesReturned = 0, resumeHandle = 0;

    // First call to get the required buffer size
    if (!EnumServicesStatusEx(
        hSCManager,
        SC_ENUM_PROCESS_INFO,
        SERVICE_WIN32,
        SERVICE_STATE_ALL,
        NULL, // No buffer for the first call
        0,    // Size of the buffer
        &bytesNeeded,
        &servicesReturned,
        &resumeHandle,
        NULL))
    {
        DWORD dwError = GetLastError();
        if (dwError != ERROR_MORE_DATA) // Check if it's the expected error
        {
            CString errorMessage;
            errorMessage.Format(_T("EnumServicesStatusEx failed with error code: %lu"), dwError);
            AfxMessageBox(errorMessage);
            CloseServiceHandle(hSCManager);
            return;
        }
    }

    // Allocate the buffer based on bytesNeeded
    LPENUM_SERVICE_STATUS_PROCESS services = (LPENUM_SERVICE_STATUS_PROCESS)malloc(bytesNeeded);
    if (services == NULL)
    {
        AfxMessageBox(_T("Failed to allocate memory for services."));
        CloseServiceHandle(hSCManager);
        return;
    }

    // Second call to actually get the services
    if (!EnumServicesStatusEx(
        hSCManager,
        SC_ENUM_PROCESS_INFO,
        SERVICE_WIN32,
        SERVICE_STATE_ALL,
        (LPBYTE)services,
        bytesNeeded,
        &bytesNeeded,
        &servicesReturned,
        &resumeHandle,
        NULL))
    {
        DWORD dwError = GetLastError();
        CString errorMessage;
        errorMessage.Format(_T("EnumServicesStatusEx failed with error code: %lu"), dwError);
        AfxMessageBox(errorMessage);
        free(services);
        CloseServiceHandle(hSCManager);
        return;
    }

    // Process the services
    int totalServices = 0, runningServices = 0;
    for (DWORD i = 0; i < servicesReturned; i++)
    {
        const ENUM_SERVICE_STATUS_PROCESS& service = services[i];

        // Add service details to List Control
        int row = m_listServices.InsertItem(i, service.lpServiceName);
        m_listServices.SetItemText(row, 1, service.lpDisplayName);
        m_listServices.SetItemText(row, 5, service.ServiceStatusProcess.dwCurrentState == SERVICE_RUNNING ? _T("Running") : _T("Stopped"));

        // Get the service configuration to retrieve the executable path and description
        SC_HANDLE hService = OpenService(hSCManager, service.lpServiceName, SERVICE_QUERY_CONFIG);
        if (hService)
        {
            DWORD bytesNeededConfig;
            QueryServiceConfig(hService, NULL, 0, &bytesNeededConfig); // First call to get the required buffer size
            LPQUERY_SERVICE_CONFIG pConfig = (LPQUERY_SERVICE_CONFIG)malloc(bytesNeededConfig);
            if (pConfig && QueryServiceConfig(hService, pConfig, bytesNeededConfig, &bytesNeededConfig))
            {
                // Set the executable path in the list control
                m_listServices.SetItemText(row, 4, pConfig->lpBinaryPathName); // Column 3 is for the path

                // Set the description in the list control
                m_listServices.SetItemText(row, 2, pConfig->lpDisplayName); // Assuming column 2 is for the description (you may want to change this to the actual description if available)
            }
			CString startupType;
            switch (pConfig->dwStartType)
            {
                case SERVICE_AUTO_START: startupType = _T("Automatic"); break;
                case SERVICE_DEMAND_START: startupType = _T("Manual"); break;
                case SERVICE_DISABLED: startupType = _T("Disabled"); break;
                default: startupType = _T("Unknown"); break;
            }
            m_listServices.SetItemText(row, 3, startupType);
            if (pConfig)
                free(pConfig);
            CloseServiceHandle(hService);
        }

        if (service.ServiceStatusProcess.dwCurrentState == SERVICE_RUNNING)
            runningServices++;
        totalServices++;
    }

    // Update static text
    CString total, running;
    total.Format(_T("Total Services: %d"), totalServices);
    running.Format(_T("Running Services: %d"), runningServices);
    m_staticTotalSer.SetWindowText(total);
    m_staticRunningSer.SetWindowText(running);

    free(services);
    CloseServiceHandle(hSCManager);
}
void CShowServicesDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CShowServicesDlg::OnPaint()
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
HCURSOR CShowServicesDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CShowServicesDlg::OnStnClickedStaticAllServices()
{
	// TODO: Add your control notification handler code here
}
