#pragma once


#include "ControlProperty.h"
// CControlView form view

class CControlView : public CFormView
{
	DECLARE_DYNCREATE(CControlView)

protected:
	CControlView();           // protected constructor used by dynamic creation
	virtual ~CControlView();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONTROLVIEW };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	CControlProperty* m_pControlProperty;
public:
	virtual void OnInitialUpdate();
	afx_msg void OnDestroy();
};


