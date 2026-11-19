// Portions derived from Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// Damage.h: SAGE damage description (DamageInfo's inputs and outputs).
// Target facts (retail game.dat):
// - Three records, each with a one-slot table holding only its xfer and no Snapshot
//   base (the tables are adjacent: VA 0x00BF91FC input -> 0x004D6FC4, VA 0x00BF9200
//   DamageInfo -> 0x004D6F86, VA 0x00BF91AC output -> 0x004D70D5).
// - DamageInfo is 0x7C bytes: its table at +0, the 0x68-byte input at +0x04, the
//   0x10-byte output at +0x6C. Its ctor (COMDAT 0x00263895) calls the input ctor
//   (COMDAT 0x00263653) and builds the output inline; DamageInfo::xfer (0x004D6F86)
//   transfers Version(1,1) and then calls each record's slot 0.
// - The input's xfer (0x004D6FC4, Version(3,1)) gives the member types in order:
//   ObjectID +0x04, uint +0x08, XferDamageType +0x0C, XferDamageFXType +0x10,
//   XferDamageSubType +0x14, XferDeathType +0x18, real +0x1C, bool +0x20, real +0x24,
//   XferFXTrigger +0x28, ObjectID +0x2C, Coord3D +0x30, real +0x3C/+0x40/+0x44/+0x48;
//   version 2 adds bool +0x21, version 3 real +0x64. The input ctor's defaults: damage
//   type 22 (UNDEFINED), FX type 29 (UNDEFINED), +0x21 true, +0x48 and +0x50 1.0, +0x54
//   100.0, the rest zero.
// - FlammableUpdate::doAflameDamage (0x0048C771) fills source +0x04, type +0x0C,
//   sub type +0x14 (2), death type +0x18 and amount +0x1C before Object::attemptDamage.
// - Enumerations: the INI name tables. DamageType VA 0x00DB8968 (27 names), DamageFXType
//   VA 0x00DBC4B0 (30), DamageSubType VA 0x00DCA3F0 (NORMAL, BECOME_UNDEAD), DeathType
//   VA 0x00DBC5B0 (24). The DAMAGEFX_ and DAMAGESUBTYPE_ prefixes are ours; the names
//   are the tables'.
// Member names follow Zero Hour's Damage.h where the transfer order and type agree.
#pragma once
#ifndef BFME2_DAMAGE_H
#define BFME2_DAMAGE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/Coord3D.h"

class Xfer;

//-------------------------------------------------------------------------------------------------
enum DamageType
{
	DAMAGE_FORCE = 0,
	DAMAGE_CRUSH = 1,
	DAMAGE_SLASH = 2,
	DAMAGE_PIERCE = 3,
	DAMAGE_SIEGE = 4,
	DAMAGE_STRUCTURAL = 5,
	DAMAGE_FLAME = 6,
	DAMAGE_HEALING = 7,
	DAMAGE_UNRESISTABLE = 8,
	DAMAGE_WATER = 9,
	DAMAGE_PENALTY = 10,
	DAMAGE_FALLING = 11,
	DAMAGE_TOPPLING = 12,
	DAMAGE_REFLECTED = 13,
	DAMAGE_PASSENGER = 14,
	DAMAGE_MAGIC = 15,
	DAMAGE_CHOP = 16,
	DAMAGE_HERO = 17,
	DAMAGE_SPECIALIST = 18,
	DAMAGE_URUK = 19,
	DAMAGE_HERO_RANGED = 20,
	DAMAGE_FLY_INTO = 21,
	DAMAGE_UNDEFINED = 22,
	DAMAGE_LOGICAL_FIRE = 23,
	DAMAGE_CAVALRY = 24,
	DAMAGE_CAVALRY_RANGED = 25,
	DAMAGE_POISON = 26,

	DAMAGE_NUM_TYPES
};

// One bit per type (bit type-1), as the BoneFX unit's flags use them.
typedef UnsignedInt DamageTypeFlags;

inline Bool getDamageTypeFlag(DamageTypeFlags flags, DamageType dt)
{
	return (flags & (1 << (dt - 1))) != 0;
}

enum DamageFXType
{
	DAMAGEFX_SWORD_SLASH = 0,
	DAMAGEFX_WITCH_KING_MORGUL_BLADE = 1,
	DAMAGEFX_REFLECTED = 2,
	DAMAGEFX_GOOD_ARROW_PIERCE = 3,
	DAMAGEFX_EVIL_ARROW_PIERCE = 4,
	DAMAGEFX_SMALL_ROCK = 5,
	DAMAGEFX_BIG_ROCK = 6,
	DAMAGEFX_CLUBBING = 7,
	DAMAGEFX_FLAME = 8,
	DAMAGEFX_MAGIC = 9,
	DAMAGEFX_BALROG_SWORD = 10,
	DAMAGEFX_BALROG_WHIP = 11,
	DAMAGEFX_ELECTRIC = 12,
	DAMAGEFX_GIMLI_LEAP = 13,
	DAMAGEFX_STRUCTURAL = 14,
	DAMAGEFX_FLOOD_HORSE = 15,
	DAMAGEFX_BOLT = 16,
	DAMAGEFX_BOLT2 = 17,
	DAMAGEFX_MAGIC2 = 18,
	DAMAGEFX_MAGIC3 = 19,
	DAMAGEFX_FIRE1 = 20,
	DAMAGEFX_FIRE2 = 21,
	DAMAGEFX_FIRE3 = 22,
	DAMAGEFX_SPARKS1 = 23,
	DAMAGEFX_SPARKS2 = 24,
	DAMAGEFX_EARTH1 = 25,
	DAMAGEFX_EARTH2 = 26,
	DAMAGEFX_POISON = 27,
	DAMAGEFX_TORNADO = 28,
	DAMAGEFX_UNDEFINED = 29,

	DAMAGEFX_NUM_TYPES
};

enum DamageSubType
{
	DAMAGESUBTYPE_NORMAL = 0,
	DAMAGESUBTYPE_BECOME_UNDEAD = 1,

	DAMAGESUBTYPE_NUM_TYPES
};

enum DeathType
{
	DEATH_NORMAL = 0,
	DEATH_NONE = 1,
	DEATH_CRUSHED = 2,
	DEATH_BURNED = 3,
	DEATH_EXPLODED = 4,
	DEATH_POISONED = 5,
	DEATH_TOPPLED = 6,
	DEATH_FLOODED = 7,
	DEATH_SUICIDED = 8,
	DEATH_LASERED = 9,
	DEATH_DETONATED = 10,
	DEATH_SPLATTED = 11,
	DEATH_POISONED_BETA = 12,
	DEATH_EXTRA_2 = 13,
	DEATH_EXTRA_3 = 14,
	DEATH_EXTRA_4 = 15,
	DEATH_EXTRA_5 = 16,
	DEATH_EXTRA_6 = 17,
	DEATH_EXTRA_7 = 18,
	DEATH_EXTRA_8 = 19,
	DEATH_KNOCKBACK = 20,
	DEATH_SUPERNATURAL = 21,
	DEATH_FADED = 22,
	DEATH_SLAUGHTERED = 23,

	DEATH_NUM_TYPES
};

// The INI name tables, one copy in each object that asks for it (retail lays them out
// in this order: DamageFX VA 0x00DBC4B0, DamageType 0x00DBC530, DamageSubType 0x00DBC5A0,
// DeathType 0x00DBC5B0, and again in every object that defines the guard).
#ifdef DEFINE_DAMAGEFX_NAMES
static const char *TheDamageFXTypeNames[] =
{
	"SWORD_SLASH", "WITCH_KING_MORGUL_BLADE", "REFLECTED", "GOOD_ARROW_PIERCE",
	"EVIL_ARROW_PIERCE", "SMALL_ROCK", "BIG_ROCK", "CLUBBING", "FLAME", "MAGIC",
	"BALROG_SWORD", "BALROG_WHIP", "ELECTRIC", "GIMLI_LEAP", "STRUCTURAL", "FLOOD_HORSE",
	"BOLT", "BOLT2", "MAGIC2", "MAGIC3", "FIRE1", "FIRE2", "FIRE3", "SPARKS1", "SPARKS2",
	"EARTH1", "EARTH2", "POISON", "TORNADO", "UNDEFINED",
	NULL
};
#endif

#ifdef DEFINE_DAMAGE_NAMES
static const char *TheDamageNames[] =
{
	"FORCE", "CRUSH", "SLASH", "PIERCE", "SIEGE", "STRUCTURAL", "FLAME", "HEALING",
	"UNRESISTABLE", "WATER", "PENALTY", "FALLING", "TOPPLING", "REFLECTED", "PASSENGER",
	"MAGIC", "CHOP", "HERO", "SPECIALIST", "URUK", "HERO_RANGED", "FLY_INTO", "UNDEFINED",
	"LOGICAL_FIRE", "CAVALRY", "CAVALRY_RANGED", "POISON",
	NULL
};

static const char *TheDamageSubTypeNames[] =
{
	"NORMAL", "BECOME_UNDEAD",
	NULL
};
#endif

#ifdef DEFINE_DEATH_NAMES
static const char *TheDeathNames[] =
{
	"NORMAL", "NONE", "CRUSHED", "BURNED", "EXPLODED", "POISONED", "TOPPLED", "FLOODED",
	"SUICIDED", "LASERED", "DETONATED", "SPLATTED", "POISONED_BETA", "EXTRA_2", "EXTRA_3",
	"EXTRA_4", "EXTRA_5", "EXTRA_6", "EXTRA_7", "EXTRA_8", "KNOCKBACK", "SUPERNATURAL",
	"FADED", "SLAUGHTERED",
	NULL
};
#endif

typedef UnsignedInt PlayerMaskType;

//-------------------------------------------------------------------------------------------------
// DamageInfoInput: what the damage source asks for. 0x68 bytes.
class DamageInfoInput
{
public:
	virtual void xfer(Xfer *xfer);				// slot 0, 0x004D6FC4

	ObjectID m_sourceID;						// 0x04
	PlayerMaskType m_sourcePlayerMask;			// 0x08
	DamageType m_damageType;					// 0x0C default DAMAGE_UNDEFINED
	DamageFXType m_damageFXOverride;			// 0x10 default DAMAGEFX_UNDEFINED
	DamageSubType m_damageSubType;				// 0x14
	DeathType m_deathType;						// 0x18
	Real m_amount;								// 0x1C
	Bool m_kill;								// 0x20
	Bool m_unk0x21;								// 0x21 xfer v2, default true
	Real m_unk0x24;								// 0x24
	Int m_fxTrigger;							// 0x28 XferFXTrigger
	ObjectID m_unk0x2C;							// 0x2C
	Coord3D m_shockWaveVector;					// 0x30
	Real m_shockWaveAmount;						// 0x3C
	Real m_shockWaveRadius;						// 0x40
	Real m_shockWaveTaperOff;					// 0x44
	Real m_unk0x48;								// 0x48 default 1.0
	Bool m_unk0x4C;								// 0x4C not transferred
	Real m_unk0x50;								// 0x50 default 1.0, not transferred
	Real m_unk0x54;								// 0x54 default 100.0, not transferred
	Real m_unk0x58[3];							// 0x58 not transferred
	Real m_unk0x64;								// 0x64 xfer v3
};
// SIZE 0x68
typedef char DamageInfoInput_size[(sizeof(DamageInfoInput) == 0x68) ? 1 : -1];
typedef char DamageInfoInput_off_m_sourceID[(offsetof(DamageInfoInput, m_sourceID) == 0x04) ? 1 : -1];
typedef char DamageInfoInput_off_m_sourcePlayerMask[(offsetof(DamageInfoInput, m_sourcePlayerMask) == 0x08) ? 1 : -1];
typedef char DamageInfoInput_off_m_damageType[(offsetof(DamageInfoInput, m_damageType) == 0x0C) ? 1 : -1];
typedef char DamageInfoInput_off_m_damageFXOverride[(offsetof(DamageInfoInput, m_damageFXOverride) == 0x10) ? 1 : -1];
typedef char DamageInfoInput_off_m_damageSubType[(offsetof(DamageInfoInput, m_damageSubType) == 0x14) ? 1 : -1];
typedef char DamageInfoInput_off_m_deathType[(offsetof(DamageInfoInput, m_deathType) == 0x18) ? 1 : -1];
typedef char DamageInfoInput_off_m_amount[(offsetof(DamageInfoInput, m_amount) == 0x1C) ? 1 : -1];
typedef char DamageInfoInput_off_m_kill[(offsetof(DamageInfoInput, m_kill) == 0x20) ? 1 : -1];
typedef char DamageInfoInput_off_m_unk0x21[(offsetof(DamageInfoInput, m_unk0x21) == 0x21) ? 1 : -1];
typedef char DamageInfoInput_off_m_unk0x24[(offsetof(DamageInfoInput, m_unk0x24) == 0x24) ? 1 : -1];
typedef char DamageInfoInput_off_m_fxTrigger[(offsetof(DamageInfoInput, m_fxTrigger) == 0x28) ? 1 : -1];
typedef char DamageInfoInput_off_m_unk0x2C[(offsetof(DamageInfoInput, m_unk0x2C) == 0x2C) ? 1 : -1];
typedef char DamageInfoInput_off_m_shockWaveVector[(offsetof(DamageInfoInput, m_shockWaveVector) == 0x30) ? 1 : -1];
typedef char DamageInfoInput_off_m_shockWaveAmount[(offsetof(DamageInfoInput, m_shockWaveAmount) == 0x3C) ? 1 : -1];
typedef char DamageInfoInput_off_m_shockWaveRadius[(offsetof(DamageInfoInput, m_shockWaveRadius) == 0x40) ? 1 : -1];
typedef char DamageInfoInput_off_m_shockWaveTaperOff[(offsetof(DamageInfoInput, m_shockWaveTaperOff) == 0x44) ? 1 : -1];
typedef char DamageInfoInput_off_m_unk0x48[(offsetof(DamageInfoInput, m_unk0x48) == 0x48) ? 1 : -1];
typedef char DamageInfoInput_off_m_unk0x4C[(offsetof(DamageInfoInput, m_unk0x4C) == 0x4C) ? 1 : -1];
typedef char DamageInfoInput_off_m_unk0x50[(offsetof(DamageInfoInput, m_unk0x50) == 0x50) ? 1 : -1];
typedef char DamageInfoInput_off_m_unk0x54[(offsetof(DamageInfoInput, m_unk0x54) == 0x54) ? 1 : -1];
typedef char DamageInfoInput_off_m_unk0x58[(offsetof(DamageInfoInput, m_unk0x58) == 0x58) ? 1 : -1];
typedef char DamageInfoInput_off_m_unk0x64[(offsetof(DamageInfoInput, m_unk0x64) == 0x64) ? 1 : -1];

//-------------------------------------------------------------------------------------------------
// DamageInfoOutput: what the damage calculation produced. 0x10 bytes; its xfer
// (0x004D70D5) transfers real +0x04, real +0x08 and bool +0x0C.
class DamageInfoOutput
{
public:
	virtual void xfer(Xfer *xfer);				// slot 0, 0x004D70D5

	Real m_actualDamageDealt;					// 0x04
	Real m_actualDamageClipped;					// 0x08
	Bool m_noEffect;							// 0x0C
};
// SIZE 0x10
typedef char DamageInfoOutput_size[(sizeof(DamageInfoOutput) == 0x10) ? 1 : -1];
typedef char DamageInfoOutput_off_m_actualDamageDealt[(offsetof(DamageInfoOutput, m_actualDamageDealt) == 0x04) ? 1 : -1];
typedef char DamageInfoOutput_off_m_actualDamageClipped[(offsetof(DamageInfoOutput, m_actualDamageClipped) == 0x08) ? 1 : -1];
typedef char DamageInfoOutput_off_m_noEffect[(offsetof(DamageInfoOutput, m_noEffect) == 0x0C) ? 1 : -1];

//-------------------------------------------------------------------------------------------------
class DamageInfo
{
public:
	// Out of line (0x00263895: stores table 0x007F9200 and the output's 0x007F91AC,
	// calls the input's ctor 0x00263653); RepairDockUpdate::action 0x004A12B6 calls it
	// on a stack DamageInfo.
	DamageInfo();
	virtual void xfer(Xfer *xfer);				// slot 0, 0x004D6F86

	DamageInfoInput in;							// 0x04
	DamageInfoOutput out;						// 0x6C
};
// SIZE 0x7C
typedef char DamageInfo_size[(sizeof(DamageInfo) == 0x7C) ? 1 : -1];
typedef char DamageInfo_off_in[(offsetof(DamageInfo, in) == 0x04) ? 1 : -1];
typedef char DamageInfo_off_out[(offsetof(DamageInfo, out) == 0x6C) ? 1 : -1];

//-------------------------------------------------------------------------------------------------
// These legacy declarations preserve the DamageInfo stack layout for callers
// that still use address-named types. The constructors now have their DamageInfo names.
class Rva00263653
{
public:
	Rva00263653() throw();
	char m_data[0x68];
};

class Rva00263895Member
{
public:
	Rva00263895Member() throw();
	virtual void rva004D6F86(void *arg);

private:
	Rva00263653 m_mem;							// 0x04
	const void *m_ptr;							// 0x6C
	float m_f70;								// 0x70
	float m_f74;								// 0x74
	UnsignedByte m_b78;							// 0x78
};
// SIZE 0x7C


#endif
