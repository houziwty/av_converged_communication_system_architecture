
// dvs_host_demoDlg.h : header file
//

#pragma once

#include "xmq_node.h"
using namespace module::network::xmq;
#include "asio_node.h"
using namespace module::network::asio;
#include "log.h"
using namespace module::file::log;
#include "utils/url/url.h"
using namespace framework::utils::url;

// CdvshostdemoDlg dialog
class CdvshostdemoDlg : public CDialogEx, protected XMQNode, protected ASIONode
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
	void processDvsControlMessage(Url& requestUrl);

public:
	afx_msg void OnBnClickedXmqConnect();
	afx_msg void OnBnClickedXmqDisconnect();

private:
//	FileLog fileLog;
public:
	afx_msg void OnBnClickedDvsLogin();
	afx_msg void OnBnClickedDvsLogout();

private:
	uint32_t sid;
	std::string dvs;
public:
	afx_msg void OnBnClickedRealplayTest();
};
