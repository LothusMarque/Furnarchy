// Borland C++ Builder
// Copyright (c) 1995, 2005 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'Gr32_layers.pas' rev: 10.00

#ifndef Gr32_layersHPP
#define Gr32_layersHPP

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
#include <Forms.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Sysutils.hpp>	// Pascal unit
#include <Math.hpp>	// Pascal unit
#include <Gr32.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Gr32_layers
{
//-- type declarations -------------------------------------------------------
typedef TMetaClass* TLayerClass;

struct TCoordXForm
{
	
public:
	Gr32::TFixed ScaleX;
	Gr32::TFixed ScaleY;
	int ShiftX;
	int ShiftY;
	Gr32::TFixed RevScaleX;
	Gr32::TFixed RevScaleY;
} ;

typedef TCoordXForm *PCoordXForm;

class DELPHICLASS TCustomLayer;
typedef void __fastcall (__closure *TLayerUpdateEvent)(System::TObject* Sender, TCustomLayer* Layer);

typedef void __fastcall (__closure *TAreaUpdateEvent)(System::TObject* Sender, const Types::TRect &Area, const unsigned Info);

#pragma option push -b-
enum TLayerListNotification { lnLayerAdded, lnLayerInserted, lnLayerDeleted, lnCleared };
#pragma option pop

class DELPHICLASS TLayerCollection;
typedef void __fastcall (__closure *TLayerListNotifyEvent)(TLayerCollection* Sender, TLayerListNotification Action, TCustomLayer* Layer, int Index);

typedef void __fastcall (__closure *TGetScaleEvent)(System::TObject* Sender, float &ScaleX, float &ScaleY);

typedef void __fastcall (__closure *TGetShiftEvent)(System::TObject* Sender, float &ShiftX, float &ShiftY);

class PASCALIMPLEMENTATION TLayerCollection : public Classes::TPersistent 
{
	typedef Classes::TPersistent inherited;
	
public:
	TCustomLayer* operator[](int Index) { return Items[Index]; }
	
private:
	Classes::TList* FItems;
	bool FMouseEvents;
	TCustomLayer* FMouseListener;
	int FUpdateCount;
	Classes::TPersistent* FOwner;
	Classes::TNotifyEvent FOnChanging;
	Classes::TNotifyEvent FOnChange;
	Classes::TNotifyEvent FOnGDIUpdate;
	TLayerListNotifyEvent FOnListNotify;
	TLayerUpdateEvent FOnLayerUpdated;
	Gr32::TAreaChangedEvent FOnAreaUpdated;
	TGetScaleEvent FOnGetViewportScale;
	TGetShiftEvent FOnGetViewportShift;
	int __fastcall GetCount(void);
	void __fastcall InsertItem(TCustomLayer* Item);
	void __fastcall RemoveItem(TCustomLayer* Item);
	void __fastcall SetMouseEvents(bool Value);
	void __fastcall SetMouseListener(TCustomLayer* Value);
	
protected:
	void __fastcall BeginUpdate(void);
	void __fastcall Changed(void);
	void __fastcall Changing(void);
	void __fastcall EndUpdate(void);
	TCustomLayer* __fastcall FindLayerAtPos(int X, int Y, unsigned OptionsMask);
	TCustomLayer* __fastcall GetItem(int Index);
	DYNAMIC Classes::TPersistent* __fastcall GetOwner(void);
	void __fastcall GDIUpdate(void);
	void __fastcall DoUpdateLayer(TCustomLayer* Layer);
	void __fastcall DoUpdateArea(const Types::TRect &Rect);
	void __fastcall Notify(TLayerListNotification Action, TCustomLayer* Layer, int Index);
	void __fastcall SetItem(int Index, TCustomLayer* Value);
	TCustomLayer* __fastcall MouseDown(Controls::TMouseButton Button, Classes::TShiftState Shift, int X, int Y);
	TCustomLayer* __fastcall MouseMove(Classes::TShiftState Shift, int X, int Y);
	TCustomLayer* __fastcall MouseUp(Controls::TMouseButton Button, Classes::TShiftState Shift, int X, int Y);
	__property Classes::TNotifyEvent OnChanging = {read=FOnChanging, write=FOnChanging};
	__property Classes::TNotifyEvent OnChange = {read=FOnChange, write=FOnChange};
	__property TLayerListNotifyEvent OnListNotify = {read=FOnListNotify, write=FOnListNotify};
	__property Classes::TNotifyEvent OnGDIUpdate = {read=FOnGDIUpdate, write=FOnGDIUpdate};
	__property TLayerUpdateEvent OnLayerUpdated = {read=FOnLayerUpdated, write=FOnLayerUpdated};
	__property Gr32::TAreaChangedEvent OnAreaUpdated = {read=FOnAreaUpdated, write=FOnAreaUpdated};
	__property TGetScaleEvent OnGetViewportScale = {read=FOnGetViewportScale, write=FOnGetViewportScale};
	__property TGetShiftEvent OnGetViewportShift = {read=FOnGetViewportShift, write=FOnGetViewportShift};
	
public:
	__fastcall TLayerCollection(Classes::TPersistent* AOwner);
	__fastcall virtual ~TLayerCollection(void);
	TCustomLayer* __fastcall Add(TMetaClass* ItemClass);
	virtual void __fastcall Assign(Classes::TPersistent* Source);
	void __fastcall Clear(void);
	void __fastcall Delete(int Index);
	TCustomLayer* __fastcall Insert(int Index, TMetaClass* ItemClass);
	Gr32::TFloatPoint __fastcall LocalToViewport(const Gr32::TFloatPoint &APoint, bool AScaled);
	Gr32::TFloatPoint __fastcall ViewportToLocal(const Gr32::TFloatPoint &APoint, bool AScaled);
	virtual void __fastcall GetViewportScale(float &ScaleX, float &ScaleY);
	virtual void __fastcall GetViewportShift(float &ShiftX, float &ShiftY);
	__property int Count = {read=GetCount, nodefault};
	__property Classes::TPersistent* Owner = {read=FOwner};
	__property TCustomLayer* Items[int Index] = {read=GetItem, write=SetItem/*, default*/};
	__property TCustomLayer* MouseListener = {read=FMouseListener, write=SetMouseListener};
	__property bool MouseEvents = {read=FMouseEvents, write=SetMouseEvents, nodefault};
};


#pragma option push -b-
enum TLayerState { lsMouseLeft, lsMouseRight, lsMouseMiddle };
#pragma option pop

typedef Set<TLayerState, lsMouseLeft, lsMouseMiddle>  TLayerStates;

typedef void __fastcall (__closure *TPaintLayerEvent)(System::TObject* Sender, Gr32::TBitmap32* Buffer);

typedef void __fastcall (__closure *THitTestEvent)(System::TObject* Sender, int X, int Y, bool &Passed);

class PASCALIMPLEMENTATION TCustomLayer : public Gr32::TNotifiablePersistent 
{
	typedef Gr32::TNotifiablePersistent inherited;
	
private:
	Controls::TCursor FCursor;
	Classes::TList* FFreeNotifies;
	TLayerCollection* FLayerCollection;
	TLayerStates FLayerStates;
	unsigned FLayerOptions;
	THitTestEvent FOnHitTest;
	Controls::TMouseEvent FOnMouseDown;
	Controls::TMouseMoveEvent FOnMouseMove;
	Controls::TMouseEvent FOnMouseUp;
	TPaintLayerEvent FOnPaint;
	int FTag;
	Classes::TNotifyEvent FOnDestroy;
	int __fastcall GetIndex(void);
	bool __fastcall GetMouseEvents(void);
	bool __fastcall GetVisible(void);
	void __fastcall SetMouseEvents(bool Value);
	void __fastcall SetVisible(bool Value);
	bool __fastcall GetInvalid(void);
	void __fastcall SetInvalid(bool Value);
	bool __fastcall GetForceUpdate(void);
	void __fastcall SetForceUpdate(bool Value);
	
protected:
	void __fastcall AddNotification(TCustomLayer* ALayer);
	void __fastcall Changing(void);
	virtual bool __fastcall DoHitTest(int X, int Y);
	void __fastcall DoPaint(Gr32::TBitmap32* Buffer);
	DYNAMIC Classes::TPersistent* __fastcall GetOwner(void);
	virtual void __fastcall MouseDown(Controls::TMouseButton Button, Classes::TShiftState Shift, int X, int Y);
	virtual void __fastcall MouseMove(Classes::TShiftState Shift, int X, int Y);
	virtual void __fastcall MouseUp(Controls::TMouseButton Button, Classes::TShiftState Shift, int X, int Y);
	virtual void __fastcall Notification(TCustomLayer* ALayer);
	virtual void __fastcall Paint(Gr32::TBitmap32* Buffer);
	virtual void __fastcall PaintGDI(Graphics::TCanvas* Canvas);
	void __fastcall RemoveNotification(TCustomLayer* ALayer);
	virtual void __fastcall SetIndex(int Value);
	virtual void __fastcall SetCursor(Controls::TCursor Value);
	virtual void __fastcall SetLayerCollection(TLayerCollection* Value);
	virtual void __fastcall SetLayerOptions(unsigned Value);
	__property bool Invalid = {read=GetInvalid, write=SetInvalid, nodefault};
	__property bool ForceUpdate = {read=GetForceUpdate, write=SetForceUpdate, nodefault};
	
public:
	__fastcall virtual TCustomLayer(TLayerCollection* ALayerCollection);
	__fastcall virtual ~TCustomLayer(void);
	virtual void __fastcall BeforeDestruction(void);
	void __fastcall BringToFront(void);
	virtual void __fastcall Changed(void)/* overload */;
	HIDESBASE void __fastcall Changed(const Types::TRect &Rect)/* overload */;
	void __fastcall Update(void)/* overload */;
	void __fastcall Update(const Types::TRect &Rect)/* overload */;
	bool __fastcall HitTest(int X, int Y);
	void __fastcall SendToBack(void);
	void __fastcall SetAsMouseListener(void);
	__property Controls::TCursor Cursor = {read=FCursor, write=SetCursor, nodefault};
	__property int Index = {read=GetIndex, write=SetIndex, nodefault};
	__property TLayerCollection* LayerCollection = {read=FLayerCollection, write=SetLayerCollection};
	__property unsigned LayerOptions = {read=FLayerOptions, write=SetLayerOptions, nodefault};
	__property TLayerStates LayerStates = {read=FLayerStates, nodefault};
	__property bool MouseEvents = {read=GetMouseEvents, write=SetMouseEvents, nodefault};
	__property int Tag = {read=FTag, write=FTag, nodefault};
	__property bool Visible = {read=GetVisible, write=SetVisible, nodefault};
	__property Classes::TNotifyEvent OnDestroy = {read=FOnDestroy, write=FOnDestroy};
	__property THitTestEvent OnHitTest = {read=FOnHitTest, write=FOnHitTest};
	__property TPaintLayerEvent OnPaint = {read=FOnPaint, write=FOnPaint};
	__property Controls::TMouseEvent OnMouseDown = {read=FOnMouseDown, write=FOnMouseDown};
	__property Controls::TMouseMoveEvent OnMouseMove = {read=FOnMouseMove, write=FOnMouseMove};
	__property Controls::TMouseEvent OnMouseUp = {read=FOnMouseUp, write=FOnMouseUp};
};


class DELPHICLASS TPositionedLayer;
class PASCALIMPLEMENTATION TPositionedLayer : public TCustomLayer 
{
	typedef TCustomLayer inherited;
	
private:
	#pragma pack(push,1)
	Gr32::TFloatRect FLocation;
	#pragma pack(pop)
	bool FScaled;
	void __fastcall SetLocation(const Gr32::TFloatRect &Value);
	void __fastcall SetScaled(bool Value);
	
protected:
	virtual bool __fastcall DoHitTest(int X, int Y);
	virtual void __fastcall DoSetLocation(const Gr32::TFloatRect &NewLocation);
	
public:
	__fastcall virtual TPositionedLayer(TLayerCollection* ALayerCollection);
	virtual Gr32::TFloatRect __fastcall GetAdjustedRect(const Gr32::TFloatRect &R);
	Gr32::TFloatRect __fastcall GetAdjustedLocation();
	__property Gr32::TFloatRect Location = {read=FLocation, write=SetLocation};
	__property bool Scaled = {read=FScaled, write=SetScaled, nodefault};
public:
	#pragma option push -w-inl
	/* TCustomLayer.Destroy */ inline __fastcall virtual ~TPositionedLayer(void) { }
	#pragma option pop
	
};


class DELPHICLASS TBitmapLayer;
class PASCALIMPLEMENTATION TBitmapLayer : public TPositionedLayer 
{
	typedef TPositionedLayer inherited;
	
private:
	Gr32::TBitmap32* FBitmap;
	bool FAlphaHit;
	bool FCropped;
	void __fastcall BitmapAreaChanged(System::TObject* Sender, const Types::TRect &Area, const unsigned Info);
	void __fastcall SetBitmap(Gr32::TBitmap32* Value);
	void __fastcall SetCropped(bool Value);
	
protected:
	virtual bool __fastcall DoHitTest(int X, int Y);
	virtual void __fastcall Paint(Gr32::TBitmap32* Buffer);
	
public:
	__fastcall virtual TBitmapLayer(TLayerCollection* ALayerCollection);
	__fastcall virtual ~TBitmapLayer(void);
	__property bool AlphaHit = {read=FAlphaHit, write=FAlphaHit, nodefault};
	__property Gr32::TBitmap32* Bitmap = {read=FBitmap, write=SetBitmap};
	__property bool Cropped = {read=FCropped, write=SetCropped, nodefault};
};


#pragma option push -b-
enum TDragState { dsNone, dsMove, dsSizeL, dsSizeT, dsSizeR, dsSizeB, dsSizeTL, dsSizeTR, dsSizeBL, dsSizeBR };
#pragma option pop

#pragma option push -b-
enum GR32_Layers__5 { rhCenter, rhSides, rhCorners, rhFrame, rhNotLeftSide, rhNotRightSide, rhNotTopSide, rhNotBottomSide, rhNotTLCorner, rhNotTRCorner, rhNotBLCorner, rhNotBRCorner };
#pragma option pop

typedef Set<GR32_Layers__5, rhCenter, rhNotBRCorner>  TRBHandles;

typedef void __fastcall (__closure *TRBResizingEvent)(System::TObject* Sender, const Gr32::TFloatRect &OldLocation, Gr32::TFloatRect &NewLocation, TDragState DragState, Classes::TShiftState Shift);

class DELPHICLASS TRubberbandLayer;
class PASCALIMPLEMENTATION TRubberbandLayer : public TPositionedLayer 
{
	typedef TPositionedLayer inherited;
	
private:
	TPositionedLayer* FChildLayer;
	DynamicArray<Gr32::TColor32 >  FFrameStipplePattern;
	float FFrameStippleStep;
	float FFrameStippleCounter;
	Gr32::TColor32 FHandleFrame;
	Gr32::TColor32 FHandleFill;
	TRBHandles FHandles;
	int FHandleSize;
	float FMinWidth;
	float FMaxHeight;
	float FMinHeight;
	float FMaxWidth;
	Classes::TNotifyEvent FOnUserChange;
	TRBResizingEvent FOnResizing;
	void __fastcall SetFrameStippleStep(const float Value);
	void __fastcall SetFrameStippleCounter(const float Value);
	void __fastcall SetChildLayer(TPositionedLayer* Value);
	void __fastcall SetHandleFill(Gr32::TColor32 Value);
	void __fastcall SetHandleFrame(Gr32::TColor32 Value);
	void __fastcall SetHandles(TRBHandles Value);
	void __fastcall SetHandleSize(int Value);
	
protected:
	bool IsDragging;
	TDragState DragState;
	#pragma pack(push,1)
	Gr32::TFloatRect OldLocation;
	#pragma pack(pop)
	Gr32::TFloatPoint MouseShift;
	virtual bool __fastcall DoHitTest(int X, int Y);
	virtual void __fastcall DoResizing(Gr32::TFloatRect &OldLocation, Gr32::TFloatRect &NewLocation, TDragState DragState, Classes::TShiftState Shift);
	virtual void __fastcall DoSetLocation(const Gr32::TFloatRect &NewLocation);
	virtual TDragState __fastcall GetDragState(int X, int Y);
	virtual void __fastcall MouseDown(Controls::TMouseButton Button, Classes::TShiftState Shift, int X, int Y);
	virtual void __fastcall MouseMove(Classes::TShiftState Shift, int X, int Y);
	virtual void __fastcall MouseUp(Controls::TMouseButton Button, Classes::TShiftState Shift, int X, int Y);
	virtual void __fastcall Notification(TCustomLayer* ALayer);
	virtual void __fastcall Paint(Gr32::TBitmap32* Buffer);
	virtual void __fastcall SetLayerOptions(unsigned Value);
	void __fastcall UpdateChildLayer(void);
	
public:
	__fastcall virtual TRubberbandLayer(TLayerCollection* ALayerCollection);
	void __fastcall SetFrameStipple(Gr32::TColor32 const * Value, const int Value_Size);
	__property TPositionedLayer* ChildLayer = {read=FChildLayer, write=SetChildLayer};
	__property TRBHandles Handles = {read=FHandles, write=SetHandles, nodefault};
	__property int HandleSize = {read=FHandleSize, write=SetHandleSize, nodefault};
	__property Gr32::TColor32 HandleFill = {read=FHandleFill, write=SetHandleFill, nodefault};
	__property Gr32::TColor32 HandleFrame = {read=FHandleFrame, write=SetHandleFrame, nodefault};
	__property float FrameStippleStep = {read=FFrameStippleStep, write=SetFrameStippleStep};
	__property float FrameStippleCounter = {read=FFrameStippleCounter, write=SetFrameStippleCounter};
	__property float MaxHeight = {read=FMaxHeight, write=FMaxHeight};
	__property float MaxWidth = {read=FMaxWidth, write=FMaxWidth};
	__property float MinHeight = {read=FMinHeight, write=FMinHeight};
	__property float MinWidth = {read=FMinWidth, write=FMinWidth};
	__property Classes::TNotifyEvent OnUserChange = {read=FOnUserChange, write=FOnUserChange};
	__property TRBResizingEvent OnResizing = {read=FOnResizing, write=FOnResizing};
public:
	#pragma option push -w-inl
	/* TCustomLayer.Destroy */ inline __fastcall virtual ~TRubberbandLayer(void) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------
static const unsigned LOB_VISIBLE = 0x80000000;
static const int LOB_GDI_OVERLAY = 0x40000000;
static const int LOB_MOUSE_EVENTS = 0x20000000;
static const int LOB_NO_UPDATE = 0x10000000;
static const int LOB_NO_CAPTURE = 0x8000000;
static const int LOB_INVALID = 0x4000000;
static const int LOB_FORCE_UPDATE = 0x2000000;
static const int LOB_RESERVED_24 = 0x1000000;
static const unsigned LOB_RESERVED_MASK = 0xff000000;

}	/* namespace Gr32_layers */
using namespace Gr32_layers;
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// Gr32_layers
