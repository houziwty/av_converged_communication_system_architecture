// CAddRsuDlg.cpp : implementation file
//

#include "pch.h"
#include "dvs_host_demo.h"
#include "afxdialogex.h"
#include "CAddRsuDlg.h"


// CAddRsuDlg dialog

IMPLEMENT_DYNAMIC(CAddRsuDlg, CDialogEx)

CAddRsuDlg::CAddRsuDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_ADD_RSU, pParent)
{

}

CAddRsuDlg::~CAddRsuDlg()
{
}

void CAddRsuDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAddRsuDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CAddRsuDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CAddRsuDlg message handlers


void CAddRsuDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here

	HWND hwnd{ GetSafeHwnd() };
	char c_longitude[64]{ 0 }, c_latitude[64]{ 0 };

	id = GetDlgItemInt(IDC_RSU_ID);
	GetDlgItemTextA(hwnd, IDC_RSU_IP, ip, 32);
	GetDlgItemTextA(hwnd, IDC_RSU_LONGITUDE, c_longitude, 32);
	longitude = std::strtod(c_longitude, NULL);
	GetDlgItemTextA(hwnd, IDC_RSU_LATITUDE, c_latitude, 32);
	latitude = std::strtod(c_latitude, NULL);
	elevation = GetDlgItemInt(IDC_RSU_ELEVATION);

	CDialogEx::OnOK();
}
