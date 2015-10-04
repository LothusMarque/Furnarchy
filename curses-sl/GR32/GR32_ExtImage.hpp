// Borland C++ Builder
// Copyright (c) 1995, 2005 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'Gr32_extimage.pas' rev: 10.00

#ifndef Gr32_extimageHPP
#define Gr32_extimageHPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member functions
#pragma pack(push,8)
#include <System.hpp>	// Pascal unit
#include <Sysinit.hpp>	// Pascal unit
#include <Gr32.hpp>	// Pascal unit
#include <Gr32_image.hpp>	// Pascal unit
#include <Gr32_rasterizers.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Controls.hpp>	// Pascal unit
#include <Messages.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Gr32_extimage
{
//-- type declarations -------------------------------------------------------
#pragma option push -b-
enum TRenderMode { rnmFull, rnmConstrained };
#pragma option pop

class DELPHICLASS TSyntheticImage32;
class DELPHICLASS TRenderThread;
class PASCALIMPLEMENTATION TRenderThread : public Classes::TThread 
{
	typedef Classes::TThread inherited;
	
private:
	Gr32::TBitmap32* FDest;
	Gr32_rasterizers::TRasterizer* FRasterizer;
	Gr32::TAreaChangedEvent FOldAreaChanged;
	#pragma pack(push,1)
	Types::TRect FArea;
	#pragma pack(pop)
	#pragma pack(push,1)
	Types::TRect FDstRect;
	#pragma pack(pop)
	void __fastcall SynchronizedAreaChanged(void);
	void __fastcall AreaChanged(System::TObject* Sender, const Types::TRect &Area, const unsigned Hint);
	
protected:
	virtual void __fastcall Execute(void);
	void __fastcall Rasterize(void);
	
public:
	__fastcall TRenderThread(Gr32_rasterizers::TRasterizer* Rasterizer, Gr32::TBitmap32* Dst, const Types::TRect &DstRect, bool Suspended);
public:
	#pragma option push -w-inl
	/* TThread.Destroy */ inline __fastcall virtual ~TRenderThread(void) { }
	#pragma option pop
	
};


class PASCALIMPLEMENTATION TSyntheticImage32 : public Gr32_image::TPaintBox32 
{
	typedef Gr32_image::TPaintBox32 inherited;
	
private:
	Gr32_rasterizers::TRasterizer* FRasterizer;
	bool FAutoRasterize;
	Classes::TWndMethod FDefaultProc;
	bool FResized;
	TRenderThread* FRenderThread;
	Gr32::TAreaChangedEvent FOldAreaChanged;
	#pragma pack(push,1)
	Types::TRect FDstRect;
	#pragma pack(pop)
	TRenderMode FRenderMode;
	bool FClearBuffer;
	void __fastcall SetRasterizer(const Gr32_rasterizers::TRasterizer* Value);
	void __fastcall StopRenderThread(void);
	void __fastcall SetDstRect(const Types::TRect &Value);
	void __fastcall SetRenderMode(const TRenderMode Value);
	
protected:
	void __fastcall RasterizerChanged(System::TObject* Sender);
	virtual void __fastcall SetParent(Controls::TWinControl* AParent);
	void __fastcall FormWindowProc(Messages::TMessage &Message);
	void __fastcall DoRasterize(void);
	__property RepaintMode  = {default=0};
	
public:
	__fastcall virtual TSyntheticImage32(Classes::TComponent* AOwner);
	__fastcall virtual ~TSyntheticImage32(void);
	DYNAMIC void __fastcall Resize(void);
	void __fastcall Rasterize(void);
	__property Types::TRect DstRect = {read=FDstRect, write=SetDstRect};
	
__published:
	__property bool AutoRasterize = {read=FAutoRasterize, write=FAutoRasterize, nodefault};
	__property Gr32_rasterizers::TRasterizer* Rasterizer = {read=FRasterizer, write=SetRasterizer};
	__property Buffer ;
	__property Color  = {default=-16777211};
	__property bool ClearBuffer = {read=FClearBuffer, write=FClearBuffer, nodefault};
	__property TRenderMode RenderMode = {read=FRenderMode, write=SetRenderMode, nodefault};
public:
	#pragma option push -w-inl
	/* TWinControl.CreateParented */ inline __fastcall TSyntheticImage32(HWND ParentWindow) : Gr32_image::TPaintBox32(ParentWindow) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE void __fastcall Rasterize(Gr32_rasterizers::TRasterizer* Rasterizer, Gr32::TBitmap32* Dst, const Types::TRect &DstRect);

}	/* namespace Gr32_extimage */
using namespace Gr32_extimage;
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// Gr32_extimage
