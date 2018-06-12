// Page1.cpp : implementation file
//

#include "stdafx.h"
#include "MapTool2.h"
#include "Page1.h"
#include "afxdialogex.h"


// CPage1 dialog

IMPLEMENT_DYNAMIC(CPage1, CPropertyPage)

CPage1::CPage1()
	: CPropertyPage(IDD_PAGE1)
{

}

CPage1::~CPage1()
{
}

void CPage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPage1, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON1, &CPage1::OnBnClickedButton1)
END_MESSAGE_MAP()


// CPage1 message handlers


void CPage1::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
}
