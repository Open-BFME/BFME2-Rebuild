// ObjectTypes.h: a named list of object type names (Zero Hour's ObjectTypes).
// Target facts (code RVAs, VA 0x00C18630 table):
//  - Snapshot-derived, SIZE 0x14: ScriptEngine::getObjectTypes (0x003582A3)
//    and ObjectTypesTemp's ctor 0x003BA7FF allocate 0x14 and call the
//    constructor 0x003769F9 (0x00376A1F for the named form). The constructors
//    store the four-slot table 0x00C18630 at +0, build the name string at +4
//    (a StringBase copy from the argument) and the vector's three words at +8
//    (_Vector_base ctor 0x00211E58).
//  - Slots: 0 deleting dtor 0x00376AC3 (-> 0x00376ADF: vector dtor, string
//    release, then Snapshot's table back), 1 the shared empty body 0x000B3FD0
//    (loadPostProcess), 2 the const name getter 0x00376A19 (the string at VA
//    0x00C18640, "ObjectTypes": GetSnapshotName), 3 xfer 0x00376B70 (the map
//    spells it protected, `M`).
//  - Elements are AsciiStrings: getNthInList 0x002041AC reads begin +8 / end
//    +0xC with a 4-byte element and returns the string by value (a shared empty
//    string at VA 0x00DE0878 past the end), isInSet 0x00376A62 is a find over
//    [+8, +0xC), addObjectType 0x00376B50 is isInSet then push_back, remove
//    0x00376B1E is isInSet then erase of the found element, operator= 0x00376A9C
//    sets the string and assigns the vector.
//  - Zero Hour's ObjectTypes.h supplies m_listName / m_objectTypes and the
//    method names; retail has no crc slot.
#pragma once
#ifndef BFME2_OBJECTTYPES_H
#define BFME2_OBJECTTYPES_H

#include "Common/GameType.h"
#include "Common/AsciiString.h"
#include "Common/Snapshot.h"
#include "Common/Vector.h"

class Player;
class ThingTemplate;
class Xfer;

class ObjectTypes : public Snapshot
{
public:
	ObjectTypes();						// 0x003769F9
	ObjectTypes(const AsciiString &listName);		// 0x00376A1F
	// No user-declared destructor: retail's 0x00376ADF (deleting 0x00376AC3) is the implicit one
	// (vector, then the name, then only Snapshot's table stored back).
	ObjectTypes &operator=(const ObjectTypes &that);	// 0x00376A9C

	void addObjectType(const AsciiString &objectType);	// 0x00376B50
	void remove(const AsciiString &objectType);		// 0x00376B1E
	Bool isInSet(const AsciiString &objectType) const;	// 0x00376A62
	Bool isInSet(const ThingTemplate *objectType) const;	// 0x00376A84 (null is false; tests the template's name at +0x64)
	Bool canBuildAny(Player *player);			// 0x00376988
	// 0x005C4AD1 (map: Rva005C4AD1LeaField): the list name, out of line in retail.
	const AsciiString &getListName() const;
	UnsignedInt getListSize() const { return m_objectTypes.size(); }
	AsciiString getNthInList(UnsignedInt index) const;	// 0x002041AC
	// 0x00376C50 (Zero Hour's name and form): every listed name ThingFactory::findTemplate
	// 0x002D06CA finds is pushed onto templates, then counts is resized to their number, which
	// it returns (ScriptConditions 0x003E977E, 0x003E9C3B, 0x003E9EB0 call it).
	Int prepForPlayerCounting(_STL::vector<const ThingTemplate *> &templates, _STL::vector<Int> &counts);

protected:
	virtual void loadPostProcess() {}			// slot 1, the shared empty body 0x000B3FD0
public:	// the map spells slot 2 as a public const getter
	virtual const char *GetSnapshotName() const;		// slot 2, 0x00376A19
protected:
	virtual void xfer(Xfer *xfer);				// slot 3, 0x00376B70

private:
	AsciiString m_listName;					// +0x04
	_STL::vector<AsciiString> m_objectTypes;		// +0x08
};
// SIZE 0x14

#endif
