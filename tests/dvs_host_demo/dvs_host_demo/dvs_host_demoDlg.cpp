
// dvs_host_demoDlg.cpp : implementation file
//

#include <iostream>
#include "pch.h"
#include "framework.h"
#include "dvs_host_demo.h"
#include "dvs_host_demoDlg.h"
#include "afxdialogex.h"
#include "CAddDvsDlg.h"
#include "CAddRsuDlg.h"

#include <string>
#include "boost/bind/bind.hpp"
using namespace boost::placeholders;
#include "boost/json.hpp"
#include "boost/format.hpp"
#include "error_code.h"

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
	: CDialogEx(IDD_DVS_HOST_DEMO_DIALOG, pParent),
	XMQNode(), ASIONode(), sid{0}, stream{0}, selectedItem{NULL}
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
	DDX_Control(pDX, IDC_LOG_LIST, loglist);
}

BEGIN_MESSAGE_MAP(CdvshostdemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_NOTIFY(NM_RCLICK, IDC_DEVICE_TREE, &CdvshostdemoDlg::OnNMRClickDeviceTree)
	ON_COMMAND(ID_DEVICE_ADD, &CdvshostdemoDlg::OnMenuClickDeviceAdd)
	ON_COMMAND(ID_DEVICE_QUERY, &CdvshostdemoDlg::OnMenuClickDeviceQuery)
	ON_COMMAND(ID_DEVICE_DELETE, &CdvshostdemoDlg::OnMenuClieckDeviceDelete)
	ON_BN_CLICKED(IDC_XMQ_CONNECT, &CdvshostdemoDlg::OnBnClickedXmqConnect)
	ON_NOTIFY(NM_DBLCLK, IDC_LOG_LIST, &CdvshostdemoDlg::OnNMDblclkLogList)
	ON_BN_CLICKED(IDC_XMQ_POSITION_SET, &CdvshostdemoDlg::OnBnClickedXmqPositionSet)
	ON_BN_CLICKED(IDC_XMQ_POSITION_GET, &CdvshostdemoDlg::OnBnClickedXmqPositionGet)
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
 	SetDlgItemText(IDC_XMQ_LONGITUDE, L"0.0000000");
 	SetDlgItemText(IDC_XMQ_LATITUDE, L"0.0000000");
 	SetDlgItemText(IDC_XMQ_ELEVATION, L"0");

	videoRoot = deviceTree.InsertItem(L"Video", TVI_ROOT, NULL);
	rsuRoot = deviceTree.InsertItem(L"RSU", TVI_ROOT, NULL);
	CRect rc;
	loglist.GetClientRect(&rc);
	loglist.InsertColumn(0, L"Nubmer", LVCFMT_LEFT, rc.Width() * 0.1);
	loglist.InsertColumn(1, L"Time", LVCFMT_LEFT, rc.Width() * 0.15);
	loglist.InsertColumn(2, L"Content", LVCFMT_LEFT, rc.Width() * 0.7);
	LONG lStyle;
	lStyle = GetWindowLong(loglist.m_hWnd, GWL_STYLE);
	lStyle &= ~LVS_TYPEMASK;
	lStyle |= LVS_REPORT;
	SetWindowLong(loglist.m_hWnd, GWL_STYLE, lStyle);

	DWORD dwStyle = loglist.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;
	loglist.SetExtendedStyle(dwStyle);

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
	CString text;
	text.Format(L"XMQ connection status : %s.", online ? L"online" : L"offline");
	log(text);
}

void CdvshostdemoDlg::afterFetchServiceCapabilitiesNotification(
	const ServiceInfo* infos /* = nullptr */, 
	const uint32_t number /* = 0 */)
{
	for (int i = 0; i != number; ++i)
	{
		const std::string name{ infos[i].name };
		CString text;
		text.Format(L"%s online.", CString(name.c_str()));
		log(text);

		if (!name.compare(DatabaseHostID))
		{
		}
	}
}

void CdvshostdemoDlg::afterPolledDataNotification(
	const uint32_t id /* = 0 */, 
	const void* data /* = nullptr */, 
	const uint64_t bytes /* = 0 */, 
	const char* from /* = nullptr */)
{
	Url url;
	int ret{ url.parse(data, bytes) };

	if (Error_Code_Success == ret)
	{
		if (!url.proto().compare("config"))
		{
			config(url);
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
//	GetDlgItemTextA(hwnd, IDC_DEMO_NAME, name, 256);
	GetDlgItemTextA(hwnd, IDC_XMQ_ADDRESS, ip, 256);
	XMQModeConf conf{ 0 };
	conf.id = 0xFFFF;
	memcpy_s(conf.name, 128, "Master-Wangkw", strlen("Master-Wangkw"));
	memcpy_s(conf.ip, 32, ip, strlen(ip));
	conf.port = port;
	conf.type = XMQModeType::XMQ_MODE_TYPE_DEALER;

	int ret{XMQNode::addConf(conf)};

	if (Error_Code_Success == ret)
	{
		ret = XMQNode::run();

 		CString text;
 		text.Format(L"Connect to XMQ server result = %d", ret);
		log(text);
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

void CdvshostdemoDlg::config(Url& url)
{
	std::string data;
	const std::vector<Parameter> parameters{ url.parameters() };

	for (int i = 0; i != parameters.size(); ++i)
	{
		if (!parameters[i].key.compare("data"))
		{
			data = parameters[i].value;
		}
	}

	if (!data.empty())
	{
		auto o{ boost::json::parse(data).as_object() };
		auto command{ o.at("command").as_string() },
			timestamp{ o.at("timestamp").as_string() },
			error{ o.at("error").as_string() };
		std::string out;

		if (!command.compare("mec.db.dvs.query"))
		{
			if (Error_Code_Success == atoi(error.c_str()) && o.contains("devices"))
			{
				dvsmap.clear();
				HTREEITEM child = deviceTree.GetChildItem(videoRoot);
				while (child)
				{
					deviceTree.DeleteItem(child);
					child = deviceTree.GetNextSiblingItem(child);
				}

				auto dvss{ o.at("devices").as_array() };
				const std::size_t count{ dvss.size() };

				for (int i = 0; i != count; ++i)
				{
					const std::string dvs{ dvss[i].as_string().c_str()};
					if (!dvs.empty())
					{
						auto o{ boost::json::parse(dvs).as_object() };
						const std::string factory{ o.at("factory").as_string().c_str() },
							name{ o.at("name").as_string().c_str() },
							id{ o.at("id").as_string().c_str() },
							ip{ o.at("ip").as_string().c_str() },
							channels{ o.at("channels").as_string().c_str() };

						HTREEITEM subitem{ deviceTree.InsertItem(CString(name.c_str()), videoRoot) };
						for (int i = 0; i != atoi(channels.c_str()); ++i)
						{
							CString cname;
							cname.Format(L"Camera%d", i + 1);
							deviceTree.InsertItem(cname, subitem);
						}
						deviceTree.Expand(subitem, TVE_EXPAND);
						dvsmap.insert(std::make_pair(id, dvs));
						nodemap.insert(std::make_pair(subitem, id));

						CString text;
						text.Format(
							L"Query DVS device configure reuslt = [ factory = %s, name = %s, id = %s, ip = %s, channels = %s ].",
							CString(factory.c_str()),
							CString(name.c_str()),
							CString(id.c_str()),
							CString(ip.c_str()),
							CString(channels.c_str()));
						log(text);
					}
				}

				deviceTree.Expand(videoRoot, TVE_EXPAND);
			}
			else
			{
				CString text;
				text.Format(
					L"Query DVS device configure result = %s, no devices existed.", CString(error.c_str()));
				log(text);
			}
		}
		else if (!command.compare("mec.db.rsu.query"))
		{
			if (Error_Code_Success == atoi(error.c_str()) && o.contains("devices"))
			{
				HTREEITEM child = deviceTree.GetChildItem(rsuRoot);
				while (child)
				{
					deviceTree.DeleteItem(child);
					child = deviceTree.GetNextSiblingItem(child);
				}

				auto rsus{ o.at("devices").as_array() };
				const std::size_t count{ rsus.size() };

				for (int i = 0; i != count; ++i)
				{
					const std::string rsu{ rsus[i].as_string().c_str() };
					if (!rsu.empty())
					{
						auto o{ boost::json::parse(rsu).as_object() };
						const std::string id{ o.at("id").as_string().c_str() }, 
							ip{ o.at("ip").as_string().c_str() };
						const double longitude{ std::strtod(o.at("longitude").as_string().c_str(), NULL) }, 
							latitude{ std::strtod(o.at("latitude").as_string().c_str(), NULL)};
						const int64_t elevation{ atoi(o.at("elevation").as_string().c_str())};

						HTREEITEM subitem{ deviceTree.InsertItem(CString(id.c_str()), rsuRoot) };
						CString cip, clongitude, clatitude, celevation;
						cip.Format(L"IP: %s", CString(ip.c_str()));
						deviceTree.InsertItem(cip, subitem);
						clongitude.Format(L"Longitude: %.7f", longitude);
						deviceTree.InsertItem(clongitude, subitem);
						clatitude.Format(L"Longitude: %.7f", latitude);
						deviceTree.InsertItem(clatitude, subitem);
						celevation.Format(L"Elevation: %d", elevation);
						deviceTree.InsertItem(celevation, subitem);
						deviceTree.Expand(subitem, TVE_EXPAND);
						deviceTree.Expand(rsuRoot, TVE_EXPAND);

						CString text;
						text.Format(
							L"Query RSU device configure reuslt = [ id = %s, ip = %s, longitude = %.7f, latitude = %.7f, elevation = %d ].",
							CString(id.c_str()),
							CString(ip.c_str()),
							longitude,
							latitude,
							elevation);
						log(text);
					}
				}

				deviceTree.Expand(videoRoot, TVE_EXPAND);
			}
			else
			{
				CString text;
				text.Format(
					L"Query DVS device configure result = %s, no devices existed.", CString(error.c_str()));
				log(text);
			}
		}
		else if (!command.compare("mec.dvs.add"))
		{
			if (Error_Code_Success == atoi(error.c_str()))
			{
				auto j = boost::json::parse(jo).as_object();
				j["command"] = "mec.db.dvs.add";
				j["id"] = o.at("id").as_string();
				j["channels"] = o.at("channels").as_string();
				out = boost::json::serialize(j);

				char request[256]{ 0 };
				sprintf_s(request, 256,
					"config://%s?data=%s", DatabaseHostID, out.c_str());
				const std::string url{ request };
				int ret{ XMQNode::send(0xFFFF, url.c_str(), url.length()) };
				CString text;
				text.Format(L"Save DVS device configure url = %s.", CString(url.c_str()));
				log(text);
			}
		}
		else if (!command.compare("mec.dvs.remove"))
		{
			if (Error_Code_Success == atoi(error.c_str()))
			{
				auto j = boost::json::parse(jo).as_object();
				j["command"] = "mec.db.dvs.remove";
				j["id"] = o.at("id").as_string();
				j["timestamp"] = timestamp;
				out = boost::json::serialize(j);

				char request[256]{ 0 };
				sprintf_s(request, 256,
					"config://%s?data=%s", DatabaseHostID, out.c_str());
				const std::string url{ request };
				int ret{ XMQNode::send(0xFFFF, url.c_str(), url.length()) };
				CString text;
				text.Format(L"Delete DVS device configure url = %s.", CString(url.c_str()));
				log(text);
			}
		}
		else if (!command.compare("mec.rsu.add"))
		{
			if (Error_Code_Success == atoi(error.c_str()))
			{
				auto j = boost::json::parse(jo).as_object();
				j["command"] = "mec.db.rsu.add";
				out = boost::json::serialize(j);

				char request[256]{ 0 };
				sprintf_s(request, 256,
					"config://%s?data=%s", DatabaseHostID, out.c_str());
				const std::string url{ request };
				int ret{ XMQNode::send(0xFFFF, url.c_str(), url.length()) };
				CString text;
				text.Format(L"Save RSU device configure url = %s.", CString(url.c_str()));
				log(text);
			}
		}
		else if (!command.compare("mec.rsu.remove"))
		{
			if (Error_Code_Success == atoi(error.c_str()))
			{
				auto j = boost::json::parse(jo).as_object();
				j["command"] = "mec.db.rsu.remove";
				out = boost::json::serialize(j);

				char request[256]{ 0 };
				sprintf_s(request, 256,
					"config://%s?data=%s", DatabaseHostID, out.c_str());
				const std::string url{ request };
				int ret{ XMQNode::send(0xFFFF, url.c_str(), url.length()) };
				CString text;
				text.Format(L"Delete RSU device configure url = %s.", CString(url.c_str()));
				log(text);
			}
		}
		else if (!command.compare("mec.db.dvs.add"))
		{
			CString text;
			text.Format(L"Save DVS device configure result = %d, timestamp = %s.", atoi(error.c_str()), CString(timestamp.c_str()));
			log(text);
		}
		else if (!command.compare("mec.db.dvs.remove"))
		{
			if (selectedItem && Error_Code_Success == atoi(error.c_str()))
			{
				deviceTree.DeleteItem(selectedItem);
				selectedItem = NULL;
			}

			CString text;
			text.Format(L"Delete DVS device configure result = %d, timestamp = %s.", atoi(error.c_str()), CString(timestamp.c_str()));
			log(text);
		}
		else if (!command.compare("mec.db.rsu.add"))
		{
			CString text;
			text.Format(L"Save RSU device configure result = %d, timestamp = %s.", atoi(error.c_str()), CString(timestamp.c_str()));
			log(text);
		}
		else if (!command.compare("mec.db.rsu.remove"))
		{
			if (selectedItem && Error_Code_Success == atoi(error.c_str()))
			{
				deviceTree.DeleteItem(selectedItem);
				selectedItem = NULL;
			}

			CString text;
			text.Format(L"Delete RSU device configure result = %d, timestamp = %s.", atoi(error.c_str()), CString(timestamp.c_str()));
			log(text);
		}
		else if (!command.compare("mec.db.position.query"))
		{
			CString text;

			if (Error_Code_Success == atoi(error.c_str()))
			{
				const double longitude{ o["longitude"].as_double() }, latitude{ o["latitude"].as_double() };
				const int64_t elevation{ o["elevation"].as_int64() };
				SetDlgItemText(IDC_XMQ_LONGITUDE, CString((boost::format("%.7f") % longitude).str().c_str()));
				SetDlgItemText(IDC_XMQ_LATITUDE, CString((boost::format("%.7f") % latitude).str().c_str()));
				SetDlgItemText(IDC_XMQ_ELEVATION, CString((boost::format("%d") % elevation).str().c_str()));

				text.Format(
					L"Query MEC position configure result = %d, longitude = %.7f, latitude = %.7f, elevation = %d.",
					atoi(error.c_str()), longitude, latitude, elevation);
				log(text);
			}
			else
			{
				text.Format(
					L"Query MEC position configure result = %d.", atoi(error.c_str()));
				log(text);
			}
		}
		else if (!command.compare("mec.db.position.set"))
		{
			CString text;
			text.Format(L"Set MEC position configure result = %d, timestamp = %s.", atoi(error.c_str()), CString(timestamp.c_str()));
			log(text);
		}
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

void CdvshostdemoDlg::OnNMRClickDeviceTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	CPoint pt;
	GetCursorPos(&pt);
	deviceTree.ScreenToClient(&pt);

	UINT flag = 0;
	HTREEITEM item = deviceTree.HitTest(pt, &flag);

	if (TVHT_ONITEM & flag)
	{
		deviceTree.ClientToScreen(&pt);
 		CMenu menu;
		menu.LoadMenu(IDR_MENU1);
		HTREEITEM parent{ deviceTree.GetParentItem(item) };

		if (videoRoot == item || rsuRoot == item || lidarRoot == item)
		{
			menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this, NULL);
		}
		else if (videoRoot == parent || rsuRoot == parent || lidarRoot == parent)
		{
			menu.GetSubMenu(1)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this, NULL);
		}
	}

	deviceTree.SelectItem(item);
}


void CdvshostdemoDlg::OnMenuClickDeviceAdd()
{
	// TODO: Add your command handler code here

	HTREEITEM item{ deviceTree.GetSelectedItem() };
	boost::json::object o;
	const char* targetID{ nullptr };

	if (videoRoot == item)
	{
		CAddDvsDlg dlg;
		if (IDOK == dlg.DoModal())
		{
			o["command"] = "mec.dvs.add";
			o["factory"] = (boost::format("%d") % dlg.factory).str();
			o["name"] = (boost::format("%s") % dlg.name).str();
			o["ip"] = (boost::format("%s") % dlg.ip).str();
			o["port"] = (boost::format("%d") % dlg.port).str();
			o["user"] = (boost::format("%d") % dlg.user).str();
			o["passwd"] = (boost::format("%d") % dlg.passwd).str();
			o["timestamp"] = (boost::format("%llu") % GetTickCount()).str();
			targetID = DVSHostID;
		}
		else
		{
			return;
		}
	}
	else if (rsuRoot == item)
	{
		CAddRsuDlg dlg;
		if (IDOK == dlg.DoModal())
		{
			o["command"] = "mec.db.rsu.add";
			o["id"] = (boost::format("%d") % dlg.id).str();
			o["ip"] = (boost::format("%s") % dlg.ip).str();
			o["longitude"] = (boost::format("%.7f") % dlg.longitude).str();
			o["latitude"] = (boost::format("%.7f") % dlg.latitude).str();
			o["elevation"] = (boost::format("%d") % dlg.elevation).str();
			o["timestamp"] = (boost::format("%llu") % GetTickCount()).str();
			targetID = DatabaseHostID;
		}
		else
		{
			return;
		}
	}

	jo = boost::json::serialize(o);
	char request[256]{ 0 };
	sprintf_s(request, 256,
		"config://%s?data=%s", targetID, jo.c_str());
	const std::string url{ request };
	int ret{ XMQNode::send(0xFFFF, url.c_str(), url.length()) };
	CString text;
	text.Format(L"Add device configure url = %s.", CString(url.c_str()));
	log(text);
}


void CdvshostdemoDlg::OnMenuClickDeviceQuery()
{
	// TODO: Add your command handler code here

	HTREEITEM item{ deviceTree.GetSelectedItem() };
	boost::json::object o;

	if (videoRoot == item)
	{
		o["command"] = "mec.db.dvs.query";
	}
	else if (rsuRoot == item)
	{
		o["command"] = "mec.db.rsu.query";
	}

	o["timestamp"] = (boost::format("%llu") % GetTickCount()).str();
	const std::string out{ boost::json::serialize(o) };
	char request[128]{ 0 };
	sprintf_s(request, 128, "config://%s?data=%s", DatabaseHostID, out.c_str());
	const std::string url{ request };
	int ret{ XMQNode::send(0xFFFF, url.c_str(), url.length()) };
	CString text;
	text.Format(L"Query devices configure url = %s.", CString(url.c_str()));
	log(text);
}

void CdvshostdemoDlg::log(const CString& text)
{
	CString time;
	CTime tm = CTime::GetCurrentTime();
	time.Format(L"%04d-%02d-%02d %02d:%02d:%02d", tm.GetYear(), tm.GetMonth(), tm.GetDay(), tm.GetHour(), tm.GetMinute(), tm.GetSecond());
	int nCount = loglist.GetItemCount();
	CString number;
	number.Format(L"%d", nCount);
	int nRow = loglist.InsertItem(LVIF_TEXT | LVIF_PARAM, 0, number, 0, 0, 2, 0);
	loglist.SetItemText(nRow, 1, time);
	loglist.SetItemText(nRow, 2, text);
}

void CdvshostdemoDlg::OnNMDblclkLogList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->iItem != -1)
	{
		TCHAR szBuffer[2048]{ 0 };
		int nColumnCount = loglist.GetHeaderCtrl()->GetItemCount();
		LVITEM lvi;
		lvi.iItem = pNMListView->iItem;
		lvi.mask = LVIF_TEXT;
		lvi.cchTextMax = 1024;
		lvi.iSubItem = 2;
		lvi.pszText = szBuffer;
		loglist.GetItem(&lvi);

		CString content(szBuffer);
		MessageBox(content, L"Detail", MB_ICONINFORMATION | MB_OK);
	}

	*pResult = 0;
}


void CdvshostdemoDlg::OnMenuClieckDeviceDelete()
{
	// TODO: Add your command handler code here

	selectedItem = deviceTree.GetSelectedItem();
	
	if (selectedItem)
	{
		HTREEITEM parent = deviceTree.GetParentItem(selectedItem);
		boost::json::object o;
		const char* targetID{ nullptr };
		std::string did;

		if (videoRoot == parent)
		{
			did = nodemap.at(selectedItem);
			o["command"] = "mec.db.dvs.remove";
			targetID = DVSHostID;
		}
		else if (rsuRoot == parent)
		{
			char temp[16]{ 0 };
			CString name{ deviceTree.GetItemText(selectedItem) };
			int bytes{ 
				WideCharToMultiByte(CP_ACP, 0, name, name.GetLength(), temp, 16, NULL, NULL)};
			did = temp;
			o["command"] = "mec.db.rsu.remove";
			targetID = DatabaseHostID;
		}
		else if (lidarRoot == parent)
		{

		}

		o["id"] = did;
		o["timestamp"] = (boost::format("%llu") % GetTickCount()).str();
		std::string out{ boost::json::serialize(o) };

		char request[128]{ 0 };
		sprintf_s(request, 128, "config://%s?data=%s", targetID, out.c_str());
		const std::string url{ request };
		int ret{ XMQNode::send(0xFFFF, url.c_str(), url.length()) };
		CString text;
		text.Format(L"Delete devices configure url = %s.", CString(url.c_str()));
		log(text);
	}
}

void CdvshostdemoDlg::OnBnClickedXmqPositionSet()
{
	// TODO: Add your control notification handler code here

	HWND hwnd{ this->GetSafeHwnd() };
	char longitude[64]{ 0 }, latitude[64]{ 0 }, elevation[64]{ 0 };
	GetDlgItemTextA(hwnd, IDC_XMQ_LONGITUDE, longitude, 64);
	GetDlgItemTextA(hwnd, IDC_XMQ_LATITUDE, latitude, 64);
	GetDlgItemTextA(hwnd, IDC_XMQ_ELEVATION, elevation, 64);

	double d_longitude{ std::strtod(longitude, NULL) };
	double d_latitude{ std::strtod(latitude, NULL) };
	int d_elevation{ atoi(elevation) };

	boost::json::object o;
	o["command"] = "mec.db.position.set";
	o["longitude"] = d_longitude;
	o["latitude"] = d_latitude;
	o["elevation"] = d_elevation;
	o["timestamp"] = (boost::format("%llu") % GetTickCount()).str();
	std::string out{ boost::json::serialize(o) };

	char request[1024]{ 0 };
	sprintf_s(request, 1024, "config://%s?data=%s", DatabaseHostID, out.c_str());
	const std::string url{ request };
	int ret{ XMQNode::send(0xFFFF, url.c_str(), url.length()) };
	CString text;
	text.Format(L"Set MEC position configure url = %s.", CString(url.c_str()));
	log(text);
}


void CdvshostdemoDlg::OnBnClickedXmqPositionGet()
{
	// TODO: Add your control notification handler code here

	boost::json::object o;
	o["command"] = "mec.db.position.query";
	o["timestamp"] = (boost::format("%llu") % GetTickCount()).str();
	const std::string out{ boost::json::serialize(o) };

	char request[128]{ 0 };
	sprintf_s(request, 128, "config://%s?data=%s", DatabaseHostID, out.c_str());
	const std::string url{ request };
	int ret{ XMQNode::send(0xFFFF, url.c_str(), url.length()) };
	CString text;
	text.Format(L"Query DVS position configure url = %s.", CString(url.c_str()));
	log(text);
}
