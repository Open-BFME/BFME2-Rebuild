// GenericObjectCreationNugget.h: the OCL nugget that creates objects or debris (INI tokens
// CreateObject and CreateDebris). SIZE 0x12C.
// Target facts (retail game.dat):
// - parseObject 0x001F311B and parseDebris 0x001F340A build a MultiIniFieldParse from the
//   common table (VA 0x00BE0CC0, returned by getCommonFieldParse 0x001F01EB) and the object
//   table (VA 0x00BE1360) or the debris table (VA 0x00BE14A8), new 0x12C bytes, run the ctor
//   0x001F0E32, set +0xA8 to true (objects) or false (debris), read the block and add the
//   nugget to the list.
// - ctor 0x001F0E32: table VA 0x00BE1160; members below with the defaults it stores. dtor
//   0x001F31C6 (protected: `MAE`) tears down +0x120, +0x114, +0x10C, +0x108, +0x7C, +0x24,
//   +0x14, +0x10, +0x04 in that order and restores the base table.
// - Every member offset below is a field-table entry (token named beside it) except +0xA8,
//   which only the two parsers write.
// - Table: 0 dtor (??_G 0x001F31AA), 1 the base's, 2 0x001F14B6, 3 0x001F2F0E, 4
//   GetAssetList 0x001F131F, 5 canCreate 0x001F1010.
// - AnimSet (AnimationSet, three names read by parseAnimSet 0x001F335D) is 0xC bytes; its
//   copy ctor 0x001F0558 and dtor 0x001F0495 are the implicit ones.
// Names: the map's (field tokens, parser and method names).
#pragma once
#ifndef BFME2_GENERICOBJECTCREATIONNUGGET_H
#define BFME2_GENERICOBJECTCREATIONNUGGET_H

#include <vector>
#include "GameLogic/ObjectCreationList.h"
#include "Common/INI/FieldParse.h"
#include "Common/AsciiString.h"
#include "Common/Coord3D.h"
#include "Common/ModelConditionFlags.h"
#include "Common/OpaqueRefElement4.h"

class INI;
class FXList;
class Matrix3D;

class GenericObjectCreationNugget : public ObjectCreationNugget
{
public:
	struct AnimSet
	{
		AsciiString m_animInitial;											// 0x0
		AsciiString m_animFlying;											// 0x4
		AsciiString m_animFinal;											// 0x8
	};

	GenericObjectCreationNugget();											// 0x001F0E32

	virtual void create(const Object *primaryObj, const Object *secondaryObj,
		UnsignedInt lifetimeFrames) const;									// 2, 0x001F14B6
	virtual void create(const Object *primaryObj, const Coord3D *primary, const Coord3D *secondary,
		UnsignedInt lifetimeFrames) const;									// 3, 0x001F2F0E
	virtual void GetAssetList(AssetList &assets, void *context) const;		// 4, 0x001F131F
	virtual Bool canCreate(Object *obj, const Coord3D *pos);				// 5, 0x001F1010

	static const FieldParse *getCommonFieldParse();							// 0x001F01EB
	static void parseObject(INI *ini, void *instance, void *store, const void *userData);	// 0x001F311B
	static void parseDebris(INI *ini, void *instance, void *store, const void *userData);	// 0x001F340A
	static void parseAnimSet(INI *ini, void *instance, void *store, const void *userData);	// 0x001F335D

	// 0x001F1513: the upgrade and player test both creates run first.
	Bool rva001F1513(const Object *obj) const;
	// COMDAT 0x001F1634 (2618 bytes): creates the objects or debris and returns them.
	std::vector<Object *> reallyCreate(const Coord3D *pos, const Matrix3D *mtx, Real orientation,
		const Object *sourceObj, UnsignedInt lifetimeFrames) const;

protected:
	virtual ~GenericObjectCreationNugget();									// 0, 0x001F31C6

public:
	std::vector<AsciiString> m_names;										// 0x004 ObjectNames / ModelNames
	AsciiString m_putInContainer;											// 0x010 PutInContainer
	std::vector<AnimSet> m_animSets;										// 0x014 AnimationSet
	const FXList *m_fxFinal;												// 0x020 FXFinal
	AsciiString m_particleSysName;											// 0x024 ParticleSystem
	Int m_debrisToGenerate;													// 0x028 Count (ctor 1)
	Real m_extraBounciness;													// 0x02C ExtraBounciness
	Real m_extraFriction;													// 0x030 ExtraFriction
	Coord3D m_offset;														// 0x034 Offset (zeroed last)
	UnsignedInt m_disposition;												// 0x040 Disposition (ctor 2)
	Real m_dispositionIntensity;											// 0x044 DispositionIntensity
	Real m_dispositionAngle;												// 0x048 DispositionAngle
	Real m_velocityScale;													// 0x04C VelocityScale (ctor 1.0)
	Real m_minMag;															// 0x050 MinForceMagnitude
	Real m_maxMag;															// 0x054 MaxForceMagnitude
	Real m_minPitch;														// 0x058 MinForcePitch
	Real m_maxPitch;														// 0x05C MaxForcePitch
	UnsignedInt m_minFrames;												// 0x060 MinLifetime
	UnsignedInt m_maxFrames;												// 0x064 MaxLifetime
	UnsignedInt m_shadowType;												// 0x068 Shadow
	UnsignedInt m_invulnerableTime;											// 0x06C InvulnerableTime
	Real m_minHealth;														// 0x070 MinHealth (ctor 1.0)
	Real m_maxHealth;														// 0x074 MaxHealth (ctor 1.0)
	UnsignedInt m_fadeFrames;												// 0x078 FadeTime
	OpaqueRefElement4 m_fadeSound;											// 0x07C FadeSound
	Real m_minDistanceA;													// 0x080 MinDistanceAFormation
	Real m_minDistanceB;													// 0x084 MinDistanceBFormation
	Real m_maxDistance;														// 0x088 MaxDistanceFormation
	Int m_objectCount;														// 0x08C ObjectCount
	Bool m_requiresLivePlayer;												// 0x090 RequiresLivePlayer
	Bool m_ignoreCommandPointLimit;											// 0x091 IgnoreCommandPointLimit
	Bool m_inheritAttributesFromSource;										// 0x092 InheritAttributesFromSource
	Bool m_inheritScriptingName;											// 0x093 InheritScriptingName
	Bool m_useJustBuiltFlag;												// 0x094 UseJustBuiltFlag
	UnsignedInt m_justBuiltDuration;										// 0x098 JustBuiltDuration (ctor 1)
	Bool m_containInsideSourceObject;										// 0x09C ContainInsideSourceObject
	Bool m_preserveLayer;													// 0x09D PreserveLayer (ctor true)
	Bool m_ignoreAllObjects;												// 0x09E IgnoreAllObjects
	Bool m_ignoreAllyUnits;													// 0x09F IgnoreAllyUnits
	Bool m_ignoreEnemyUnits;												// 0x0A0 IgnoreEnemyUnits
	UnsignedInt m_startingBusyTime;											// 0x0A4 StartingBusyTime
	Bool m_requiresObjects;													// 0x0A8 parseObject true, parseDebris false (ctor true)
	Bool m_okToChangeModelColor;											// 0x0A9 OkToChangeModelColor
	Bool m_orientInForceDirection;											// 0x0AA OrientInForceDirection
	Bool m_spreadFormation;													// 0x0AB SpreadFormation
	Bool m_fadeIn;															// 0x0AC FadeIn
	Bool m_fadeOut;															// 0x0AD FadeOut
	Bool m_inheritsVeterancy;												// 0x0AE InheritsVeterancy
	Bool m_skipIfSignificantlyAirborne;										// 0x0AF SkipIfSignificantlyAirborne
	Int m_veterancyLevel;													// 0x0B0 VeterancyLevel
	ModelConditionFlags m_startingConditions;								// 0x0B4 StartingConditions
	Bool m_issueMoveAfterCreation;											// 0x100 IssueMoveAfterCreation (ctor true)
	Bool m_orientInPrimaryDirection;										// 0x101 OrientInPrimaryDirection
	Bool m_orientInSecondaryDirection;										// 0x102 OrientInSecondaryDirection
	Bool m_moveUsesStrafeUpdate;											// 0x103 MoveUsesStrafeUpdate
	Real m_orientationOffset;												// 0x104 OrientationOffset
	AsciiString m_destinationPlayer;										// 0x108 DestinationPlayer
	AsciiString m_waypointSpawnPoints;										// 0x10C WaypointSpawnPoints
	Bool m_offsetInLocalSpace;												// 0x110 OffsetInLocalSpace
	Bool m_clearRemovables;													// 0x111 ClearRemovables
	std::vector<AsciiString> m_requiredUpgrades;							// 0x114 RequiredUpgrades
	std::vector<AsciiString> m_forbiddenUpgrades;							// 0x120 ForbiddenUpgrades
};
typedef char GenericObjectCreationNugget_AnimSet_size[(sizeof(GenericObjectCreationNugget::AnimSet) == 0xC) ? 1 : -1];
typedef char GenericObjectCreationNugget_size[(sizeof(GenericObjectCreationNugget) == 0x12C) ? 1 : -1];
typedef char GenericObjectCreationNugget_off_m_offset[(offsetof(GenericObjectCreationNugget, m_offset) == 0x34) ? 1 : -1];
typedef char GenericObjectCreationNugget_off_m_fadeSound[(offsetof(GenericObjectCreationNugget, m_fadeSound) == 0x7C) ? 1 : -1];
typedef char GenericObjectCreationNugget_off_m_justBuiltDuration[(offsetof(GenericObjectCreationNugget, m_justBuiltDuration) == 0x98) ? 1 : -1];
typedef char GenericObjectCreationNugget_off_m_startingConditions[(offsetof(GenericObjectCreationNugget, m_startingConditions) == 0xB4) ? 1 : -1];
typedef char GenericObjectCreationNugget_off_m_orientationOffset[(offsetof(GenericObjectCreationNugget, m_orientationOffset) == 0x104) ? 1 : -1];
typedef char GenericObjectCreationNugget_off_m_requiredUpgrades[(offsetof(GenericObjectCreationNugget, m_requiredUpgrades) == 0x114) ? 1 : -1];

#endif
