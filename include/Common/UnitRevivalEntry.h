// UnitRevivalEntry.h: one unit a player can revive or rebuild, kept in UnitRevivalTracker's vector.
// SIZE 0xD8. Target facts (retail game.dat): the default constructor 0x0037E352 clears +0x00,
// +0x04, +0x08 (float), sets +0x0C and +0x10 to 1, clears the 0x80-byte mask at +0x14 (helper
// 0x001EAE6F), then +0x94 = 0, +0x98 = -1, +0x9C = 0, bytes at +0xA0 and +0xA1, ints at
// +0xA4, +0xA8, +0xAC, the helper object at +0xB0, +0xC8 = 0, +0xCC = 1.0f, +0xD0 = 0 and the
// name string at +0xD4. getThingTemplate (0x0037E270) looks the name up in TheThingFactory;
// removeBuildUnit (0x0037EEB9) compares it with a template's name; rva0037EF2D (0x0037EF2D)
// finds an entry by the int at +0xA4.
#pragma once
#ifndef BFME2_UNITREVIVALENTRY_H
#define BFME2_UNITREVIVALENTRY_H

#include "Common/GameType.h"
#include "Common/AsciiString.h"

class Image;
class ThingTemplate;
class Player;
class Object;

class UnitRevivalEntry
{
public:
	void setThingTemplateName(const AsciiString &name);				// 0x0037E3D9
	void *getThingTemplate();										// 0x0037E270

	const Image *calcButtonImage(const Player *player);				// 0x0037EBEA
	Int revivalEntryCalcCostToBuild(const Player *player, Object *object);	// 0x0037E18A
	Int revivalEntryCalcTimeToBuild(const Player *player, Object *object);	// 0x0037E1FD

	unsigned char m_unk0x0[0x94];									// +0x0 not established beyond the constructor's stores
	Int m_buildCost;												// +0x94 ThingTemplate::m_buildCost (ctor 0x0037E289, setThingTemplateName 0x0037E3D9)
	Int m_unk0x98;													// +0x98 -1 until the entry is queued, then the frame (productionSystemQueueCreateUnit)
	UnsignedInt m_buildFrames;										// +0x9C the template's build time converted to an unsigned int (ctor 0x0037E289, setThingTemplateName 0x0037E3D9)
	Bool m_unk0xA0;													// +0xA0 cleared by the constructors; setThingTemplateName copies cost and time only while it is false
	unsigned char m_unk0xA1[3];										// +0xA1 a byte cleared by the constructors, then padding
	Int m_unk0xA4;													// +0xA4 (rva0037EF2D matches an entry by it)
	unsigned char m_unk0xA8[0xC8 - 0xA8];							// +0xA8 not established
	Int m_unk0xC8;													// +0xC8 the constructor 0x0037E352 clears it; RespawnUpdate::triggerDeathBeforeRespawn 0x004AF63E stores the experience tracker's +0x28 into it
	Real m_unk0xCC;													// +0xCC 1.0f after the constructor, set to 0.998f when queued
	unsigned char m_unk0xD0[4];										// +0xD0 not established
	AsciiString m_thingTemplateName;								// +0xD4, the name getThingTemplate looks up
};
// SIZE 0xD8

#endif
