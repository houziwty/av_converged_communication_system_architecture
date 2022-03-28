
// dvs_host_demoDlg.cpp : implementation file
//

#include <iostream>
#include "pch.h"
#include "framework.h"
#include "dvs_host_demo.h"
#include "dvs_host_demoDlg.h"
#include "afxdialogex.h"

#include <string>
#include "boost/bind/bind.hpp"
using namespace boost::placeholders;
#include "boost/format.hpp"
#include "error_code.h"
#include "json/json.h"
using namespace framework::utils::data;

#include "av_pkt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int CdvshostdemoDlg::WM_OPEN_REALPLAY = WM_USER + 1000;


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
	: CDialogEx(IDD_DVS_HOST_DEMO_DIALOG, pParent), XMQNode(), ASIONode(), sid{0}, stream{0}
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	drawInfo.number = 0;
	drawInfo.areas = NULL;
	drawInfo.enable = false;
}

void CdvshostdemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DEVICE_TREE, deviceTree);
}

BEGIN_MESSAGE_MAP(CdvshostdemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_NOTIFY(NM_RCLICK, IDC_DEVICE_TREE, &CdvshostdemoDlg::OnNMRClickDeviceTree)
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

// 	SetDlgItemText(IDC_DVS_ADDRESS, L"192.168.2.225");
// 	SetDlgItemText(IDC_DVS_PORT, L"8000");
// 	SetDlgItemText(IDC_DVS_USER, L"admin");
// 	SetDlgItemText(IDC_DVS_PASSWD, L"Vrc123456");
// 	SetDlgItemText(IDC_DVS_FACTORY, L"0");
// 	SetDlgItemText(IDC_STREAM_URL, L"realplay://1?command=1&channel=1&stream=0");
// 	SetDlgItemText(IDC_DVS_NAME, L"Test_hk");

	videoRoot = deviceTree.InsertItem(L"video", TVI_ROOT, NULL);
	deviceTree.InsertItem(L"192.168.2.225", videoRoot);

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

LRESULT CdvshostdemoDlg::OnOpenRealplay(WPARAM wParam, LPARAM lParam)
{
	HWND hwnd{ GetDlgItem(IDC_REALPLAY_WND1 + wParam)->GetSafeHwnd() };
	AVModeConf conf{ ++wParam, AVModeType::AV_MODE_TYPE_REALPLAY, hwnd };
//	conf.callback = boost::bind(&CdvshostdemoDlg::avframeDataCallback, this, _1, _2);
//	conf.infos = &drawInfo;
	int ret = AVNode::addConf(conf);

	char temp[256]{ 0 };
	GetDlgItemTextA(this->GetSafeHwnd(), IDC_STREAM_URL, temp, 256);

//	stream = 2;
	const std::string url{ temp };
	const uint64_t bytes{ 32 + url.length() };
	char* data{ new char[bytes] };
	*((uint32_t*)data) = 0xFF050301;
	*((uint32_t*)(data + 4)) = (uint32_t)AVMainType::AV_MAIN_TYPE_NONE;
	*((uint32_t*)(data + 8)) = (uint32_t)AVSubType::AV_SUB_TYPE_NONE;
	*((uint32_t*)(data + 12)) = url.length();
	*((uint32_t*)(data + 16)) = 0;
	*((uint64_t*)(data + 24)) = 0;
	memcpy_s(data + 32, url.length(), url.c_str(), url.length());

	while (Error_Code_Object_Not_Exist == ASIONode::send(wParam, data, bytes))
	{
		Sleep(10);
	}
	boost::checked_array_delete(data);
	DWORD id;
	CreateThread(NULL, 0, &CdvshostdemoDlg::ThreadFunc, this, 0, &id);

	return 0;
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
		if (!name.compare(DatabaseHostID))
		{
			char tick[256]{ 0 };
			sprintf_s(tick, 128, "config://%s?command=query&name=%s.dvs.devices&timestamp=%d", DatabaseHostID, DVSHostID, GetTickCount());
			const std::string url{"config://database_host_server?command=query&name=dvs_host_server.dvs.login"};
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

	Url url;
	int ret{ url.parse(data, bytes) };

	if (Error_Code_Success == ret)
	{
		if (!url.proto().compare("config"))
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

// 	char user[256]{ 0 }, passwd[256]{0}, ip[256]{ 0 }, name[256]{ 0 };
// 	unsigned short port{
// 		static_cast<unsigned short>(GetDlgItemInt(IDC_DVS_PORT)) };
// 	unsigned short factory{
// 		static_cast<unsigned short>(GetDlgItemInt(IDC_DVS_FACTORY)) };
// 
// 	HWND hwnd{ this->GetSafeHwnd() };
// 	GetDlgItemTextA(hwnd, IDC_DVS_ADDRESS, ip, 256);
// 	GetDlgItemTextA(hwnd, IDC_DVS_USER, user, 256);
// 	GetDlgItemTextA(hwnd, IDC_DVS_PASSWD, passwd, 256);
// 	GetDlgItemTextA(hwnd, IDC_DVS_NAME, name, 256);
// 
// 	Json j;
// 	j.add("factory", (boost::format("%d") % factory).str());
// 	j.add("name", (boost::format("%s") % name).str());
// 	j.add("ip", (boost::format("%s") % ip).str());
// 	j.add("port", (boost::format("%d") % port).str());
// 	j.add("user", (boost::format("%d") % user).str());
// 	j.add("passwd", (boost::format("%d") % passwd).str());
// 	j.add("timestamp", "1");
// 	std::string jout;
// 	j.serialize(jout);
// 
// 	char url[1024]{ 0 };
// 	sprintf_s(url, 1024, 
// 		"config://dvs_host_server?command=add&data=%s", jout.c_str());
// 
//  	const std::string urlstr{url};
// 
// 	int ret{ XMQNode::send(0xFFFF, urlstr.c_str(), urlstr.length()) };
// 
// 	if (Error_Code_Success == ret)
// 	{
// 		//				fileLog.write(SeverityLevel::SEVERITY_LEVEL_INFO, "Send query device information to dvs host service successed.");
// 	}
// 	else
// 	{
// 		// 				fileLog.write(
// 		// 					SeverityLevel::SEVERITY_LEVEL_ERROR,
// 		// 					"Send query device information to dvs host service failed, result = [ %d ].",
// 		// 					ret);
// 	}
}


void CdvshostdemoDlg::OnBnClickedDvsLogout()
{
	// TODO: Add your control notification handler code here

	ASIONode::removeConf(sid);
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
	PostMessage(WM_OPEN_REALPLAY, sid, 0);
	return ++sid;
}

void CdvshostdemoDlg::afterPolledReadDataNotification(
	const uint32_t id /* = 0 */, 
	const void* data /* = nullptr */, 
	const uint64_t bytes /* = 0 */, 
	const int32_t e /* = 0 */)
{
	if (1 > id || !data || !bytes || e)
	{
		return;
	}

	AVPkt avpkt;
	avpkt.input(data, bytes);
	AVNode::input(id, &avpkt);
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

	ASIOModeConf conf;
	conf.proto = ASIOProtoType::ASIO_PROTO_TYPE_TCP;
	conf.port = 60820;
	conf.tcp.mode = ASIOModeType::ASIO_MODE_TYPE_CONNECT;
	char ip[256]{ 0 };
	HWND hwnd{ this->GetSafeHwnd() };
	GetDlgItemTextA(hwnd, IDC_XMQ_ADDRESS, ip, 256);
	conf.tcp.ip = ip;
	int ret{ ASIONode::addConf(conf) };

	if (Error_Code_Success == ret)
	{
	}
}

void CdvshostdemoDlg::processDvsControlMessage(Url& requestUrl)
{
	const std::vector<Parameter> parameters{ requestUrl.parameters() };
	std::string command, data;

	for (int i = 0; i != parameters.size(); ++i)
	{
		if (!parameters[i].key.compare("command"))
		{
			command = parameters[i].value;
		}
		else if (!parameters[i].key.compare("data"))
		{
			data = parameters[i].value;
		}
	}

	if (!command.compare("add"))
	{
		MessageBox(CString(dvs.c_str()), NULL, MB_ICONINFORMATION | MB_OK);

// 		char url[1024]{ 0 };
// 		sprintf_s(url, 1024, "config://%s?command=add&name=dvs_host_server.dvs.devices&data=%s", DatabaseHostID, dvs.c_str());
// 		const std::string urlstr{ url };
// 		int ret{ XMQNode::send(0xFFFF, urlstr.c_str(), urlstr.length()) };
	}
	else if (!command.compare("query"))
	{

	}
}


void CdvshostdemoDlg::OnBnClickedGrabTest()
{
	// TODO: Add your control notification handler code here

	stream = 1;
	const std::string url{ "realplay://1?command=1&channel=1&stream=0" };
	const uint64_t bytes{ 32 + url.length() };
	char* data{ new char[bytes] };
	*((uint32_t*)data) = 0xFF050301;
	*((uint32_t*)(data + 4)) = (uint32_t)AVMainType::AV_MAIN_TYPE_NONE;
	*((uint32_t*)(data + 8)) = (uint32_t)AVSubType::AV_SUB_TYPE_NONE;
	*((uint32_t*)(data + 12)) = url.length();
	*((uint32_t*)(data + 16)) = 0;
	*((uint64_t*)(data + 24)) = 0;
	memcpy_s(data + 32, url.length(), url.c_str(), url.length());

	ASIONode::send(sid, data, bytes);
	boost::checked_array_delete(data);
}

void CdvshostdemoDlg::avframeDataCallback(
	const uint32_t id /* = 0 */, 
	const void* avpkt /* = nullptr */)
{
	AVPkt* pkt{(AVPkt*)avpkt};

	if (pkt)
	{
// 		std::cout << (int)pkt->maintype() << " " << 
// 			(int)pkt->subtype() << " " << pkt->sequence() << " " 
// 			<< pkt->timestamp() << " " << pkt->width() << " " 
// 			<< pkt->height() << std::endl;
//  		static FILE* fd{ nullptr };
//  		if (!fd)
//  		{
//  			fopen_s(&fd, "d:\\test.bgr24", "wb+");
//  		}
//  		fwrite(pkt->data(), pkt->bytes(), 1, fd);
	}
}


void CdvshostdemoDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	ASIONode::stop();
	AVNode::removeConf(sid);
	XMQNode::removeConf(0xFFFF);
	XMQNode::stop();
	__super::OnClose();
}

DWORD WINAPI CdvshostdemoDlg::ThreadFunc(LPVOID c)
{
	CdvshostdemoDlg* demo{ (CdvshostdemoDlg*)c };

	while (1)
	{
		Sleep(40);
		demo->drawInfo.enable = false;
//		boost::checked_array_delete(demo->drawInfo.areas);
//		demo->drawInfo.number = 1000;
//		demo->drawInfo.areas = new AVDrawParam[demo->drawInfo.number];
// 		for (int i = 0; i != demo->drawInfo.number; ++i)
// 		{
// 			demo->drawInfo.areas[i].left = i * 10;
// 			demo->drawInfo.areas[i].top = i * 10;
// 			demo->drawInfo.areas[i].right = demo->drawInfo.areas[i].left + 400;
// 			demo->drawInfo.areas[i].bottom = demo->drawInfo.areas[i].top + 300;
// 			demo->drawInfo.areas[i].color[0] = 0x00;
// 			demo->drawInfo.areas[i].color[1] = 0x80;
// 			demo->drawInfo.areas[i].color[2] = 0x00;
// 			sprintf_s(demo->drawInfo.areas[i].text, 256, "Target 2\r\nSpeed: 90 KM/H\r\nDirection: West");
// 		}
// 		demo->drawInfo.enable = true;
	}
}


void CdvshostdemoDlg::OnBnClickedDvsQuery()
{
	// TODO: Add your control notification handler code here

	Json j;
	j.add("timestamp", "10000");
	std::string jout;
	j.serialize(jout);

	char url[1024]{ 0 };
	sprintf_s(url, 1024,
		"config://dvs_host_server?command=add&data=%s", jout.c_str());

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


void CdvshostdemoDlg::OnNMRClickDeviceTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	int x = GetSystemMetrics(SM_CXSCREEN); //»ñÈ¡ÆÁÄ»XÏñËØ
	int y = GetSystemMetrics(SM_CYSCREEN);  //»ñÈ¡ÆÁÄ»YÏñËØ

	CPoint pt;
	GetCursorPos(&pt);
	deviceTree.ScreenToClient(&pt);

	UINT flag = 0;
	HTREEITEM item = deviceTree.HitTest(pt, &flag);

	if ((item != NULL) && (TVHT_ONITEM & flag))
	{
		deviceTree.SelectItem(item);
// 		CMenu menu;
// 		menu.LoadMenu(IDR_MENU1);
// 		menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this, NULL);
	}
}
