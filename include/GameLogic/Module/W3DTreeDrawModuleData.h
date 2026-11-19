// W3DTreeDrawModuleData.h: the INI data of W3DTreeDraw. SIZE 0x64.
// Target facts: ModuleFactory registers the module with operator new 0x64 and
// constructor RVA 0x000CEBBF; buildFieldParse RVA 0x000CED03 adds VA 0x00BCCFE8 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
// Retail's table 0x007CCDF0 (31 slots) overrides slot 0 (the destructor), slot 14 (0x000CED14: a copy of the string at
// +0x08, shared with W3DPropDrawModuleData's table), slot 15 (the folded `mov al,1; ret`) and slot 16 (the folded
// `mov eax,ecx; ret`, this); constructor defaults (MoveOutwardTime/MoveInwardTime 1, MoveOutwardDistanceFactor 1.0,
// InitialVelocityPercent 0.2, InitialAccelPercent 0.01, BounceVelocityPercent 0.3, MinimumToppleSpeed 0.5,
// KillWhenFinishedToppling true, SinkTime and MorphTime ten seconds of frames, SinkDistance 20, FadeRate 5, FadeTarget
// 105, FadeDistance 40) are the constructor's 0x000CEBBF.
#pragma once
#ifndef BFME2_W3_D_TREE_DRAW_MODULE_DATA_H
#define BFME2_W3_D_TREE_DRAW_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "Common/AsciiString.h"

class INI;
class MultiIniFieldParse;
class FXList;

class W3DTreeDrawModuleData : public ModuleData
{
public:
	W3DTreeDrawModuleData();                               // 0x000CEBBF
	virtual ~W3DTreeDrawModuleData();                      // 0x000CECA6: releases the strings at +0x48, +0x28, +0x0C and +0x08
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	// Retail's table 0x007CCDF0: slot 14 is the body shared with W3DPropDrawModuleData (0x000CED14),
	// slots 15 and 16 the folded `mov al,1` and `mov eax,ecx` (names wait for the ModuleData base).
	virtual AsciiString getBestModelNameForWB(const ModelConditionFlags &) const { return m_modelName; }
	virtual Bool slot15() const { return true; }
	virtual const void *slot16() const { return this; }

	AsciiString m_modelName;                               // +0x8 ModelName
	AsciiString m_textureName;                             // +0xC TextureName
	UnsignedInt m_moveOutwardTime;                         // +0x10 MoveOutwardTime; duration, frames
	UnsignedInt m_moveInwardTime;                          // +0x14 MoveInwardTime; duration, frames
	Real m_moveOutwardDistanceFactor;                      // +0x18 MoveOutwardDistanceFactor
	Real m_darkeningFactor;                                // +0x1C DarkeningFactor
	const FXList * m_toppleFX;                             // +0x20 ToppleFX
	const FXList * m_bounceFX;                             // +0x24 BounceFX
	AsciiString m_stumpName;                               // +0x28 StumpName
	Real m_initialVelocityPercent;                         // +0x2C InitialVelocityPercent; percent
	Real m_initialAccelPercent;                            // +0x30 InitialAccelPercent; percent
	Real m_bounceVelocityPercent;                          // +0x34 BounceVelocityPercent; percent
	Real m_minimumToppleSpeed;                             // +0x38 MinimumToppleSpeed; positive, non-zero
	Bool m_killWhenFinishedToppling;                       // +0x3C KillWhenFinishedToppling
	Bool m_doTopple;                                       // +0x3D DoTopple
	UnsignedInt m_sinkTime;                                // +0x40 SinkTime; duration, frames
	Real m_sinkDistance;                                   // +0x44 SinkDistance; positive, non-zero
	AsciiString m_morphTree;                               // +0x48 MorphTree
	UnsignedInt m_morphTime;                               // +0x4C MorphTime; duration, frames
	const FXList * m_morphFX;                              // +0x50 MorphFX
	Bool m_taintedTree;                                    // +0x54 TaintedTree
	UnsignedInt m_fadeRate;                                // +0x58 FadeRate
	UnsignedInt m_fadeTarget;                              // +0x5C FadeTarget
	Real m_fadeDistance;                                   // +0x60 FadeDistance
};
// SIZE 0x64
typedef char W3DTreeDrawModuleData_size[(sizeof(W3DTreeDrawModuleData) == 0x64) ? 1 : -1];
typedef char W3DTreeDrawModuleData_off_m_modelName[(offsetof(W3DTreeDrawModuleData, m_modelName) == 0x8) ? 1 : -1];
typedef char W3DTreeDrawModuleData_off_m_textureName[(offsetof(W3DTreeDrawModuleData, m_textureName) == 0xC) ? 1 : -1];
typedef char W3DTreeDrawModuleData_off_m_moveOutwardTime[(offsetof(W3DTreeDrawModuleData, m_moveOutwardTime) == 0x10) ? 1 : -1];
typedef char W3DTreeDrawModuleData_off_m_moveInwardTime[(offsetof(W3DTreeDrawModuleData, m_moveInwardTime) == 0x14) ? 1 : -1];
typedef char W3DTreeDrawModuleData_off_m_moveOutwardDistanceFactor[(offsetof(W3DTreeDrawModuleData, m_moveOutwardDistanceFactor) == 0x18) ? 1 : -1];
typedef char W3DTreeDrawModuleData_off_m_darkeningFactor[(offsetof(W3DTreeDrawModuleData, m_darkeningFactor) == 0x1C) ? 1 : -1];
typedef char W3DTreeDrawModuleData_off_m_toppleFX[(offsetof(W3DTreeDrawModuleData, m_toppleFX) == 0x20) ? 1 : -1];
typedef char W3DTreeDrawModuleData_off_m_bounceFX[(offsetof(W3DTreeDrawModuleData, m_bounceFX) == 0x24) ? 1 : -1];
typedef char W3DTreeDrawModuleData_off_m_stumpName[(offsetof(W3DTreeDrawModuleData, m_stumpName) == 0x28) ? 1 : -1];
typedef char W3DTreeDrawModuleData_off_m_initialVelocityPercent[(offsetof(W3DTreeDrawModuleData, m_initialVelocityPercent) == 0x2C) ? 1 : -1];
typedef char W3DTreeDrawModuleData_off_m_initialAccelPercent[(offsetof(W3DTreeDrawModuleData, m_initialAccelPercent) == 0x30) ? 1 : -1];
typedef char W3DTreeDrawModuleData_off_m_bounceVelocityPercent[(offsetof(W3DTreeDrawModuleData, m_bounceVelocityPercent) == 0x34) ? 1 : -1];
typedef char W3DTreeDrawModuleData_off_m_minimumToppleSpeed[(offsetof(W3DTreeDrawModuleData, m_minimumToppleSpeed) == 0x38) ? 1 : -1];
typedef char W3DTreeDrawModuleData_off_m_killWhenFinishedToppling[(offsetof(W3DTreeDrawModuleData, m_killWhenFinishedToppling) == 0x3C) ? 1 : -1];
typedef char W3DTreeDrawModuleData_off_m_doTopple[(offsetof(W3DTreeDrawModuleData, m_doTopple) == 0x3D) ? 1 : -1];
typedef char W3DTreeDrawModuleData_off_m_sinkTime[(offsetof(W3DTreeDrawModuleData, m_sinkTime) == 0x40) ? 1 : -1];
typedef char W3DTreeDrawModuleData_off_m_sinkDistance[(offsetof(W3DTreeDrawModuleData, m_sinkDistance) == 0x44) ? 1 : -1];
typedef char W3DTreeDrawModuleData_off_m_morphTree[(offsetof(W3DTreeDrawModuleData, m_morphTree) == 0x48) ? 1 : -1];
typedef char W3DTreeDrawModuleData_off_m_morphTime[(offsetof(W3DTreeDrawModuleData, m_morphTime) == 0x4C) ? 1 : -1];
typedef char W3DTreeDrawModuleData_off_m_morphFX[(offsetof(W3DTreeDrawModuleData, m_morphFX) == 0x50) ? 1 : -1];
typedef char W3DTreeDrawModuleData_off_m_taintedTree[(offsetof(W3DTreeDrawModuleData, m_taintedTree) == 0x54) ? 1 : -1];
typedef char W3DTreeDrawModuleData_off_m_fadeRate[(offsetof(W3DTreeDrawModuleData, m_fadeRate) == 0x58) ? 1 : -1];
typedef char W3DTreeDrawModuleData_off_m_fadeTarget[(offsetof(W3DTreeDrawModuleData, m_fadeTarget) == 0x5C) ? 1 : -1];
typedef char W3DTreeDrawModuleData_off_m_fadeDistance[(offsetof(W3DTreeDrawModuleData, m_fadeDistance) == 0x60) ? 1 : -1];

#endif
