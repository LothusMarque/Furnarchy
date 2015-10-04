// Borland C++ Builder
// Copyright (c) 1995, 2005 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'Gr32_dsgn_color.pas' rev: 10.00

#ifndef Gr32_dsgn_colorHPP
#define Gr32_dsgn_colorHPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member functions
#pragma pack(push,8)
#include <System.hpp>	// Pascal unit
#include <Sysinit.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Sysutils.hpp>	// Pascal unit
#include <Consts.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <Registry.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <Dialogs.hpp>	// Pascal unit
#include <Forms.hpp>	// Pascal unit
#include <Gr32.hpp>	// Pascal unit
#include <Gr32_image.hpp>	// Pascal unit
#include <Designintf.hpp>	// Pascal unit
#include <Designeditors.hpp>	// Pascal unit
#include <Vcleditors.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Gr32_dsgn_color
{
//-- type declarations -------------------------------------------------------
struct TColorEntry;
typedef TColorEntry *PColorEntry;

struct TColorEntry
{
	
public:
	SmallString<31>  Name;
	Gr32::TColor32 Color;
} ;

class DELPHICLASS TColorManager;
class PASCALIMPLEMENTATION TColorManager : public Classes::TList 
{
	typedef Classes::TList inherited;
	
public:
	__fastcall virtual ~TColorManager(void);
	void __fastcall AddColor(const AnsiString AName, Gr32::TColor32 AColor);
	void __fastcall EnumColors(Classes::TGetStrProc Proc);
	Gr32::TColor32 __fastcall FindColor(const AnsiString AName);
	Gr32::TColor32 __fastcall GetColor(const AnsiString AName);
	AnsiString __fastcall GetColorName(Gr32::TColor32 AColor);
	void __fastcall RegisterDefaultColors(void);
	void __fastcall RemoveColor(const AnsiString AName);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TColorManager(void) : Classes::TList() { }
	#pragma option pop
	
};


class DELPHICLASS TColor32Property;
class PASCALIMPLEMENTATION TColor32Property : public Designeditors::TIntegerProperty 
{
	typedef Designeditors::TIntegerProperty inherited;
	
public:
	virtual Designintf::TPropertyAttributes __fastcall GetAttributes(void);
	virtual AnsiString __fastcall GetValue();
	virtual void __fastcall GetValues(Classes::TGetStrProc Proc);
	virtual void __fastcall SetValue(const AnsiString Value)/* overload */;
	virtual void __fastcall Edit(void);
	void __fastcall ListMeasureWidth(const AnsiString Value, Graphics::TCanvas* ACanvas, int &AWidth);
	void __fastcall ListMeasureHeight(const AnsiString Value, Graphics::TCanvas* ACanvas, int &AHeight);
	void __fastcall ListDrawValue(const AnsiString Value, Graphics::TCanvas* ACanvas, const Types::TRect &ARect, bool ASelected);
	void __fastcall PropDrawName(Graphics::TCanvas* ACanvas, const Types::TRect &ARect, bool ASelected);
	void __fastcall PropDrawValue(Graphics::TCanvas* ACanvas, const Types::TRect &ARect, bool ASelected);
	Types::TRect __fastcall PropDrawNameRect(const Types::TRect &ARect);
	Types::TRect __fastcall PropDrawValueRect(const Types::TRect &ARect);
public:
	#pragma option push -w-inl
	/* TPropertyEditor.Create */ inline __fastcall virtual TColor32Property(const Designintf::_di_IDesigner ADesigner, int APropCount) : Designeditors::TIntegerProperty(ADesigner, APropCount) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TPropertyEditor.Destroy */ inline __fastcall virtual ~TColor32Property(void) { }
	#pragma option pop
	
	
/* Hoisted overloads: */
	
public:
	inline void __fastcall  SetValue(const WideString Value){ TPropertyEditor::SetValue(Value); }
	
private:
	void *__ICustomPropertyListDrawing;	/* Vcleditors::ICustomPropertyListDrawing */
	void *__ICustomPropertyDrawing80;	/* Vcleditors::ICustomPropertyDrawing80 */
	
public:
	operator ICustomPropertyDrawing80*(void) { return (ICustomPropertyDrawing80*)&__ICustomPropertyDrawing80; }
	operator ICustomPropertyDrawing*(void) { return (ICustomPropertyDrawing*)&__ICustomPropertyDrawing80; }
	operator ICustomPropertyListDrawing*(void) { return (ICustomPropertyListDrawing*)&__ICustomPropertyListDrawing; }
	
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE TColorManager* ColorManager;
extern PACKAGE void __fastcall RegisterColor(const AnsiString AName, Gr32::TColor32 AColor);
extern PACKAGE void __fastcall UnregisterColor(const AnsiString AName);

}	/* namespace Gr32_dsgn_color */
using namespace Gr32_dsgn_color;
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// Gr32_dsgn_color
