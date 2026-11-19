// ToppleUpdateModuleData.h: the INI data of ToppleUpdate. SIZE 0x2C.
// Target facts: ModuleFactory registers the module with operator new 0x2C and
// constructor RVA 0x004A8173; buildFieldParse RVA 0x004A81F0 adds VA 0x00C53980 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_TOPPLE_UPDATE_MODULE_DATA_H
#define BFME2_TOPPLE_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "Common/AsciiString.h"

class INI;
class MultiIniFieldParse;
class FXList;

class ToppleUpdateModuleData : public ModuleData
{
public:
	ToppleUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	const FXList * m_toppleFX;                             // +0x8 ToppleFX
	const FXList * m_bounceFX;                             // +0xC BounceFX
	AsciiString m_stumpName;                               // +0x10 StumpName
	Real m_initialVelocityPercent;                         // +0x14 InitialVelocityPercent; percent
	Real m_initialAccelPercent;                            // +0x18 InitialAccelPercent; percent
	Real m_bounceVelocityPercent;                          // +0x1C BounceVelocityPercent; percent
	Real m_minimumToppleSpeed;                             // +0x20 MinimumToppleSpeed; positive, non-zero
	Bool m_killWhenFinishedToppling;                       // +0x24 KillWhenFinishedToppling
	Bool m_killWhenStartToppling;                          // +0x25 KillWhenStartToppling
	Bool m_killStumpWhenToppled;                           // +0x26 KillStumpWhenToppled
	Bool m_toppleLeftOrRightOnly;                          // +0x27 ToppleLeftOrRightOnly
	Bool m_reorientToppledRubble;                          // +0x28 ReorientToppledRubble
};
// SIZE 0x2C
typedef char ToppleUpdateModuleData_size[(sizeof(ToppleUpdateModuleData) == 0x2C) ? 1 : -1];
typedef char ToppleUpdateModuleData_off_m_toppleFX[(offsetof(ToppleUpdateModuleData, m_toppleFX) == 0x8) ? 1 : -1];
typedef char ToppleUpdateModuleData_off_m_bounceFX[(offsetof(ToppleUpdateModuleData, m_bounceFX) == 0xC) ? 1 : -1];
typedef char ToppleUpdateModuleData_off_m_stumpName[(offsetof(ToppleUpdateModuleData, m_stumpName) == 0x10) ? 1 : -1];
typedef char ToppleUpdateModuleData_off_m_initialVelocityPercent[(offsetof(ToppleUpdateModuleData, m_initialVelocityPercent) == 0x14) ? 1 : -1];
typedef char ToppleUpdateModuleData_off_m_initialAccelPercent[(offsetof(ToppleUpdateModuleData, m_initialAccelPercent) == 0x18) ? 1 : -1];
typedef char ToppleUpdateModuleData_off_m_bounceVelocityPercent[(offsetof(ToppleUpdateModuleData, m_bounceVelocityPercent) == 0x1C) ? 1 : -1];
typedef char ToppleUpdateModuleData_off_m_minimumToppleSpeed[(offsetof(ToppleUpdateModuleData, m_minimumToppleSpeed) == 0x20) ? 1 : -1];
typedef char ToppleUpdateModuleData_off_m_killWhenFinishedToppling[(offsetof(ToppleUpdateModuleData, m_killWhenFinishedToppling) == 0x24) ? 1 : -1];
typedef char ToppleUpdateModuleData_off_m_killWhenStartToppling[(offsetof(ToppleUpdateModuleData, m_killWhenStartToppling) == 0x25) ? 1 : -1];
typedef char ToppleUpdateModuleData_off_m_killStumpWhenToppled[(offsetof(ToppleUpdateModuleData, m_killStumpWhenToppled) == 0x26) ? 1 : -1];
typedef char ToppleUpdateModuleData_off_m_toppleLeftOrRightOnly[(offsetof(ToppleUpdateModuleData, m_toppleLeftOrRightOnly) == 0x27) ? 1 : -1];
typedef char ToppleUpdateModuleData_off_m_reorientToppledRubble[(offsetof(ToppleUpdateModuleData, m_reorientToppledRubble) == 0x28) ? 1 : -1];

#endif
