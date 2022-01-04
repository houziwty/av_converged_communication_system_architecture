
// dvs_host_demoDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "dvs_host_demo.h"
#include "dvs_host_demoDlg.h"
#include "afxdialogex.h"

#include <string>
#include "boost/format.hpp"
#include "utils/url/url.h"
using namespace framework::utils::url;
#include "error_code.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CdvshostdemoDlg dialog



CdvshostdemoDlg::CdvshostdemoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DVS_HOST_DEMO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CdvshostdemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CdvshostdemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_XMQ_CONNECT, &CdvshostdemoDlg::OnBnClickedXmqConnect)
	ON_BN_CLICKED(IDC_XMQ_DISCONNECT, &CdvshostdemoDlg::OnBnClickedXmqDisconnect)
	ON_BN_CLICKED(IDC_DVS_LOGIN, &CdvshostdemoDlg::OnBnClickedDvsLogin)
	ON_BN_CLICKED(IDC_DVS_LOGOUT, &CdvshostdemoDlg::OnBnClickedDvsLogout)
END_MESSAGE_MAP()


// CdvshostdemoDlg message handlers

BOOL CdvshostdemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
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

	// TODO: Add extra initialization here

	SetDlgItemText(IDC_XMQ_ADDRESS, L"127.0.0.1");
	SetDlgItemText(IDC_XMQ_PORT, L"50531");
	SetDlgItemText(IDC_DEMO_NAME, L"test_demo_name");

	SetDlgItemText(IDC_DVS_ADDRESS, L"192.168.2.225");
	SetDlgItemText(IDC_DVS_PORT, L"8000");
	SetDlgItemText(IDC_DVS_USER, L"admin");
	SetDlgItemText(IDC_DVS_PASSWORD, L"Vrc123456");

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CdvshostdemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CdvshostdemoDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CdvshostdemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CdvshostdemoDlg::fetchXmqHostClientOnlineStatusNotification(bool online)
{
	if (online)
	{
		MessageBox(L"Online", L"Connection Status", MB_ICONINFORMATION | MB_OK);
	} 
	else
	{
		MessageBox(L"Offline", L"Connection Status", MB_ICONINFORMATION | MB_OK);
	}
}

void CdvshostdemoDlg::fetchXmqHostServiceCapabilitiesNotification(
	const ServiceInfo* infos /* = nullptr */, 
	const int number /* = 0 */)
{
// 	fileLog.write(
// 		SeverityLevel::SEVERITY_LEVEL_INFO, 
// 		"Fetch xmq host service capabilities size = [ %d ].", 
// 		number);

	for (int i = 0; i != number; ++i)
	{
// 		fileLog.write(
// 			SeverityLevel::SEVERITY_LEVEL_INFO, 
// 			" ****** Service name = [ %s ].", 
// 			infos[i]);

		const std::string name{ infos[i].name };
		if (!name.compare("dvs_host_server"))
		{
			const std::string url{
				"dvs://dvs_host_service?from=test_dvs_host_client&command=query" };
			int ret{ send(url.c_str(), url.length()) };

			if (Error_Code_Success == ret)
			{
//				fileLog.write(SeverityLevel::SEVERITY_LEVEL_INFO, "Send query device information to dvs host service successed.");
			}
			else
			{
// 				fileLog.write(
// 					SeverityLevel::SEVERITY_LEVEL_ERROR,
// 					"Send query device information to dvs host service failed, result = [ %d ].",
// 					ret);
			}
		}
	}
}

void CdvshostdemoDlg::fetchXmqHostClientReceivedDataNotification(
	const void* data /* = nullptr */, 
	const int bytes /* = 0 */)
{
// 	fileLog.write(
// 		SeverityLevel::SEVERITY_LEVEL_INFO, 
// 		"Fetch test dvs host client received data = [ %s ].", 
// 		data);

	const std::string msg{ (const char*)data, (const unsigned int)bytes };
	Url url;
	int ret{ url.parse(msg) };

	if (Error_Code_Success == ret)
	{
		int dvsnum{ 0 };
		const std::vector<ParamItem> params{ url.getParameters() };

		for (int i = 0; i != params.size(); ++i)
		{
			if (!params[i].key.compare("dvs"))
			{
				++dvsnum;
				const std::string dvsid{ params[i].value.substr(0, params[i].value.find_first_of('_')) };
				const std::string url{
					(boost::format("dvs://dvs_host_service?from=test_dvs_host_client&command=remove&id=%s") % dvsid).str() };
				int ret{ send(url.c_str(), url.length()) };

				if (Error_Code_Success == ret)
				{
//					fileLog.write(SeverityLevel::SEVERITY_LEVEL_INFO, "Send remove device [ %s ] successed.", dvsid.c_str());
				}
				else
				{
// 					fileLog.write(
// 						SeverityLevel::SEVERITY_LEVEL_ERROR,
// 						"Send remove device [ %s ] failed, result = [ %d ].",
// 						dvsid.c_str(), ret);
				}
			}
		}

// 		if (!dvsnum)
// 		{
// 			const std::string url{
// 				"dvs://dvs_host_service?from=test_dvs_host_client&command=add&ip=192.168.2.225&port=8000&user=admin&passwd=Vrc123456&name=test" };
// 			int ret{ send(url.c_str(), url.length()) };
// 
// 			if (Error_Code_Success == ret)
// 			{
// //				fileLog.write(SeverityLevel::SEVERITY_LEVEL_INFO, "Send add device information to dvs host service successed.");
// 			}
// 			else
// 			{
// // 				fileLog.write(
// // 					SeverityLevel::SEVERITY_LEVEL_ERROR,
// // 					"Send add device information to dvs host service failed, result = [ %d ].",
// // 					ret);
// 			}
// 		}
	}
}


void CdvshostdemoDlg::OnBnClickedXmqConnect()
{
	// TODO: Add your control notification handler code here

	char name[256]{ 0 }, ip[256]{ 0 };
	unsigned short port{ 
		static_cast<unsigned short>(GetDlgItemInt(IDC_XMQ_PORT)) };

	HWND hwnd{ this->GetSafeHwnd() };
	GetDlgItemTextA(hwnd, IDC_DEMO_NAME, name, 256);
	GetDlgItemTextA(hwnd, IDC_XMQ_ADDRESS, ip, 256);

	int ret{ LibXmqHostClient::registerXmqHostClient(name, strlen(name), ip, port) };

	if (ret)
	{
		CString text;
		text.Format(L"registerXmqHostClient invoke failed = %d", ret);
		MessageBox(text, L"Connect", MB_ICONERROR | MB_OK);
	}
}


void CdvshostdemoDlg::OnBnClickedXmqDisconnect()
{
	// TODO: Add your control notification handler code here

	int ret{ LibXmqHostClient::unregisterXmqHostClient() };

	if (ret)
	{
		CString text;
		text.Format(L"unregisterXmqHostClient invoke failed = %d", ret);
		MessageBox(text, L"Disconnect", MB_ICONERROR | MB_OK);
	}
}


void CdvshostdemoDlg::OnBnClickedDvsLogin()
{
	// TODO: Add your control notification handler code here
}


void CdvshostdemoDlg::OnBnClickedDvsLogout()
{
	// TODO: Add your control notification handler code here
}
