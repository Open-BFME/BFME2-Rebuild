// StructureToppleUpdateModuleData.h: promoted from include/Draft by h05 (round 10). Proven by 2 rows that MATCH against it on main (0x004A651B, 0x00257BE7);
// name, size, bases and virtual order are retail facts given below.
// StructureToppleUpdateModuleData.h: the INI data of StructureToppleUpdate. SIZE 0xBC.
// Target facts: ModuleFactory registers the module with operator new 0xBC and
// constructor RVA 0x00257A5E; buildFieldParse RVA 0x004A651B adds VA 0x00C52BD8 at +0x0 and DieMuxData at +0x8.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_STRUCTURE_TOPPLE_UPDATE_MODULE_DATA_H
#define BFME2_STRUCTURE_TOPPLE_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "GameLogic/Module/DieMuxData.h"
#include "GameLogic/Damage.h"
#include "Common/AsciiString.h"
#include "Common/Vector.h"

class INI;
class MultiIniFieldParse;
class FXList;
class ObjectCreationList;
class ParticleSystemTemplate;

// One AngleFX entry: the FX plays when the toppling angle passes `angle` (doAngleFX 0x004A57DB walks the list).
struct StructureToppleAngleFX
{
	Real m_angle;										// +0x0 (parseAngleFX 0x004A64CD stores the INI angle in radians)
	const FXList *m_fx;									// +0x4
};

// One bone effect: the bone's name and the particle system started on it (doToppleDelayBurstFX 0x004A5B61).
struct StructureToppleFXBone
{
	AsciiString m_boneName;								// +0x0
	const ParticleSystemTemplate *m_particleSystemTemplate;	// +0x4
};

class StructureToppleUpdateModuleData : public ModuleData
{
public:
	StructureToppleUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static void rva004A646B(INI *ini, void *instance, void *store, const void *userData);	// 0x004A646B: the "OCL" line (a phase name, then OCL names)
	static ModuleData *friend_newModuleData(INI *ini);	// 0x00257BE7

	DieMuxData m_dieMuxData;                               // +0x8; added by buildFieldParse at +0x8
	UnsignedInt m_minToppleDelay;                          // +0x38 MinToppleDelay; duration, frames
	UnsignedInt m_maxToppleDelay;                          // +0x3C MaxToppleDelay; duration, frames
	Real m_structuralIntegrity;                            // +0x40 StructuralIntegrity
	Real m_structuralDecay;                                // +0x44 StructuralDecay
	Real m_toppleAccelerationFactor;                       // +0x48 ToppleAccelerationFactor
	DamageTypeFlags m_damageFXTypes;                       // +0x4C DamageFXTypes
	const FXList * m_toppleStartFX;                        // +0x50 ToppleStartFX
	const FXList * m_toppleDelayFX;                        // +0x54 ToppleDelayFX
	const FXList * m_topplingFX;                           // +0x58 TopplingFX
	const FXList * m_toppleDoneFX;                         // +0x5C ToppleDoneFX
	const FXList * m_crushingFX;                           // +0x60 CrushingFX
	AsciiString m_crushingWeaponName;                      // +0x64 CrushingWeaponName
	UnsignedInt m_minToppleBurstDelay;                     // +0x68 MinToppleBurstDelay; duration, frames
	UnsignedInt m_maxToppleBurstDelay;                     // +0x6C MaxToppleBurstDelay; duration, frames
	_STL::vector<const ObjectCreationList *> m_phaseOCLs[3];	// +0x70, +0x7C, +0x88: the OCL entries of each topple phase (rva004A646B parses "OCL" lines into the vector its phase index selects)
	UnsignedInt m_phaseOCLCounts[3];						// +0x94..+0x9F: how many of a phase's OCLs doPhaseStuff 0x004A584B creates
	_STL::vector<StructureToppleFXBone> m_fxBones;			// +0xA0: the bone particle systems doToppleDelayBurstFX 0x004A5B61 starts (begin/end read at +0xA0/+0xA4)
	_STL::vector<StructureToppleAngleFX> m_angleFXs;		// +0xAC AngleFX (parseAngleFX 0x004A64CD)
	Real m_forceToppleAngle;                               // +0xB8 ForceToppleAngle
};
// SIZE 0xBC
typedef char StructureToppleUpdateModuleData_size[(sizeof(StructureToppleUpdateModuleData) == 0xBC) ? 1 : -1];
typedef char StructureToppleUpdateModuleData_off_m_dieMuxData[(offsetof(StructureToppleUpdateModuleData, m_dieMuxData) == 0x8) ? 1 : -1];
typedef char StructureToppleUpdateModuleData_off_m_minToppleDelay[(offsetof(StructureToppleUpdateModuleData, m_minToppleDelay) == 0x38) ? 1 : -1];
typedef char StructureToppleUpdateModuleData_off_m_maxToppleDelay[(offsetof(StructureToppleUpdateModuleData, m_maxToppleDelay) == 0x3C) ? 1 : -1];
typedef char StructureToppleUpdateModuleData_off_m_structuralIntegrity[(offsetof(StructureToppleUpdateModuleData, m_structuralIntegrity) == 0x40) ? 1 : -1];
typedef char StructureToppleUpdateModuleData_off_m_structuralDecay[(offsetof(StructureToppleUpdateModuleData, m_structuralDecay) == 0x44) ? 1 : -1];
typedef char StructureToppleUpdateModuleData_off_m_toppleAccelerationFactor[(offsetof(StructureToppleUpdateModuleData, m_toppleAccelerationFactor) == 0x48) ? 1 : -1];
typedef char StructureToppleUpdateModuleData_off_m_damageFXTypes[(offsetof(StructureToppleUpdateModuleData, m_damageFXTypes) == 0x4C) ? 1 : -1];
typedef char StructureToppleUpdateModuleData_off_m_toppleStartFX[(offsetof(StructureToppleUpdateModuleData, m_toppleStartFX) == 0x50) ? 1 : -1];
typedef char StructureToppleUpdateModuleData_off_m_toppleDelayFX[(offsetof(StructureToppleUpdateModuleData, m_toppleDelayFX) == 0x54) ? 1 : -1];
typedef char StructureToppleUpdateModuleData_off_m_topplingFX[(offsetof(StructureToppleUpdateModuleData, m_topplingFX) == 0x58) ? 1 : -1];
typedef char StructureToppleUpdateModuleData_off_m_toppleDoneFX[(offsetof(StructureToppleUpdateModuleData, m_toppleDoneFX) == 0x5C) ? 1 : -1];
typedef char StructureToppleUpdateModuleData_off_m_crushingFX[(offsetof(StructureToppleUpdateModuleData, m_crushingFX) == 0x60) ? 1 : -1];
typedef char StructureToppleUpdateModuleData_off_m_crushingWeaponName[(offsetof(StructureToppleUpdateModuleData, m_crushingWeaponName) == 0x64) ? 1 : -1];
typedef char StructureToppleUpdateModuleData_off_m_minToppleBurstDelay[(offsetof(StructureToppleUpdateModuleData, m_minToppleBurstDelay) == 0x68) ? 1 : -1];
typedef char StructureToppleUpdateModuleData_off_m_maxToppleBurstDelay[(offsetof(StructureToppleUpdateModuleData, m_maxToppleBurstDelay) == 0x6C) ? 1 : -1];
typedef char StructureToppleUpdateModuleData_off_m_forceToppleAngle[(offsetof(StructureToppleUpdateModuleData, m_forceToppleAngle) == 0xB8) ? 1 : -1];

#endif
