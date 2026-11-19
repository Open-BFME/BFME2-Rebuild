// TransitionDamageFXModuleData.h: the INI data of TransitionDamageFX. SIZE 0x1040. Target facts (f14, 2026-11-18, read
// from the constructor 0x004BA58A, the parse rows 0x004B9917..0x004B9B2E and the field-parse table 0x00C6BB18):
// the same structures as Zero Hour's (an FX location is a Bone: name with RandomBone, or a Loc: point), in four
// BodyDamageType groups of twelve effects each (group 0, pristine, is never parsed). The constructor fills each
// location with locType 1 and a zero point, every DamageTypeFlags with all ones, builds the sub-object
// name lists (four vectors of AsciiString each, hide at +0xFD4 and show at +0x1004) and the neighbour list at +0x1034.
#pragma once
#ifndef BFME2_TRANSITION_DAMAGE_F_X_MODULE_DATA_H
#define BFME2_TRANSITION_DAMAGE_F_X_MODULE_DATA_H

#include <stddef.h>
#include <vector>
#include "Common/GameType.h"
#include "Common/AsciiString.h"
#include "GameLogic/Module/ModuleData.h"
#include "GameLogic/Damage.h"

class INI;
class MultiIniFieldParse;
class FXList;
class ObjectCreationList;
class ParticleSystemTemplate;

enum { DAMAGE_MODULE_MAX_FX = 12 };

typedef Char FXDamageLocType;
enum
{
	FX_DAMAGE_LOC_TYPE_BONE = 0,
	FX_DAMAGE_LOC_TYPE_COORD = 1
};

struct FXLocInfo
{
	FXDamageLocType locType;	// +0 bone or coordinate
	AsciiString boneName;		// +4 the bone, or a prefix of the bones when randomBone
	Bool randomBone;			// +8 pick one of the bones that start with boneName
	Coord3D loc;				// +0xC the point for FX_DAMAGE_LOC_TYPE_COORD
};

struct FXDamageFXListInfo
{
	const FXList *fx;
	FXLocInfo locInfo;
};

struct FXDamageOCLInfo
{
	const ObjectCreationList *ocl;
	FXLocInfo locInfo;
};

struct FXDamageParticleSystemInfo
{
	const ParticleSystemTemplate *particleSysTemplate;
	FXLocInfo locInfo;
};

// One neighbouring object (found by the map-load pass at a point beside this object) whose sub-objects are hidden and
// beside which an object creation list runs when the damage state changes to rubble: the element of the lists at
// TransitionDamageFXModuleData +0x1034 (parsed by NeighborIDSubobjectNameDataAppend) and TransitionDamageFX +0xD4.
struct TransitionNeighborInfo
{
	ObjectID neighborID;						// +0 the neighbour found at map load (0 in the module data)
	_STL::vector<AsciiString> hideSubObject;	// +4 SubObject = names
	const ObjectCreationList *ocl;				// +0x10 OCL =
	Coord3D neighborOffset;						// +0x14 NeighborOffset = X: Y: Z: (where to look for the neighbour)
	Coord3D oclOffset;							// +0x20 OCLOffset = X: Y: Z:
};
typedef char TransitionNeighborInfo_size[(sizeof(TransitionNeighborInfo) == 0x2C) ? 1 : -1];

class TransitionDamageFXModuleData : public ModuleData
{
public:
	TransitionDamageFXModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static void parseFXList(INI *ini, void *instance, void *store, const void *userData);
	static void parseObjectCreationList(INI *ini, void *instance, void *store, const void *userData);
	static void parseParticleSystem(INI *ini, void *instance, void *store, const void *userData);
	static void NeighborIDSubobjectNameDataAppend(INI *ini, void *instance, void *store, const void *userData);

	DamageTypeFlags m_damageFXTypes;											// +0x8 DamageFXTypes
	FXDamageFXListInfo m_fxList[4][DAMAGE_MODULE_MAX_FX];						// +0xC
	DamageTypeFlags m_damageOCLTypes;											// +0x54C DamageOCLTypes
	FXDamageOCLInfo m_OCL[4][DAMAGE_MODULE_MAX_FX];								// +0x550
	DamageTypeFlags m_damageParticleTypes;										// +0xA90 DamageParticleTypes
	FXDamageParticleSystemInfo m_particleSystem[4][DAMAGE_MODULE_MAX_FX];		// +0xA94
	_STL::vector<AsciiString> m_hideSubObject[4];								// +0xFD4
	_STL::vector<AsciiString> m_showSubObject[4];								// +0x1004
	_STL::vector<TransitionNeighborInfo> m_neighbors;							// +0x1034
};
typedef char TransitionDamageFXModuleData_size[(sizeof(TransitionDamageFXModuleData) == 0x1040) ? 1 : -1];
typedef char TransitionDamageFXModuleData_off_ocl[(offsetof(TransitionDamageFXModuleData, m_OCL) == 0x550) ? 1 : -1];
typedef char TransitionDamageFXModuleData_off_ps[(offsetof(TransitionDamageFXModuleData, m_particleSystem) == 0xA94) ? 1 : -1];
typedef char TransitionDamageFXModuleData_off_hide[(offsetof(TransitionDamageFXModuleData, m_hideSubObject) == 0xFD4) ? 1 : -1];

#endif
