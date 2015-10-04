// Borland C++ Builder
// Copyright (c) 1995, 2005 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'Gr32_transforms.pas' rev: 10.00

#ifndef Gr32_transformsHPP
#define Gr32_transformsHPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member functions
#pragma pack(push,8)
#include <System.hpp>	// Pascal unit
#include <Sysinit.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <Sysutils.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Gr32.hpp>	// Pascal unit
#include <Gr32_blend.hpp>	// Pascal unit
#include <Gr32_vectormaps.hpp>	// Pascal unit
#include <Gr32_rasterizers.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Gr32_transforms
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS ETransformError;
class PASCALIMPLEMENTATION ETransformError : public Sysutils::Exception 
{
	typedef Sysutils::Exception inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall ETransformError(const AnsiString Msg) : Sysutils::Exception(Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall ETransformError(const AnsiString Msg, System::TVarRec const * Args, const int Args_Size) : Sysutils::Exception(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall ETransformError(int Ident)/* overload */ : Sysutils::Exception(Ident) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall ETransformError(int Ident, System::TVarRec const * Args, const int Args_Size)/* overload */ : Sysutils::Exception(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall ETransformError(const AnsiString Msg, int AHelpContext) : Sysutils::Exception(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall ETransformError(const AnsiString Msg, System::TVarRec const * Args, const int Args_Size, int AHelpContext) : Sysutils::Exception(Msg, Args, Args_Size, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall ETransformError(int Ident, int AHelpContext)/* overload */ : Sysutils::Exception(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall ETransformError(System::PResStringRec ResStringRec, System::TVarRec const * Args, const int Args_Size, int AHelpContext)/* overload */ : Sysutils::Exception(ResStringRec, Args, Args_Size, AHelpContext) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~ETransformError(void) { }
	#pragma option pop
	
};


class DELPHICLASS ETransformNotImplemented;
class PASCALIMPLEMENTATION ETransformNotImplemented : public Sysutils::Exception 
{
	typedef Sysutils::Exception inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall ETransformNotImplemented(const AnsiString Msg) : Sysutils::Exception(Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall ETransformNotImplemented(const AnsiString Msg, System::TVarRec const * Args, const int Args_Size) : Sysutils::Exception(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall ETransformNotImplemented(int Ident)/* overload */ : Sysutils::Exception(Ident) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall ETransformNotImplemented(int Ident, System::TVarRec const * Args, const int Args_Size)/* overload */ : Sysutils::Exception(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall ETransformNotImplemented(const AnsiString Msg, int AHelpContext) : Sysutils::Exception(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall ETransformNotImplemented(const AnsiString Msg, System::TVarRec const * Args, const int Args_Size, int AHelpContext) : Sysutils::Exception(Msg, Args, Args_Size, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall ETransformNotImplemented(int Ident, int AHelpContext)/* overload */ : Sysutils::Exception(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall ETransformNotImplemented(System::PResStringRec ResStringRec, System::TVarRec const * Args, const int Args_Size, int AHelpContext)/* overload */ : Sysutils::Exception(ResStringRec, Args, Args_Size, AHelpContext) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~ETransformNotImplemented(void) { }
	#pragma option pop
	
};


typedef float TFloatMatrix[3][3];

typedef Gr32::TFixed TFixedMatrix[3][3];

typedef float TVector3f[3];

typedef int TVector3i[3];

class DELPHICLASS TTransformation;
class PASCALIMPLEMENTATION TTransformation : public Gr32::TNotifiablePersistent 
{
	typedef Gr32::TNotifiablePersistent inherited;
	
private:
	#pragma pack(push,1)
	Gr32::TFloatRect FSrcRect;
	#pragma pack(pop)
	void __fastcall SetSrcRect(const Gr32::TFloatRect &Value);
	
protected:
	bool TransformValid;
	virtual void __fastcall PrepareTransform(void);
	virtual void __fastcall ReverseTransformInt(int DstX, int DstY, /* out */ int &SrcX, /* out */ int &SrcY);
	virtual void __fastcall ReverseTransformFixed(Gr32::TFixed DstX, Gr32::TFixed DstY, /* out */ Gr32::TFixed &SrcX, /* out */ Gr32::TFixed &SrcY);
	virtual void __fastcall ReverseTransformFloat(float DstX, float DstY, /* out */ float &SrcX, /* out */ float &SrcY);
	virtual void __fastcall TransformInt(int SrcX, int SrcY, /* out */ int &DstX, /* out */ int &DstY);
	virtual void __fastcall TransformFixed(Gr32::TFixed SrcX, Gr32::TFixed SrcY, /* out */ Gr32::TFixed &DstX, /* out */ Gr32::TFixed &DstY);
	virtual void __fastcall TransformFloat(float SrcX, float SrcY, /* out */ float &DstX, /* out */ float &DstY);
	
public:
	virtual void __fastcall Changed(void);
	virtual bool __fastcall HasTransformedBounds(void);
	Types::TRect __fastcall GetTransformedBounds()/* overload */;
	virtual Types::TRect __fastcall GetTransformedBounds(const Gr32::TFloatRect &ASrcRect)/* overload */;
	virtual Types::TPoint __fastcall ReverseTransform(const Types::TPoint &P)/* overload */;
	virtual Gr32::TFixedPoint __fastcall ReverseTransform(const Gr32::TFixedPoint &P)/* overload */;
	virtual Gr32::TFloatPoint __fastcall ReverseTransform(const Gr32::TFloatPoint &P)/* overload */;
	virtual Types::TPoint __fastcall Transform(const Types::TPoint &P)/* overload */;
	virtual Gr32::TFixedPoint __fastcall Transform(const Gr32::TFixedPoint &P)/* overload */;
	virtual Gr32::TFloatPoint __fastcall Transform(const Gr32::TFloatPoint &P)/* overload */;
	__property Gr32::TFloatRect SrcRect = {read=FSrcRect, write=SetSrcRect};
public:
	#pragma option push -w-inl
	/* TPersistent.Destroy */ inline __fastcall virtual ~TTransformation(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TTransformation(void) : Gr32::TNotifiablePersistent() { }
	#pragma option pop
	
};


class DELPHICLASS TAffineTransformation;
class PASCALIMPLEMENTATION TAffineTransformation : public TTransformation 
{
	typedef TTransformation inherited;
	
protected:
	float FInverseMatrix[3][3];
	Gr32::TFixed FFixedMatrix[3][3];
	Gr32::TFixed FInverseFixedMatrix[3][3];
	virtual void __fastcall PrepareTransform(void);
	virtual void __fastcall ReverseTransformFloat(float DstX, float DstY, /* out */ float &SrcX, /* out */ float &SrcY);
	virtual void __fastcall ReverseTransformFixed(Gr32::TFixed DstX, Gr32::TFixed DstY, /* out */ Gr32::TFixed &SrcX, /* out */ Gr32::TFixed &SrcY);
	virtual void __fastcall TransformFloat(float SrcX, float SrcY, /* out */ float &DstX, /* out */ float &DstY);
	virtual void __fastcall TransformFixed(Gr32::TFixed SrcX, Gr32::TFixed SrcY, /* out */ Gr32::TFixed &DstX, /* out */ Gr32::TFixed &DstY);
	
public:
	float Matrix[3][3];
	__fastcall virtual TAffineTransformation(void);
	virtual Types::TRect __fastcall GetTransformedBounds(const Gr32::TFloatRect &ASrcRect)/* overload */;
	void __fastcall Clear(void);
	void __fastcall Rotate(float Cx, float Cy, float Alpha);
	void __fastcall Skew(float Fx, float Fy);
	void __fastcall Scale(float Sx, float Sy);
	void __fastcall Translate(float Dx, float Dy);
public:
	#pragma option push -w-inl
	/* TPersistent.Destroy */ inline __fastcall virtual ~TAffineTransformation(void) { }
	#pragma option pop
	
	
/* Hoisted overloads: */
	
public:
	inline Types::TRect __fastcall  GetTransformedBounds(){ return TTransformation::GetTransformedBounds(); }
	
};


class DELPHICLASS TProjectiveTransformation;
class PASCALIMPLEMENTATION TProjectiveTransformation : public TTransformation 
{
	typedef TTransformation inherited;
	
private:
	float Wx0;
	float Wx1;
	float Wx2;
	float Wx3;
	float Wy0;
	float Wy1;
	float Wy2;
	float Wy3;
	void __fastcall SetX0(float Value);
	void __fastcall SetX1(float Value);
	void __fastcall SetX2(float Value);
	void __fastcall SetX3(float Value);
	void __fastcall SetY0(float Value);
	void __fastcall SetY1(float Value);
	void __fastcall SetY2(float Value);
	void __fastcall SetY3(float Value);
	
protected:
	float FMatrix[3][3];
	float FInverseMatrix[3][3];
	Gr32::TFixed FFixedMatrix[3][3];
	Gr32::TFixed FInverseFixedMatrix[3][3];
	virtual void __fastcall PrepareTransform(void);
	virtual void __fastcall ReverseTransformFloat(float DstX, float DstY, /* out */ float &SrcX, /* out */ float &SrcY);
	virtual void __fastcall ReverseTransformFixed(Gr32::TFixed DstX, Gr32::TFixed DstY, /* out */ Gr32::TFixed &SrcX, /* out */ Gr32::TFixed &SrcY);
	virtual void __fastcall TransformFloat(float SrcX, float SrcY, /* out */ float &DstX, /* out */ float &DstY);
	virtual void __fastcall TransformFixed(Gr32::TFixed SrcX, Gr32::TFixed SrcY, /* out */ Gr32::TFixed &DstX, /* out */ Gr32::TFixed &DstY);
	
public:
	virtual Types::TRect __fastcall GetTransformedBounds(const Gr32::TFloatRect &ASrcRect)/* overload */;
	
__published:
	__property float X0 = {read=Wx0, write=SetX0};
	__property float X1 = {read=Wx1, write=SetX1};
	__property float X2 = {read=Wx2, write=SetX2};
	__property float X3 = {read=Wx3, write=SetX3};
	__property float Y0 = {read=Wy0, write=SetY0};
	__property float Y1 = {read=Wy1, write=SetY1};
	__property float Y2 = {read=Wy2, write=SetY2};
	__property float Y3 = {read=Wy3, write=SetY3};
public:
	#pragma option push -w-inl
	/* TPersistent.Destroy */ inline __fastcall virtual ~TProjectiveTransformation(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TProjectiveTransformation(void) : TTransformation() { }
	#pragma option pop
	
	
/* Hoisted overloads: */
	
public:
	inline Types::TRect __fastcall  GetTransformedBounds(){ return TTransformation::GetTransformedBounds(); }
	
};


class DELPHICLASS TTwirlTransformation;
class PASCALIMPLEMENTATION TTwirlTransformation : public TTransformation 
{
	typedef TTransformation inherited;
	
private:
	float Frx;
	float Fry;
	float FTwirl;
	void __fastcall SetTwirl(const float Value);
	
protected:
	virtual void __fastcall PrepareTransform(void);
	virtual void __fastcall ReverseTransformFloat(float DstX, float DstY, /* out */ float &SrcX, /* out */ float &SrcY);
	
public:
	__fastcall virtual TTwirlTransformation(void);
	virtual Types::TRect __fastcall GetTransformedBounds(const Gr32::TFloatRect &ASrcRect)/* overload */;
	
__published:
	__property float Twirl = {read=FTwirl, write=SetTwirl};
public:
	#pragma option push -w-inl
	/* TPersistent.Destroy */ inline __fastcall virtual ~TTwirlTransformation(void) { }
	#pragma option pop
	
	
/* Hoisted overloads: */
	
public:
	inline Types::TRect __fastcall  GetTransformedBounds(){ return TTransformation::GetTransformedBounds(); }
	
};


class DELPHICLASS TBloatTransformation;
class PASCALIMPLEMENTATION TBloatTransformation : public TTransformation 
{
	typedef TTransformation inherited;
	
private:
	float FBloatPower;
	float FBP;
	float FPiW;
	float FPiH;
	void __fastcall SetBloatPower(const float Value);
	
protected:
	virtual void __fastcall PrepareTransform(void);
	virtual void __fastcall ReverseTransformFloat(float DstX, float DstY, /* out */ float &SrcX, /* out */ float &SrcY);
	
public:
	__fastcall virtual TBloatTransformation(void);
	
__published:
	__property float BloatPower = {read=FBloatPower, write=SetBloatPower};
public:
	#pragma option push -w-inl
	/* TPersistent.Destroy */ inline __fastcall virtual ~TBloatTransformation(void) { }
	#pragma option pop
	
};


class DELPHICLASS TDisturbanceTransformation;
class PASCALIMPLEMENTATION TDisturbanceTransformation : public TTransformation 
{
	typedef TTransformation inherited;
	
private:
	float FDisturbance;
	void __fastcall SetDisturbance(const float Value);
	
protected:
	virtual void __fastcall ReverseTransformFloat(float DstX, float DstY, /* out */ float &SrcX, /* out */ float &SrcY);
	
public:
	virtual Types::TRect __fastcall GetTransformedBounds(const Gr32::TFloatRect &ASrcRect)/* overload */;
	
__published:
	__property float Disturbance = {read=FDisturbance, write=SetDisturbance};
public:
	#pragma option push -w-inl
	/* TPersistent.Destroy */ inline __fastcall virtual ~TDisturbanceTransformation(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TDisturbanceTransformation(void) : TTransformation() { }
	#pragma option pop
	
	
/* Hoisted overloads: */
	
public:
	inline Types::TRect __fastcall  GetTransformedBounds(){ return TTransformation::GetTransformedBounds(); }
	
};


class DELPHICLASS TFishEyeTransformation;
class PASCALIMPLEMENTATION TFishEyeTransformation : public TTransformation 
{
	typedef TTransformation inherited;
	
private:
	float Frx;
	float Fry;
	float Faw;
	float Fsr;
	float Sx;
	float Sy;
	float FMinR;
	
protected:
	virtual void __fastcall PrepareTransform(void);
	virtual void __fastcall ReverseTransformFloat(float DstX, float DstY, /* out */ float &SrcX, /* out */ float &SrcY);
public:
	#pragma option push -w-inl
	/* TPersistent.Destroy */ inline __fastcall virtual ~TFishEyeTransformation(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TFishEyeTransformation(void) : TTransformation() { }
	#pragma option pop
	
};


class DELPHICLASS TPolarTransformation;
class PASCALIMPLEMENTATION TPolarTransformation : public TTransformation 
{
	typedef TTransformation inherited;
	
private:
	#pragma pack(push,1)
	Gr32::TFloatRect FDstRect;
	#pragma pack(pop)
	float FPhase;
	float Sx;
	float Sy;
	float Cx;
	float Cy;
	float Dx;
	float Dy;
	float Rt;
	float Rt2;
	float Rr;
	float Rcx;
	float Rcy;
	void __fastcall SetDstRect(const Gr32::TFloatRect &Value);
	void __fastcall SetPhase(const float Value);
	
protected:
	virtual void __fastcall PrepareTransform(void);
	virtual void __fastcall TransformFloat(float SrcX, float SrcY, /* out */ float &DstX, /* out */ float &DstY);
	virtual void __fastcall ReverseTransformFloat(float DstX, float DstY, /* out */ float &SrcX, /* out */ float &SrcY);
	
public:
	__property Gr32::TFloatRect DstRect = {read=FDstRect, write=SetDstRect};
	__property float Phase = {read=FPhase, write=SetPhase};
public:
	#pragma option push -w-inl
	/* TPersistent.Destroy */ inline __fastcall virtual ~TPolarTransformation(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TPolarTransformation(void) : TTransformation() { }
	#pragma option pop
	
};


class DELPHICLASS TPathTransformation;
class PASCALIMPLEMENTATION TPathTransformation : public TTransformation 
{
	typedef TTransformation inherited;
	
private:
	float FTopLength;
	float FBottomLength;
	DynamicArray<Gr32::TFloatPoint >  FBottomCurve;
	DynamicArray<Gr32::TFloatPoint >  FTopCurve;
	DynamicArray<float >  FTopHypot;
	DynamicArray<float >  FBottomHypot;
	void __fastcall SetBottomCurve(const Gr32::TArrayOfFloatPoint Value);
	void __fastcall SetTopCurve(const Gr32::TArrayOfFloatPoint Value);
	
protected:
	virtual void __fastcall PrepareTransform(void);
	virtual void __fastcall TransformFloat(float SrcX, float SrcY, /* out */ float &DstX, /* out */ float &DstY);
	
public:
	__property Gr32::TArrayOfFloatPoint TopCurve = {read=FTopCurve, write=SetTopCurve};
	__property Gr32::TArrayOfFloatPoint BottomCurve = {read=FBottomCurve, write=SetBottomCurve};
public:
	#pragma option push -w-inl
	/* TPersistent.Destroy */ inline __fastcall virtual ~TPathTransformation(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TPathTransformation(void) : TTransformation() { }
	#pragma option pop
	
};


class DELPHICLASS TRemapTransformation;
class PASCALIMPLEMENTATION TRemapTransformation : public TTransformation 
{
	typedef TTransformation inherited;
	
private:
	Gr32_vectormaps::TVectorMap* FVectorMap;
	Gr32::TFixedPoint FScalingFixed;
	Gr32::TFloatPoint FScalingFloat;
	Gr32::TFixedPoint FCombinedScalingFixed;
	Gr32::TFloatPoint FCombinedScalingFloat;
	Gr32::TFixedPoint FSrcTranslationFixed;
	Gr32::TFixedPoint FSrcScaleFixed;
	Gr32::TFixedPoint FDstTranslationFixed;
	Gr32::TFixedPoint FDstScaleFixed;
	Gr32::TFloatPoint FSrcTranslationFloat;
	Gr32::TFloatPoint FSrcScaleFloat;
	Gr32::TFloatPoint FDstTranslationFloat;
	Gr32::TFloatPoint FDstScaleFloat;
	Gr32::TFixedPoint FOffsetFixed;
	#pragma pack(push,1)
	Types::TPoint FOffsetInt;
	#pragma pack(pop)
	#pragma pack(push,1)
	Gr32::TFloatRect FMappingRect;
	#pragma pack(pop)
	Gr32::TFloatPoint FOffset;
	void __fastcall SetMappingRect(const Gr32::TFloatRect &Rect);
	void __fastcall SetOffset(const Gr32::TFloatPoint &Value);
	
protected:
	virtual void __fastcall PrepareTransform(void);
	virtual void __fastcall ReverseTransformInt(int DstX, int DstY, /* out */ int &SrcX, /* out */ int &SrcY);
	virtual void __fastcall ReverseTransformFloat(float DstX, float DstY, /* out */ float &SrcX, /* out */ float &SrcY);
	virtual void __fastcall ReverseTransformFixed(Gr32::TFixed DstX, Gr32::TFixed DstY, /* out */ Gr32::TFixed &SrcX, /* out */ Gr32::TFixed &SrcY);
	
public:
	__fastcall virtual TRemapTransformation(void);
	__fastcall virtual ~TRemapTransformation(void);
	virtual bool __fastcall HasTransformedBounds(void);
	virtual Types::TRect __fastcall GetTransformedBounds(const Gr32::TFloatRect &ASrcRect)/* overload */;
	void __fastcall Scale(float Sx, float Sy);
	__property Gr32::TFloatRect MappingRect = {read=FMappingRect, write=SetMappingRect};
	__property Gr32::TFloatPoint Offset = {read=FOffset, write=SetOffset};
	__property Gr32_vectormaps::TVectorMap* VectorMap = {read=FVectorMap, write=FVectorMap};
	
/* Hoisted overloads: */
	
public:
	inline Types::TRect __fastcall  GetTransformedBounds(){ return TTransformation::GetTransformedBounds(); }
	
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE float IdentityMatrix[3][3];
extern PACKAGE bool FullEdge;
extern PACKAGE void __fastcall Adjoint(float * M);
extern PACKAGE float __fastcall Determinant(float const * M);
extern PACKAGE void __fastcall Scale(float * M, float Factor);
extern PACKAGE void __fastcall Invert(float * M);
extern PACKAGE float __fastcall Mult(float const * M1, float const * M2);
extern PACKAGE float __fastcall VectorTransform(float const * M, float const * V);
extern PACKAGE Gr32::TArrayOfArrayOfFixedPoint __fastcall TransformPoints(Gr32::TArrayOfArrayOfFixedPoint Points, TTransformation* Transformation);
extern PACKAGE void __fastcall Transform(Gr32::TBitmap32* Dst, Gr32::TBitmap32* Src, TTransformation* Transformation)/* overload */;
extern PACKAGE void __fastcall Transform(Gr32::TBitmap32* Dst, Gr32::TBitmap32* Src, TTransformation* Transformation, const Types::TRect &DstClip)/* overload */;
extern PACKAGE void __fastcall Transform(Gr32::TBitmap32* Dst, Gr32::TBitmap32* Src, TTransformation* Transformation, Gr32_rasterizers::TRasterizer* Rasterizer)/* overload */;
extern PACKAGE void __fastcall Transform(Gr32::TBitmap32* Dst, Gr32::TBitmap32* Src, TTransformation* Transformation, Gr32_rasterizers::TRasterizer* Rasterizer, const Types::TRect &DstClip)/* overload */;
extern PACKAGE void __fastcall SetBorderTransparent(Gr32::TBitmap32* ABitmap, const Types::TRect &ARect);
extern PACKAGE void __fastcall RasterizeTransformation(Gr32_vectormaps::TVectorMap* Vectormap, TTransformation* Transformation, const Types::TRect &DstRect, Gr32_vectormaps::TVectorCombineMode CombineMode = (Gr32_vectormaps::TVectorCombineMode)(0x0), Gr32_vectormaps::TVectorCombineEvent CombineCallback = 0x0);
extern PACKAGE Gr32::TFixed __fastcall FixedMatrix(float const * FloatMatrix)/* overload */;
extern PACKAGE float __fastcall FloatMatrix(Gr32::TFixed const * FixedMatrix)/* overload */;

}	/* namespace Gr32_transforms */
using namespace Gr32_transforms;
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// Gr32_transforms
