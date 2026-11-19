// MapCache.h: the cache of every map the game has seen. SIZE 0x24.
//
// Target facts (retail RVAs): no vptr. The class is itself the map from the map's
// file name to its MapMetaData (the red-black tree at +0x00: findMap 0x003024BC
// lowercases the name, searches it and returns the value at node+0x14), followed by
// the set of maps seen on this scan, a map from file name to a Bool (+0x0C:
// clearUnseenMaps 0x0030469E reads its header and the one-byte value at node+0x14)
// and a set of allowed names (+0x18). The constructor 0x0022E28B calls the three
// member constructors (0x0022E272, 0x0033C432, 0x000D3A71) and the destructor
// 0x0022E22E the three destructors in reverse; GameEngine::init allocates 0x24 bytes
// and stores TheMapCache (VA 0x00DFF12C). updateCache 0x00305749 runs loadUserMaps
// (0x0030529C), writeCacheINI(true) (0x00303409) when it succeeded and loadStandardMaps
// (0x003006E1).
#pragma once
#ifndef BFME2_MAPCACHE_H
#define BFME2_MAPCACHE_H

#include "Common/GameType.h"
#include "Common/AsciiString.h"
#include "GameClient/MapMetaData.h"

#if defined(_STLPORT_VERSION)
struct FileInfo;

class MapCache : public _STL::map<AsciiString, MapMetaData>
{
public:
	MapCache();																// 0x0022E28B
	~MapCache();															// 0x0022E22E

	void updateCache();														// 0x00305749
	AsciiString getMapExtension() const;									// 0x003004A5
	AsciiString getMapDir() const;										// 0x00300489
	AsciiString getUserMapDir();											// 0x00300D7A (the map spells it rva00300D7A@Rva00300D7A; role-inferred name)
	const MapMetaData *findMap(AsciiString mapName);						// 0x003024BC
	void loadStandardMaps();												// 0x003006E1
	bool loadUserMaps();													// 0x0030529C
	bool addMap(AsciiString dirName, AsciiString fname, FileInfo *fileInfo, bool isOfficial);	// 0x00304E64

private:
	bool clearUnseenMaps(AsciiString dirName);								// 0x0030469E
	void writeCacheINI(bool userDir);										// 0x00303409

	_STL::map<AsciiString, bool> m_seen;									// +0x0C
	_STL::set<AsciiString> m_allowedMaps;									// +0x18
};
typedef char MapCacheSizeCheck[sizeof(MapCache) == 0x24 ? 1 : -1];

extern MapCache *TheMapCache;												// VA 0x00DFF12C
#endif

#endif
