
// MapTool2.h : MapTool2 ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.

// �� Ŭ������ ������ ���ؼ��� MapTool2.cpp�� �����Ͻʽÿ�.
//
#include "MapScene.h"

class CMapTool2App : public CWinApp
{
public:
	CMapTool2App();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	
// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);


};

extern CMapTool2App theApp;
