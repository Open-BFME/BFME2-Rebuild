// ThreatFinder.h: the per-module threat table the threat-finder update owns.
// Target facts (old repo ThreatFinderUpdate.cpp, ThreatFinderGetThreatForPlayer.cpp):
// twenty 0x44-byte ThreatInfo records from +0, a pad dword, the search position at
// +0x554, the radius at +0x560, the frame the table was last built at +0x564 and
// the exclude-structures flag at +0x568. calculateThreatLevel 0x003ECFBC refills
// the records and getThreatForPlayer 0x003ED0C4 merges the ones whose owner is an
// ally or enemy of the searching player.
#pragma once
#ifndef BFME2_THREATFINDER_H
#define BFME2_THREATFINDER_H

#include "Common/Coord3D.h"
#include "Common/GameType.h"
#include "GameLogic/Module/Rva003ECD60Object.h"

class Player;

class ThreatFinder
{
public:
	ThreatInfo getThreatForPlayer(Player *searchingPlayer, Int mode, Player *specificEnemy);
	void calculateThreatLevel();

	ThreatInfo m_playerThreat[20];		// 0x000
	char m_pad550[4];					// 0x550
	Coord3D m_position;					// 0x554
	Real m_radius;						// 0x560
	UnsignedInt m_lastFrame;			// 0x564
	Bool m_excludeStructures;			// 0x568
};
// SIZE 0x56C

// The record table's clear helper (retail 0x003ECB13): resets the twenty
// ThreatInfo records in place.
class Rva003ECB13Array
{
public:
	void clear();
};

// The relationship helpers the mode switch selects between (retail 0x0030...).
Int isAlly(const Player *a, const Player *b);
Int isEnemy(const Player *a, const Player *b);

#endif
