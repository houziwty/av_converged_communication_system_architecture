
// dvs_host_demoDlg.h : header file
//

#pragma once

#include <unordered_map>
#include "xmq_node.h"
using namespace module::network::xmq;
#include "asio_node.h"
using namespace module::network::asio;
#include "av_node.h"
#include "av_player.h"
using namespace module::av::stream;
#include "url/url.h"
using namespace framework::utils::data;

// CdvshostdemoDlg dialog
class CdvshostdemoDlg 
	: public CDialogEx, protected XMQNode, protected ASIONode, protected AVNode
{
// Construction
public:
	CdvshostdemoDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DVS_HOST_DEMO_DIALOG };
#endif

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
	afx_msg LRESULT OnOpenRealplay(WPARAM wParam, LPARAM lParam);
	afx_msg void OnNMRClickDeviceTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMenuClickDeviceQuery();
	afx_msg void OnMenuClickDeviceAdd();
	afx_msg void OnMenuClieckDeviceDelete();
	afx_msg void OnBnClickedXmqConnect();
	afx_msg void OnBnClickedXmqDisconnect();
	afx_msg void OnBnClickedRealplayTest();
	afx_msg void OnBnClickedGrabTest();
	afx_msg void OnClose();
	afx_msg void OnNMDblclkLogList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedXmqPositionSet();
	afx_msg void OnBnClickedXmqPositionGet();
	DECLARE_MESSAGE_MAP()

protected:
	void afterPolledDataNotification(
		const uint32_t id = 0,
		const void* data = nullptr,
		const uint64_t bytes = 0,
		const char* from = nullptr) override;
	void afterFetchOnlineStatusNotification(const bool online = false) override;
	void afterFetchServiceCapabilitiesNotification(
		const ServiceInfo* infos = nullptr,
		const uint32_t number = 0) override;
	uint32_t afterFetchAcceptedEventNotification(
		const char* ip = nullptr,
		const uint16_t port = 0,
		const int32_t e = 0) override;
	uint32_t afterFetchConnectedEventNotification(const int32_t e = 0) override;
	void afterPolledReadDataNotification(
		const uint32_t id = 0,
		const void* data = nullptr,
		const uint64_t bytes = 0,
		const int32_t e = 0) override;
	void afterPolledSendDataNotification(
		const uint32_t id = 0,
		const uint64_t bytes = 0,
		const int32_t e = 0) override;

private:
	void config(Url& url);
	void avframeDataCallback(
		const uint32_t id = 0, 
		const void* avpkt = nullptr);
	static DWORD WINAPI ThreadFunc(LPVOID c);
	void log(const CString& text);

private:
	uint32_t sid;
	//设备添加JSON数据备份
	//添加成功后发送给数据库
	std::string jo;
	uint32_t stream;
	ExtendDrawInfo drawInfo;
	static int WM_OPEN_REALPLAY;
	CTreeCtrl deviceTree;
	HTREEITEM videoRoot;
	HTREEITEM lidarRoot;
	HTREEITEM rsuRoot;
	HTREEITEM selectedItem;
	CListCtrl loglist;
	std::unordered_map<std::string, const std::string> dvsmap;
	std::unordered_map<HTREEITEM, const std::string> nodemap;
};
