#pragma once


// CPage1 dialog

class CPage1 : public CPropertyPage
{
	DECLARE_DYNAMIC(CPage1)

public:
	CPage1();
	virtual ~CPage1();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PAGE1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
