
// dvs_host_demoDlg.h : header file
//

#pragma once

#include "xmq_node.h"
using namespace module::network::xmq;
#include "log.h"
using namespace module::file::log;

// CdvshostdemoDlg dialog
class CdvshostdemoDlg : public CDialogEx, protected XMQNode
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
		const char* name = nullptr,
		const void* data = nullptr,
		const uint64_t bytes = 0) override;
	void afterFetchOnlineStatusNotification(const bool online = false) override;
	void afterFetchServiceCapabilitiesNotification(
		const ServiceInfo* infos = nullptr,
		const uint32_t number = 0) override;
public:
	afx_msg void OnBnClickedXmqConnect();
	afx_msg void OnBnClickedXmqDisconnect();

private:
//	FileLog fileLog;
public:
	afx_msg void OnBnClickedDvsLogin();
	afx_msg void OnBnClickedDvsLogout();
};
