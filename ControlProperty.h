#pragma once



// CControlProperty
#include "Page1.h"

class CControlProperty : public CPropertySheet
{
	DECLARE_DYNAMIC(CControlProperty)

public:
	CControlProperty(CWnd* pParentWnd);
	CControlProperty(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CControlProperty(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CControlProperty();

public:
	CPage1 m_Page1;

protected:
	DECLARE_MESSAGE_MAP()
};


