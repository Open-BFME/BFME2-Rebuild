// ProductionPrerequisite.h: what a player must own before a thing can be built: units
// (groups of alternatives), sciences and upgrades. SIZE 0x24, no virtual functions.
// Target facts (retail game.dat):
//  - ctor 0x004F527A builds three vector bases (+0x00, +0x0C, +0x18) and calls clear();
//    dtor 0x004F5115 frees the +0x18 and +0x0C storage inline and runs the unit
//    vector's destructor 0x006D040C last.
//  - +0x00 is a vector of PrereqUnitRec (12 bytes: unit pointer, flag word, name): the
//    name is the INI spelling, resolveNames 0x004F4B6D swaps it for the template through
//    TheThingFactory and clears it. Bit 0 of the flag word marks an alternative of the
//    previous record (getAllPossibleBuildFacilityTemplates 0x004F4C22, isSatisfied).
//  - +0x0C is a vector of ScienceType (hasScience on each), +0x18 a vector of upgrade
//    templates (only player-type upgrades count: isSatisfied tests UpgradeTemplate +4).
//  - clear 0x004F5227 empties the unit and science vectors only.
//  - calcNumPrereqUnitsOwned 0x004F4BC0 is private (the map spells it A): it hands the
//    first 32 unit templates to Player::countObjectsByThingTemplate.
#pragma once
#ifndef BFME2_PRODUCTIONPREREQUISITE_H
#define BFME2_PRODUCTIONPREREQUISITE_H

#include "Common/GameType.h"
#include "Common/AsciiString.h"
#include "Common/UnicodeString.h"
#include "Common/Science.h"
#include "Common/Vector.h"

class Player;
class ThingTemplate;
class UpgradeTemplate;

class ProductionPrerequisite
{
public:
	struct PrereqUnitRec
	{
		const ThingTemplate *unit;									// 0x0
		Int flags;													// 0x4 bit 0: alternative of the previous record
		AsciiString name;											// 0x8
	};

	ProductionPrerequisite();										// 0x004F527A
	~ProductionPrerequisite();										// 0x004F5115

	void clear();													// 0x004F5227
	void resolveNames();											// 0x004F4B6D
	void addUnitPrereq(AsciiString name, Bool orWithPrevious);		// 0x004F52CB
	// Inline (Zero Hour's): ThingTemplate.cpp's parsePrerequisiteScience 0x0033EABD and
	// parsePrerequisiteUpgrade 0x0033EB1A push straight onto +0x0C / +0x18.
	void addSciencePrereq(ScienceType science) { m_prereqSciences.push_back(science); }
	void addUpgradePrereq(const UpgradeTemplate *upgrade) { m_prereqUpgrades.push_back(upgrade); }
	Bool isSatisfied(const Player *player) const;					// 0x004F4CE5
	UnicodeString getRequiresList(const Player *player) const;		// 0x004F4DF2
	Int getAllPossibleBuildFacilityTemplates(const ThingTemplate *list[], Int maxCount) const;	// 0x004F4C22
	const ThingTemplate *getExistingBuildFacilityTemplate(const Player *player) const;			// 0x004F4C85

private:
	Int calcNumPrereqUnitsOwned(const Player *player, Int counts[32]) const;	// 0x004F4BC0

	_STL::vector<PrereqUnitRec> m_prereqUnits;						// 0x00
	_STL::vector<ScienceType> m_prereqSciences;						// 0x0C
	_STL::vector<const UpgradeTemplate *> m_prereqUpgrades;			// 0x18
};
// SIZE 0x24

#endif
