
// dvs_host_demoDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "dvs_host_demo.h"
#include "dvs_host_demoDlg.h"
#include "afxdialogex.h"

#include <string>
#include "boost/format.hpp"
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
	: CDialogEx(IDD_DVS_HOST_DEMO_DIALOG, pParent), XMQNode(), ASIONode(), sid{0}
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
	ON_BN_CLICKED(IDC_REALPLAY_TEST, &CdvshostdemoDlg::OnBnClickedRealplayTest)
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
	SetDlgItemText(IDC_XMQ_PORT, L"60531");
	SetDlgItemText(IDC_DEMO_NAME, L"test_demo_name");

	SetDlgItemText(IDC_DVS_ADDRESS, L"192.168.2.225");
	SetDlgItemText(IDC_DVS_PORT, L"8000");
	SetDlgItemText(IDC_DVS_USER, L"admin");
	SetDlgItemText(IDC_DVS_PASSWORD, L"Vrc123456");

	ASIONode::run();

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

void CdvshostdemoDlg::afterFetchOnlineStatusNotification(const bool online /* = false */)
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

void CdvshostdemoDlg::afterFetchServiceCapabilitiesNotification(
	const ServiceInfo* infos /* = nullptr */, 
	const uint32_t number /* = 0 */)
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
			char tick[256]{ 0 };
			sprintf_s(tick, 128, "config://dvs_host_server?command=query&timestamp=%d", GetTickCount());
			const std::string url{tick};
			int ret{ XMQNode::send(0xFFFF, url.c_str(), url.length())};

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

void CdvshostdemoDlg::afterPolledDataNotification(
	const uint32_t id /* = 0 */, 
	const void* data /* = nullptr */, 
	const uint64_t bytes /* = 0 */, 
	const char* from /* = nullptr */)
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
		if (!url.getProtocol().compare("config"))
		{
			processDvsControlMessage(url);
		}
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
	XMQModeConf conf{ 0 };
	conf.id = 0xFFFF;
	memcpy_s(conf.name, 128, name, strlen(name));
	memcpy_s(conf.ip, 32, ip, strlen(ip));
	conf.port = port;
	conf.type = XMQModeType::XMQ_MODE_TYPE_DEALER;

	int ret{XMQNode::addConf(conf)};

	if (Error_Code_Success == ret)
	{
		ret = XMQNode::run();
// 		CString text;
// 		text.Format(L"registerXmqHostClient invoke failed = %d", ret);
// 		MessageBox(text, L"Connect", MB_ICONERROR | MB_OK);
	}
}


void CdvshostdemoDlg::OnBnClickedXmqDisconnect()
{
	// TODO: Add your control notification handler code here

	int ret{ XMQNode::stop() };

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

	char user[256]{ 0 }, passwd[256]{0}, ip[256]{ 0 };
	unsigned short port{
		static_cast<unsigned short>(GetDlgItemInt(IDC_DVS_PORT)) };

	HWND hwnd{ this->GetSafeHwnd() };
	GetDlgItemTextA(hwnd, IDC_DVS_ADDRESS, ip, 256);
	GetDlgItemTextA(hwnd, IDC_DVS_USER, user, 256);
	GetDlgItemTextA(hwnd, IDC_DVS_PASSWORD, passwd, 256);

	char url[1024]{ 0 };
	sprintf_s(url, 1024, "config://dvs_host_server?command=add&ip=%s&port=%d&user=%s&passwd=%s&name=test_dvs", ip, port, user, passwd);
	const std::string urlstr{ url };
	int ret{ XMQNode::send(0xFFFF, urlstr.c_str(), urlstr.length()) };

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


void CdvshostdemoDlg::OnBnClickedDvsLogout()
{
	// TODO: Add your control notification handler code here

	ASIOModeConf conf;
	conf.proto = ASIOProtoType::ASIO_PROTO_TYPE_TCP;
	conf.port = 60820;
	conf.tcp.mode = ASIOModeType::ASIO_MODE_TYPE_CONNECT;
	conf.tcp.ip = "127.0.0.1";
	int ret{ ASIONode::addConf(conf) };

	if (Error_Code_Success == ret)
	{
	}
}

uint32_t CdvshostdemoDlg::afterFetchAcceptedEventNotification(
	const char* ip /* = nullptr */, 
	const uint16_t port /* = 0 */, 
	const int32_t e /* = 0 */)
{
	return 0;
}

uint32_t CdvshostdemoDlg::afterFetchConnectedEventNotification(const int32_t e /* = 0 */)
{
	return ++sid;
}

void CdvshostdemoDlg::afterPolledReadDataNotification(
	const uint32_t id /* = 0 */, 
	const void* data /* = nullptr */, 
	const uint64_t bytes /* = 0 */, 
	const int32_t e /* = 0 */)
{

}

void CdvshostdemoDlg::afterPolledSendDataNotification(
	const uint32_t id /* = 0 */, 
	const uint64_t bytes /* = 0 */, 
	const int32_t e /* = 0 */)
{

}


void CdvshostdemoDlg::OnBnClickedRealplayTest()
{
	// TODO: Add your control notification handler code here

	const std::string url{ "realplay://1?command=1&channel=1&stream=0" };
	const uint64_t bytes{ 36 + url.length() };
	char* data{ new char[bytes] };
	*((uint32_t*)data) = 0xFF050301;
	*((uint32_t*)(data + 4)) = 0;
	*((uint32_t*)(data + 8)) = 0;
	*((uint32_t*)(data + 12)) = 1;
	*((uint32_t*)(data + 16)) = url.length();
	*((uint64_t*)(data + 20)) = 50;
	*((uint64_t*)(data + 28)) = 11223344;
	memcpy_s(data + 36, url.length(), url.c_str(), url.length());

	ASIONode::send(sid, data, bytes);
}

void CdvshostdemoDlg::processDvsControlMessage(Url& requestUrl)
{
	const std::vector<ParamItem> parameters{ requestUrl.getParameters() };
	std::string command, error;

	for (int i = 0; i != parameters.size(); ++i)
	{
		if (!parameters[i].key.compare("command"))
		{
			command = parameters[i].value;
		}
		else if (!parameters[i].key.compare("error"))
		{
			error = parameters[i].value;
		}
		else if (!parameters[i].key.compare("dvs"))
		{
			dvs = parameters[i].value;
		}
	}

	if (!command.compare("add") && 0 == atoi(error.c_str()))
	{
		MessageBox(CString(dvs.c_str()), NULL, MB_ICONINFORMATION | MB_OK);
	}
}
