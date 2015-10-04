// Borland C++ Builder
// Copyright (c) 1995, 2005 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'Gr32_ordinalmaps.pas' rev: 10.00

#ifndef Gr32_ordinalmapsHPP
#define Gr32_ordinalmapsHPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member functions
#pragma pack(push,8)
#include <System.hpp>	// Pascal unit
#include <Sysinit.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <Controls.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Sysutils.hpp>	// Pascal unit
#include <Gr32.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Gr32_ordinalmaps
{
//-- type declarations -------------------------------------------------------
#pragma option push -b-
enum TConversionType { ctRed, ctGreen, ctBlue, ctAlpha, ctUniformRGB, ctWeightedRGB };
#pragma option pop

class DELPHICLASS TBooleanMap;
class PASCALIMPLEMENTATION TBooleanMap : public Gr32::TCustomMap 
{
	typedef Gr32::TCustomMap inherited;
	
private:
	DynamicArray<Byte >  FBits;
	bool __fastcall GetValue(int X, int Y);
	void __fastcall SetValue(int X, int Y, const bool Value);
	Gr32::PByteArray __fastcall GetBits(void);
	
protected:
	virtual void __fastcall ChangeSize(int &Width, int &Height, int NewWidth, int NewHeight);
	
public:
	__fastcall virtual ~TBooleanMap(void);
	virtual bool __fastcall Empty(void);
	void __fastcall Clear(Byte FillValue);
	void __fastcall ToggleBit(int X, int Y);
	__property bool Value[int X][int Y] = {read=GetValue, write=SetValue/*, default*/};
	__property Gr32::PByteArray Bits = {read=GetBits};
public:
	#pragma option push -w-inl
	/* TThreadPersistent.Create */ inline __fastcall virtual TBooleanMap(void) : Gr32::TCustomMap() { }
	#pragma option pop
	
};


class DELPHICLASS TByteMap;
class PASCALIMPLEMENTATION TByteMap : public Gr32::TCustomMap 
{
	typedef Gr32::TCustomMap inherited;
	
private:
	DynamicArray<Byte >  FBits;
	Byte __fastcall GetValue(int X, int Y);
	System::PByte __fastcall GetValPtr(int X, int Y);
	void __fastcall SetValue(int X, int Y, Byte Value);
	Gr32::PByteArray __fastcall GetBits(void);
	
protected:
	virtual void __fastcall AssignTo(Classes::TPersistent* Dst);
	virtual void __fastcall ChangeSize(int &Width, int &Height, int NewWidth, int NewHeight);
	
public:
	__fastcall virtual ~TByteMap(void);
	virtual void __fastcall Assign(Classes::TPersistent* Source);
	virtual bool __fastcall Empty(void);
	void __fastcall Clear(Byte FillValue);
	void __fastcall ReadFrom(Gr32::TBitmap32* Source, TConversionType Conversion);
	void __fastcall WriteTo(Gr32::TBitmap32* Dest, TConversionType Conversion)/* overload */;
	void __fastcall WriteTo(Gr32::TBitmap32* Dest, Gr32::TColor32 const * Palette)/* overload */;
	__property Gr32::PByteArray Bits = {read=GetBits};
	__property System::PByte ValPtr[int X][int Y] = {read=GetValPtr};
	__property Byte Value[int X][int Y] = {read=GetValue, write=SetValue/*, default*/};
public:
	#pragma option push -w-inl
	/* TThreadPersistent.Create */ inline __fastcall virtual TByteMap(void) : Gr32::TCustomMap() { }
	#pragma option pop
	
};


class DELPHICLASS TWordMap;
class PASCALIMPLEMENTATION TWordMap : public Gr32::TCustomMap 
{
	typedef Gr32::TCustomMap inherited;
	
private:
	DynamicArray<Word >  FBits;
	PWORD __fastcall GetValPtr(int X, int Y);
	Word __fastcall GetValue(int X, int Y);
	void __fastcall SetValue(int X, int Y, const Word Value);
	Gr32::PWordArray __fastcall GetBits(void);
	
protected:
	virtual void __fastcall ChangeSize(int &Width, int &Height, int NewWidth, int NewHeight);
	
public:
	__fastcall virtual ~TWordMap(void);
	virtual bool __fastcall Empty(void);
	void __fastcall Clear(Word FillValue);
	__property PWORD ValPtr[int X][int Y] = {read=GetValPtr};
	__property Word Value[int X][int Y] = {read=GetValue, write=SetValue/*, default*/};
	__property Gr32::PWordArray Bits = {read=GetBits};
public:
	#pragma option push -w-inl
	/* TThreadPersistent.Create */ inline __fastcall virtual TWordMap(void) : Gr32::TCustomMap() { }
	#pragma option pop
	
};


class DELPHICLASS TIntegerMap;
class PASCALIMPLEMENTATION TIntegerMap : public Gr32::TCustomMap 
{
	typedef Gr32::TCustomMap inherited;
	
private:
	DynamicArray<int >  FBits;
	System::PInteger __fastcall GetValPtr(int X, int Y);
	int __fastcall GetValue(int X, int Y);
	void __fastcall SetValue(int X, int Y, const int Value);
	Gr32::PIntegerArray __fastcall GetBits(void);
	
protected:
	virtual void __fastcall ChangeSize(int &Width, int &Height, int NewWidth, int NewHeight);
	
public:
	__fastcall virtual ~TIntegerMap(void);
	virtual bool __fastcall Empty(void);
	void __fastcall Clear(int FillValue);
	__property System::PInteger ValPtr[int X][int Y] = {read=GetValPtr};
	__property int Value[int X][int Y] = {read=GetValue, write=SetValue/*, default*/};
	__property Gr32::PIntegerArray Bits = {read=GetBits};
public:
	#pragma option push -w-inl
	/* TThreadPersistent.Create */ inline __fastcall virtual TIntegerMap(void) : Gr32::TCustomMap() { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Gr32_ordinalmaps */
using namespace Gr32_ordinalmaps;
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// Gr32_ordinalmaps
