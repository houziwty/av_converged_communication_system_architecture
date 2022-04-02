#pragma once
#include "afxdialogex.h"


// CAddRsuDlg dialog

class CAddRsuDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddRsuDlg)

public:
	CAddRsuDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CAddRsuDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ADD_RSU };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnBnClickedOk();

	DECLARE_MESSAGE_MAP()

public:
	int id;
	char ip[32];
	double longitude;
	double latitude;
	int elevation;
};
