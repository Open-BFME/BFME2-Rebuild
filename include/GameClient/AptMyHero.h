// AptMyHero.h: the create-a-hero screen's hero model: a CreateAHeroHero with the screen's bling and power state. It is embedded
// in AptCreateAHero at +0x27C (0x190 bytes).
//
// Target facts (retail game.dat, RVAs; table addresses are VAs):
//  - Constructor 0x005B12C5 takes the screen (it calls CreateAHeroData's value constructor 0x00409C3D first, then stores the table
//    VA 0x00872A80), destructor 0x005B190D (ends with ~CreateAHeroData 0x00409285), deleting destructor 0x005B1A50. Table slots:
//    0 deleting destructor, 1 the empty stub 0x000B3FD0, 2 GetSnapshotName 0x00409353, 3 xfer 0x00408C11, 4 Update 0x004083FF,
//    5 slot14 0x005B023A (called as `[vptr+0x14]` by rva005B1288 and SwitchToPendingHero: it re-applies the model).
//  - Layout beyond CreateAHeroHero (0x140): +0x140 the screen (AptCreateAHero*; the destructor and 0x005B1830 reach the screen's callback
//    host through it at +0x228), +0x144 the pending hero (a CreateAHeroData pointer; SwitchToPendingHero 0x005B1E71 copies it over
//    this hero when it differs and is non-null), +0x148 a flag SwitchToPendingHero clears, +0x14C an int, +0x150 and +0x151 flags
//    (1 at construction), +0x154 and +0x158 ints (the bounds rva005B0EE6 wraps a bling value into), +0x15C a vector of 8-byte
//    records (an int and a float: GetObjectInfo 0x005B0E9F returns a pointer into it), +0x168 and +0x16C floats (0 at construction;
//    rva005B0487 stores 0 and the hero's value at +0x60 of its view record), +0x170 an int (1), +0x174 two vectors of 0x14-byte
//    records (the bling ranges rva005B0923 walks: each record's +8/+0xC are the random bounds), +0x18C an int.
#pragma once
#ifndef BFME2_APTMYHERO_H
#define BFME2_APTMYHERO_H

#include "GameLogic/CreateAHeroHero.h"
#include "Common/Vector.h"

class AptCreateAHero;
class Object;

// One 8-byte record of the hero's object-info vector (the map spells it BfmePod8).
struct BfmePod8
{
	BfmePod8() : m_id(0), m_value(0.0f) {}		// the function-local empty record of GetObjectInfo 0x005B0E9F is built by it at run time
	int m_id;
	float m_value;
};

// One 0x14-byte record of the hero's bling ranges: +0x08 and +0x0C are the random bounds rva005B0923 walks.
struct AptBlingRange
{
	int m_key;										// +0x00 (rva00407E28's key)
	int m_x04;
	int m_low;										// +0x08
	int m_high;										// +0x0C
	int m_x10;
};

class AptMyHero : public CreateAHeroHero
{
public:
	AptMyHero(AptCreateAHero *);					// 0x005B12C5
	virtual ~AptMyHero();							// 0x005B190D
	virtual bool slot4(int);						// 0x004083FF (CreateAHeroHero::Update in the map)
	virtual void slot14();							// 0x005B023A

	void AdjustBling(int kind, int index, int delta);	// 0x005B0EE6 (kind 0: attribute, 1: appearance)
	void rva005B0FCD(int);							// 0x005B0FCD
	void rva005B0923(int);							// 0x005B0923
	void rva005B097F(int);							// 0x005B097F
	void RotateLeft();								// 0x005B0249 (the screen's rotate-left key flag; the map spells its owner Rva005B02B5)
	void RotateRight();								// 0x005B027F
	void ZoomIn();									// 0x005B02B5
	void ZoomOut();									// 0x005B02FD
	void rva005B1019();								// 0x005B1019
	void FrameUpdate();								// 0x005B1EE2
	void rva005B0416(int);							// 0x005B0416
	void rva005B0446();								// 0x005B0446: rva005B0416 for the ten power slots
	void rva005B1A6C();								// 0x005B1A6C
	void rva005B1288();								// 0x005B1288
	void rva005B0487();								// 0x005B0487: resets the view range (+0x168/+0x16C) from the manager's view info
	void *rva005B045D();							// 0x005B045D: the pending hero while a switch is in progress (+0x14C is 1), else this hero
	int rva005B0E60(const Object *);				// 0x005B0E60: the location number after the first '_' of the object's name, or -1
	BfmePod8 *GetObjectInfo(int);					// 0x005B0E9F: the object-info record for a location (an empty record past the end)
	void rva005B0608(unsigned int index, char *out, int);	// 0x005B0608: Apt query, the attribute range's low bound plus one
	void rva005B0648(unsigned int index, char *out, int);	// 0x005B0648: Apt query, the bling value for the range's key plus one
	void rva005B068E(unsigned int index, char *out, int);	// 0x005B068E: Apt query, the attribute range's high bound plus one
	void rva005B06CE(unsigned int index, char *out, bool skip);	// 0x005B06CE: Apt query, the appearance range's +4 field
	void rva005B04B6();								// 0x005B04B6
	void rva00407BC3();								// 0x00407BC3 (the map spells its owner Rva004076EE: a method of the hero's base)
	void rva00407C17();								// 0x00407C17 (same owner)
	void rva005B21DA(CreateAHeroData *hero, bool, int);	// 0x005B21DA: takes the given hero as the screen's hero (the third argument is the manager page's refresh flag)
	void rva005B034A(int);							// 0x005B034A (the map has no row for it)
	void SetBling(int kind, int index, int value);	// 0x005B07C1

	AptCreateAHero *m_screen;						// +0x140
	CreateAHeroData *m_pendingHero;					// +0x144
	bool m_flag148;									// +0x148
	unsigned char m_pad149[3];
	int m_x14C;										// +0x14C
	bool m_flag150;									// +0x150 (1 at construction)
	bool m_flag151;									// +0x151 (1 at construction)
	unsigned char m_pad152[2];
	int m_x154;										// +0x154
	int m_x158;										// +0x158
	_STL::vector<BfmePod8> m_objectInfo;			// +0x15C
	float m_x168;									// +0x168
	float m_x16C;									// +0x16C
	int m_x170;										// +0x170 (1 at construction)
	_STL::vector<AptBlingRange> m_ranges[2];		// +0x174 attribute ranges, +0x180 appearance ranges (rva005B0923 and rva005B097F index them by kind)
	int m_x18C;										// +0x18C
};
typedef char AptMyHeroSizeCheck[sizeof(AptMyHero)==0x190 ? 1 : -1];

#endif
