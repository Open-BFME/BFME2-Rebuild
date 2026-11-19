// FireWeaponUpdate.h: the FireWeaponUpdate module, its module data and the
// per-nugget INI block.
//
// Target facts (retail). The module data ctor row 0x0048BC03 news 0x10 bytes:
// vptr +0, an unknown dword at +4, the FireWeaponNugget pointer list at +8 and
// the HeroModeTrigger/ChargingModeTrigger/AliveOnly flag bytes at +0x0C/+0x0D/
// +0x0E (INI table 0x00C4C210). The module after UpdateModule keeps a Weapon at
// +0x20, its entry list at +0x24 and a ready flag at +0x28. Each nugget block is
// 0x18 bytes: AsciiString WeaponName, FireDelay, OneShot, Coord3D Offset.
// The entry list is STLport list<int> holding FireWeaponNuggetEntry pointers,
// exactly as the rowed list<int> bodies (0x0005548F push_back) show.
#pragma once
#ifndef BFME2_FIREWEAPONUPDATE_H
#define BFME2_FIREWEAPONUPDATE_H

#include "Common/GameType.h"
#include "Common/AsciiString.h"
#include "Common/Coord3D.h"
#include "Common/StlList.h"
#include "Common/Snapshot.h"
#include "Common/INI/FieldParse.h"
#include "Common/NameKeyGenerator.h"
#include "GameLogic/Module/UpdateModule.h"
#include "GameLogic/Module/ModuleData.h"

class Weapon;
class WeaponTemplate;
class Object;
class Thing;
class Xfer;

// The module data is a real ModuleData: retail's destructor 0x0048BC46 stores
// its own table 0x00C4C110 at +0, walks the +0x08 list releasing each nugget,
// calls _List_base's out-of-line destructor (0x0006074F) and finally stores
// Snapshot's 0x00BBB554. +4 is ModuleData's module tag key, which the ctor row
// 0x0048BC03 leaves alone.

class FireWeaponUpdateModuleData : public ModuleData
{
public:
	friend class FireWeaponUpdate;
	// The nugget block proc appends through the +0x08 list itself: retail's
	// 0x0048BCBB binds a temporary and calls list<int>::push_back in place
	// instead of calling addNugget (0x0048BBF3).
	friend void Rva0048BCBBParse(INI *ini, void *instance, void *store,
		const void *userData);
	// One FireWeaponNugget INI block (the module data table's token "FireWeaponNugget", block proc
	// 0x0048BCBB, field table 0x00C4C210). Nested: the object creation list's FireWeaponNugget
	// (token "FireWeapon", parse 0x001F0D61) is a different class with the same name.
	class FireWeaponNugget
	{
	public:
		FireWeaponNugget();

		AsciiString m_weaponName; // +0x00 WeaponName
		UnsignedInt m_fireDelay; // +0x04 FireDelay
		Bool m_oneShot; // +0x08 OneShot
		Coord3D m_offset; // +0x0C Offset
	};
	// SIZE 0x18

	FireWeaponUpdateModuleData();
	virtual ~FireWeaponUpdateModuleData();

	void addNugget(int nugget);
	static void buildFieldParse(MultiIniFieldParse &parse);

private:
	_STL::list<int> m_nuggets; // +0x08
	Bool m_heroModeTrigger; // +0x0C HeroModeTrigger
	Bool m_chargingModeTrigger; // +0x0D ChargingModeTrigger
	Bool m_aliveOnly; // +0x0E AliveOnly
};
// SIZE 0x10

class FireWeaponUpdate : public UpdateModule
{
public:
	FireWeaponUpdate(Thing *thing, const ModuleData *moduleData);
	virtual ~FireWeaponUpdate();

	virtual UpdateSleepTime update();
	virtual NameKeyType getModuleNameKey() const;
	void rva0048B9D0(const WeaponTemplate *weaponTemplate);
	void Rva0048BDF8Helper();

protected:
	virtual void loadPostProcess();
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);
	UpdateSleepTime rva0048BA3E();
	const FireWeaponUpdateModuleData *getFireWeaponUpdateModuleData() const
	{
		return (const FireWeaponUpdateModuleData *)m_moduleData;
	}

private:
	Weapon *m_weapon; // +0x20
	_STL::list<int> m_entries; // +0x24
	Bool m_entriesReady; // +0x28
};
// SIZE 0x2C (h05): ModuleFactory allocates 0x2C for FireWeaponUpdate (constructor 0x0048C0C5),
// and the members above end with m_entriesReady at +0x28.
typedef char FireWeaponUpdate_size[(sizeof(FireWeaponUpdate) == 0x2C) ? 1 : -1];

#endif
