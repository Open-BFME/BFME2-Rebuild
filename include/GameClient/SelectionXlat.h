// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// SelectionXlat.h: the message-stream translator that turns mouse messages into unit selection.
// Class, member and function names: Zero Hour's SelectionXlat.h (GPL-3.0-or-later, see NOTICE).
// Target facts: constructor 0x0042FD9E (SIZE 0x40) stores vtable 0x00C3C960 (slot 0 translateGameMessage
// 0x0043003C, slot 1 the deleting destructor 0x0042FE1F over 0x0042FCE6), -1 at +0xC, zero bytes at
// +4..+7, words at +8/+0x10..+0x1C/+0x24, builds the map at +0x28 with 0x0033C432 and clears it, zeroes
// the three floats at +0x34..+0x3C, the byte at +0x20, and stores itself in TheSelectionTranslator
// (VA 0x00E03220). The destructor 0x0042FCE6 runs the tree destructor on +0x28 and restores the base
// table. +6 and +7 are two more zeroed bytes with no member in Zero Hour's class.
#pragma once
#ifndef BFME2_SELECTIONXLAT_H
#define BFME2_SELECTIONXLAT_H

#include "Common/GameMessageTranslator.h"
#include "Common/ICoord2D.h"
#include "Common/Coord3D.h"
#include "Common/StlMap.h"

class Drawable;
class GameMessage;
class ThingTemplate;

typedef _STL::map<const ThingTemplate *, int> SelectCountMap;

class SelectionTranslator : public GameMessageTranslator
{
public:
	SelectionTranslator();																// 0x0042FD9E
	virtual GameMessageDisposition translateGameMessage(const GameMessage *msg);		// 0x0043003C
	virtual ~SelectionTranslator();														// 0x0042FE1F (deleting), 0x0042FCE6

	bool selectFriends(Drawable *draw, GameMessage *createTeamMsg, bool dragSelecting);	// 0x0042FE7F

	bool m_leftMouseButtonIsDown;								// +0x04
	bool m_dragSelecting;										// +0x05
	bool m_unk0x06;												// +0x06 (no Zero Hour member)
	bool m_unk0x07;												// +0x07 (no Zero Hour member)
	unsigned int m_lastGroupSelTime;							// +0x08
	int m_lastGroupSelGroup;									// +0x0C
	ICoord2D m_selectFeedbackAnchor;							// +0x10
	ICoord2D m_deselectFeedbackAnchor;							// +0x18
	bool m_displayedMaxWarning;									// +0x20
	unsigned int m_lastClick;									// +0x24
	SelectCountMap m_selectCountMap;							// +0x28
	Coord3D m_deselectDownCameraPosition;						// +0x34
};
typedef char SelectionTranslatorSize[(sizeof(SelectionTranslator) == 0x40) ? 1 : -1];

// Zero Hour's callback record: passed as userData to the drawable iteration; not part of the translator.
struct SFWRec
{
	SelectionTranslator *translator;
	GameMessage *createTeamMsg;
	bool dragSelecting;
};

extern SelectionTranslator *TheSelectionTranslator;				// VA 0x00E03220

#endif
