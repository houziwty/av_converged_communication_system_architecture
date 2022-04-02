#pragma once
#include "afxdialogex.h"


// CAddDvsDlg dialog

class CAddDvsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddDvsDlg)

public:
	CAddDvsDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CAddDvsDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ADD_DVS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();

	DECLARE_MESSAGE_MAP()

public:
	int factory;
	char name[256];
	char ip[32];
	unsigned short port;
	char user[64];
	char passwd[64];
};
