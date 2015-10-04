// Borland C++ Builder
// Copyright (c) 1995, 2005 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'Gr32_system.pas' rev: 10.00

#ifndef Gr32_systemHPP
#define Gr32_systemHPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member functions
#pragma pack(push,8)
#include <System.hpp>	// Pascal unit
#include <Sysinit.hpp>	// Pascal unit
#include <Sysutils.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Gr32_system
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TPerfTimer;
class PASCALIMPLEMENTATION TPerfTimer : public System::TObject 
{
	typedef System::TObject inherited;
	
private:
	__int64 FFrequency;
	__int64 FPerformanceCountStart;
	__int64 FPerformanceCountStop;
	
public:
	void __fastcall Start(void);
	AnsiString __fastcall ReadNanoseconds();
	AnsiString __fastcall ReadMilliseconds();
	__int64 __fastcall ReadValue(void);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TPerfTimer(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TPerfTimer(void) { }
	#pragma option pop
	
};


#pragma option push -b-
enum TCPUInstructionSet { ciMMX, ciEMMX, ciSSE, ciSSE2, ci3DNow, ci3DNowExt };
#pragma option pop

typedef unsigned HIMAGELIST;

typedef unsigned HTHEME;

struct _MARGINS
{
	
public:
	int cxLeftWidth;
	int cxRightWidth;
	int cyTopHeight;
	int cyBottomHeight;
} ;

typedef _MARGINS  MARGINS;

typedef _MARGINS *PMARGINS;

typedef _MARGINS  TMargins;

//-- var, const, procedure ---------------------------------------------------
extern PACKAGE unsigned CPUISChecks[6];
extern PACKAGE TPerfTimer* GlobalPerfTimer;
extern PACKAGE bool USE_THEMES;
extern PACKAGE unsigned SCROLLBAR_THEME;
extern PACKAGE unsigned GLOBALS_THEME;
static const Shortint THEMEMGR_VERSION = 0x1;
static const Word WM_THEMECHANGED = 0x31a;
static const Shortint SBP_ARROWBTN = 0x1;
static const Shortint SBP_THUMBBTNHORZ = 0x2;
static const Shortint SBP_THUMBBTNVERT = 0x3;
static const Shortint SBP_LOWERTRACKHORZ = 0x4;
static const Shortint SBP_UPPERTRACKHORZ = 0x5;
static const Shortint SBP_LOWERTRACKVERT = 0x6;
static const Shortint SBP_UPPERTRACKVERT = 0x7;
static const Shortint SBP_GRIPPERHORZ = 0x8;
static const Shortint SBP_GRIPPERVERT = 0x9;
static const Shortint SBP_SIZEBOX = 0xa;
static const Shortint ABS_UPNORMAL = 0x1;
static const Shortint ABS_UPHOT = 0x2;
static const Shortint ABS_UPPRESSED = 0x3;
static const Shortint ABS_UPDISABLED = 0x4;
static const Shortint ABS_DOWNNORMAL = 0x5;
static const Shortint ABS_DOWNHOT = 0x6;
static const Shortint ABS_DOWNPRESSED = 0x7;
static const Shortint ABS_DOWNDISABLED = 0x8;
static const Shortint ABS_LEFTNORMAL = 0x9;
static const Shortint ABS_LEFTHOT = 0xa;
static const Shortint ABS_LEFTPRESSED = 0xb;
static const Shortint ABS_LEFTDISABLED = 0xc;
static const Shortint ABS_RIGHTNORMAL = 0xd;
static const Shortint ABS_RIGHTHOT = 0xe;
static const Shortint ABS_RIGHTPRESSED = 0xf;
static const Shortint ABS_RIGHTDISABLED = 0x10;
static const Shortint SCRBS_NORMAL = 0x1;
static const Shortint SCRBS_HOT = 0x2;
static const Shortint SCRBS_PRESSED = 0x3;
static const Shortint SCRBS_DISABLED = 0x4;
static const Shortint SZB_RIGHTALIGN = 0x1;
static const Shortint SZB_LEFTALIGN = 0x2;
extern PACKAGE unsigned __stdcall (*OpenThemeData)(HWND hwnd, WideChar * pszClassList);
extern PACKAGE HRESULT __stdcall (*CloseThemeData)(unsigned hTheme);
extern PACKAGE HRESULT __stdcall (*DrawThemeBackground)(unsigned hTheme, HDC hdc, int iPartId, int iStateId, const Types::TRect &Rect, Types::PRect pClipRect);
extern PACKAGE HRESULT __stdcall (*DrawThemeEdge)(unsigned hTheme, HDC hdc, int iPartId, int iStateId, const Types::TRect &pDestRect, unsigned uEdge, unsigned uFlags, Types::PRect pContentRect);
extern PACKAGE HRESULT __stdcall (*GetThemeColor)(unsigned hTheme, int iPartId, int iStateId, int iPropId, unsigned &pColor);
extern PACKAGE HRESULT __stdcall (*GetThemeMetric)(unsigned hTheme, HDC hdc, int iPartId, int iStateId, int iPropId, int &piVal);
extern PACKAGE HRESULT __stdcall (*GetThemeMargins)(unsigned hTheme, HDC hdc, int iPartId, int iStateId, int iPropId, Types::PRect prc, _MARGINS &pMargins);
extern PACKAGE HRESULT __stdcall (*SetWindowTheme)(HWND hwnd, WideChar * pszSubAppName, WideChar * pszSubIdList);
extern PACKAGE BOOL __stdcall (*IsThemeActive)(void);
extern PACKAGE BOOL __stdcall (*IsAppThemed)(void);
extern PACKAGE HRESULT __stdcall (*EnableTheming)(BOOL fEnable);
extern PACKAGE unsigned __fastcall GetTickCount(void);
extern PACKAGE unsigned __fastcall GetProcessorCount(void);
extern PACKAGE bool __fastcall HasInstructionSet(const TCPUInstructionSet InstructionSet);
extern PACKAGE bool __fastcall HasMMX(void);
extern PACKAGE bool __fastcall HasEMMX(void);
extern PACKAGE bool __fastcall HasSSE(void);
extern PACKAGE bool __fastcall HasSSE2(void);
extern PACKAGE bool __fastcall Has3DNow(void);
extern PACKAGE bool __fastcall Has3DNowExt(void);

}	/* namespace Gr32_system */
using namespace Gr32_system;
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// Gr32_system
