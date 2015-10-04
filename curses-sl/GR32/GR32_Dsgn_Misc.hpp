// Borland C++ Builder
// Copyright (c) 1995, 2005 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'Gr32_dsgn_misc.pas' rev: 10.00

#ifndef Gr32_dsgn_miscHPP
#define Gr32_dsgn_miscHPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member functions
#pragma pack(push,8)
#include <System.hpp>	// Pascal unit
#include <Sysinit.hpp>	// Pascal unit
#include <Designintf.hpp>	// Pascal unit
#include <Designeditors.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Typinfo.hpp>	// Pascal unit
#include <Gr32_containers.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Gr32_dsgn_misc
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TCustomClassProperty;
class PASCALIMPLEMENTATION TCustomClassProperty : public Designeditors::TClassProperty 
{
	typedef Designeditors::TClassProperty inherited;
	
private:
	bool __fastcall HasSubProperties(void);
	
protected:
	#pragma option push -w-inl
	/* virtual class method */ virtual Gr32_containers::TClassList* __fastcall GetClassList() { return GetClassList(__classid(TCustomClassProperty)); }
	#pragma option pop
	/*         class method */ static Gr32_containers::TClassList* __fastcall GetClassList(TMetaClass* vmt);
	virtual void __fastcall SetClassName(const AnsiString CustomClass) = 0 ;
	virtual System::TObject* __fastcall GetObject(void) = 0 ;
	
public:
	virtual Designintf::TPropertyAttributes __fastcall GetAttributes(void);
	virtual void __fastcall GetValues(Classes::TGetStrProc Proc);
	virtual void __fastcall SetValue(const AnsiString Value)/* overload */;
	virtual AnsiString __fastcall GetValue();
public:
	#pragma option push -w-inl
	/* TPropertyEditor.Create */ inline __fastcall virtual TCustomClassProperty(const Designintf::_di_IDesigner ADesigner, int APropCount) : Designeditors::TClassProperty(ADesigner, APropCount) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TPropertyEditor.Destroy */ inline __fastcall virtual ~TCustomClassProperty(void) { }
	#pragma option pop
	
	
/* Hoisted overloads: */
	
public:
	inline void __fastcall  SetValue(const WideString Value){ TPropertyEditor::SetValue(Value); }
	
};


class DELPHICLASS TKernelClassProperty;
class PASCALIMPLEMENTATION TKernelClassProperty : public TCustomClassProperty 
{
	typedef TCustomClassProperty inherited;
	
protected:
	#pragma option push -w-inl
	/* virtual class method */ virtual Gr32_containers::TClassList* __fastcall GetClassList() { return GetClassList(__classid(TKernelClassProperty)); }
	#pragma option pop
	/*         class method */ static Gr32_containers::TClassList* __fastcall GetClassList(TMetaClass* vmt);
	virtual void __fastcall SetClassName(const AnsiString CustomClass);
	virtual System::TObject* __fastcall GetObject(void);
public:
	#pragma option push -w-inl
	/* TPropertyEditor.Create */ inline __fastcall virtual TKernelClassProperty(const Designintf::_di_IDesigner ADesigner, int APropCount) : TCustomClassProperty(ADesigner, APropCount) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TPropertyEditor.Destroy */ inline __fastcall virtual ~TKernelClassProperty(void) { }
	#pragma option pop
	
};


class DELPHICLASS TResamplerClassProperty;
class PASCALIMPLEMENTATION TResamplerClassProperty : public TCustomClassProperty 
{
	typedef TCustomClassProperty inherited;
	
protected:
	#pragma option push -w-inl
	/* virtual class method */ virtual Gr32_containers::TClassList* __fastcall GetClassList() { return GetClassList(__classid(TResamplerClassProperty)); }
	#pragma option pop
	/*         class method */ static Gr32_containers::TClassList* __fastcall GetClassList(TMetaClass* vmt);
	virtual void __fastcall SetClassName(const AnsiString CustomClass);
	virtual System::TObject* __fastcall GetObject(void);
public:
	#pragma option push -w-inl
	/* TPropertyEditor.Create */ inline __fastcall virtual TResamplerClassProperty(const Designintf::_di_IDesigner ADesigner, int APropCount) : TCustomClassProperty(ADesigner, APropCount) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TPropertyEditor.Destroy */ inline __fastcall virtual ~TResamplerClassProperty(void) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Gr32_dsgn_misc */
using namespace Gr32_dsgn_misc;
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// Gr32_dsgn_misc
