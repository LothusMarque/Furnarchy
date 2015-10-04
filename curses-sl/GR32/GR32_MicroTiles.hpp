// Borland C++ Builder
// Copyright (c) 1995, 2005 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'Gr32_microtiles.pas' rev: 10.00

#ifndef Gr32_microtilesHPP
#define Gr32_microtilesHPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member functions
#pragma pack(push,8)
#include <System.hpp>	// Pascal unit
#include <Sysinit.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <Types.hpp>	// Pascal unit
#include <Sysutils.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Gr32.hpp>	// Pascal unit
#include <Gr32_system.hpp>	// Pascal unit
#include <Gr32_containers.hpp>	// Pascal unit
#include <Gr32_layers.hpp>	// Pascal unit
#include <Gr32_repaintopt.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Gr32_microtiles
{
//-- type declarations -------------------------------------------------------
typedef int TMicroTile;

typedef TMicroTile *PMicroTile;

typedef TMicroTile TMicroTileArray[134217727];

typedef TMicroTile *PMicroTileArray;

struct TMicroTiles;
typedef TMicroTiles *PMicroTiles;

typedef PMicroTiles *PPMicroTiles;

struct TMicroTiles
{
	
public:
	#pragma pack(push,1)
	Types::TRect BoundsRect;
	#pragma pack(pop)
	int Columns;
	int Rows;
	#pragma pack(push,1)
	Types::TRect BoundsUsedTiles;
	#pragma pack(pop)
	int Count;
	TMicroTile *Tiles;
} ;

class DELPHICLASS TMicroTilesMap;
class PASCALIMPLEMENTATION TMicroTilesMap : public Gr32_containers::TPointerMap 
{
	typedef Gr32_containers::TPointerMap inherited;
	
public:
	PMicroTiles operator[](void * Item) { return Data[Item]; }
	
private:
	HIDESBASE PMicroTiles __fastcall GetData(void * Item);
	HIDESBASE void __fastcall SetData(void * Item, const PMicroTiles Data);
	
protected:
	virtual void * __fastcall Delete(int BucketIndex, int ItemIndex);
	
public:
	HIDESBASE PPMicroTiles __fastcall Add(void * Item);
	__property PMicroTiles Data[void * Item] = {read=GetData, write=SetData/*, default*/};
public:
	#pragma option push -w-inl
	/* TPointerMap.Destroy */ inline __fastcall virtual ~TMicroTilesMap(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TMicroTilesMap(void) : Gr32_containers::TPointerMap() { }
	#pragma option pop
	
};


class DELPHICLASS TMicroTilesRepaintOptimizer;
class PASCALIMPLEMENTATION TMicroTilesRepaintOptimizer : public Gr32_repaintopt::TCustomRepaintOptimizer 
{
	typedef Gr32_repaintopt::TCustomRepaintOptimizer inherited;
	
private:
	#pragma pack(push,1)
	Types::TRect FBufferBounds;
	#pragma pack(pop)
	TMicroTiles *FWorkMicroTiles;
	TMicroTiles FTempTiles;
	TMicroTiles FInvalidTiles;
	TMicroTiles FForcedInvalidTiles;
	Classes::TList* FInvalidLayers;
	TMicroTilesMap* FOldInvalidTilesMap;
	bool FWorkingTilesValid;
	bool FOldInvalidTilesValid;
	bool FUseInvalidTiles;
	bool FAdaptiveMode;
	Gr32_system::TPerfTimer* FPerfTimer;
	int FPerformanceLevel;
	__int64 FElapsedTimeForLastRepaint;
	__int64 FElapsedTimeForFullSceneRepaint;
	bool FAdaptionFailed;
	bool FTimedCheck;
	int FTimeDelta;
	int FNextCheck;
	__int64 FElapsedTimeOnLastPenalty;
	int FOldInvalidRectsCount;
	void __fastcall DrawLayerToMicroTiles(TMicroTiles &DstTiles, Gr32_layers::TCustomLayer* Layer);
	void __fastcall DrawMeasuringHandler(System::TObject* Sender, const Types::TRect &Area, const unsigned Info);
	void __fastcall ValidateWorkingTiles(void);
	void __fastcall UpdateOldInvalidTiles(void);
	void __fastcall SetAdaptiveMode(const bool Value);
	void __fastcall ResetAdaptiveMode(void);
	void __fastcall BeginAdaption(void);
	void __fastcall EndAdaption(void);
	void __fastcall AddArea(TMicroTiles &Tiles, const Types::TRect &Area, const unsigned Info);
	
protected:
	virtual void __fastcall SetEnabled(const bool Value);
	virtual void __fastcall LayerCollectionNotifyHandler(Gr32_layers::TLayerCollection* Sender, Gr32_layers::TLayerListNotification Action, Gr32_layers::TCustomLayer* Layer, int Index);
	
public:
	__fastcall virtual TMicroTilesRepaintOptimizer(Gr32::TBitmap32* Buffer, Gr32_containers::TRectList* InvalidRects);
	__fastcall virtual ~TMicroTilesRepaintOptimizer(void);
	virtual void __fastcall RegisterLayerCollection(Gr32_layers::TLayerCollection* Layers);
	virtual void __fastcall UnregisterLayerCollection(Gr32_layers::TLayerCollection* Layers);
	virtual void __fastcall Reset(void);
	virtual bool __fastcall UpdatesAvailable(void);
	virtual void __fastcall PerformOptimization(void);
	virtual void __fastcall BeginPaintBuffer(void);
	virtual void __fastcall EndPaintBuffer(void);
	virtual void __fastcall AreaUpdateHandler(System::TObject* Sender, const Types::TRect &Area, const unsigned Info);
	virtual void __fastcall LayerUpdateHandler(System::TObject* Sender, Gr32_layers::TCustomLayer* Layer);
	virtual void __fastcall BufferResizedHandler(const int NewWidth, const int NewHeight);
	__property bool AdaptiveMode = {read=FAdaptiveMode, write=SetAdaptiveMode, nodefault};
};


//-- var, const, procedure ---------------------------------------------------
static const Shortint MICROTILE_SHIFT = 0x5;
static const Shortint MICROTILE_SIZE = 0x20;
static const Shortint MICROTILE_EMPTY = 0x0;
static const Word MICROTILE_FULL = 0x2020;
extern PACKAGE void __fastcall (*MicroTileUnion)(TMicroTile &DstTile, const TMicroTile SrcTile);
extern PACKAGE TMicroTile __fastcall MakeMicroTile(const int Left, const int Top, const int Right, const int Bottom);
extern PACKAGE int __fastcall MicroTileHeight(const TMicroTile Tile);
extern PACKAGE int __fastcall MicroTileWidth(const TMicroTile Tile);
extern PACKAGE void __fastcall _MicroTileUnion(TMicroTile &DstTile, const TMicroTile SrcTile);
extern PACKAGE void __fastcall M_MicroTileUnion(TMicroTile &DstTile, const TMicroTile SrcTile);
extern PACKAGE TMicroTiles __fastcall MakeEmptyMicroTiles();
extern PACKAGE void __fastcall MicroTilesCreate(TMicroTiles &MicroTiles);
extern PACKAGE void __fastcall MicroTilesDestroy(TMicroTiles &MicroTiles);
extern PACKAGE void __fastcall MicroTilesSetSize(TMicroTiles &MicroTiles, const Types::TRect &DstRect);
extern PACKAGE void __fastcall MicroTilesClear(TMicroTiles &MicroTiles, const TMicroTile Value = (TMicroTile)(0x0));
extern PACKAGE void __fastcall MicroTilesClearUsed(TMicroTiles &MicroTiles, const TMicroTile Value = (TMicroTile)(0x0));
extern PACKAGE void __fastcall MicroTilesCopy(TMicroTiles &DstTiles, const TMicroTiles &SrcTiles);
extern PACKAGE void __fastcall MicroTilesAddLine(TMicroTiles &MicroTiles, int X1, int Y1, int X2, int Y2, int LineWidth, bool RoundToWholeTiles = false);
extern PACKAGE void __fastcall MicroTilesAddRect(TMicroTiles &MicroTiles, const Types::TRect &Rect, bool RoundToWholeTiles = false);
extern PACKAGE void __fastcall _MicroTilesUnion(TMicroTiles &DstTiles, const TMicroTiles &SrcTiles);
extern PACKAGE void __fastcall M_MicroTilesUnion(TMicroTiles &DstTiles, const TMicroTiles &SrcTiles);
extern PACKAGE void __fastcall MicroTilesUnion(TMicroTiles &DstTiles, const TMicroTiles &SrcTiles, bool RoundToWholeTiles = false);
extern PACKAGE int __fastcall MicroTilesCalcRects(const TMicroTiles &MicroTiles, Gr32_containers::TRectList* DstRects, bool CountOnly = false, bool RoundToWholeTiles = false)/* overload */;
extern PACKAGE int __fastcall MicroTilesCalcRects(const TMicroTiles &MicroTiles, Gr32_containers::TRectList* DstRects, const Types::TRect &Clip, bool CountOnly = false, bool RoundToWholeTiles = false)/* overload */;
extern PACKAGE int __fastcall MicroTilesCountEmptyTiles(const TMicroTiles &MicroTiles);

}	/* namespace Gr32_microtiles */
using namespace Gr32_microtiles;
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// Gr32_microtiles
