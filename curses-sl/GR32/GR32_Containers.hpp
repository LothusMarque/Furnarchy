// Borland C++ Builder
// Copyright (c) 1995, 2005 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'Gr32_containers.pas' rev: 10.00

#ifndef Gr32_containersHPP
#define Gr32_containersHPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member functions
#pragma pack(push,8)
#include <System.hpp>	// Pascal unit
#include <Sysinit.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <Rtlconsts.hpp>	// Pascal unit
#include <Gr32.hpp>	// Pascal unit
#include <Sysutils.hpp>	// Pascal unit
#include <Gr32_lowlevel.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Typinfo.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Gr32_containers
{
//-- type declarations -------------------------------------------------------
typedef void * *PPItem;

typedef void *PItem;

typedef void * *PPData;

typedef void *PData;

struct TPointerBucketItem;
typedef TPointerBucketItem *PPointerBucketItem;

struct TPointerBucketItem
{
	
public:
	void *Item;
	void *Data;
} ;

typedef DynamicArray<TPointerBucketItem >  TPointerBucketItemArray;

struct TPointerBucket
{
	
public:
	int Count;
	DynamicArray<TPointerBucketItem >  Items;
} ;

typedef TPointerBucket TPointerBucketArray[256];

class DELPHICLASS TPointerMap;
class PASCALIMPLEMENTATION TPointerMap : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	void * operator[](void * Item) { return Data[Item]; }
	
private:
	TPointerBucket FBuckets[256];
	int FCount;
	
protected:
	void * __fastcall GetData(void * Item);
	void __fastcall SetData(void * Item, const void * Data);
	bool __fastcall Exists(void * Item, /* out */ int &BucketIndex, /* out */ int &ItemIndex);
	virtual void * __fastcall Delete(int BucketIndex, int ItemIndex);
	
public:
	__fastcall virtual ~TPointerMap(void);
	PPData __fastcall Add(void * NewItem)/* overload */;
	PPData __fastcall Add(void * NewItem, /* out */ bool &IsNew)/* overload */;
	PPData __fastcall Add(void * NewItem, void * NewData)/* overload */;
	PPData __fastcall Add(void * NewItem, void * NewData, /* out */ bool &IsNew)/* overload */;
	void * __fastcall Remove(void * Item);
	void __fastcall Clear(void);
	bool __fastcall Contains(void * Item);
	bool __fastcall Find(void * Item, /* out */ PPData &Data);
	__property void * Data[void * Item] = {read=GetData, write=SetData/*, default*/};
	__property int Count = {read=FCount, nodefault};
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TPointerMap(void) : System::TObject() { }
	#pragma option pop
	
};


class DELPHICLASS TPointerMapIterator;
class PASCALIMPLEMENTATION TPointerMapIterator : public System::TObject 
{
	typedef System::TObject inherited;
	
private:
	TPointerMap* FSrcPointerMap;
	void *FItem;
	void *FData;
	int FCurBucketIndex;
	int FCurItemIndex;
	
public:
	__fastcall TPointerMapIterator(TPointerMap* SrcPointerMap);
	bool __fastcall Next(void);
	__property void * Item = {read=FItem};
	__property void * Data = {read=FData};
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TPointerMapIterator(void) { }
	#pragma option pop
	
};


typedef Types::TRect TPolyRects[67108863];

typedef Types::TRect *PPolyRects;

class DELPHICLASS TRectList;
class PASCALIMPLEMENTATION TRectList : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	Types::PRect operator[](int Index) { return Items[Index]; }
	
private:
	Types::TRect *FList;
	int FCount;
	int FCapacity;
	
protected:
	Types::PRect __fastcall Get(int Index);
	virtual void __fastcall Grow(void);
	void __fastcall SetCapacity(int NewCapacity);
	void __fastcall SetCount(int NewCount);
	
public:
	__fastcall virtual ~TRectList(void);
	int __fastcall Add(const Types::TRect &Rect);
	virtual void __fastcall Clear(void);
	void __fastcall Delete(int Index);
	void __fastcall Exchange(int Index1, int Index2);
	int __fastcall IndexOf(const Types::TRect &Rect);
	void __fastcall Insert(int Index, const Types::TRect &Rect);
	void __fastcall Move(int CurIndex, int NewIndex);
	int __fastcall Remove(const Types::TRect &Rect);
	void __fastcall Pack(void);
	__property int Capacity = {read=FCapacity, write=SetCapacity, nodefault};
	__property int Count = {read=FCount, write=SetCount, nodefault};
	__property Types::PRect Items[int Index] = {read=Get/*, default*/};
	__property PPolyRects List = {read=FList};
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TRectList(void) : System::TObject() { }
	#pragma option pop
	
};


class DELPHICLASS TClassList;
class PASCALIMPLEMENTATION TClassList : public Classes::TList 
{
	typedef Classes::TList inherited;
	
public:
	TMetaClass* operator[](int Index) { return Items[Index]; }
	
protected:
	TMetaClass* __fastcall GetItems(int Index);
	void __fastcall SetItems(int Index, TMetaClass* AClass);
	
public:
	HIDESBASE int __fastcall Add(TMetaClass* AClass);
	HIDESBASE TMetaClass* __fastcall Extract(TMetaClass* Item);
	HIDESBASE int __fastcall Remove(TMetaClass* AClass);
	HIDESBASE int __fastcall IndexOf(TMetaClass* AClass);
	HIDESBASE TMetaClass* __fastcall First(void);
	HIDESBASE TMetaClass* __fastcall Last(void);
	TMetaClass* __fastcall Find(AnsiString AClassName);
	void __fastcall GetClassNames(Classes::TStrings* Strings);
	HIDESBASE void __fastcall Insert(int Index, TMetaClass* AClass);
	__property TMetaClass* Items[int Index] = {read=GetItems, write=SetItems/*, default*/};
public:
	#pragma option push -w-inl
	/* TList.Destroy */ inline __fastcall virtual ~TClassList(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TClassList(void) : Classes::TList() { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------
static const Byte BUCKET_MASK = 0xff;
static const Word BUCKET_COUNT = 0x100;
extern PACKAGE void __fastcall SmartAssign(Classes::TPersistent* Src, Classes::TPersistent* Dst, Typinfo::TTypeKinds TypeKinds = (Set<Typinfo::TTypeKind, tkUnknown, tkDynArray> () << Typinfo::TTypeKind(1) << Typinfo::TTypeKind(2) << Typinfo::TTypeKind(3) << Typinfo::TTypeKind(4) << Typinfo::TTypeKind(5) << Typinfo::TTypeKind(6) << Typinfo::TTypeKind(7) << Typinfo::TTypeKind(9) << Typinfo::TTypeKind(10) << Typinfo::TTypeKind(11) << Typinfo::TTypeKind(12) << Typinfo::TTypeKind(13) << Typinfo::TTypeKind(14) << Typinfo::TTypeKind(15) << Typinfo::TTypeKind(16) << Typinfo::TTypeKind(17) ));

}	/* namespace Gr32_containers */
using namespace Gr32_containers;
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// Gr32_containers
