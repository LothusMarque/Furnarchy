// Borland C++ Builder
// Copyright (c) 1995, 2005 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'Gr32_repaintopt.pas' rev: 10.00

#ifndef Gr32_repaintoptHPP
#define Gr32_repaintoptHPP

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
#include <Gr32_lowlevel.hpp>	// Pascal unit
#include <Gr32_containers.hpp>	// Pascal unit
#include <Gr32_layers.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Gr32_repaintopt
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TCustomRepaintOptimizer;
class PASCALIMPLEMENTATION TCustomRepaintOptimizer : public System::TObject 
{
	typedef System::TObject inherited;
	
private:
	bool FEnabled;
	Classes::TList* FLayerCollections;
	Gr32_containers::TRectList* FInvalidRects;
	Gr32::TBitmap32* FBuffer;
	
protected:
	virtual bool __fastcall GetEnabled(void);
	virtual void __fastcall SetEnabled(const bool Value);
	__property Classes::TList* LayerCollections = {read=FLayerCollections, write=FLayerCollections};
	__property Gr32::TBitmap32* Buffer = {read=FBuffer, write=FBuffer};
	__property Gr32_containers::TRectList* InvalidRects = {read=FInvalidRects, write=FInvalidRects};
	virtual void __fastcall LayerCollectionNotifyHandler(Gr32_layers::TLayerCollection* Sender, Gr32_layers::TLayerListNotification Action, Gr32_layers::TCustomLayer* Layer, int Index) = 0 ;
	
public:
	__fastcall virtual TCustomRepaintOptimizer(Gr32::TBitmap32* Buffer, Gr32_containers::TRectList* InvalidRects);
	__fastcall virtual ~TCustomRepaintOptimizer(void);
	virtual void __fastcall RegisterLayerCollection(Gr32_layers::TLayerCollection* Layers);
	virtual void __fastcall UnregisterLayerCollection(Gr32_layers::TLayerCollection* Layers);
	virtual void __fastcall BeginPaint(void);
	virtual void __fastcall EndPaint(void);
	virtual void __fastcall BeginPaintBuffer(void);
	virtual void __fastcall EndPaintBuffer(void);
	virtual void __fastcall Reset(void) = 0 ;
	virtual bool __fastcall UpdatesAvailable(void) = 0 ;
	virtual void __fastcall PerformOptimization(void) = 0 ;
	virtual void __fastcall AreaUpdateHandler(System::TObject* Sender, const Types::TRect &Area, const unsigned Info) = 0 ;
	virtual void __fastcall LayerUpdateHandler(System::TObject* Sender, Gr32_layers::TCustomLayer* Layer) = 0 ;
	virtual void __fastcall BufferResizedHandler(const int NewWidth, const int NewHeight) = 0 ;
	__property bool Enabled = {read=GetEnabled, write=SetEnabled, nodefault};
};


typedef TMetaClass* TCustomRepaintOptimizerClass;

//-- var, const, procedure ---------------------------------------------------
extern PACKAGE void __fastcall InflateArea(Types::TRect &Area, int Dx, int Dy);

}	/* namespace Gr32_repaintopt */
using namespace Gr32_repaintopt;
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// Gr32_repaintopt
