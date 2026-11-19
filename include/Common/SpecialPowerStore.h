// SpecialPowerStore.h: the subsystem that owns every SpecialPower INI block (global
// TheSpecialPowerStore, VA 0x00E02D4C). SIZE 0x1C.
// Target facts (retail game.dat):
//  - constructor 0x003B11AD: SubsystemInterface base (0x001B4E63), then vtable
//    0x00C1ED1C, then the vector's _Vector_base at +0x0C, then +0x18 = 0.
//  - destructor 0x003B1546 (scalar-deleting 0x003B17F3, slot 0): stores the vtable
//    back, deletes every template pointer of the vector at +0x0C (virtual destructor
//    with flag 0, then the global operator delete), clears the vector, zeroes +0x18,
//    frees the vector's storage and runs ~SubsystemInterface.
//  - vtable 0x00C1ED1C slots: 0 own scalar-deleting dtor, 1 init (empty, folded to
//    0x000B3FD0), 9 reset 0x001FF5C3, 10 update (empty); every other slot is the
//    base's own body (loadIniFilesFromLegend, attemptReload, ...).
//  - reset 0x001FF5C3 walks the vector and, for each template, calls
//    deleteOverrides(); a template that returns null (the first definition is only
//    an override holder) is erased from the vector. Same shape as every Overridable
//    store.
//  - +0x18 is a running counter: each new template increments it and stores the
//    result as the template's id (SpecialPowerTemplate +0x14,
//    parseSpecialPowerDefinition 0x003B15D6, findSpecialPowerTemplateByID 0x003B0FD0).
#pragma once
#ifndef BFME2_SPECIALPOWERSTORE_H
#define BFME2_SPECIALPOWERSTORE_H

#include "Common/GameType.h"
#include "Common/AsciiString.h"
#include "Common/SubsystemInterface.h"
#include "Common/Vector.h"

class INI;
class Object;
class SpecialPowerTemplate;

class SpecialPowerStore : public SubsystemInterface
{
public:
	SpecialPowerStore();													// 0x003B11AD
	virtual ~SpecialPowerStore();											// 0x003B1546 (map: Rva003B1546)

	virtual void init() {}													// slot 1, empty (folded)
	virtual void reset();													// slot 9, 0x001FF5C3
	virtual void update() {}												// slot 10, empty (folded)

	const SpecialPowerTemplate *findSpecialPowerTemplate(AsciiString name);	// 0x0029B6EB
	const SpecialPowerTemplate *findSpecialPowerTemplateByID(UnsignedInt id);	// 0x003B0FD0
	Bool canUseSpecialPower(Object *obj, const SpecialPowerTemplate *power);	// 0x003B1244
	Int getNumSpecialPowers() const;										// 0x003B0FC6 (map: Rva003B0FC6SarAvgField): the template vector's size, (end - begin) >> 2

	static void parseSpecialPowerDefinition(INI *ini);						// 0x003B15D6

protected:
	SpecialPowerTemplate *findSpecialPowerTemplatePrivate(AsciiString name);	// 0x003B11D2

private:
	_STL::vector<SpecialPowerTemplate *> m_templates;						// 0x0C
	UnsignedInt m_nextId;													// 0x18
};
// SIZE 0x1C

extern SpecialPowerStore *TheSpecialPowerStore;							// VA 0x00E02D4C

#endif
