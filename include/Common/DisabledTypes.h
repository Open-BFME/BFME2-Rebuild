// DisabledTypes.h: the reasons an object can be disabled (BitFlags<11>).
// Target facts: the name table 0x00DC828C that BitFlags<11>'s parse reads has
// 11 entries in this order (DEFAULT, DISABLED_USER_PARALYZED, ... ,
// DISABLED_SCRIPT_UNDERPOWERED). CaveContain::onContaining 0x004664BB
// disables a contained object with 3 (DISABLED_HELD).
// Donor facts: Zero Hour spells the first entry DISABLED_DEFAULT.
#pragma once
#ifndef BFME2_DISABLEDTYPES_H
#define BFME2_DISABLEDTYPES_H

enum DisabledType
{
	DISABLED_DEFAULT = 0,
	DISABLED_USER_PARALYZED,
	DISABLED_EMP,
	DISABLED_HELD,
	DISABLED_PARALYZED,
	DISABLED_UNMANNED,
	DISABLED_UNDERPOWERED,
	DISABLED_FREEFALL,
	DISABLED_TEMPORARILY_BUSY,
	DISABLED_SCRIPT_DISABLED,
	DISABLED_SCRIPT_UNDERPOWERED,

	DISABLED_COUNT
};

#endif
