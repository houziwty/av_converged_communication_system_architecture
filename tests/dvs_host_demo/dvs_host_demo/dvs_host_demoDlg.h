
// dvs_host_demoDlg.h : header file
//

#pragma once

#include "libxmq_host_client.h"
#include "log.h"
using namespace module::file::log;

// CdvshostdemoDlg dialog
class CdvshostdemoDlg : public CDialogEx, protected LibXmqHostClient
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
	void fetchXmqHostClientOnlineStatusNotification(bool online) override;
	void fetchXmqHostServiceCapabilitiesNotification(
		const ServiceInfo* infos = nullptr,
		const int number = 0) override;
	void fetchXmqHostClientReceivedDataNotification(
		const void* data = nullptr,
		const int bytes = 0) override;
public:
	afx_msg void OnBnClickedXmqConnect();
	afx_msg void OnBnClickedXmqDisconnect();

private:
//	FileLog fileLog;
};
