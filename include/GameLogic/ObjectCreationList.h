// ObjectCreationList.h: an INI-defined list of creation nuggets (ObjectCreationList), the
// nugget base (ObjectCreationNugget). The store is in GameLogic/ObjectCreationListStore.h.
//
// ObjectCreationNugget, target facts (retail game.dat):
// - Table VA 0x00BE09D0, 6 slots: 0 dtor (??_G 0x001F010B; the dtor 0x001F0409 only restores
//   the table, so it is inline and empty), 1 0x001F0163 forwards its first three and fifth
//   arguments to slot 3 and drops the fourth, 2 0x001F0132 turns two objects into their
//   positions (+0x38) and calls slot 3, 3 the coordinate create (no body in the base), 4 an
//   empty two-argument body (fold 0x0050B238, `ret 8`), 5 `return true` with two arguments
//   (fold 0x0050BD7C).
// - Slot 4 is GetAssetList (GenericObjectCreationNugget's override 0x001F131F carries the
//   name); slot 5 is canCreate (0x001F1010). ObjectCreationList 0x001F0410 and 0x001F0434
//   dispatch them over the list.
// - Same-name overloads take reverse declaration order: the three creates are declared
//   slot 3, 2, 1 below. Names and argument roles: the donor's (Zero Hour) overload set,
//   which has the same three shapes.
// - FireWeaponNugget allocates 8 bytes and ApplyRandomForceNugget 0x14, so the base is the
//   table pointer alone.
//
// ObjectCreationList, target facts:
// - ctor COMDAT 0x001F073A: STLport vector at +0x00, byte +0x0C = 0, AsciiString +0x10 copied
//   from the argument. dtor COMDAT 0x001F077D releases +0x10, then frees the vector buffer.
//   ObjectCreationListStore::parseObjectCreationListDefinition news 0x14 bytes.
// - The three creates (0x001F081A, 0x001F0878, 0x001F08D3) first follow +0x0C: while it is
//   set they look the list up again by its name (+0x10) through the store, then call
//   nugget slots 1, 3 and 2 in order. The parser sets +0x0C on a list that a reload replaced
//   and keeps that list in the store's retired vector.
// - The map spells the creates with void * arguments (old ledger names); the three- and
//   four-argument declarations below keep that spelling so the rows compile as the map's
//   symbols.
#pragma once
#ifndef BFME2_OBJECTCREATIONLIST_H
#define BFME2_OBJECTCREATIONLIST_H

#include <stddef.h>
#include <vector>
#include "Common/GameType.h"
#include "Common/AsciiString.h"

class Object;
class AssetList;
class Coord3D;

class ObjectCreationNugget
{
public:
	ObjectCreationNugget() {}
	virtual ~ObjectCreationNugget() {}											// 0  (dtor 0x001F0409)
	// slot 3: no body in the base
	virtual void create(const Object *primaryObj, const Coord3D *primary, const Coord3D *secondary,
		UnsignedInt lifetimeFrames) const = 0;
	// slot 2, 0x001F0132
	virtual void create(const Object *primaryObj, const Object *secondaryObj, UnsignedInt lifetimeFrames) const;
	// slot 1, 0x001F0163
	virtual void create(const Object *primaryObj, const Coord3D *primary, const Coord3D *secondary,
		Bool createOwner, UnsignedInt lifetimeFrames) const;
	virtual void GetAssetList(AssetList &assets, void *context) const = 0;	// 4: retail's base table (stored first by every nugget constructor) has __purecall here; the derived nuggets override it (Generic 0x001F131F, FireWeapon 0x001F01B9, Attack and ApplyRandomForce the empty fold 0x0050B238)
	virtual Bool canCreate(Object *obj, const Coord3D *pos) { return true; }	// 5  (fold 0x0050BD7C)
};
typedef char ObjectCreationNugget_size[(sizeof(ObjectCreationNugget) == 0x4) ? 1 : -1];

class ObjectCreationList
{
public:
	ObjectCreationList(const AsciiString &name) : m_isRetired(false), m_name(name) {}	// 0x001F073A

	// 0x001F081A, nugget slot 1. The map's void * spelling can't pass the Bool through
	// unconverted, so this one carries the real argument types.
	void create(const Object *primaryObj, const Coord3D *primary, const Coord3D *secondary,
		Bool createOwner, UnsignedInt lifetimeFrames);
	void create(void *, void *, void *, int);				// 0x001F0878, nugget slot 3
	void create(void *, void *, void *);					// 0x001F08D3, nugget slot 2
	// Typed four-argument form kept for includers (BoneFXUpdate); the donor's static
	// wrapper tests the list first.
	void create(Object *primaryObj, const Coord3D *primary, const Coord3D *secondary, int lifetimeFrames);
	static void create(const ObjectCreationList *ocl, Object *primaryObj, const Coord3D *primary, const Coord3D *secondary)
	{
		if (ocl)
			((ObjectCreationList *)ocl)->create(primaryObj, primary, secondary, 0);
	}

	void rva001F0410(AssetList &assets, void *context) const;	// 0x001F0410, slot 4 over the list (GetAssetList)
	Bool rva001F0434(Object *obj, const Coord3D *pos) const;	// 0x001F0434, slot 5 over the list (canCreate)
	void addObjectCreationNugget(ObjectCreationNugget *nugget);	// 0x001F0BDA

	const AsciiString &getName() const { return m_name; }

	std::vector<ObjectCreationNugget *> m_nuggets;			// 0x00
	Bool m_isRetired;										// 0x0C set when a reload replaced this list
	AsciiString m_name;										// 0x10
};
typedef char ObjectCreationList_size[(sizeof(ObjectCreationList) == 0x14) ? 1 : -1];
typedef char ObjectCreationList_off_m_isRetired[(offsetof(ObjectCreationList, m_isRetired) == 0x0C) ? 1 : -1];
typedef char ObjectCreationList_off_m_name[(offsetof(ObjectCreationList, m_name) == 0x10) ? 1 : -1];

#endif
