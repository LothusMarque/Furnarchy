// Borland C++ Builder
// Copyright (c) 1995, 2005 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'Gr32_rangebars.pas' rev: 10.00

#ifndef Gr32_rangebarsHPP
#define Gr32_rangebarsHPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member functions
#pragma pack(push,8)
#include <System.hpp>	// Pascal unit
#include <Sysinit.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <Messages.hpp>	// Pascal unit
#include <Gr32.hpp>	// Pascal unit
#include <Types.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <Controls.hpp>	// Pascal unit
#include <Forms.hpp>	// Pascal unit
#include <Dialogs.hpp>	// Pascal unit
#include <Extctrls.hpp>	// Pascal unit
#include <Sysutils.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Gr32_rangebars
{
//-- type declarations -------------------------------------------------------
#pragma option push -b-
enum TRBDirection { drLeft, drUp, drRight, drDown };
#pragma option pop

typedef Set<TRBDirection, drLeft, drDown>  TRBDirections;

#pragma option push -b-
enum TRBZone { zNone, zBtnPrev, zTrackPrev, zHandle, zTrackNext, zBtnNext };
#pragma option pop

#pragma option push -b-
enum TRBStyle { rbsDefault, rbsMac };
#pragma option pop

#pragma option push -b-
enum TRBBackgnd { bgPattern, bgSolid };
#pragma option pop

typedef void __fastcall (__closure *TRBGetSizeEvent)(System::TObject* Sender, int &Size);

class DELPHICLASS TArrowBar;
class PASCALIMPLEMENTATION TArrowBar : public Controls::TCustomControl 
{
	typedef Controls::TCustomControl inherited;
	
private:
	TRBBackgnd FBackgnd;
	Forms::TFormBorderStyle FBorderStyle;
	int FButtonSize;
	Graphics::TColor FHandleColor;
	Graphics::TColor FButtoncolor;
	Graphics::TColor FHighLightColor;
	Graphics::TColor FShadowColor;
	Graphics::TColor FBorderColor;
	Forms::TScrollBarKind FKind;
	bool FShowArrows;
	bool FShowHandleGrip;
	TRBStyle FStyle;
	Classes::TNotifyEvent FOnChange;
	Classes::TNotifyEvent FOnUserChange;
	void __fastcall SetButtonSize(int Value);
	void __fastcall SetBorderStyle(Forms::TBorderStyle Value);
	void __fastcall SetHandleColor(Graphics::TColor Value);
	void __fastcall SetHighLightColor(Graphics::TColor Value);
	void __fastcall SetShadowColor(Graphics::TColor Value);
	void __fastcall SetButtonColor(Graphics::TColor Value);
	void __fastcall SetBorderColor(Graphics::TColor Value);
	void __fastcall SetKind(Forms::TScrollBarKind Value);
	void __fastcall SetShowArrows(bool Value);
	void __fastcall SetShowHandleGrip(bool Value);
	void __fastcall SetStyle(TRBStyle Value);
	void __fastcall SetBackgnd(TRBBackgnd Value);
	HIDESBASE MESSAGE void __fastcall CMEnabledChanged(Messages::TMessage &Message);
	HIDESBASE MESSAGE void __fastcall CMMouseLeave(Messages::TMessage &Message);
	HIDESBASE MESSAGE void __fastcall WMNCCalcSize(Messages::TWMNCCalcSize &Message);
	HIDESBASE MESSAGE void __fastcall WMNCPaint(Messages::TMessage &Message);
	HIDESBASE MESSAGE void __fastcall WMEraseBkgnd(Messages::TWMEraseBkgnd &Message);
	
protected:
	bool GenChange;
	TRBZone DragZone;
	TRBZone HotZone;
	Extctrls::TTimer* Timer;
	int TimerMode;
	int StoredX;
	int StoredY;
	float PosBeforeDrag;
	virtual void __fastcall DoChange(void);
	virtual void __fastcall DoDrawButton(const Types::TRect &R, TRBDirection Direction, bool Pushed, bool Enabled, bool Hot);
	virtual void __fastcall DoDrawHandle(const Types::TRect &R, bool Horz, bool Pushed, bool Hot);
	virtual void __fastcall DoDrawTrack(const Types::TRect &R, TRBDirection Direction, bool Pushed, bool Enabled, bool Hot);
	DYNAMIC void __fastcall DrawNCArea(HDC ADC, const HRGN Clip);
	virtual bool __fastcall DrawEnabled(void);
	int __fastcall GetBorderSize(void);
	virtual Types::TRect __fastcall GetHandleRect();
	int __fastcall GetButtonSize(void);
	Types::TRect __fastcall GetTrackBoundary();
	TRBZone __fastcall GetZone(int X, int Y);
	Types::TRect __fastcall GetZoneRect(TRBZone Zone);
	virtual void __fastcall MouseLeft(void);
	DYNAMIC void __fastcall MouseDown(Controls::TMouseButton Button, Classes::TShiftState Shift, int X, int Y);
	DYNAMIC void __fastcall MouseMove(Classes::TShiftState Shift, int X, int Y);
	DYNAMIC void __fastcall MouseUp(Controls::TMouseButton Button, Classes::TShiftState Shift, int X, int Y);
	virtual void __fastcall Paint(void);
	void __fastcall StartDragTracking(void);
	void __fastcall StartHotTracking(void);
	void __fastcall StopDragTracking(void);
	void __fastcall StopHotTracking(void);
	virtual void __fastcall TimerHandler(System::TObject* Sender);
	
public:
	__fastcall virtual TArrowBar(Classes::TComponent* AOwner);
	__property Color  = {default=-16777216};
	__property TRBBackgnd Backgnd = {read=FBackgnd, write=SetBackgnd, nodefault};
	__property Forms::TBorderStyle BorderStyle = {read=FBorderStyle, write=SetBorderStyle, default=1};
	__property int ButtonSize = {read=FButtonSize, write=SetButtonSize, default=0};
	__property Graphics::TColor HandleColor = {read=FHandleColor, write=SetHandleColor, default=-16777200};
	__property Graphics::TColor ButtonColor = {read=FButtoncolor, write=SetButtonColor, default=-16777201};
	__property Graphics::TColor HighLightColor = {read=FHighLightColor, write=SetHighLightColor, default=-16777196};
	__property Graphics::TColor ShadowColor = {read=FShadowColor, write=SetShadowColor, default=-16777200};
	__property Graphics::TColor BorderColor = {read=FBorderColor, write=SetBorderColor, default=-16777210};
	__property Forms::TScrollBarKind Kind = {read=FKind, write=SetKind, default=0};
	__property bool ShowArrows = {read=FShowArrows, write=SetShowArrows, default=1};
	__property bool ShowHandleGrip = {read=FShowHandleGrip, write=SetShowHandleGrip, nodefault};
	__property TRBStyle Style = {read=FStyle, write=SetStyle, default=0};
	__property Classes::TNotifyEvent OnChange = {read=FOnChange, write=FOnChange};
	__property Classes::TNotifyEvent OnUserChange = {read=FOnUserChange, write=FOnUserChange};
public:
	#pragma option push -w-inl
	/* TCustomControl.Destroy */ inline __fastcall virtual ~TArrowBar(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TWinControl.CreateParented */ inline __fastcall TArrowBar(HWND ParentWindow) : Controls::TCustomControl(ParentWindow) { }
	#pragma option pop
	
};


typedef Word TRBIncrement;

class DELPHICLASS TCustomRangeBar;
class PASCALIMPLEMENTATION TCustomRangeBar : public TArrowBar 
{
	typedef TArrowBar inherited;
	
private:
	bool FCentered;
	int FEffectiveWindow;
	TRBIncrement FIncrement;
	float FPosition;
	int FRange;
	int FWindow;
	bool __fastcall IsPositionStored(void);
	void __fastcall SetPosition(float Value);
	void __fastcall SetRange(int Value);
	void __fastcall SetWindow(int Value);
	
protected:
	void __fastcall AdjustPosition(void);
	DYNAMIC bool __fastcall DoMouseWheel(Classes::TShiftState Shift, int WheelDelta, const Types::TPoint &MousePos);
	virtual bool __fastcall DrawEnabled(void);
	virtual Types::TRect __fastcall GetHandleRect();
	DYNAMIC void __fastcall MouseDown(Controls::TMouseButton Button, Classes::TShiftState Shift, int X, int Y);
	DYNAMIC void __fastcall MouseMove(Classes::TShiftState Shift, int X, int Y);
	virtual void __fastcall TimerHandler(System::TObject* Sender);
	void __fastcall UpdateEffectiveWindow(void);
	__property int EffectiveWindow = {read=FEffectiveWindow, nodefault};
	
public:
	__fastcall virtual TCustomRangeBar(Classes::TComponent* AOwner);
	DYNAMIC void __fastcall Resize(void);
	void __fastcall SetParams(int NewRange, int NewWindow);
	__property bool Centered = {read=FCentered, write=FCentered, nodefault};
	__property TRBIncrement Increment = {read=FIncrement, write=FIncrement, default=8};
	__property float Position = {read=FPosition, write=SetPosition, stored=IsPositionStored};
	__property int Range = {read=FRange, write=SetRange, default=0};
	__property int Window = {read=FWindow, write=SetWindow, default=0};
public:
	#pragma option push -w-inl
	/* TCustomControl.Destroy */ inline __fastcall virtual ~TCustomRangeBar(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TWinControl.CreateParented */ inline __fastcall TCustomRangeBar(HWND ParentWindow) : TArrowBar(ParentWindow) { }
	#pragma option pop
	
};


class DELPHICLASS TRangeBar;
class PASCALIMPLEMENTATION TRangeBar : public TCustomRangeBar 
{
	typedef TCustomRangeBar inherited;
	
__published:
	__property Align  = {default=0};
	__property Anchors  = {default=3};
	__property Constraints ;
	__property Color  = {default=-16777216};
	__property Backgnd ;
	__property BorderStyle  = {default=1};
	__property ButtonSize  = {default=0};
	__property Enabled  = {default=1};
	__property HandleColor  = {default=-16777200};
	__property ButtonColor  = {default=-16777201};
	__property HighLightColor  = {default=-16777196};
	__property ShadowColor  = {default=-16777200};
	__property BorderColor  = {default=-16777210};
	__property Increment  = {default=8};
	__property Kind  = {default=0};
	__property Range  = {default=0};
	__property Style  = {default=0};
	__property Visible  = {default=1};
	__property Window  = {default=0};
	__property ShowArrows  = {default=1};
	__property ShowHandleGrip ;
	__property Position ;
	__property OnChange ;
	__property OnDragDrop ;
	__property OnDragOver ;
	__property OnEndDrag ;
	__property OnMouseDown ;
	__property OnMouseMove ;
	__property OnMouseUp ;
	__property OnMouseWheelUp ;
	__property OnMouseWheelDown ;
	__property OnStartDrag ;
	__property OnUserChange ;
public:
	#pragma option push -w-inl
	/* TCustomRangeBar.Create */ inline __fastcall virtual TRangeBar(Classes::TComponent* AOwner) : TCustomRangeBar(AOwner) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TCustomControl.Destroy */ inline __fastcall virtual ~TRangeBar(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TWinControl.CreateParented */ inline __fastcall TRangeBar(HWND ParentWindow) : TCustomRangeBar(ParentWindow) { }
	#pragma option pop
	
};


class DELPHICLASS TCustomGaugeBar;
class PASCALIMPLEMENTATION TCustomGaugeBar : public TArrowBar 
{
	typedef TArrowBar inherited;
	
private:
	int FHandleSize;
	int FLargeChange;
	int FMax;
	int FMin;
	int FPosition;
	int FSmallChange;
	void __fastcall SetHandleSize(int Value);
	void __fastcall SetMax(int Value);
	void __fastcall SetMin(int Value);
	void __fastcall SetPosition(int Value);
	void __fastcall SetLargeChange(int Value);
	void __fastcall SetSmallChange(int Value);
	
protected:
	void __fastcall AdjustPosition(void);
	DYNAMIC bool __fastcall DoMouseWheel(Classes::TShiftState Shift, int WheelDelta, const Types::TPoint &MousePos);
	virtual Types::TRect __fastcall GetHandleRect();
	int __fastcall GetHandleSize(void);
	DYNAMIC void __fastcall MouseDown(Controls::TMouseButton Button, Classes::TShiftState Shift, int X, int Y);
	DYNAMIC void __fastcall MouseMove(Classes::TShiftState Shift, int X, int Y);
	virtual void __fastcall TimerHandler(System::TObject* Sender);
	
public:
	__fastcall virtual TCustomGaugeBar(Classes::TComponent* AOwner);
	__property int HandleSize = {read=FHandleSize, write=SetHandleSize, default=0};
	__property int LargeChange = {read=FLargeChange, write=SetLargeChange, default=1};
	__property int Max = {read=FMax, write=SetMax, default=100};
	__property int Min = {read=FMin, write=SetMin, default=0};
	__property int Position = {read=FPosition, write=SetPosition, nodefault};
	__property int SmallChange = {read=FSmallChange, write=SetSmallChange, default=1};
	__property OnChange ;
	__property OnUserChange ;
public:
	#pragma option push -w-inl
	/* TCustomControl.Destroy */ inline __fastcall virtual ~TCustomGaugeBar(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TWinControl.CreateParented */ inline __fastcall TCustomGaugeBar(HWND ParentWindow) : TArrowBar(ParentWindow) { }
	#pragma option pop
	
};


class DELPHICLASS TGaugeBar;
class PASCALIMPLEMENTATION TGaugeBar : public TCustomGaugeBar 
{
	typedef TCustomGaugeBar inherited;
	
__published:
	__property Align  = {default=0};
	__property Anchors  = {default=3};
	__property Constraints ;
	__property Color  = {default=-16777216};
	__property Backgnd ;
	__property BorderStyle  = {default=1};
	__property ButtonSize  = {default=0};
	__property Enabled  = {default=1};
	__property HandleColor  = {default=-16777200};
	__property ButtonColor  = {default=-16777201};
	__property HighLightColor  = {default=-16777196};
	__property ShadowColor  = {default=-16777200};
	__property BorderColor  = {default=-16777210};
	__property HandleSize  = {default=0};
	__property Kind  = {default=0};
	__property LargeChange  = {default=1};
	__property Max  = {default=100};
	__property Min  = {default=0};
	__property ShowArrows  = {default=1};
	__property ShowHandleGrip ;
	__property Style  = {default=0};
	__property SmallChange  = {default=1};
	__property Visible  = {default=1};
	__property Position ;
	__property OnChange ;
	__property OnDragDrop ;
	__property OnDragOver ;
	__property OnEndDrag ;
	__property OnMouseDown ;
	__property OnMouseMove ;
	__property OnMouseUp ;
	__property OnStartDrag ;
	__property OnUserChange ;
public:
	#pragma option push -w-inl
	/* TCustomGaugeBar.Create */ inline __fastcall virtual TGaugeBar(Classes::TComponent* AOwner) : TCustomGaugeBar(AOwner) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TCustomControl.Destroy */ inline __fastcall virtual ~TGaugeBar(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TWinControl.CreateParented */ inline __fastcall TGaugeBar(HWND ParentWindow) : TCustomGaugeBar(ParentWindow) { }
	#pragma option pop
	
};


class DELPHICLASS TArrowBarAccess;
class PASCALIMPLEMENTATION TArrowBarAccess : public Classes::TPersistent 
{
	typedef Classes::TPersistent inherited;
	
private:
	TArrowBar* FMaster;
	TArrowBar* FSlave;
	TRBBackgnd __fastcall GetBackgnd(void);
	int __fastcall GetButtonSize(void);
	Graphics::TColor __fastcall GetColor(void);
	Graphics::TColor __fastcall GetHandleColor(void);
	Graphics::TColor __fastcall GetHighLightColor(void);
	Graphics::TColor __fastcall GetButtonColor(void);
	Graphics::TColor __fastcall GetBorderColor(void);
	Graphics::TColor __fastcall GetShadowColor(void);
	bool __fastcall GetShowArrows(void);
	bool __fastcall GetShowHandleGrip(void);
	TRBStyle __fastcall GetStyle(void);
	void __fastcall SetBackgnd(TRBBackgnd Value);
	void __fastcall SetButtonSize(int Value);
	void __fastcall SetColor(Graphics::TColor Value);
	void __fastcall SetHandleColor(Graphics::TColor Value);
	void __fastcall SetShowArrows(bool Value);
	void __fastcall SetShowHandleGrip(bool Value);
	void __fastcall SetStyle(TRBStyle Value);
	void __fastcall SetHighLightColor(Graphics::TColor Value);
	void __fastcall SetShadowColor(Graphics::TColor Value);
	void __fastcall SetButtonColor(Graphics::TColor Value);
	void __fastcall SetBorderColor(Graphics::TColor Value);
	
public:
	__property TArrowBar* Master = {read=FMaster, write=FMaster};
	__property TArrowBar* Slave = {read=FSlave, write=FSlave};
	
__published:
	__property Graphics::TColor Color = {read=GetColor, write=SetColor, default=-16777216};
	__property TRBBackgnd Backgnd = {read=GetBackgnd, write=SetBackgnd, default=0};
	__property int ButtonSize = {read=GetButtonSize, write=SetButtonSize, default=0};
	__property Graphics::TColor HandleColor = {read=GetHandleColor, write=SetHandleColor, default=-16777200};
	__property Graphics::TColor ButtonColor = {read=GetButtonColor, write=SetButtonColor, default=-16777201};
	__property Graphics::TColor HighLightColor = {read=GetHighLightColor, write=SetHighLightColor, default=-16777196};
	__property Graphics::TColor ShadowColor = {read=GetShadowColor, write=SetShadowColor, default=-16777200};
	__property Graphics::TColor BorderColor = {read=GetBorderColor, write=SetBorderColor, default=-16777210};
	__property bool ShowArrows = {read=GetShowArrows, write=SetShowArrows, default=1};
	__property bool ShowHandleGrip = {read=GetShowHandleGrip, write=SetShowHandleGrip, nodefault};
	__property TRBStyle Style = {read=GetStyle, write=SetStyle, nodefault};
public:
	#pragma option push -w-inl
	/* TPersistent.Destroy */ inline __fastcall virtual ~TArrowBarAccess(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TArrowBarAccess(void) : Classes::TPersistent() { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Gr32_rangebars */
using namespace Gr32_rangebars;
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// Gr32_rangebars
