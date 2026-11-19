// LocomotorSet.h: the set of locomotors an AI update owns (AIUpdateInterface +0x1CC).
// Target facts (retail game.dat):
// - SIZE 0x24: AIUpdateInterface's members continue at +0x1F0.
// - Ctor 0x001E7087 (map placeholder Rva001E7087): table 0x00BDE950 (Snapshot's
//   shape; GetSnapshotName 0x001E70BC returns "LocomotorSet", slot 3 0x001E8730
//   which the map spells DoXfer), an STLport vector<Locomotor *> at +4, then zero at
//   +0x10, Bools +0x14/+0x15, +0x18, +0x1C and Real +0x20.
// - addLocomotor 0x001E88CE: TheLocomotorStore->newLocomotor, push_back, ORs the
//   template's Surfaces (+0x14) into +0x10 and sets +0x14/+0x15 when the template's
//   DownhillOnly (+0xD3) / ScalesWalls (+0x150) is set; the members are named for those.
// - clear 0x001E7E25 (map placeholder Rva001E7E25): `::delete`s every Locomotor,
//   erases the vector, zeroes +0x10 and the two Bools.
// - xferSelfAndCurLocoPtr 0x001E70C2, setLocomotorAndBaseSpeed 0x001EA2CF.
// WorldBuilder: "LocomotorSet::addLocomotor", "LocomotorSet::DoXfer",
// "LocomotorSet::xferSelfAndCurLocoPtr", "LocomotorSet::setLocomotorAndBaseSpeed".
// "clear" is the donor's name (Zero Hour); no string names it.
#pragma once
#ifndef BFME2_LOCOMOTORSET_H
#define BFME2_LOCOMOTORSET_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/Snapshot.h"
#include "Common/Vector.h"

class Locomotor;
class LocomotorTemplate;
class INI;
class ThingTemplate;

class LocomotorSet : public Snapshot
{
public:
	LocomotorSet();												// 0x001E7087
	virtual ~LocomotorSet();

	void clear();												// 0x001E7E25
	void addLocomotor(const LocomotorTemplate *lt, Bool flag);	// 0x001E88CE
	void xferSelfAndCurLocoPtr(Xfer *xfer, Locomotor **loco);	// 0x001E70C2
	void setLocomotorAndBaseSpeed(INI *ini, ThingTemplate *tt);	// 0x001EA2CF

protected:
	virtual void loadPostProcess() {}
	virtual const char *GetSnapshotName() const;				// 0x001E70BC
	virtual void xfer(Xfer *xfer);								// 0x001E8730 (the map spells it DoXfer)

public:
	// 0x001E48E8 (Zero Hour's findLocomotor; map placeholder ?rva001E48E8@Rva001E48E8@@QAEPAXI@Z, ecx is the set: AIUpdateInterface::chooseGoodLocomotorFromCurrentSet 0x00263FA7 passes &m_locomotorSet): the first locomotor whose template's Surfaces (+0x14) share a bit with t.
	Locomotor *findLocomotor(UnsignedInt t);
	_STL::vector<Locomotor *> m_locomotors;		// +0x04
	UnsignedInt m_validLocomotorSurfaces;		// +0x10
	Bool m_downhillOnly;						// +0x14
	Bool m_scalesWalls;							// +0x15
	UnsignedInt m_unk0x18;						// +0x18
	UnsignedInt m_unk0x1C;						// +0x1C
	Real m_unk0x20;								// +0x20
};

typedef char LocomotorSet_off_m_validLocomotorSurfaces[(offsetof(LocomotorSet, m_validLocomotorSurfaces) == 0x10) ? 1 : -1];
typedef char LocomotorSet_off_m_unk0x20[(offsetof(LocomotorSet, m_unk0x20) == 0x20) ? 1 : -1];
typedef char LocomotorSetSize[(sizeof(LocomotorSet) == 0x24) ? 1 : -1];

#endif
