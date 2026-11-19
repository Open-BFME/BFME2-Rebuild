// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C42290).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// SlowDeathBehaviorModuleData.h: the INI data of SlowDeathBehavior. SIZE 0x190.
// Target facts: ModuleFactory registers the module with operator new 0x190 and
// constructor RVA 0x0045E386; buildFieldParse RVA 0x0045E93F adds VA 0x00C42290 at +0x0 and DieMuxData at +0x8.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_SLOW_DEATH_BEHAVIOR_MODULE_DATA_H
#define BFME2_SLOW_DEATH_BEHAVIOR_MODULE_DATA_H

#include <stddef.h>
#include <vector>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "GameLogic/Module/DieMuxData.h"
#include "Common/AudioEventRef.h"
#include "Common/ModelConditionFlags.h"
#include "Common/BitFlags.h"

class INI;
class MultiIniFieldParse;
class FXList;
class ObjectCreationList;
class WeaponTemplate;

class SlowDeathBehaviorModuleData : public ModuleData
{
public:
	SlowDeathBehaviorModuleData();
	virtual ~SlowDeathBehaviorModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static void parseFX(INI *ini, void *instance, void *store, const void *userData);
	static void parseOCL(INI *ini, void *instance, void *store, const void *userData);
	static void parseWeapon(INI *ini, void *instance, void *store, const void *userData);
	static void parseDeathFlags(INI *ini, void *instance, void *store, const void *userData);
	static void rva0045E8A9(INI *ini, void *instance, void *store, const void *userData);	// Sound entry, 0x0045E8A9 (placeholder name)

	DieMuxData m_dieMuxData;                               // +0x8; added by buildFieldParse at +0x8
	Real m_sinkRate;                                       // +0x38 SinkRate; velocity
	Int m_probabilityModifier;                             // +0x3C ProbabilityModifier
	Real m_modifierBonusPerOverkillPercent;                // +0x40 ModifierBonusPerOverkillPercent; percent
	UnsignedInt m_sinkDelay;                               // +0x44 SinkDelay; duration, frames
	UnsignedInt m_sinkDelayVariance;                       // +0x48 SinkDelayVariance; duration, frames
	UnsignedInt m_destructionDelay;                        // +0x4C DestructionDelay; duration, frames
	UnsignedInt m_destructionDelayVariance;                // +0x50 DestructionDelayVariance; duration, frames
	UnsignedInt m_decayBeginTime;                          // +0x54 DecayBeginTime; duration, frames
	// One list per phase (INITIAL, MIDPOINT, FINAL, HIT_GROUND: the name table at VA 0x00DC9598).
	// The constructor 0x0045E386 builds each array through the vector constructor iterator with
	// four 12-byte elements; the parse rows below prove the three pointer arrays.
	std::vector<const FXList *> m_fx[4];                   // +0x58 FX (parseFX 0x0045E533)
	std::vector<const ObjectCreationList *> m_ocl[4];      // +0x88 OCL (parseOCL 0x0045E5A4)
	std::vector<const WeaponTemplate *> m_weapons[4];      // +0xB8 Weapon (parseWeapon 0x0045E618)
	std::vector<AudioEventInfoRef> m_sounds[4];            // +0xE8 Sound (parse 0x0045E8A9): the element has a destructor (0x0085D137), so the array is torn down through the vector destructor iterator
	Real m_flingForce;                                     // +0x118 FlingForce
	Real m_flingForceVariance;                             // +0x11C FlingForceVariance
	Real m_flingPitch;                                     // +0x120 FlingPitch; angle
	Real m_flingPitchVariance;                             // +0x124 FlingPitchVariance; angle
	ModelConditionFlags m_deathFlags;                      // +0x128 DeathFlags (parseDeathFlags 0x0045E293 reads the model conditions into it)
	ObjectStatusMaskType m_deathStatusFlags;               // +0x174 the second mask parseDeathFlags fills (BitFlags<101>, 0x10 bytes)
	UnsignedInt m_fadeTime;                                // +0x184 FadeTime; duration, frames
	UnsignedInt m_fadeDelay;                               // +0x188 FadeDelay; duration, frames
	UnsignedByte m_hasLists;                               // +0x18C bits 1, 2, 4, 8: set when parseFX, parseOCL, parseWeapon and the Sound parse (0x0045E8A9) read a non-empty name
	Bool m_shadowWhenDead;                                 // +0x18D ShadowWhenDead
};
// SIZE 0x190
typedef char SlowDeathBehaviorModuleData_size[(sizeof(SlowDeathBehaviorModuleData) == 0x190) ? 1 : -1];
typedef char SlowDeathBehaviorModuleData_off_m_dieMuxData[(offsetof(SlowDeathBehaviorModuleData, m_dieMuxData) == 0x8) ? 1 : -1];
typedef char SlowDeathBehaviorModuleData_off_m_sinkRate[(offsetof(SlowDeathBehaviorModuleData, m_sinkRate) == 0x38) ? 1 : -1];
typedef char SlowDeathBehaviorModuleData_off_m_probabilityModifier[(offsetof(SlowDeathBehaviorModuleData, m_probabilityModifier) == 0x3C) ? 1 : -1];
typedef char SlowDeathBehaviorModuleData_off_m_modifierBonusPerOverkillPercent[(offsetof(SlowDeathBehaviorModuleData, m_modifierBonusPerOverkillPercent) == 0x40) ? 1 : -1];
typedef char SlowDeathBehaviorModuleData_off_m_sinkDelay[(offsetof(SlowDeathBehaviorModuleData, m_sinkDelay) == 0x44) ? 1 : -1];
typedef char SlowDeathBehaviorModuleData_off_m_sinkDelayVariance[(offsetof(SlowDeathBehaviorModuleData, m_sinkDelayVariance) == 0x48) ? 1 : -1];
typedef char SlowDeathBehaviorModuleData_off_m_destructionDelay[(offsetof(SlowDeathBehaviorModuleData, m_destructionDelay) == 0x4C) ? 1 : -1];
typedef char SlowDeathBehaviorModuleData_off_m_destructionDelayVariance[(offsetof(SlowDeathBehaviorModuleData, m_destructionDelayVariance) == 0x50) ? 1 : -1];
typedef char SlowDeathBehaviorModuleData_off_m_decayBeginTime[(offsetof(SlowDeathBehaviorModuleData, m_decayBeginTime) == 0x54) ? 1 : -1];
typedef char SlowDeathBehaviorModuleData_off_m_flingForce[(offsetof(SlowDeathBehaviorModuleData, m_flingForce) == 0x118) ? 1 : -1];
typedef char SlowDeathBehaviorModuleData_off_m_flingForceVariance[(offsetof(SlowDeathBehaviorModuleData, m_flingForceVariance) == 0x11C) ? 1 : -1];
typedef char SlowDeathBehaviorModuleData_off_m_flingPitch[(offsetof(SlowDeathBehaviorModuleData, m_flingPitch) == 0x120) ? 1 : -1];
typedef char SlowDeathBehaviorModuleData_off_m_flingPitchVariance[(offsetof(SlowDeathBehaviorModuleData, m_flingPitchVariance) == 0x124) ? 1 : -1];
typedef char SlowDeathBehaviorModuleData_off_m_fadeTime[(offsetof(SlowDeathBehaviorModuleData, m_fadeTime) == 0x184) ? 1 : -1];
typedef char SlowDeathBehaviorModuleData_off_m_fadeDelay[(offsetof(SlowDeathBehaviorModuleData, m_fadeDelay) == 0x188) ? 1 : -1];
typedef char SlowDeathBehaviorModuleData_off_m_shadowWhenDead[(offsetof(SlowDeathBehaviorModuleData, m_shadowWhenDead) == 0x18D) ? 1 : -1];

#endif
