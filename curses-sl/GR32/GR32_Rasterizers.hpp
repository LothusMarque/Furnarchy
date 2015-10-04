// Borland C++ Builder
// Copyright (c) 1995, 2005 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'Gr32_rasterizers.pas' rev: 10.00

#ifndef Gr32_rasterizersHPP
#define Gr32_rasterizersHPP

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
#include <Gr32_blend.hpp>	// Pascal unit
#include <Gr32_ordinalmaps.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Gr32_rasterizers
{
//-- type declarations -------------------------------------------------------
typedef void __fastcall (__closure *TAssignColor)(Gr32::TColor32 &Dst, Gr32::TColor32 Src);

struct TCombineInfo;
typedef TCombineInfo *PCombineInfo;

struct TCombineInfo
{
	
public:
	int SrcAlpha;
	Gr32::TDrawMode DrawMode;
	Gr32::TCombineMode CombineMode;
	Gr32::TPixelCombineEvent CombineCallBack;
	Gr32::TColor32 TransparentColor;
} ;

class DELPHICLASS TRasterizer;
class PASCALIMPLEMENTATION TRasterizer : public Gr32::TThreadPersistent 
{
	typedef Gr32::TThreadPersistent inherited;
	
private:
	Gr32::TCustomSampler* FSampler;
	int FSrcAlpha;
	Gr32_blend::TBlendMemEx FBlendMemEx;
	Gr32::TPixelCombineEvent FCombineCallBack;
	TAssignColor FAssignColor;
	Gr32::TColor32 FTransparentColor;
	void __fastcall SetSampler(const Gr32::TCustomSampler* Value);
	void __fastcall SetCombineInfo(const TCombineInfo &CombineInfo);
	void __fastcall AssignColorOpaque(Gr32::TColor32 &Dst, Gr32::TColor32 Src);
	void __fastcall AssignColorBlend(Gr32::TColor32 &Dst, Gr32::TColor32 Src);
	void __fastcall AssignColorCustom(Gr32::TColor32 &Dst, Gr32::TColor32 Src);
	void __fastcall AssignColorTransparent(Gr32::TColor32 &Dst, Gr32::TColor32 Src);
	
protected:
	virtual void __fastcall AssignTo(Classes::TPersistent* Dst);
	virtual void __fastcall DoRasterize(Gr32::TBitmap32* Dst, const Types::TRect &DstRect) = 0 ;
	__property TAssignColor AssignColor = {read=FAssignColor, write=FAssignColor};
	
public:
	__fastcall virtual TRasterizer(void);
	virtual void __fastcall Assign(Classes::TPersistent* Source);
	void __fastcall Rasterize(Gr32::TBitmap32* Dst)/* overload */;
	void __fastcall Rasterize(Gr32::TBitmap32* Dst, const Types::TRect &DstRect)/* overload */;
	void __fastcall Rasterize(Gr32::TBitmap32* Dst, const Types::TRect &DstRect, const TCombineInfo &CombineInfo)/* overload */;
	void __fastcall Rasterize(Gr32::TBitmap32* Dst, const Types::TRect &DstRect, Gr32::TBitmap32* Src)/* overload */;
	
__published:
	__property Gr32::TCustomSampler* Sampler = {read=FSampler, write=SetSampler};
public:
	#pragma option push -w-inl
	/* TThreadPersistent.Destroy */ inline __fastcall virtual ~TRasterizer(void) { }
	#pragma option pop
	
};


typedef TMetaClass* TRasterizerClass;

class DELPHICLASS TRegularRasterizer;
class PASCALIMPLEMENTATION TRegularRasterizer : public TRasterizer 
{
	typedef TRasterizer inherited;
	
private:
	int FUpdateRowCount;
	
protected:
	virtual void __fastcall DoRasterize(Gr32::TBitmap32* Dst, const Types::TRect &DstRect);
	
public:
	__fastcall virtual TRegularRasterizer(void);
	
__published:
	__property int UpdateRowCount = {read=FUpdateRowCount, write=FUpdateRowCount, nodefault};
public:
	#pragma option push -w-inl
	/* TThreadPersistent.Destroy */ inline __fastcall virtual ~TRegularRasterizer(void) { }
	#pragma option pop
	
};


class DELPHICLASS TSwizzlingRasterizer;
class PASCALIMPLEMENTATION TSwizzlingRasterizer : public TRasterizer 
{
	typedef TRasterizer inherited;
	
private:
	int FBlockSize;
	void __fastcall SetBlockSize(const int Value);
	
protected:
	virtual void __fastcall DoRasterize(Gr32::TBitmap32* Dst, const Types::TRect &DstRect);
	
public:
	__fastcall virtual TSwizzlingRasterizer(void);
	
__published:
	__property int BlockSize = {read=FBlockSize, write=SetBlockSize, default=3};
public:
	#pragma option push -w-inl
	/* TThreadPersistent.Destroy */ inline __fastcall virtual ~TSwizzlingRasterizer(void) { }
	#pragma option pop
	
};


class DELPHICLASS TProgressiveRasterizer;
class PASCALIMPLEMENTATION TProgressiveRasterizer : public TRasterizer 
{
	typedef TRasterizer inherited;
	
private:
	int FSteps;
	bool FUpdateRows;
	void __fastcall SetSteps(const int Value);
	void __fastcall SetUpdateRows(const bool Value);
	
protected:
	virtual void __fastcall DoRasterize(Gr32::TBitmap32* Dst, const Types::TRect &DstRect);
	
public:
	__fastcall virtual TProgressiveRasterizer(void);
	
__published:
	__property int Steps = {read=FSteps, write=SetSteps, default=4};
	__property bool UpdateRows = {read=FUpdateRows, write=SetUpdateRows, default=1};
public:
	#pragma option push -w-inl
	/* TThreadPersistent.Destroy */ inline __fastcall virtual ~TProgressiveRasterizer(void) { }
	#pragma option pop
	
};


class DELPHICLASS TTesseralRasterizer;
class PASCALIMPLEMENTATION TTesseralRasterizer : public TRasterizer 
{
	typedef TRasterizer inherited;
	
protected:
	virtual void __fastcall DoRasterize(Gr32::TBitmap32* Dst, const Types::TRect &DstRect);
public:
	#pragma option push -w-inl
	/* TRasterizer.Create */ inline __fastcall virtual TTesseralRasterizer(void) : TRasterizer() { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TThreadPersistent.Destroy */ inline __fastcall virtual ~TTesseralRasterizer(void) { }
	#pragma option pop
	
};


class DELPHICLASS TContourRasterizer;
class PASCALIMPLEMENTATION TContourRasterizer : public TRasterizer 
{
	typedef TRasterizer inherited;
	
protected:
	virtual void __fastcall DoRasterize(Gr32::TBitmap32* Dst, const Types::TRect &DstRect);
public:
	#pragma option push -w-inl
	/* TRasterizer.Create */ inline __fastcall virtual TContourRasterizer(void) : TRasterizer() { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TThreadPersistent.Destroy */ inline __fastcall virtual ~TContourRasterizer(void) { }
	#pragma option pop
	
};


class DELPHICLASS TMultithreadedRegularRasterizer;
class PASCALIMPLEMENTATION TMultithreadedRegularRasterizer : public TRasterizer 
{
	typedef TRasterizer inherited;
	
protected:
	virtual void __fastcall DoRasterize(Gr32::TBitmap32* Dst, const Types::TRect &DstRect);
public:
	#pragma option push -w-inl
	/* TRasterizer.Create */ inline __fastcall virtual TMultithreadedRegularRasterizer(void) : TRasterizer() { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TThreadPersistent.Destroy */ inline __fastcall virtual ~TMultithreadedRegularRasterizer(void) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE TCombineInfo DEFAULT_COMBINE_INFO;
extern PACKAGE TMetaClass* DefaultRasterizerClass;
extern PACKAGE int NumberOfProcessors;
extern PACKAGE TCombineInfo __fastcall CombineInfo(Gr32::TBitmap32* Bitmap);

}	/* namespace Gr32_rasterizers */
using namespace Gr32_rasterizers;
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// Gr32_rasterizers
