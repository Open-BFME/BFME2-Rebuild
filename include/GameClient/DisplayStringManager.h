// DisplayStringManager.h: owns every DisplayString the UI allocates (TheDisplayStringManager),
// keeps them on a doubly linked list, and hands out the numeral and formation-letter strings
// the selection group icons use. W3DDisplayStringManager (GameClient/W3DDisplayStringManager.h)
// is the one implementation.
//
// Evidence (retail game.dat): the constructor 0x00262002 runs the SubsystemInterface
// constructor, clears +0x0C and +0x10 and stores table 0x007F9010 (18 slots). In that table
// slots 1, 3, 9 and 10 (init, postProcessLoad, reset, update) point at the folded empty body
// 0x000B3FD0 and slots 14..17 at the folded _purecall 0x0003B810: the four new slots are pure.
// link 0x00262027 pushes a string on the front of the list at +0x0C (its +0x0C is next, +0x10
// is prev) and unLink 0x00262045 removes one. +0x10 is the cursor W3DDisplayStringManager::update
// resumes from (it substitutes +0x0C when it is null). Size 0x14 (the derived class's own members
// start there). Names are Zero Hour's; slot 17's getter returns the formation letter string that
// W3DDisplayStringManager::postProcessLoad 0x000904CA builds.
#pragma once
#ifndef BFME2_DISPLAYSTRINGMANAGER_H
#define BFME2_DISPLAYSTRINGMANAGER_H

#include "Common/SubsystemInterface.h"

class DisplayString;

class DisplayStringManager : public SubsystemInterface
{
public:
	DisplayStringManager();															// 0x00262002
	virtual ~DisplayStringManager();												// 0x0026201C

	virtual void init() {}															// slot 1
	virtual void reset() {}															// slot 9
	virtual void update() {}														// slot 10

	virtual DisplayString *newDisplayString() = 0;									// slot 14
	virtual void freeDisplayString(DisplayString *string) = 0;						// slot 15
	virtual DisplayString *getGroupNumeralString(int numeral) = 0;					// slot 16
	virtual DisplayString *getFormationLetterDisplayString() = 0;					// slot 17

protected:
	void link(DisplayString *string);												// 0x00262027
	void unLink(DisplayString *string);												// 0x00262045

	DisplayString *m_stringList;													// +0x0C
	DisplayString *m_currentCheckpoint;												// +0x10
};
// SIZE 0x14

// The global string manager (VA 0x00DFEAD8).
extern DisplayStringManager *TheDisplayStringManager;

#endif
