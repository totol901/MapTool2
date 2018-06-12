// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // �Ϻ� CString �����ڴ� ��������� ����˴ϴ�.

// MFC�� ���� �κа� ���� ������ ��� �޽����� ���� ����⸦ �����մϴ�.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC �ٽ� �� ǥ�� ���� ����Դϴ�.
#include <afxext.h>         // MFC Ȯ���Դϴ�.

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC�� ���� �� ��Ʈ�� ���� ����
#include <afxdlgs.h>

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

#include <d3d11.h>
#include <d3dx11tex.h>
#include <d3dcommon.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <fstream>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <assert.h>
#include <bitset>
#include <string>
#include <math.h>
#include <dinput.h>
#include <mmsystem.h>
#include <pdh.h>
#include <fbxsdk.h>
#include <chrono>
#include <iostream>

#if defined(DEBUG) || defined(_DEBUG)
#include <DxErr.h>
#endif

using namespace std;

//����� �� include
#include "MiniMapView.h"
#include "MapTool2View.h"
#include "ProperyView.h"
#include "D3dClass.h"

/** ���� d3d error checker (��ó : 3D ���� ���α׷��� �Թ�, ����ũ D.�糪)**/
#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)													\
	{                                                           \
		HRESULT hr = (x);                                       \
		if(FAILED(hr))                                          \
		{                                                       \
			DXTrace(__FILE__, (DWORD)__LINE__, hr, L#x, true); \
		}                                                       \
	}
#endif

#else
#ifndef HR
#define HR(x) (x)
#endif
#endif
#pragma comment(lib, "libfbxsdk-md.lib")

/** COM ��ü ������ ���� ��ũ��**/
#define RELEASE_COM(x) { if(x){ x->Release(); x = NULL; } }

/** delete ���� ��ũ�ε�**/
#define SAFE_DELETE(x) { delete x; x = NULL; }
#define SAFE_DELETE_ARRAY(x) { delete[] x; x = NULL;}

struct ST_PNT_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR3 n;
	D3DXVECTOR2 t;
	ST_PNT_VERTEX()
		: p(0, 0, 0)
		, n(0, 0, 0)
		, t(0, 0)
	{
	}

	ST_PNT_VERTEX(D3DXVECTOR3 _p, D3DXVECTOR3 _n, D3DXVECTOR2 _t)
		: p(_p)
		, n(_n)
		, t(_t)
	{
	}
};

struct ST_BOUNDING_BOX
{
	D3DXVECTOR3		vMin;
	D3DXVECTOR3		vMax;
	ST_BOUNDING_BOX()
		: vMin(0, 0, 0)
		, vMax(0, 0, 0) {}
};

struct ST_SPHERE
{
	float			fRadius;
	D3DXVECTOR3		vCenter;
	bool		isPicked;

	void SetUp(D3DXVECTOR3 centerPosit, float radius)
	{
		isPicked = false;
		vCenter = centerPosit;
		fRadius = radius;
	}
};

struct TAG_MATERIAL
{
	D3DXVECTOR4   Diffuse;        /* Diffuse color RGBA */
	D3DXVECTOR4   Ambient;        /* Ambient color RGB */
	D3DXVECTOR4   Specular;       /* Specular 'shininess' */
	D3DXVECTOR4   Emissive;       /* Emissive color RGB */
	float           Power;          /* Sharpness if specular highlight */
};

struct Vertex
{
	D3DXVECTOR3 mPos;
	D3DXVECTOR3 mNormal;
	D3DXVECTOR2 mUV;
	D3DXVECTOR4 mTangentU;
	FLOAT mWeight[3];
	BYTE mIndices[4];
};
struct Material
{
	Material() { ZeroMemory(this, sizeof(this)); }

	D3DXVECTOR4 mAmbient;
	D3DXVECTOR4 mDiffuse;
	D3DXVECTOR4 mSpecular;
	D3DXVECTOR4 mReflect;
};
