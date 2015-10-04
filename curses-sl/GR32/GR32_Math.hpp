// Borland C++ Builder
// Copyright (c) 1995, 2005 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'Gr32_math.pas' rev: 10.00

#ifndef Gr32_mathHPP
#define Gr32_mathHPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member functions
#pragma pack(push,8)
#include <System.hpp>	// Pascal unit
#include <Sysinit.hpp>	// Pascal unit
#include <Gr32.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Gr32_math
{
//-- type declarations -------------------------------------------------------
//-- var, const, procedure ---------------------------------------------------
extern PACKAGE int __fastcall FixedFloor(Gr32::TFixed A);
extern PACKAGE int __fastcall FixedCeil(Gr32::TFixed A);
extern PACKAGE int __fastcall FixedRound(Gr32::TFixed A);
extern PACKAGE Gr32::TFixed __fastcall FixedMul(Gr32::TFixed A, Gr32::TFixed B);
extern PACKAGE Gr32::TFixed __fastcall FixedDiv(Gr32::TFixed A, Gr32::TFixed B);
extern PACKAGE Gr32::TFixed __fastcall OneOver(Gr32::TFixed Value);
extern PACKAGE Gr32::TFixed __fastcall FixedSqr(Gr32::TFixed Value);
extern PACKAGE Gr32::TFixed __fastcall FixedSqrtLP(Gr32::TFixed Value);
extern PACKAGE Gr32::TFixed __fastcall FixedSqrtHP(Gr32::TFixed Value);
extern PACKAGE Gr32::TFixed __fastcall FixedCombine(Gr32::TFixed W, Gr32::TFixed X, Gr32::TFixed Y);
extern PACKAGE void __fastcall SinCos(const float Theta, float &Sin, float &Cos)/* overload */;
extern PACKAGE void __fastcall SinCos(const float Theta, const float Radius, float &Sin, float &Cos)/* overload */;
extern PACKAGE float __fastcall Hypot(const float X, const float Y);
extern PACKAGE int __fastcall MulDiv(int Multiplicand, int Multiplier, int Divisor);
extern PACKAGE bool __fastcall IsPowerOf2(int Value);
extern PACKAGE int __fastcall PrevPowerOf2(int Value);
extern PACKAGE int __fastcall NextPowerOf2(int Value);
extern PACKAGE int __fastcall Average(int A, int B);
extern PACKAGE int __fastcall Sign(int Value);

}	/* namespace Gr32_math */
using namespace Gr32_math;
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// Gr32_math
