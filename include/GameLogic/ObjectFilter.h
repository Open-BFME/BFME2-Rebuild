// ObjectFilter.h: a module data's INI-parsed object filter (iniParseObjectFilter).
// SIZE 0x4.
// Target facts: module-data constructors build it through 0x003623E5 and their
// destructors tear it down through 0x00360D26 (old repo
// BloodthirstyUpdateModuleDataCtor.cpp / Dtor.cpp, SpecialPowerModuleData.h).
// The signed word indexes the interned records used by isValid and INI parsing.
#pragma once
#ifndef BFME2_OBJECTFILTER_H
#define BFME2_OBJECTFILTER_H

#include "Common/GameType.h"

class Object;
#include "Common/BitFlags.h"
class Player;
class ThingTemplate;

class ObjectFilter
{
public:
	ObjectFilter();
 void initFromStorages(KindOfMaskType,KindOfMaskType);
	// 0x00362192 is address-named: retail's module-data constructor passes it a KindOf list and a storage.
	void rva00362192(KindOfMaskType,KindOfMaskType);
 bool testKindOf(const KindOfMaskType *,const Player *,const Player *);
	~ObjectFilter();

	// 0x00362437: does obj pass the filter for player? Const as every caller holds it (module data filters are
	// const); the map names the row only with a placeholder (Rva2225E0Filter), so the const spelling is the real one.
	Bool accepts(Object *obj, Player *player) const;
	// 0x003618B9: the template test accepts runs with the object owner and the asking player (const with accepts; the
	// map spells it QAE, so its own row needs the map to follow when it is written).
	bool testTemplate(const ThingTemplate *tmpl, const Player *owner, const Player *player) const;
	// 0x00360CED: the filter has been set (Weapon::computeStatus 0x002CC422 and
	// getRemainingAmmo 0x002CC578 test the template's ProjectileFilterInContainer).
	Bool isValid() const;

private:
	friend void iniParseObjectFilter(INI *ini, void *instance, void *store, const void *userData);
	Int m_recordIndex;	// 0x0, -1 until a record is assigned
};
// SIZE 0x4

#endif
