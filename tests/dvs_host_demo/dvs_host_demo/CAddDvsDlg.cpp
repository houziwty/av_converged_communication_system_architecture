// CAddDvsDlg.cpp : implementation file
//

#include "pch.h"
#include "dvs_host_demo.h"
#include "afxdialogex.h"
#include "CAddDvsDlg.h"


// CAddDvsDlg dialog

IMPLEMENT_DYNAMIC(CAddDvsDlg, CDialogEx)

CAddDvsDlg::CAddDvsDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_ADD_DVS, pParent)
{
	memset(name, 0, 265);
	memset(ip, 0, 32);
	memset(user, 0, 64);
	memset(passwd, 0, 64);
}

CAddDvsDlg::~CAddDvsDlg()
{
}

void CAddDvsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAddDvsDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CAddDvsDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CAddDvsDlg message handlers


void CAddDvsDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here

	HWND hwnd = this->GetSafeHwnd();
	factory = ((CComboBox*)GetDlgItem(IDC_COMBO_DVS_FACTORY))->GetCurSel();
	GetDlgItemTextA(hwnd, IDC_DVS_NAME, name, 256);
	GetDlgItemTextA(hwnd, IDC_DVS_IP, ip, 32);
	port = (unsigned short)GetDlgItemInt(IDC_DVS_PORT);
	GetDlgItemTextA(hwnd, IDC_DVS_USER, user, 64);
	GetDlgItemTextA(hwnd, IDC_DVS_PASSWD, passwd, 64);

	CDialogEx::OnOK();
}


BOOL CAddDvsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	((CComboBox*)GetDlgItem(IDC_COMBO_DVS_FACTORY))->SetCurSel(0);
	SetDlgItemText(IDC_DVS_NAME, L"IPC-HK");
	SetDlgItemText(IDC_DVS_IP, L"192.168.2.225");
	SetDlgItemText(IDC_DVS_PORT, L"8000");
	SetDlgItemText(IDC_DVS_USER, L"admin");
	SetDlgItemText(IDC_DVS_PASSWD, L"Vrc123456");

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
