// Borland C++ Builder
// Copyright (c) 1995, 2005 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'Gr32_resamplers.pas' rev: 10.00

#ifndef Gr32_resamplersHPP
#define Gr32_resamplersHPP

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
#include <Gr32_transforms.hpp>	// Pascal unit
#include <Gr32_containers.hpp>	// Pascal unit
#include <Gr32_ordinalmaps.hpp>	// Pascal unit
#include <Gr32_blend.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Gr32_resamplers
{
//-- type declarations -------------------------------------------------------
typedef int TKernelEntry[9];

typedef int *PKernelEntry;

typedef DynamicArray<DynamicArray<int > >  TArrayOfKernelEntry;

typedef DynamicArray<int >  TKernelEntryArray[1];

typedef Gr32::TArrayOfInteger *PKernelEntryArray;

typedef float __fastcall (__closure *TFilterMethod)(float Value);

class DELPHICLASS EBitmapException;
class PASCALIMPLEMENTATION EBitmapException : public Sysutils::Exception 
{
	typedef Sysutils::Exception inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EBitmapException(const AnsiString Msg) : Sysutils::Exception(Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EBitmapException(const AnsiString Msg, System::TVarRec const * Args, const int Args_Size) : Sysutils::Exception(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EBitmapException(int Ident)/* overload */ : Sysutils::Exception(Ident) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EBitmapException(int Ident, System::TVarRec const * Args, const int Args_Size)/* overload */ : Sysutils::Exception(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EBitmapException(const AnsiString Msg, int AHelpContext) : Sysutils::Exception(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EBitmapException(const AnsiString Msg, System::TVarRec const * Args, const int Args_Size, int AHelpContext) : Sysutils::Exception(Msg, Args, Args_Size, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EBitmapException(int Ident, int AHelpContext)/* overload */ : Sysutils::Exception(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EBitmapException(System::PResStringRec ResStringRec, System::TVarRec const * Args, const int Args_Size, int AHelpContext)/* overload */ : Sysutils::Exception(ResStringRec, Args, Args_Size, AHelpContext) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EBitmapException(void) { }
	#pragma option pop
	
};


class DELPHICLASS ESrcInvalidException;
class PASCALIMPLEMENTATION ESrcInvalidException : public Sysutils::Exception 
{
	typedef Sysutils::Exception inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall ESrcInvalidException(const AnsiString Msg) : Sysutils::Exception(Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall ESrcInvalidException(const AnsiString Msg, System::TVarRec const * Args, const int Args_Size) : Sysutils::Exception(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall ESrcInvalidException(int Ident)/* overload */ : Sysutils::Exception(Ident) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall ESrcInvalidException(int Ident, System::TVarRec const * Args, const int Args_Size)/* overload */ : Sysutils::Exception(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall ESrcInvalidException(const AnsiString Msg, int AHelpContext) : Sysutils::Exception(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall ESrcInvalidException(const AnsiString Msg, System::TVarRec const * Args, const int Args_Size, int AHelpContext) : Sysutils::Exception(Msg, Args, Args_Size, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall ESrcInvalidException(int Ident, int AHelpContext)/* overload */ : Sysutils::Exception(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall ESrcInvalidException(System::PResStringRec ResStringRec, System::TVarRec const * Args, const int Args_Size, int AHelpContext)/* overload */ : Sysutils::Exception(ResStringRec, Args, Args_Size, AHelpContext) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~ESrcInvalidException(void) { }
	#pragma option pop
	
};


class DELPHICLASS ENestedException;
class PASCALIMPLEMENTATION ENestedException : public Sysutils::Exception 
{
	typedef Sysutils::Exception inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall ENestedException(const AnsiString Msg) : Sysutils::Exception(Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall ENestedException(const AnsiString Msg, System::TVarRec const * Args, const int Args_Size) : Sysutils::Exception(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall ENestedException(int Ident)/* overload */ : Sysutils::Exception(Ident) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall ENestedException(int Ident, System::TVarRec const * Args, const int Args_Size)/* overload */ : Sysutils::Exception(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall ENestedException(const AnsiString Msg, int AHelpContext) : Sysutils::Exception(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall ENestedException(const AnsiString Msg, System::TVarRec const * Args, const int Args_Size, int AHelpContext) : Sysutils::Exception(Msg, Args, Args_Size, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall ENestedException(int Ident, int AHelpContext)/* overload */ : Sysutils::Exception(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall ENestedException(System::PResStringRec ResStringRec, System::TVarRec const * Args, const int Args_Size, int AHelpContext)/* overload */ : Sysutils::Exception(ResStringRec, Args, Args_Size, AHelpContext) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~ENestedException(void) { }
	#pragma option pop
	
};


typedef Gr32::TColor32 __fastcall (__closure *TGetSampleInt)(int X, int Y);

typedef Gr32::TColor32 __fastcall (__closure *TGetSampleFloat)(float X, float Y);

typedef Gr32::TColor32 __fastcall (__closure *TGetSampleFixed)(Gr32::TFixed X, Gr32::TFixed Y);

class DELPHICLASS TCustomKernel;
class PASCALIMPLEMENTATION TCustomKernel : public Classes::TPersistent 
{
	typedef Classes::TPersistent inherited;
	
protected:
	Gr32::TNotifiablePersistent* FObserver;
	virtual void __fastcall AssignTo(Classes::TPersistent* Dst);
	virtual bool __fastcall RangeCheck(void);
	
public:
	__fastcall virtual TCustomKernel(void);
	void __fastcall Changed(void);
	virtual float __fastcall Filter(float Value) = 0 ;
	virtual float __fastcall GetWidth(void) = 0 ;
	__property Gr32::TNotifiablePersistent* Observer = {read=FObserver};
public:
	#pragma option push -w-inl
	/* TPersistent.Destroy */ inline __fastcall virtual ~TCustomKernel(void) { }
	#pragma option pop
	
};


typedef TMetaClass* TCustomKernelClass;

class DELPHICLASS TBoxKernel;
class PASCALIMPLEMENTATION TBoxKernel : public TCustomKernel 
{
	typedef TCustomKernel inherited;
	
public:
	virtual float __fastcall Filter(float Value);
	virtual float __fastcall GetWidth(void);
public:
	#pragma option push -w-inl
	/* TCustomKernel.Create */ inline __fastcall virtual TBoxKernel(void) : TCustomKernel() { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TPersistent.Destroy */ inline __fastcall virtual ~TBoxKernel(void) { }
	#pragma option pop
	
};


class DELPHICLASS TLinearKernel;
class PASCALIMPLEMENTATION TLinearKernel : public TCustomKernel 
{
	typedef TCustomKernel inherited;
	
public:
	virtual float __fastcall Filter(float Value);
	virtual float __fastcall GetWidth(void);
public:
	#pragma option push -w-inl
	/* TCustomKernel.Create */ inline __fastcall virtual TLinearKernel(void) : TCustomKernel() { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TPersistent.Destroy */ inline __fastcall virtual ~TLinearKernel(void) { }
	#pragma option pop
	
};


class DELPHICLASS TCosineKernel;
class PASCALIMPLEMENTATION TCosineKernel : public TCustomKernel 
{
	typedef TCustomKernel inherited;
	
public:
	virtual float __fastcall Filter(float Value);
	virtual float __fastcall GetWidth(void);
public:
	#pragma option push -w-inl
	/* TCustomKernel.Create */ inline __fastcall virtual TCosineKernel(void) : TCustomKernel() { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TPersistent.Destroy */ inline __fastcall virtual ~TCosineKernel(void) { }
	#pragma option pop
	
};


class DELPHICLASS TSplineKernel;
class PASCALIMPLEMENTATION TSplineKernel : public TCustomKernel 
{
	typedef TCustomKernel inherited;
	
protected:
	virtual bool __fastcall RangeCheck(void);
	
public:
	virtual float __fastcall Filter(float Value);
	virtual float __fastcall GetWidth(void);
public:
	#pragma option push -w-inl
	/* TCustomKernel.Create */ inline __fastcall virtual TSplineKernel(void) : TCustomKernel() { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TPersistent.Destroy */ inline __fastcall virtual ~TSplineKernel(void) { }
	#pragma option pop
	
};


class DELPHICLASS TMitchellKernel;
class PASCALIMPLEMENTATION TMitchellKernel : public TCustomKernel 
{
	typedef TCustomKernel inherited;
	
protected:
	virtual bool __fastcall RangeCheck(void);
	
public:
	virtual float __fastcall Filter(float Value);
	virtual float __fastcall GetWidth(void);
public:
	#pragma option push -w-inl
	/* TCustomKernel.Create */ inline __fastcall virtual TMitchellKernel(void) : TCustomKernel() { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TPersistent.Destroy */ inline __fastcall virtual ~TMitchellKernel(void) { }
	#pragma option pop
	
};


class DELPHICLASS TCubicKernel;
class PASCALIMPLEMENTATION TCubicKernel : public TCustomKernel 
{
	typedef TCustomKernel inherited;
	
private:
	float FCoeff;
	void __fastcall SetCoeff(const float Value);
	
protected:
	virtual bool __fastcall RangeCheck(void);
	
public:
	__fastcall virtual TCubicKernel(void);
	virtual float __fastcall Filter(float Value);
	virtual float __fastcall GetWidth(void);
	
__published:
	__property float Coeff = {read=FCoeff, write=SetCoeff};
public:
	#pragma option push -w-inl
	/* TPersistent.Destroy */ inline __fastcall virtual ~TCubicKernel(void) { }
	#pragma option pop
	
};


class DELPHICLASS THermiteKernel;
class PASCALIMPLEMENTATION THermiteKernel : public TCustomKernel 
{
	typedef TCustomKernel inherited;
	
private:
	float FBias;
	float FTension;
	void __fastcall SetBias(const float Value);
	void __fastcall SetTension(const float Value);
	
protected:
	virtual bool __fastcall RangeCheck(void);
	
public:
	__fastcall virtual THermiteKernel(void);
	virtual float __fastcall Filter(float Value);
	virtual float __fastcall GetWidth(void);
	
__published:
	__property float Bias = {read=FBias, write=SetBias};
	__property float Tension = {read=FTension, write=SetTension};
public:
	#pragma option push -w-inl
	/* TPersistent.Destroy */ inline __fastcall virtual ~THermiteKernel(void) { }
	#pragma option pop
	
};


class DELPHICLASS TWindowedSincKernel;
class PASCALIMPLEMENTATION TWindowedSincKernel : public TCustomKernel 
{
	typedef TCustomKernel inherited;
	
private:
	float FWidth;
	
protected:
	virtual bool __fastcall RangeCheck(void);
	virtual float __fastcall Window(float Value) = 0 ;
	
public:
	__fastcall virtual TWindowedSincKernel(void);
	virtual float __fastcall Filter(float Value);
	void __fastcall SetWidth(float Value);
	virtual float __fastcall GetWidth(void);
	
__published:
	__property float Width = {read=FWidth, write=SetWidth};
public:
	#pragma option push -w-inl
	/* TPersistent.Destroy */ inline __fastcall virtual ~TWindowedSincKernel(void) { }
	#pragma option pop
	
};


class DELPHICLASS TLanczosKernel;
class PASCALIMPLEMENTATION TLanczosKernel : public TWindowedSincKernel 
{
	typedef TWindowedSincKernel inherited;
	
protected:
	virtual float __fastcall Window(float Value);
public:
	#pragma option push -w-inl
	/* TWindowedSincKernel.Create */ inline __fastcall virtual TLanczosKernel(void) : TWindowedSincKernel() { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TPersistent.Destroy */ inline __fastcall virtual ~TLanczosKernel(void) { }
	#pragma option pop
	
};


class DELPHICLASS TGaussianKernel;
class PASCALIMPLEMENTATION TGaussianKernel : public TWindowedSincKernel 
{
	typedef TWindowedSincKernel inherited;
	
private:
	float FSigma;
	void __fastcall SetSigma(const float Value);
	
protected:
	virtual float __fastcall Window(float Value);
	
public:
	__fastcall virtual TGaussianKernel(void);
	
__published:
	__property float Sigma = {read=FSigma, write=SetSigma};
public:
	#pragma option push -w-inl
	/* TPersistent.Destroy */ inline __fastcall virtual ~TGaussianKernel(void) { }
	#pragma option pop
	
};


class DELPHICLASS TBlackmanKernel;
class PASCALIMPLEMENTATION TBlackmanKernel : public TWindowedSincKernel 
{
	typedef TWindowedSincKernel inherited;
	
protected:
	virtual float __fastcall Window(float Value);
public:
	#pragma option push -w-inl
	/* TWindowedSincKernel.Create */ inline __fastcall virtual TBlackmanKernel(void) : TWindowedSincKernel() { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TPersistent.Destroy */ inline __fastcall virtual ~TBlackmanKernel(void) { }
	#pragma option pop
	
};


class DELPHICLASS THannKernel;
class PASCALIMPLEMENTATION THannKernel : public TWindowedSincKernel 
{
	typedef TWindowedSincKernel inherited;
	
protected:
	virtual float __fastcall Window(float Value);
public:
	#pragma option push -w-inl
	/* TWindowedSincKernel.Create */ inline __fastcall virtual THannKernel(void) : TWindowedSincKernel() { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TPersistent.Destroy */ inline __fastcall virtual ~THannKernel(void) { }
	#pragma option pop
	
};


class DELPHICLASS THammingKernel;
class PASCALIMPLEMENTATION THammingKernel : public TWindowedSincKernel 
{
	typedef TWindowedSincKernel inherited;
	
protected:
	virtual float __fastcall Window(float Value);
public:
	#pragma option push -w-inl
	/* TWindowedSincKernel.Create */ inline __fastcall virtual THammingKernel(void) : TWindowedSincKernel() { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TPersistent.Destroy */ inline __fastcall virtual ~THammingKernel(void) { }
	#pragma option pop
	
};


class DELPHICLASS TSinshKernel;
class PASCALIMPLEMENTATION TSinshKernel : public TCustomKernel 
{
	typedef TCustomKernel inherited;
	
private:
	float FWidth;
	float FCoeff;
	void __fastcall SetCoeff(const float Value);
	
protected:
	virtual bool __fastcall RangeCheck(void);
	
public:
	__fastcall virtual TSinshKernel(void);
	void __fastcall SetWidth(float Value);
	virtual float __fastcall GetWidth(void);
	virtual float __fastcall Filter(float Value);
	
__published:
	__property float Coeff = {read=FCoeff, write=SetCoeff};
	__property float Width = {read=GetWidth, write=SetWidth};
public:
	#pragma option push -w-inl
	/* TPersistent.Destroy */ inline __fastcall virtual ~TSinshKernel(void) { }
	#pragma option pop
	
};


typedef TMetaClass* TTransformerClass;

#pragma option push -b-
enum TPixelAccessMode { pamUnsafe, pamSafe, pamWrap, pamTransparentEdge };
#pragma option pop

class DELPHICLASS TBitmap32Resampler;
class PASCALIMPLEMENTATION TBitmap32Resampler : public Gr32::TCustomResampler 
{
	typedef Gr32::TCustomResampler inherited;
	
private:
	Gr32::TBitmap32* FBitmap;
	#pragma pack(push,1)
	Types::TRect FClipRect;
	#pragma pack(pop)
	TMetaClass* FTransformerClass;
	TPixelAccessMode FPixelAccessMode;
	void __fastcall SetPixelAccessMode(const TPixelAccessMode Value);
	
protected:
	virtual void __fastcall AssignTo(Classes::TPersistent* Dst);
	
public:
	__fastcall virtual TBitmap32Resampler(void)/* overload */;
	__fastcall virtual TBitmap32Resampler(Gr32::TBitmap32* ABitmap)/* overload */;
	virtual void __fastcall Changed(void);
	virtual void __fastcall PrepareSampling(void);
	virtual bool __fastcall HasBounds(void);
	virtual Types::TRect __fastcall GetSampleBounds();
	__property Gr32::TBitmap32* Bitmap = {read=FBitmap, write=FBitmap};
	__property TMetaClass* TransformerClass = {read=FTransformerClass, write=FTransformerClass};
	
__published:
	__property TPixelAccessMode PixelAccessMode = {read=FPixelAccessMode, write=SetPixelAccessMode, default=1};
public:
	#pragma option push -w-inl
	/* TPersistent.Destroy */ inline __fastcall virtual ~TBitmap32Resampler(void) { }
	#pragma option pop
	
};


typedef TMetaClass* TBitmap32ResamplerClass;

class DELPHICLASS TNearestResampler;
class PASCALIMPLEMENTATION TNearestResampler : public TBitmap32Resampler 
{
	typedef TBitmap32Resampler inherited;
	
private:
	TGetSampleInt FGetSampleInt;
	
protected:
	Gr32::TColor32 __fastcall GetPixelTransparentEdge(int X, int Y);
	virtual float __fastcall GetWidth(void);
	virtual void __fastcall Resample(Gr32::TBitmap32* Dst, const Types::TRect &DstRect, const Types::TRect &DstClip, Gr32::TBitmap32* Src, const Types::TRect &SrcRect, Gr32::TDrawMode CombineOp, Gr32::TPixelCombineEvent CombineCallBack);
	
public:
	__fastcall virtual TNearestResampler(void)/* overload */;
	virtual Gr32::TColor32 __fastcall GetSampleInt(int X, int Y);
	virtual Gr32::TColor32 __fastcall GetSampleFixed(Gr32::TFixed X, Gr32::TFixed Y);
	virtual Gr32::TColor32 __fastcall GetSampleFloat(float X, float Y);
	virtual void __fastcall PrepareSampling(void);
public:
	#pragma option push -w-inl
	/* TPersistent.Destroy */ inline __fastcall virtual ~TNearestResampler(void) { }
	#pragma option pop
	
};


class DELPHICLASS TLinearResampler;
class PASCALIMPLEMENTATION TLinearResampler : public TBitmap32Resampler 
{
	typedef TBitmap32Resampler inherited;
	
private:
	TLinearKernel* FLinearKernel;
	TGetSampleFixed FGetSampleFixed;
	
protected:
	virtual float __fastcall GetWidth(void);
	Gr32::TColor32 __fastcall GetPixelTransparentEdge(Gr32::TFixed X, Gr32::TFixed Y);
	virtual void __fastcall Resample(Gr32::TBitmap32* Dst, const Types::TRect &DstRect, const Types::TRect &DstClip, Gr32::TBitmap32* Src, const Types::TRect &SrcRect, Gr32::TDrawMode CombineOp, Gr32::TPixelCombineEvent CombineCallBack);
	
public:
	__fastcall virtual TLinearResampler(void)/* overload */;
	__fastcall virtual ~TLinearResampler(void);
	virtual Gr32::TColor32 __fastcall GetSampleFixed(Gr32::TFixed X, Gr32::TFixed Y);
	virtual Gr32::TColor32 __fastcall GetSampleFloat(float X, float Y);
	virtual void __fastcall PrepareSampling(void);
};


class DELPHICLASS TDraftResampler;
class PASCALIMPLEMENTATION TDraftResampler : public TLinearResampler 
{
	typedef TLinearResampler inherited;
	
protected:
	virtual void __fastcall Resample(Gr32::TBitmap32* Dst, const Types::TRect &DstRect, const Types::TRect &DstClip, Gr32::TBitmap32* Src, const Types::TRect &SrcRect, Gr32::TDrawMode CombineOp, Gr32::TPixelCombineEvent CombineCallBack);
public:
	#pragma option push -w-inl
	/* TLinearResampler.Create */ inline __fastcall virtual TDraftResampler(void)/* overload */ : TLinearResampler() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TLinearResampler.Destroy */ inline __fastcall virtual ~TDraftResampler(void) { }
	#pragma option pop
	
};


#pragma option push -b-
enum TKernelMode { kmDynamic, kmTableNearest, kmTableLinear };
#pragma option pop

class DELPHICLASS TKernelResampler;
class PASCALIMPLEMENTATION TKernelResampler : public TBitmap32Resampler 
{
	typedef TBitmap32Resampler inherited;
	
private:
	TCustomKernel* FKernel;
	TKernelMode FKernelMode;
	Gr32_ordinalmaps::TIntegerMap* FWeightTable;
	int FTableSize;
	Gr32::TColor32 FOuterColor;
	void __fastcall SetKernel(const TCustomKernel* Value);
	AnsiString __fastcall GetKernelClassName();
	void __fastcall SetKernelClassName(AnsiString Value);
	void __fastcall SetKernelMode(const TKernelMode Value);
	void __fastcall SetTableSize(int Value);
	
protected:
	virtual float __fastcall GetWidth(void);
	
public:
	__fastcall virtual TKernelResampler(void)/* overload */;
	__fastcall virtual ~TKernelResampler(void);
	virtual Gr32::TColor32 __fastcall GetSampleFloat(float X, float Y);
	virtual void __fastcall Resample(Gr32::TBitmap32* Dst, const Types::TRect &DstRect, const Types::TRect &DstClip, Gr32::TBitmap32* Src, const Types::TRect &SrcRect, Gr32::TDrawMode CombineOp, Gr32::TPixelCombineEvent CombineCallBack);
	virtual void __fastcall PrepareSampling(void);
	virtual void __fastcall FinalizeSampling(void);
	
__published:
	__property AnsiString KernelClassName = {read=GetKernelClassName, write=SetKernelClassName};
	__property TCustomKernel* Kernel = {read=FKernel, write=SetKernel};
	__property TKernelMode KernelMode = {read=FKernelMode, write=SetKernelMode, nodefault};
	__property int TableSize = {read=FTableSize, write=SetTableSize, nodefault};
};


class DELPHICLASS TNestedSampler;
class PASCALIMPLEMENTATION TNestedSampler : public Gr32::TCustomSampler 
{
	typedef Gr32::TCustomSampler inherited;
	
private:
	Gr32::TCustomSampler* FSampler;
	TGetSampleInt FGetSampleInt;
	TGetSampleFixed FGetSampleFixed;
	TGetSampleFloat FGetSampleFloat;
	void __fastcall SetSampler(const Gr32::TCustomSampler* Value);
	
protected:
	virtual void __fastcall AssignTo(Classes::TPersistent* Dst);
	
public:
	__fastcall virtual TNestedSampler(Gr32::TCustomSampler* ASampler);
	virtual void __fastcall PrepareSampling(void);
	virtual void __fastcall FinalizeSampling(void);
	virtual bool __fastcall HasBounds(void);
	virtual Types::TRect __fastcall GetSampleBounds();
	
__published:
	__property Gr32::TCustomSampler* Sampler = {read=FSampler, write=SetSampler};
public:
	#pragma option push -w-inl
	/* TPersistent.Destroy */ inline __fastcall virtual ~TNestedSampler(void) { }
	#pragma option pop
	
};


typedef void __fastcall (__closure *TReverseTransformInt)(int DstX, int DstY, /* out */ int &SrcX, /* out */ int &SrcY);

typedef void __fastcall (__closure *TReverseTransformFixed)(Gr32::TFixed DstX, Gr32::TFixed DstY, /* out */ Gr32::TFixed &SrcX, /* out */ Gr32::TFixed &SrcY);

typedef void __fastcall (__closure *TReverseTransformFloat)(float DstX, float DstY, /* out */ float &SrcX, /* out */ float &SrcY);

class DELPHICLASS TTransformer;
class PASCALIMPLEMENTATION TTransformer : public TNestedSampler 
{
	typedef TNestedSampler inherited;
	
private:
	Gr32_transforms::TTransformation* FTransformation;
	TReverseTransformInt FTransformationReverseTransformInt;
	TReverseTransformFixed FTransformationReverseTransformFixed;
	TReverseTransformFloat FTransformationReverseTransformFloat;
	void __fastcall SetTransformation(const Gr32_transforms::TTransformation* Value);
	
public:
	__fastcall TTransformer(Gr32::TCustomSampler* ASampler, Gr32_transforms::TTransformation* ATransformation);
	virtual void __fastcall PrepareSampling(void);
	virtual Gr32::TColor32 __fastcall GetSampleInt(int X, int Y);
	virtual Gr32::TColor32 __fastcall GetSampleFixed(Gr32::TFixed X, Gr32::TFixed Y);
	virtual Gr32::TColor32 __fastcall GetSampleFloat(float X, float Y);
	virtual bool __fastcall HasBounds(void);
	virtual Types::TRect __fastcall GetSampleBounds();
	
__published:
	__property Gr32_transforms::TTransformation* Transformation = {read=FTransformation, write=SetTransformation};
public:
	#pragma option push -w-inl
	/* TPersistent.Destroy */ inline __fastcall virtual ~TTransformer(void) { }
	#pragma option pop
	
};


class DELPHICLASS TNearestTransformer;
class PASCALIMPLEMENTATION TNearestTransformer : public TTransformer 
{
	typedef TTransformer inherited;
	
public:
	virtual Gr32::TColor32 __fastcall GetSampleInt(int X, int Y);
	virtual Gr32::TColor32 __fastcall GetSampleFixed(Gr32::TFixed X, Gr32::TFixed Y);
	virtual Gr32::TColor32 __fastcall GetSampleFloat(float X, float Y);
public:
	#pragma option push -w-inl
	/* TTransformer.Create */ inline __fastcall TNearestTransformer(Gr32::TCustomSampler* ASampler, Gr32_transforms::TTransformation* ATransformation) : TTransformer(ASampler, ATransformation) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TPersistent.Destroy */ inline __fastcall virtual ~TNearestTransformer(void) { }
	#pragma option pop
	
};


typedef int TSamplingRange;

class DELPHICLASS TSuperSampler;
class PASCALIMPLEMENTATION TSuperSampler : public TNestedSampler 
{
	typedef TNestedSampler inherited;
	
private:
	TSamplingRange FSamplingY;
	TSamplingRange FSamplingX;
	Gr32::TFixed FDistanceX;
	Gr32::TFixed FDistanceY;
	Gr32::TFixed FOffsetX;
	Gr32::TFixed FOffsetY;
	Gr32::TFixed FScale;
	void __fastcall SetSamplingX(const TSamplingRange Value);
	void __fastcall SetSamplingY(const TSamplingRange Value);
	
public:
	__fastcall virtual TSuperSampler(Gr32::TCustomSampler* Sampler);
	virtual Gr32::TColor32 __fastcall GetSampleFixed(Gr32::TFixed X, Gr32::TFixed Y);
	
__published:
	__property TSamplingRange SamplingX = {read=FSamplingX, write=SetSamplingX, nodefault};
	__property TSamplingRange SamplingY = {read=FSamplingY, write=SetSamplingY, nodefault};
public:
	#pragma option push -w-inl
	/* TPersistent.Destroy */ inline __fastcall virtual ~TSuperSampler(void) { }
	#pragma option pop
	
};


typedef Gr32::TColor32 __fastcall (__closure *TRecurseProc)(Gr32::TFixed X, Gr32::TFixed Y, Gr32::TFixed W, const Gr32::TColor32 C1, const Gr32::TColor32 C2);

class DELPHICLASS TAdaptiveSuperSampler;
class PASCALIMPLEMENTATION TAdaptiveSuperSampler : public TNestedSampler 
{
	typedef TNestedSampler inherited;
	
private:
	Gr32::TFixed FMinOffset;
	int FLevel;
	int FTolerance;
	void __fastcall SetLevel(const int Value);
	Gr32::TColor32 __fastcall DoRecurse(Gr32::TFixed X, Gr32::TFixed Y, Gr32::TFixed Offset, const Gr32::TColor32 A, const Gr32::TColor32 B, const Gr32::TColor32 C, const Gr32::TColor32 D, const Gr32::TColor32 E);
	Gr32::TColor32 __fastcall QuadrantColor(const Gr32::TColor32 C1, const Gr32::TColor32 C2, Gr32::TFixed X, Gr32::TFixed Y, Gr32::TFixed Offset, TRecurseProc Proc);
	Gr32::TColor32 __fastcall RecurseAC(Gr32::TFixed X, Gr32::TFixed Y, Gr32::TFixed Offset, const Gr32::TColor32 A, const Gr32::TColor32 C);
	Gr32::TColor32 __fastcall RecurseBD(Gr32::TFixed X, Gr32::TFixed Y, Gr32::TFixed Offset, const Gr32::TColor32 B, const Gr32::TColor32 D);
	
protected:
	virtual bool __fastcall CompareColors(Gr32::TColor32 C1, Gr32::TColor32 C2);
	
public:
	__fastcall virtual TAdaptiveSuperSampler(Gr32::TCustomSampler* Sampler);
	virtual Gr32::TColor32 __fastcall GetSampleFixed(Gr32::TFixed X, Gr32::TFixed Y);
	
__published:
	__property int Level = {read=FLevel, write=SetLevel, nodefault};
	__property int Tolerance = {read=FTolerance, write=FTolerance, nodefault};
public:
	#pragma option push -w-inl
	/* TPersistent.Destroy */ inline __fastcall virtual ~TAdaptiveSuperSampler(void) { }
	#pragma option pop
	
};


typedef DynamicArray<DynamicArray<Gr32::TFloatPoint > >  GR32_Resamplers__92;

typedef DynamicArray<DynamicArray<DynamicArray<Gr32::TFloatPoint > > >  TFloatSamplePattern;

typedef DynamicArray<DynamicArray<Gr32::TFixedPoint > >  GR32_Resamplers__03;

typedef DynamicArray<DynamicArray<DynamicArray<Gr32::TFixedPoint > > >  TFixedSamplePattern;

class DELPHICLASS TPatternSampler;
class PASCALIMPLEMENTATION TPatternSampler : public TNestedSampler 
{
	typedef TNestedSampler inherited;
	
private:
	DynamicArray<DynamicArray<DynamicArray<Gr32::TFixedPoint > > >  FPattern;
	void __fastcall SetPattern(const TFixedSamplePattern Value);
	
public:
	__fastcall virtual ~TPatternSampler(void);
	virtual Gr32::TColor32 __fastcall GetSampleFixed(Gr32::TFixed X, Gr32::TFixed Y);
	__property TFixedSamplePattern Pattern = {read=FPattern, write=SetPattern};
public:
	#pragma option push -w-inl
	/* TNestedSampler.Create */ inline __fastcall virtual TPatternSampler(Gr32::TCustomSampler* ASampler) : TNestedSampler(ASampler) { }
	#pragma option pop
	
};


struct TBufferEntry;
typedef TBufferEntry *PBufferEntry;

struct TBufferEntry
{
	
public:
	int B;
	int G;
	int R;
	int A;
} ;

class DELPHICLASS TKernelSampler;
class PASCALIMPLEMENTATION TKernelSampler : public TNestedSampler 
{
	typedef TNestedSampler inherited;
	
private:
	Gr32_ordinalmaps::TIntegerMap* FKernel;
	TBufferEntry FStartEntry;
	int FCenterX;
	int FCenterY;
	
protected:
	virtual void __fastcall UpdateBuffer(TBufferEntry &Buffer, Gr32::TColor32 Color, int Weight) = 0 ;
	virtual Gr32::TColor32 __fastcall ConvertBuffer(TBufferEntry &Buffer);
	
public:
	__fastcall virtual TKernelSampler(Gr32::TCustomSampler* ASampler);
	__fastcall virtual ~TKernelSampler(void);
	virtual Gr32::TColor32 __fastcall GetSampleInt(int X, int Y);
	virtual Gr32::TColor32 __fastcall GetSampleFixed(Gr32::TFixed X, Gr32::TFixed Y);
	
__published:
	__property Gr32_ordinalmaps::TIntegerMap* Kernel = {read=FKernel, write=FKernel};
	__property int CenterX = {read=FCenterX, write=FCenterX, nodefault};
	__property int CenterY = {read=FCenterY, write=FCenterY, nodefault};
};


class DELPHICLASS TConvolver;
class PASCALIMPLEMENTATION TConvolver : public TKernelSampler 
{
	typedef TKernelSampler inherited;
	
protected:
	virtual void __fastcall UpdateBuffer(TBufferEntry &Buffer, Gr32::TColor32 Color, int Weight);
public:
	#pragma option push -w-inl
	/* TKernelSampler.Create */ inline __fastcall virtual TConvolver(Gr32::TCustomSampler* ASampler) : TKernelSampler(ASampler) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TKernelSampler.Destroy */ inline __fastcall virtual ~TConvolver(void) { }
	#pragma option pop
	
};


class DELPHICLASS TSelectiveConvolver;
class PASCALIMPLEMENTATION TSelectiveConvolver : public TConvolver 
{
	typedef TConvolver inherited;
	
private:
	Gr32::TColor32 FRefColor;
	int FDelta;
	TBufferEntry FWeightSum;
	
protected:
	virtual void __fastcall UpdateBuffer(TBufferEntry &Buffer, Gr32::TColor32 Color, int Weight);
	virtual Gr32::TColor32 __fastcall ConvertBuffer(TBufferEntry &Buffer);
	
public:
	__fastcall virtual TSelectiveConvolver(Gr32::TCustomSampler* ASampler);
	virtual Gr32::TColor32 __fastcall GetSampleInt(int X, int Y);
	virtual Gr32::TColor32 __fastcall GetSampleFixed(Gr32::TFixed X, Gr32::TFixed Y);
	
__published:
	__property int Delta = {read=FDelta, write=FDelta, nodefault};
public:
	#pragma option push -w-inl
	/* TKernelSampler.Destroy */ inline __fastcall virtual ~TSelectiveConvolver(void) { }
	#pragma option pop
	
};


class DELPHICLASS TMorphologicalSampler;
class PASCALIMPLEMENTATION TMorphologicalSampler : public TKernelSampler 
{
	typedef TKernelSampler inherited;
	
protected:
	virtual Gr32::TColor32 __fastcall ConvertBuffer(TBufferEntry &Buffer);
public:
	#pragma option push -w-inl
	/* TKernelSampler.Create */ inline __fastcall virtual TMorphologicalSampler(Gr32::TCustomSampler* ASampler) : TKernelSampler(ASampler) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TKernelSampler.Destroy */ inline __fastcall virtual ~TMorphologicalSampler(void) { }
	#pragma option pop
	
};


class DELPHICLASS TDilater;
class PASCALIMPLEMENTATION TDilater : public TMorphologicalSampler 
{
	typedef TMorphologicalSampler inherited;
	
protected:
	virtual void __fastcall UpdateBuffer(TBufferEntry &Buffer, Gr32::TColor32 Color, int Weight);
public:
	#pragma option push -w-inl
	/* TKernelSampler.Create */ inline __fastcall virtual TDilater(Gr32::TCustomSampler* ASampler) : TMorphologicalSampler(ASampler) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TKernelSampler.Destroy */ inline __fastcall virtual ~TDilater(void) { }
	#pragma option pop
	
};


class DELPHICLASS TEroder;
class PASCALIMPLEMENTATION TEroder : public TMorphologicalSampler 
{
	typedef TMorphologicalSampler inherited;
	
protected:
	virtual void __fastcall UpdateBuffer(TBufferEntry &Buffer, Gr32::TColor32 Color, int Weight);
	
public:
	__fastcall virtual TEroder(Gr32::TCustomSampler* ASampler);
public:
	#pragma option push -w-inl
	/* TKernelSampler.Destroy */ inline __fastcall virtual ~TEroder(void) { }
	#pragma option pop
	
};


class DELPHICLASS TExpander;
class PASCALIMPLEMENTATION TExpander : public TKernelSampler 
{
	typedef TKernelSampler inherited;
	
protected:
	virtual void __fastcall UpdateBuffer(TBufferEntry &Buffer, Gr32::TColor32 Color, int Weight);
public:
	#pragma option push -w-inl
	/* TKernelSampler.Create */ inline __fastcall virtual TExpander(Gr32::TCustomSampler* ASampler) : TKernelSampler(ASampler) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TKernelSampler.Destroy */ inline __fastcall virtual ~TExpander(void) { }
	#pragma option pop
	
};


class DELPHICLASS TContracter;
class PASCALIMPLEMENTATION TContracter : public TExpander 
{
	typedef TExpander inherited;
	
private:
	Gr32::TColor32 FMaxWeight;
	
protected:
	virtual void __fastcall UpdateBuffer(TBufferEntry &Buffer, Gr32::TColor32 Color, int Weight);
	
public:
	virtual void __fastcall PrepareSampling(void);
	virtual Gr32::TColor32 __fastcall GetSampleInt(int X, int Y);
	virtual Gr32::TColor32 __fastcall GetSampleFixed(Gr32::TFixed X, Gr32::TFixed Y);
public:
	#pragma option push -w-inl
	/* TKernelSampler.Create */ inline __fastcall virtual TContracter(Gr32::TCustomSampler* ASampler) : TExpander(ASampler) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TKernelSampler.Destroy */ inline __fastcall virtual ~TContracter(void) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------
static const Shortint MAX_KERNEL_WIDTH = 0x4;
extern PACKAGE Gr32_containers::TClassList* KernelList;
extern PACKAGE Gr32_containers::TClassList* ResamplerList;
extern PACKAGE TBufferEntry EMPTY_ENTRY;
extern PACKAGE void __fastcall Convolve(Gr32::TBitmap32* Src, Gr32::TBitmap32* Dst, Gr32_ordinalmaps::TIntegerMap* Kernel, int CenterX, int CenterY);
extern PACKAGE void __fastcall Dilate(Gr32::TBitmap32* Src, Gr32::TBitmap32* Dst, Gr32_ordinalmaps::TIntegerMap* Kernel, int CenterX, int CenterY);
extern PACKAGE void __fastcall Erode(Gr32::TBitmap32* Src, Gr32::TBitmap32* Dst, Gr32_ordinalmaps::TIntegerMap* Kernel, int CenterX, int CenterY);
extern PACKAGE void __fastcall Expand(Gr32::TBitmap32* Src, Gr32::TBitmap32* Dst, Gr32_ordinalmaps::TIntegerMap* Kernel, int CenterX, int CenterY);
extern PACKAGE void __fastcall Contract(Gr32::TBitmap32* Src, Gr32::TBitmap32* Dst, Gr32_ordinalmaps::TIntegerMap* Kernel, int CenterX, int CenterY);
extern PACKAGE void __fastcall IncBuffer(TBufferEntry &Buffer, Gr32::TColor32 Color);
extern PACKAGE void __fastcall MultiplyBuffer(TBufferEntry &Buffer, int W);
extern PACKAGE void __fastcall ShrBuffer(TBufferEntry &Buffer, int Shift);
extern PACKAGE Gr32::TColor32 __fastcall BufferToColor32(const TBufferEntry &Buffer, int Shift);
extern PACKAGE void __fastcall BlockTransfer(Gr32::TBitmap32* Dst, int DstX, int DstY, const Types::TRect &DstClip, Gr32::TBitmap32* Src, const Types::TRect &SrcRect, Gr32::TDrawMode CombineOp, Gr32::TPixelCombineEvent CombineCallBack = 0x0);
extern PACKAGE void __fastcall BlockTransferX(Gr32::TBitmap32* Dst, Gr32::TFixed DstX, Gr32::TFixed DstY, Gr32::TBitmap32* Src, const Types::TRect &SrcRect, Gr32::TDrawMode CombineOp, Gr32::TPixelCombineEvent CombineCallBack = 0x0);
extern PACKAGE void __fastcall BlendTransfer(Gr32::TBitmap32* Dst, int DstX, int DstY, const Types::TRect &DstClip, Gr32::TBitmap32* SrcF, const Types::TRect &SrcRectF, Gr32::TBitmap32* SrcB, const Types::TRect &SrcRectB, Gr32_blend::TBlendReg BlendCallback)/* overload */;
extern PACKAGE void __fastcall BlendTransfer(Gr32::TBitmap32* Dst, int DstX, int DstY, const Types::TRect &DstClip, Gr32::TBitmap32* SrcF, const Types::TRect &SrcRectF, Gr32::TBitmap32* SrcB, const Types::TRect &SrcRectB, Gr32_blend::TBlendRegEx BlendCallback, int MasterAlpha)/* overload */;
extern PACKAGE void __fastcall StretchTransfer(Gr32::TBitmap32* Dst, const Types::TRect &DstRect, const Types::TRect &DstClip, Gr32::TBitmap32* Src, const Types::TRect &SrcRect, Gr32::TCustomResampler* Resampler, Gr32::TDrawMode CombineOp, Gr32::TPixelCombineEvent CombineCallBack = 0x0);
extern PACKAGE TFixedSamplePattern __fastcall CreateJitteredPattern(int TileWidth, int TileHeight, int SamplesX, int SamplesY);
extern PACKAGE void __fastcall RegisterResampler(TMetaClass* ResamplerClass);
extern PACKAGE void __fastcall RegisterKernel(TMetaClass* KernelClass);

}	/* namespace Gr32_resamplers */
using namespace Gr32_resamplers;
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// Gr32_resamplers
