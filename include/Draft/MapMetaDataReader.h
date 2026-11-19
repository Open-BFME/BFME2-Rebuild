// DRAFT (h05 fpgen, renamed by h09): the INI reader record for one MapCache.ini entry (typed table VA 0x00C68AE8, which the ledger spells "MapCache"; it is NOT MapCache, whose size is 0x24: this record is 0xA8 bytes of parsed fields that MapCache::addMap copies into a MapMetaData).
// Offsets are compile-checked below; no function matches against it yet, so it stays
// in Draft until one does. Types are what each entry's parse function stores; names are
// the INI tokens.
// SIZE not established: members reach 0xA8; bases and vptr, if any, are the leading gap.
// Unresolved members: m_extentMin (+0x0, INI::parseCoord3D).
#pragma once
#ifndef BFME2_MAP_META_DATA_READER_H
#define BFME2_MAP_META_DATA_READER_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/Coord3D.h"
#include "Common/AsciiString.h"
#include "Common/UnicodeString.h"
#include <list>
#include "GameClient/MapMetaData.h"


class MapMetaDataReader
{
public:

	UnsignedByte m_extentMin[0xC];                         // +0x0 extentMin (also supplyPosition, techPosition, PlayerPosition); unresolved: INI::parseCoord3D, parsePlayerPosition, parseSupplyPositionCoord3D, parseTechPositionCoord3D
	Coord3D m_extentMax;                                   // +0xC extentMax
	Int m_numPlayers;                                      // +0x18 numPlayers
	Bool m_isMultiplayer;                                  // +0x1C isMultiplayer
	Bool m_isScenarioMP;                                   // +0x1D isScenarioMP
	AsciiString m_displayName;                             // +0x20 displayName
	AsciiString m_description;                             // +0x24 description
	Bool m_isOfficial;                                     // +0x28 isOfficial
	Int m_timestampLo;                                     // +0x2C timestampLo
	Int m_timestampHi;                                     // +0x30 timestampHi
	UnsignedInt m_fileSize;                                // +0x34 fileSize
	UnsignedInt m_fileCRC;                                 // +0x38 fileCRC
	Coord3D m_player1Start;                                // +0x3C Player_1_Start
	Coord3D m_player2Start;                                // +0x48 Player_2_Start
	Coord3D m_player3Start;                                // +0x54 Player_3_Start
	Coord3D m_player4Start;                                // +0x60 Player_4_Start
	Coord3D m_player5Start;                                // +0x6C Player_5_Start
	Coord3D m_player6Start;                                // +0x78 Player_6_Start
	Coord3D m_player7Start;                                // +0x84 Player_7_Start
	Coord3D m_player8Start;                                // +0x90 Player_8_Start
	Coord3D m_initialCameraPosition;                       // +0x9C InitialCameraPosition
	// The two coordinate lists the supply and tech parsers push onto (0x00534BAF +0xA8, 0x00534BDC +0xAC; push_front of each parsed Coord3D).
	std::list<Coord3D> m_supplyPositions;                  // +0xA8 supplyPosition
	std::list<Coord3D> m_techPositions;                    // +0xAC techPosition
	// Eight 0x14-byte PlayerPosition records from +0xB0: parsePlayerPosition 0x00535087 reads record (index - 1).
	PlayerPosition m_playerPositions[8];                   // +0xB0 playerPosition
};
typedef char MapMetaDataReader_off_m_extentMin[(offsetof(MapMetaDataReader, m_extentMin) == 0x0) ? 1 : -1];
typedef char MapMetaDataReader_off_m_extentMax[(offsetof(MapMetaDataReader, m_extentMax) == 0xC) ? 1 : -1];
typedef char MapMetaDataReader_off_m_numPlayers[(offsetof(MapMetaDataReader, m_numPlayers) == 0x18) ? 1 : -1];
typedef char MapMetaDataReader_off_m_isMultiplayer[(offsetof(MapMetaDataReader, m_isMultiplayer) == 0x1C) ? 1 : -1];
typedef char MapMetaDataReader_off_m_isScenarioMP[(offsetof(MapMetaDataReader, m_isScenarioMP) == 0x1D) ? 1 : -1];
typedef char MapMetaDataReader_off_m_displayName[(offsetof(MapMetaDataReader, m_displayName) == 0x20) ? 1 : -1];
typedef char MapMetaDataReader_off_m_description[(offsetof(MapMetaDataReader, m_description) == 0x24) ? 1 : -1];
typedef char MapMetaDataReader_off_m_isOfficial[(offsetof(MapMetaDataReader, m_isOfficial) == 0x28) ? 1 : -1];
typedef char MapMetaDataReader_off_m_timestampLo[(offsetof(MapMetaDataReader, m_timestampLo) == 0x2C) ? 1 : -1];
typedef char MapMetaDataReader_off_m_timestampHi[(offsetof(MapMetaDataReader, m_timestampHi) == 0x30) ? 1 : -1];
typedef char MapMetaDataReader_off_m_fileSize[(offsetof(MapMetaDataReader, m_fileSize) == 0x34) ? 1 : -1];
typedef char MapMetaDataReader_off_m_fileCRC[(offsetof(MapMetaDataReader, m_fileCRC) == 0x38) ? 1 : -1];
typedef char MapMetaDataReader_off_m_player1Start[(offsetof(MapMetaDataReader, m_player1Start) == 0x3C) ? 1 : -1];
typedef char MapMetaDataReader_off_m_player2Start[(offsetof(MapMetaDataReader, m_player2Start) == 0x48) ? 1 : -1];
typedef char MapMetaDataReader_off_m_player3Start[(offsetof(MapMetaDataReader, m_player3Start) == 0x54) ? 1 : -1];
typedef char MapMetaDataReader_off_m_player4Start[(offsetof(MapMetaDataReader, m_player4Start) == 0x60) ? 1 : -1];
typedef char MapMetaDataReader_off_m_player5Start[(offsetof(MapMetaDataReader, m_player5Start) == 0x6C) ? 1 : -1];
typedef char MapMetaDataReader_off_m_player6Start[(offsetof(MapMetaDataReader, m_player6Start) == 0x78) ? 1 : -1];
typedef char MapMetaDataReader_off_m_player7Start[(offsetof(MapMetaDataReader, m_player7Start) == 0x84) ? 1 : -1];
typedef char MapMetaDataReader_off_m_player8Start[(offsetof(MapMetaDataReader, m_player8Start) == 0x90) ? 1 : -1];
typedef char MapMetaDataReader_off_m_initialCameraPosition[(offsetof(MapMetaDataReader, m_initialCameraPosition) == 0x9C) ? 1 : -1];
typedef char MapMetaDataReader_off_m_supplyPositions[(offsetof(MapMetaDataReader, m_supplyPositions) == 0xA8) ? 1 : -1];
typedef char MapMetaDataReader_off_m_techPositions[(offsetof(MapMetaDataReader, m_techPositions) == 0xAC) ? 1 : -1];

typedef char MapMetaDataReader_off_m_playerPositions[(offsetof(MapMetaDataReader, m_playerPositions) == 0xB0) ? 1 : -1];

#endif
