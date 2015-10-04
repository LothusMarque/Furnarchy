// Borland C++ Builder
// Copyright (c) 1995, 2005 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'Gr32_drawingex.pas' rev: 10.00

#ifndef Gr32_drawingexHPP
#define Gr32_drawingexHPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member functions
#pragma pack(push,8)
#include <System.hpp>	// Pascal unit
#include <Sysinit.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <Gr32.hpp>	// Pascal unit
#include <Sysutils.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Gr32_drawingex
{
//-- type declarations -------------------------------------------------------
typedef void __fastcall (*TBlendLineProc)(Gr32::PColor32 Src, Gr32::PColor32 Dst, int Count);

//-- var, const, procedure ---------------------------------------------------
extern PACKAGE bool __fastcall ClipLine(int &X1, int &Y1, int &X2, int &Y2, int MinX, int MinY, int MaxX, int MaxY);

}	/* namespace Gr32_drawingex */
using namespace Gr32_drawingex;
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// Gr32_drawingex
