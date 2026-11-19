// FXList.h: a named list of FX nuggets (FXList) and the nugget base (FXNugget).
// Target facts (retail game.dat):
// - doFXObj 0x000B2235 is __cdecl with (const FXList *, const Object *, const Object *) and
//   returns void; every call site pushes the two objects and the list and adds 0xC to esp.
//
// FXNugget (the map's Rva001DFEAABase; name from the field table FXNuggetFieldParse, RVA
// 0x007DCA18). SIZE 0x148.
// - ctor COMDAT 0x001DFEAA: table VA 0x00BDC940, ObjectFilter ctors at +0x08 and +0x0C, four
//   ModelConditionFlags at +0x10, +0x5C, +0xA8, +0xF4 (COMDAT 0x00042526), then +0x04 = 0,
//   +0x140 = 2, +0x144 = false. dtor COMDAT 0x001DFA48 tears down +0x0C and +0x08. Each
//   derived ctor stores its own constant at +0x04 (Sound 1, RayEffect 2, LightPulse 4,
//   CameraShakerVolume 5, ViewShake 6, TerrainScorch 7, EvaEvent 15; others keep 0).
// - Field table: ObjectFilter +0x0C, SourceObjectFilter +0x08, RequiredSecondaryModelConditions
//   +0xA8, ExcludedSecondaryModelConditions +0xF4, RequiredSourceModelConditions +0x10,
//   ExcludedSourceModelConditions +0x5C, StopIfNuggetPlayed +0x144 (parseBool), Weather +0x140
//   (parseIndexList).
// - Table, 5 slots: 0 dtor, 1 doFXPos (pure: _purecall), 2 doFXObj 0x001DFF1F (positions and
//   transform of the two objects, then slot 1 with speed 0), 3 a two-argument asset-list
//   slot (pure; DynamicDecal's 0x001E1709 is `GetAssetList(AssetList &, int)`), 4 0x001DFF5D,
//   the filter/condition/weather test FXList::doFXPos runs before each nugget.
//
// FXList (the map's Rva001E2747). SIZE 0x28.
// - ctor COMDAT 0x001E2747 (const AsciiString &): table VA 0x00BDD970 (1 slot: the dtor), list
//   +0x04, name +0x0C, +0x10 = false, list +0x18, then +0x08 = false, +0x14 = 0, +0x1C = 30,
//   +0x20 = 60, +0x18 cleared, +0x24 = false. dtor COMDAT 0x001E27C6 runs clear() 0x001E11A6.
// - Field table TheFXListFieldParse (RVA 0x007DC720): one token per nugget kind, CullingInfo
//   (parseCullingInfo 0x001DFD57; its asserts name m_cullTrackingMax and m_cullTrackingMin) and
//   PlayEvenIfShrouded +0x10.
// - +0x24 is set by FXListStore::parseFXListDefinition 0x001E2CAE on a list a reload replaced
//   (load type 5); doFXPos 0x001E296E and doFXObj 0x001E2A18 then look the list up again by
//   its name (+0x0C).
#pragma once
#ifndef BFME2_FXLIST_H
#define BFME2_FXLIST_H

#include <stddef.h>
#include <list>
#include "Common/GameType.h"
#include "Common/AsciiString.h"
#include "Common/Coord2D.h"
#include "Common/Coord3D.h"
#include "Common/ICoord2D.h"
#include "Common/RGBColor.h"
#include "Common/RandomValue.h"
#include "Common/OpaqueRefElement4.h"
#include "Common/ModelConditionFlags.h"
#include "Common/INI/FieldParse.h"
#include "GameLogic/ObjectFilter.h"

class Object;
class Matrix3D;
class AssetList;
class INI;

class FXNugget
{
public:
	FXNugget()
	{
		m_type = 0;
		m_weather = 2;
		m_stopIfNuggetPlayed = false;
	}
	virtual ~FXNugget() {}																// 0

	virtual void doFXPos(const Coord3D *primary, const Matrix3D *primaryMtx, Real primarySpeed,
		const Coord3D *secondary) const = 0;												// 1
	virtual void doFXObj(const Object *primary, const Object *secondary) const;			// 2, 0x001DFF1F
	virtual void GetAssetList(AssetList &assets, Int context) const = 0;				// 3
	virtual Bool rva001DFF5D(Object *primary, Object *secondary);						// 4, 0x001DFF5D

	Int m_type;																			// 0x004 per-class constant
	ObjectFilter m_sourceObjectFilter;													// 0x008 SourceObjectFilter
	ObjectFilter m_objectFilter;														// 0x00C ObjectFilter
	ModelConditionFlags m_requiredSourceModelConditions;								// 0x010 RequiredSourceModelConditions
	ModelConditionFlags m_excludedSourceModelConditions;								// 0x05C ExcludedSourceModelConditions
	ModelConditionFlags m_requiredSecondaryModelConditions;								// 0x0A8 RequiredSecondaryModelConditions
	ModelConditionFlags m_excludedSecondaryModelConditions;								// 0x0F4 ExcludedSecondaryModelConditions
	Int m_weather;																		// 0x140 Weather (ctor 2)
	Bool m_stopIfNuggetPlayed;															// 0x144 StopIfNuggetPlayed
};
typedef char FXNugget_size[(sizeof(FXNugget) == 0x148) ? 1 : -1];
typedef char FXNugget_off_m_requiredSourceModelConditions[(offsetof(FXNugget, m_requiredSourceModelConditions) == 0x10) ? 1 : -1];
typedef char FXNugget_off_m_weather[(offsetof(FXNugget, m_weather) == 0x140) ? 1 : -1];

extern const FieldParse FXNuggetFieldParse[];											// RVA 0x007DCA18

class FXList
{
public:
	FXList(const AsciiString &name) :
		m_name(name),
		m_playEvenIfShrouded(false)
	{
		m_unk0x08 = false;
		m_cullTrackingTime = 0;
		m_cullTrackingMax = 30;
		m_cullTrackingMin = 60;
		m_recentFrames.clear();
		m_isRetired = false;
	}
	virtual ~FXList() { clear(); }

	static void doFXObj(const FXList *list, const Object *primary, const Object *secondary);
	void doFXObj(const Object *primary, const Object *secondary) const;					// 0x001E2A18
	static void doFXPos(const FXList *list, const Coord3D *primary, const Matrix3D *primaryMtx = 0,
		float primarySpeed = 0.0f, const Coord3D *secondary = 0);
	void doFXPos(const Coord3D *primary, const Matrix3D *primaryMtx, Real primarySpeed,
		const Coord3D *secondary) const;													// 0x001E296E
	// 0x001E11D6 (map: Gen_00428090::bfmeContains): a nugget with this per-class constant (+0x04) is in the list.
	Bool containsNuggetType(Int type) const;
	void clear();																		// 0x001E11A6
	// 0x001E11F8: hands the asset list to every nugget's slot 3.
	void rva001E11F8(AssetList &assets, Int context) const;
	// 0x001E2EF1: drops frames older than m_cullTrackingTime from m_recentFrames, then decides from the
	// count whether this play is culled.
	Bool rva001E2EF1() const;
	static void parseCullingInfo(INI *ini, void *instance, void *store, const void *userData);	// 0x001DFD57

	std::list<FXNugget *> m_nuggets;													// 0x04
	Bool m_unk0x08;																		// 0x08 ctor false; not in the field table
	AsciiString m_name;																	// 0x0C
	Bool m_playEvenIfShrouded;															// 0x10 PlayEvenIfShrouded
	UnsignedInt m_cullTrackingTime;														// 0x14 CullingInfo TrackingSeconds (in frames)
	mutable std::list<UnsignedInt> m_recentFrames;										// 0x18 CullingInfo
	UnsignedInt m_cullTrackingMax;														// 0x1C CullingInfo CullAllAbove (ctor 30)
	UnsignedInt m_cullTrackingMin;														// 0x20 CullingInfo StartCullingAbove (ctor 60)
	Bool m_isRetired;																	// 0x24
};
typedef char FXList_size[(sizeof(FXList) == 0x28) ? 1 : -1];
typedef char FXList_off_m_name[(offsetof(FXList, m_name) == 0x0C) ? 1 : -1];
typedef char FXList_off_m_recentFrames[(offsetof(FXList, m_recentFrames) == 0x18) ? 1 : -1];
typedef char FXList_off_m_isRetired[(offsetof(FXList, m_isRetired) == 0x24) ? 1 : -1];

extern const FieldParse TheFXListFieldParse[];											// RVA 0x007DC720

#endif
