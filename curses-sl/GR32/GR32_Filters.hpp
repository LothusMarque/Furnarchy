// Borland C++ Builder
// Copyright (c) 1995, 2005 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'Gr32_filters.pas' rev: 10.00

#ifndef Gr32_filtersHPP
#define Gr32_filtersHPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member functions
#pragma pack(push,8)
#include <System.hpp>	// Pascal unit
#include <Sysinit.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Sysutils.hpp>	// Pascal unit
#include <Gr32.hpp>	// Pascal unit
#include <Gr32_blend.hpp>	// Pascal unit
#include <Gr32_system.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Gr32_filters
{
//-- type declarations -------------------------------------------------------
typedef Byte TLUT8[256];

#pragma option push -b-
enum TLogicalOperator { loXOR, loAND, loOR };
#pragma option pop

//-- var, const, procedure ---------------------------------------------------
extern PACKAGE void __fastcall CheckParams(Gr32::TBitmap32* Dst, Gr32::TBitmap32* Src, bool ResizeDst = true);
extern PACKAGE void __fastcall CopyComponents(Gr32::TBitmap32* Dst, Gr32::TBitmap32* Src, Gr32::TColor32Components Components)/* overload */;
extern PACKAGE void __fastcall CopyComponents(Gr32::TBitmap32* Dst, int DstX, int DstY, Gr32::TBitmap32* Src, const Types::TRect &SrcRect, Gr32::TColor32Components Components)/* overload */;
extern PACKAGE void __fastcall AlphaToGrayscale(Gr32::TBitmap32* Dst, Gr32::TBitmap32* Src);
extern PACKAGE void __fastcall IntensityToAlpha(Gr32::TBitmap32* Dst, Gr32::TBitmap32* Src);
extern PACKAGE void __fastcall Invert(Gr32::TBitmap32* Dst, Gr32::TBitmap32* Src, Gr32::TColor32Components Components = (Set<Gr32::TColor32Component, ccBlue, ccAlpha> () << Gr32::TColor32Component(0) << Gr32::TColor32Component(1) << Gr32::TColor32Component(2) << Gr32::TColor32Component(3) ));
extern PACKAGE void __fastcall InvertRGB(Gr32::TBitmap32* Dst, Gr32::TBitmap32* Src);
extern PACKAGE void __fastcall ColorToGrayscale(Gr32::TBitmap32* Dst, Gr32::TBitmap32* Src, bool PreserveAlpha = false);
extern PACKAGE void __fastcall ApplyLUT(Gr32::TBitmap32* Dst, Gr32::TBitmap32* Src, Byte const * LUT, bool PreserveAlpha = false);
extern PACKAGE void __fastcall ChromaKey(Gr32::TBitmap32* ABitmap, Gr32::TColor32 TrColor);
extern PACKAGE Gr32::TColor32 __fastcall CreateBitmask(Gr32::TColor32Components Components);
extern PACKAGE void __fastcall ApplyBitmask(Gr32::TBitmap32* Dst, int DstX, int DstY, Gr32::TBitmap32* Src, const Types::TRect &SrcRect, Gr32::TColor32 Bitmask, TLogicalOperator LogicalOperator)/* overload */;
extern PACKAGE void __fastcall ApplyBitmask(Gr32::TBitmap32* ABitmap, const Types::TRect &ARect, Gr32::TColor32 Bitmask, TLogicalOperator LogicalOperator)/* overload */;

}	/* namespace Gr32_filters */
using namespace Gr32_filters;
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// Gr32_filters
