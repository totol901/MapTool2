// ControlProperty.cpp : implementation file
//

#include "stdafx.h"
#include "MapTool2.h"
#include "ControlProperty.h"


// CControlProperty

IMPLEMENT_DYNAMIC(CControlProperty, CPropertySheet)

CControlProperty::CControlProperty(CWnd * pParentWnd)
{
	AddPage(&m_Page1);
}

CControlProperty::CControlProperty(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

}

CControlProperty::CControlProperty(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{

}

CControlProperty::~CControlProperty()
{
}


BEGIN_MESSAGE_MAP(CControlProperty, CPropertySheet)
END_MESSAGE_MAP()


// CControlProperty message handlers
