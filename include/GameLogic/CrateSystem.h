// CrateSystem.h: crate templates and the subsystem that owns them.
//
// Target facts:
// - CrateTemplate (constructor 0x0035CC36): Overridable (0x10) first, then the
//   name AsciiString +0x10 (set to a default literal), a Real +0x14 = 0, an Int
//   +0x18 = 5, a 0x1C-byte set at +0x1C (member constructor 0x0024C7B3, then
//   memset 0x1C: BitFlags<218>'s seven words), an Int +0x38 = -1, an STLport
//   list at +0x3C (8-byte entries: an AsciiString and a 4-byte value) and a Bool
//   +0x40. setName 0x0035C972 takes its AsciiString by value and sets +0x10.
// - CrateSystem (constructor 0x0035CAD6): SubsystemInterface (0xC) and a vector
//   of CrateTemplate* at +0x0C. reset 0x0035C9CF walks it, erasing null entries
//   and entries whose deleteOverrides() returns null.
// Donor facts: Zero Hour's CrateSystem.h has the same members in the same
// order (m_name, m_creationChance, m_veterancyLevel, m_killedByTypeKindof,
// m_killerScience, m_possibleCrates, m_isOwnedByMaker; crateCreationEntry
// {crateName, crateChance}; m_crateTemplateVector).
#pragma once
#ifndef BFME2_CRATESYSTEM_H
#define BFME2_CRATESYSTEM_H

#include <stddef.h>
#include <list>
#include <vector>
#include "Common/GameType.h"
#include "Common/AsciiString.h"
#include "Common/BitFlags.h"
#include "Common/Overridable.h"
#include "Common/SubsystemInterface.h"

class INI;
struct FieldParse;

struct crateCreationEntry
{
	AsciiString crateName;												// 0x0
	Real crateChance;													// 0x4
};

typedef std::list<crateCreationEntry> crateCreationEntryList;

class CrateTemplate : public Overridable
{
public:
	CrateTemplate();													// 0x0035CC36
	CrateTemplate &operator=(const CrateTemplate &that);				// 0x0035CE52

	void setName(AsciiString name);										// 0x0035C972 (out of line: a retail row)

	static void parseCrateCreationEntry(INI *ini, void *instance, void *store, const void *userData);	// 0x0035CD23

	// 0x00C162A0: six entries (CreationChance +0x14, VeterancyLevel +0x18, KilledByType
	// +0x1C, CrateObject, KillerScience +0x38, OwnedByMaker +0x40).
	static const FieldParse TheCrateTemplateFieldParseTable[];

protected:
	// Protected; CrateSystem's destructor deletes its templates.
	friend class CrateSystem;
	virtual ~CrateTemplate();											// 0x0035CCCA

public:
	AsciiString m_name;													// 0x10
	Real m_creationChance;												// 0x14
	Int m_veterancyLevel;												// 0x18 = 5
	KindOfMaskType m_killedByTypeKindof;								// 0x1C
	Int m_killerScience;												// 0x38 = -1
	crateCreationEntryList m_possibleCrates;							// 0x3C
	Bool m_isOwnedByMaker;												// 0x40
};
// SIZE 0x44

typedef char CrateTemplate_off_m_name[(offsetof(CrateTemplate, m_name) == 0x10) ? 1 : -1];
typedef char CrateTemplate_off_m_killerScience[(offsetof(CrateTemplate, m_killerScience) == 0x38) ? 1 : -1];
typedef char CrateTemplate_off_m_isOwnedByMaker[(offsetof(CrateTemplate, m_isOwnedByMaker) == 0x40) ? 1 : -1];

class CrateSystem : public SubsystemInterface
{
public:
	CrateSystem();														// 0x0035CAD6
	virtual ~CrateSystem();												// 0x0035CB07

	virtual void init() { reset(); }	// 0x000D20D6: `mov eax,[ecx]; jmp [eax+0x24]`, a tail call to reset (slot 9)
	virtual void reset();												// 0x0035C9CF
	virtual void update() {}

	CrateTemplate *friend_findCrateTemplate(AsciiString name);			// 0x0035CA2F
	CrateTemplate *newCrateTemplate(AsciiString name);					// 0x0035CF2C
	CrateTemplate *newCrateTemplateOverride(CrateTemplate *crateToOverride);	// 0x0035CEA6

	static void parseCrateTemplateDefinition(INI *ini);					// 0x0035CFEA

	std::vector<CrateTemplate *> m_crateTemplateVector;					// 0x0C
};
// SIZE 0x18

extern CrateSystem *TheCrateSystem;	// 0x00A01E58 (VA 0x00E01E58; map: ?TheCrateSystem@@3PAVCrateSystem@@A)

typedef char CrateSystem_off_m_crateTemplateVector[(offsetof(CrateSystem, m_crateTemplateVector) == 0x0C) ? 1 : -1];
typedef char CrateSystem_size[(sizeof(CrateSystem) == 0x18) ? 1 : -1];

#endif
