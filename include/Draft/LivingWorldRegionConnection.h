// LivingWorldRegionConnection.h: one detour connection between two War of the Ring regions.
//
// Target facts (retail RVAs):
//  - Snapshot-derived, SIZE 0x18. The constructor 0x003F24F3 stores the table
//    0x00C36E48 at +0, empties the name at +4, stores -1 at +8 and builds the
//    detour vector at +0xC through the _Vector_base constructor 0x00211E58.
//  - The copy constructor 0x003F247D copies the name (StringBase copy
//    0x000365F0), the count word and the vector (copy 0x004334D7).
//  - The destructor 0x003F2517 frees the vector storage through 0x00030830
//    when it is set, destroys the name through 0x00036410, then leaves
//    Snapshot's table 0x00BBB554 in place. 0x003F2A70 is the deleting form.
//  - Slots: 0 deleting dtor, 1 the shared empty body 0x000B3FD0
//    (loadPostProcess), 2 the name getter 0x003F24D0, 3 DoXfer 0x003F3054.
//  - The name getter is spelled non-const (UAE), unlike Snapshot's const
//    pure slot; the map names it that way.
//  - The detour element is the 8-byte pair the map names BfmeE8 (two floats);
//    its owner class has no other name yet.
//  - Member names are the retail offsets only: no original name is evidenced.
#pragma once
#ifndef BFME2_LIVINGWORLDREGIONCONNECTION_H
#define BFME2_LIVINGWORLDREGIONCONNECTION_H

#include "Common/AsciiString.h"
#include "Common/Vector.h"

class Xfer;

// The detour point element: two floats (the map's placeholder name BfmeE8).
struct BfmeE8
{
	float x;	// +0x0
	float y;	// +0x4
};

// No Snapshot base: its name slot is const and pure, while retail's connection name slot is
// non-const, so a base would add a fifth vtable slot. Retail's table has four slots.
class LivingWorldRegionConnection
{
public:
	LivingWorldRegionConnection();				// 0x003F24F3
	LivingWorldRegionConnection(const LivingWorldRegionConnection &that);	// 0x003F247D
	virtual ~LivingWorldRegionConnection();			// 0x003F2517 (deleting 0x003F2A70)
protected:
	virtual void loadPostProcess();				// slot 1, shared empty body 0x000B3FD0
public:

	virtual const char *GetSnapshotName();			// slot 2, 0x003F24D0
	virtual void DoXfer(Xfer &xfer);			// slot 3, 0x003F3054


private:
	AsciiString m_regionName;				// +0x04
	int m_numberAllowed;					// +0x08, the constructor writes -1
	_STL::vector<BfmeE8> m_detourPoints;			// +0x0C
};

#endif
