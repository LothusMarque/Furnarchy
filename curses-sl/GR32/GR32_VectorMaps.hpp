// Borland C++ Builder
// Copyright (c) 1995, 2005 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'Gr32_vectormaps.pas' rev: 10.00

#ifndef Gr32_vectormapsHPP
#define Gr32_vectormapsHPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member functions
#pragma pack(push,8)
#include <System.hpp>	// Pascal unit
#include <Sysinit.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Gr32.hpp>	// Pascal unit
#include <Gr32_containers.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Gr32_vectormaps
{
//-- type declarations -------------------------------------------------------
typedef Gr32::TFixedPoint  TFixedVector;

typedef Gr32::TFixedPoint *PFixedVector;

typedef Gr32::TFloatPoint  TFloatVector;

typedef Gr32::TFloatPoint *PFloatVector;

typedef DynamicArray<Gr32::TFixedPoint >  TArrayOfFixedVector;

typedef TArrayOfFixedVector *PArrayOfFixedVector;

typedef DynamicArray<Gr32::TFloatPoint >  TArrayOfFloatVector;

typedef TArrayOfFixedVector *PArrayOfFloatVector;

#pragma option push -b-
enum TVectorCombineMode { vcmAdd, vcmReplace, vcmCustom };
#pragma option pop

typedef void __fastcall (__closure *TVectorCombineEvent)(const Gr32::TFixedPoint &F, const Gr32::TFixedPoint &P, Gr32::TFixedPoint &B);

class DELPHICLASS TVectorMap;
class PASCALIMPLEMENTATION TVectorMap : public Gr32::TCustomMap 
{
	typedef Gr32::TCustomMap inherited;
	
private:
	DynamicArray<Gr32::TFixedPoint >  FVectors;
	TVectorCombineEvent FOnVectorCombine;
	TVectorCombineMode FVectorCombineMode;
	Gr32::PFixedPointArray __fastcall GetVectors(void);
	Gr32::TFixedPoint __fastcall GetFixedVector(int X, int Y);
	Gr32::TFixedPoint __fastcall GetFixedVectorS(int X, int Y);
	Gr32::TFixedPoint __fastcall GetFixedVectorX(Gr32::TFixed X, Gr32::TFixed Y);
	Gr32::TFixedPoint __fastcall GetFixedVectorXS(Gr32::TFixed X, Gr32::TFixed Y);
	Gr32::TFloatPoint __fastcall GetFloatVector(int X, int Y);
	Gr32::TFloatPoint __fastcall GetFloatVectorS(int X, int Y);
	Gr32::TFloatPoint __fastcall GetFloatVectorF(float X, float Y);
	Gr32::TFloatPoint __fastcall GetFloatVectorFS(float X, float Y);
	void __fastcall SetFixedVector(int X, int Y, const Gr32::TFixedPoint &Point);
	void __fastcall SetFixedVectorS(int X, int Y, const Gr32::TFixedPoint &Point);
	void __fastcall SetFixedVectorX(Gr32::TFixed X, Gr32::TFixed Y, const Gr32::TFixedPoint &Point);
	void __fastcall SetFixedVectorXS(Gr32::TFixed X, Gr32::TFixed Y, const Gr32::TFixedPoint &Point);
	void __fastcall SetFloatVector(int X, int Y, const Gr32::TFloatPoint &Point);
	void __fastcall SetFloatVectorS(int X, int Y, const Gr32::TFloatPoint &Point);
	void __fastcall SetFloatVectorF(float X, float Y, const Gr32::TFloatPoint &Point);
	void __fastcall SetFloatVectorFS(float X, float Y, const Gr32::TFloatPoint &Point);
	void __fastcall SetVectorCombineMode(const TVectorCombineMode Value);
	
protected:
	virtual void __fastcall ChangeSize(int &Width, int &Height, int NewWidth, int NewHeight);
	
public:
	__fastcall virtual ~TVectorMap(void);
	void __fastcall Clear(void);
	void __fastcall Merge(int DstLeft, int DstTop, TVectorMap* Src, const Types::TRect &SrcRect);
	__property Gr32::PFixedPointArray Vectors = {read=GetVectors};
	Types::TRect __fastcall BoundsRect();
	Types::TRect __fastcall GetTrimmedBounds();
	virtual bool __fastcall Empty(void);
	void __fastcall LoadFromFile(const AnsiString FileName);
	void __fastcall SaveToFile(const AnsiString FileName);
	__property Gr32::TFixedPoint FixedVector[int X][int Y] = {read=GetFixedVector, write=SetFixedVector/*, default*/};
	__property Gr32::TFixedPoint FixedVectorS[int X][int Y] = {read=GetFixedVectorS, write=SetFixedVectorS};
	__property Gr32::TFixedPoint FixedVectorX[Gr32::TFixed X][Gr32::TFixed Y] = {read=GetFixedVectorX, write=SetFixedVectorX};
	__property Gr32::TFixedPoint FixedVectorXS[Gr32::TFixed X][Gr32::TFixed Y] = {read=GetFixedVectorXS, write=SetFixedVectorXS};
	__property Gr32::TFloatPoint FloatVector[int X][int Y] = {read=GetFloatVector, write=SetFloatVector};
	__property Gr32::TFloatPoint FloatVectorS[int X][int Y] = {read=GetFloatVectorS, write=SetFloatVectorS};
	__property Gr32::TFloatPoint FloatVectorF[float X][float Y] = {read=GetFloatVectorF, write=SetFloatVectorF};
	__property Gr32::TFloatPoint FloatVectorFS[float X][float Y] = {read=GetFloatVectorFS, write=SetFloatVectorFS};
	
__published:
	__property TVectorCombineMode VectorCombineMode = {read=FVectorCombineMode, write=SetVectorCombineMode, nodefault};
	__property TVectorCombineEvent OnVectorCombine = {read=FOnVectorCombine, write=FOnVectorCombine};
public:
	#pragma option push -w-inl
	/* TThreadPersistent.Create */ inline __fastcall virtual TVectorMap(void) : Gr32::TCustomMap() { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Gr32_vectormaps */
using namespace Gr32_vectormaps;
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// Gr32_vectormaps
