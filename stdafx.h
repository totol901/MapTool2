
// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이 
// 들어 있는 포함 파일입니다.

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 일부 CString 생성자는 명시적으로 선언됩니다.

// MFC의 공통 부분과 무시 가능한 경고 메시지에 대한 숨기기를 해제합니다.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 핵심 및 표준 구성 요소입니다.
#include <afxext.h>         // MFC 확장입니다.

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 공용 컨트롤에 대한 MFC 지원입니다.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows 공용 컨트롤에 대한 MFC 지원입니다.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC의 리본 및 컨트롤 막대 지원
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

#if defined(DEBUG) || defined(_DEBUG)  
#include <DxErr.h>
#endif

using namespace std;

//사용자 용 include
#include "MiniMapView.h"
#include "MapTool2View.h"
#include "ProperyView.h"
#include "D3dClass.h"

/** 간단 d3d error checker (출처 : 3D 게임 프로그래밍 입문, 프랭크 D.루나)**/
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

/** COM 객체 릴리즈 간편 매크로**/
#define RELEASE_COM(x) { if(x){ x->Release(); x = NULL; } }

/** delete 간편 매크로들**/
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