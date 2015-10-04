// Borland C++ Builder
// Copyright (c) 1995, 2005 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'Gr32_dsgn_bitmap.pas' rev: 10.00

#ifndef Gr32_dsgn_bitmapHPP
#define Gr32_dsgn_bitmapHPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member functions
#pragma pack(push,8)
#include <System.hpp>	// Pascal unit
#include <Sysinit.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <Controls.hpp>	// Pascal unit
#include <Forms.hpp>	// Pascal unit
#include <Dialogs.hpp>	// Pascal unit
#include <Extctrls.hpp>	// Pascal unit
#include <Stdctrls.hpp>	// Pascal unit
#include <Extdlgs.hpp>	// Pascal unit
#include <Comctrls.hpp>	// Pascal unit
#include <Menus.hpp>	// Pascal unit
#include <Toolwin.hpp>	// Pascal unit
#include <Registry.hpp>	// Pascal unit
#include <Imglist.hpp>	// Pascal unit
#include <Clipbrd.hpp>	// Pascal unit
#include <Sysutils.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Consts.hpp>	// Pascal unit
#include <Gr32.hpp>	// Pascal unit
#include <Gr32_image.hpp>	// Pascal unit
#include <Gr32_layers.hpp>	// Pascal unit
#include <Gr32_filters.hpp>	// Pascal unit
#include <Designintf.hpp>	// Pascal unit
#include <Designeditors.hpp>	// Pascal unit
#include <Vcleditors.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Gr32_dsgn_bitmap
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TPictureEditorForm;
class PASCALIMPLEMENTATION TPictureEditorForm : public Forms::TForm 
{
	typedef Forms::TForm inherited;
	
__published:
	Comctrls::TToolBar* ToolBar1;
	Comctrls::TToolButton* Load;
	Comctrls::TToolButton* Save;
	Controls::TImageList* ImageList;
	Comctrls::TToolButton* Clear;
	Comctrls::TToolButton* ToolButton2;
	Comctrls::TToolButton* Copy;
	Comctrls::TToolButton* Paste;
	Extctrls::TTimer* Timer;
	Comctrls::TPageControl* PageControl;
	Comctrls::TTabSheet* ImageSheet;
	Comctrls::TTabSheet* AlphaSheet;
	Menus::TPopupMenu* PopupMenu;
	Menus::TMenuItem* mnSave;
	Menus::TMenuItem* mnSeparator;
	Menus::TMenuItem* mnCopy;
	Menus::TMenuItem* mnPaste;
	Menus::TMenuItem* mnClear;
	Menus::TMenuItem* Load1;
	Menus::TMenuItem* mnSeparator2;
	Menus::TMenuItem* mnInvert;
	Extctrls::TPanel* Panel1;
	Stdctrls::TButton* OKButton;
	Stdctrls::TButton* Cancel;
	Stdctrls::TLabel* Label1;
	Stdctrls::TComboBox* MagnCombo;
	Extctrls::TPanel* Panel2;
	Extctrls::TBevel* Bevel1;
	void __fastcall LoadClick(System::TObject* Sender);
	void __fastcall SaveClick(System::TObject* Sender);
	void __fastcall ClearClick(System::TObject* Sender);
	void __fastcall CopyClick(System::TObject* Sender);
	void __fastcall PasteClick(System::TObject* Sender);
	void __fastcall TimerTimer(System::TObject* Sender);
	void __fastcall PopupMenuPopup(System::TObject* Sender);
	void __fastcall mnInvertClick(System::TObject* Sender);
	void __fastcall FormCreate(System::TObject* Sender);
	void __fastcall MagnComboChange(System::TObject* Sender);
	
protected:
	Extdlgs::TOpenPictureDialog* OpenDialog;
	Extdlgs::TSavePictureDialog* SaveDialog;
	Gr32_image::TImage32* AlphaChannel;
	Gr32_image::TImage32* RGBChannels;
	void __fastcall AlphaChannelMouseMove(System::TObject* Sender, Classes::TShiftState Shift, int X, int Y, Gr32_layers::TCustomLayer* Layer);
	void __fastcall RGBChannelsMouseMove(System::TObject* Sender, Classes::TShiftState Shift, int X, int Y, Gr32_layers::TCustomLayer* Layer);
	Gr32_image::TImage32* __fastcall CurrentImage(void);
	
public:
	__fastcall virtual TPictureEditorForm(Classes::TComponent* AOwner);
public:
	#pragma option push -w-inl
	/* TCustomForm.CreateNew */ inline __fastcall virtual TPictureEditorForm(Classes::TComponent* AOwner, int Dummy) : Forms::TForm(AOwner, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TCustomForm.Destroy */ inline __fastcall virtual ~TPictureEditorForm(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TWinControl.CreateParented */ inline __fastcall TPictureEditorForm(HWND ParentWindow) : Forms::TForm(ParentWindow) { }
	#pragma option pop
	
};


class DELPHICLASS TBitmap32Editor;
class PASCALIMPLEMENTATION TBitmap32Editor : public Classes::TComponent 
{
	typedef Classes::TComponent inherited;
	
private:
	Gr32::TBitmap32* FBitmap32;
	TPictureEditorForm* FPicDlg;
	void __fastcall SetBitmap32(Gr32::TBitmap32* Value);
	
public:
	__fastcall virtual TBitmap32Editor(Classes::TComponent* AOwner);
	__fastcall virtual ~TBitmap32Editor(void);
	bool __fastcall Execute(void);
	__property Gr32::TBitmap32* Bitmap32 = {read=FBitmap32, write=SetBitmap32};
};


class DELPHICLASS TBitmap32Property;
class PASCALIMPLEMENTATION TBitmap32Property : public Designeditors::TClassProperty 
{
	typedef Designeditors::TClassProperty inherited;
	
public:
	virtual void __fastcall Edit(void);
	virtual Designintf::TPropertyAttributes __fastcall GetAttributes(void);
	virtual AnsiString __fastcall GetValue();
	virtual void __fastcall SetValue(const AnsiString Value)/* overload */;
	void __fastcall PropDrawName(Graphics::TCanvas* ACanvas, const Types::TRect &ARect, bool ASelected);
	void __fastcall PropDrawValue(Graphics::TCanvas* Canvas, const Types::TRect &ARect, bool ASelected);
	Types::TRect __fastcall PropDrawNameRect(const Types::TRect &ARect);
	Types::TRect __fastcall PropDrawValueRect(const Types::TRect &ARect);
public:
	#pragma option push -w-inl
	/* TPropertyEditor.Create */ inline __fastcall virtual TBitmap32Property(const Designintf::_di_IDesigner ADesigner, int APropCount) : Designeditors::TClassProperty(ADesigner, APropCount) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TPropertyEditor.Destroy */ inline __fastcall virtual ~TBitmap32Property(void) { }
	#pragma option pop
	
	
/* Hoisted overloads: */
	
public:
	inline void __fastcall  SetValue(const WideString Value){ TPropertyEditor::SetValue(Value); }
	
private:
	void *__ICustomPropertyDrawing80;	/* Vcleditors::ICustomPropertyDrawing80 */
	
public:
	operator ICustomPropertyDrawing80*(void) { return (ICustomPropertyDrawing80*)&__ICustomPropertyDrawing80; }
	operator ICustomPropertyDrawing*(void) { return (ICustomPropertyDrawing*)&__ICustomPropertyDrawing80; }
	
};


class DELPHICLASS TImage32Editor;
class PASCALIMPLEMENTATION TImage32Editor : public Designeditors::TComponentEditor 
{
	typedef Designeditors::TComponentEditor inherited;
	
public:
	virtual void __fastcall ExecuteVerb(int Index);
	virtual AnsiString __fastcall GetVerb(int Index);
	virtual int __fastcall GetVerbCount(void);
public:
	#pragma option push -w-inl
	/* TComponentEditor.Create */ inline __fastcall virtual TImage32Editor(Classes::TComponent* AComponent, Designintf::_di_IDesigner ADesigner) : Designeditors::TComponentEditor(AComponent, ADesigner) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TImage32Editor(void) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Gr32_dsgn_bitmap */
using namespace Gr32_dsgn_bitmap;
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// Gr32_dsgn_bitmap
