// NameKeyGenerator.h: SAGE name <-> key interning subsystem (Zero Hour's
// Common/NameKeyGenerator.h; BFME 2 builds it from
// Code\Libraries\Source\NameKey\NameKey.cpp, the path WorldBuilder's asserts carry:
// "NameKeyGenerator::init", "NameKeyGen already inited", "StaticNameKey::key").
//
// Target facts (retail game.dat):
// - ctor 0x001491AC: SubsystemInterface (0xC bytes), vftable 0x007D3568, the
//   key -> Bucket hash_map at +0x2BF4C (its ctor 0x0014918D), the CriticalSection
//   at +0x2BF60 (CriticalSection(0) 0x000411C1), m_nextID +0x2BF48 = 0, the
//   45007 sockets at +0x0C cleared (rep stos 0xAFCF dwords), then the section is
//   left (inline exit). SIZE 0x2BF84.
// - dtor 0x00148BC2 frees the sockets under the lock, then destroys the
//   CriticalSection, the hash_map and the base, in that order (member order).
// - vftable 0x007D3568, 14 slots: 0 deleting dtor 0x00148DC3, 1 init and 9 reset
//   (one ICF-folded body 0x00148C41: lock, freeSockets, m_nextID = 1), 10 update
//   (the folded bare ret 0x000B3FD0); the rest are SubsystemInterface's.
// - nameToKey(const char *) 0x00148E1A and keyToName 0x00148C95 take a
//   ScopedCriticalSection on the lock; nameToKey hashes with the out-of-line
//   calcHashForString 0x00148A98 and adds each new (key, bucket) pair to the
//   hash_map (insert 0x004DA240); keyToName looks the key up there (_M_find
//   0x002888D4) instead of Zero Hour's linear walk, and returns a reference.
// - Bucket: vftable 0x007D3564 (one slot, deleting dtor 0x00148AFD), inline ctor
//   0x00148AB0 (next, key, string cleared), inline dtor 0x00148B19. Not pooled:
//   freeSockets 0x00148B42 deletes with the global operator.
#pragma once
#ifndef BFME2_NAMEKEYGENERATOR_H
#define BFME2_NAMEKEYGENERATOR_H

#include <hash_map>
#include "Common/GameType.h"
#include "Common/AsciiString.h"
#include "Common/SubsystemInterface.h"
#include "Common/STLTypedefs.h"
#include "sys_util/Synchronization.h"

class INI;

class Bucket
{
public:
	Bucket() : m_nextInSocket(NULL), m_key(NAMEKEY_INVALID) {}	// 0x00148AB0
	virtual ~Bucket() {}										// 0x00148B19

	Bucket *m_nextInSocket;										// +0x04
	NameKeyType m_key;											// +0x08
	AsciiString m_nameString;									// +0x0C
};
// SIZE 0x10

class NameKeyGenerator : public SubsystemInterface
{
public:
	NameKeyGenerator();											// 0x001491AC
	virtual ~NameKeyGenerator();								// 0x00148BC2

	virtual void init();										// slot 1
	virtual void reset();										// slot 9: 0x00148C41
	virtual void update() {}									// slot 10

	NameKeyType nameToKey(const char *name);					// 0x00148E1A
	NameKeyType nameToKey(const AsciiString &name);				// 0x0009FA65 (out of line in retail)
	NameKeyType nameToLowercaseKey(const char *name);			// 0x00148F02 (the map's ledger name sits on 0x00620500, which is another class's lookup table)
	NameKeyType nameToLowercaseKey(const AsciiString &name);	// 0x002D91AF (out of line in Image.cpp's range): forwards name.str() to the text form; the image collection uses it
	const AsciiString &keyToName(NameKeyType key);				// 0x00148C95
	static void parseStringAsNameKeyType(INI *ini, void *instance, void *store, const void *userData);	// 0x00148FBE

private:
	enum
	{
		SOCKET_COUNT = 45007									// 0xAFCF: the ctor's rep stos count and nameToKey's divisor
	};
	// Keyed by int with STLport's own hash: the map's ledger names of this unit's COMDATs (hash_map ctor
	// 0x0014918D hash_map<int, ..., hash<int>, equal_to<int>>, dtor 0x00148B84 hashtable<pair<const int, ...>,
	// int, hash<int>, ...>), and keyToName 0x00148C95 converts its NameKeyType argument to an int temporary.
	typedef _STL::hash_map<Int, Bucket *, _STL::hash<Int>, _STL::equal_to<Int> > KeyToBucketMap;

	void freeSockets();											// 0x00148B42

	Bucket *m_sockets[SOCKET_COUNT];							// +0x0C
	UnsignedInt m_nextID;										// +0x2BF48
	KeyToBucketMap m_keyToBucket;								// +0x2BF4C
	CriticalSection m_lock;										// +0x2BF60
};
// SIZE 0x2BF84

extern NameKeyGenerator *TheNameKeyGenerator;

// Zero Hour's KEYNAME, out of line in retail (0x00148DB3) and returning the
// generator's reference.
const AsciiString &KEYNAME(NameKeyType key);
#include "Common/StaticNameKey.h"	// StaticNameKey and the TheKey_ globals (Player 0x002A9ACC reads TheKey_playerColor / TheKey_playerNightColor)

#endif
