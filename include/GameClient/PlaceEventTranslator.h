// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// PlaceEventTranslator.h: the message-stream translator that turns raw mouse input into the
// "place a building" messages. Class and virtual names: Zero Hour's PlaceEventTranslator.h
// (GPL-3.0-or-later, see NOTICE); its single member m_frameOfUpButton is one of the fields below.
// Target facts: constructor 0x0043216D (SIZE 0x18) stores vtable 0x00C3C9C0 (slot 0 translateGameMessage
// 0x0043218A, slot 1 the deleting destructor 0x004296EE, shared with other translators), zeroes the
// words at +4/+8/+0xC/+0x10 and the bytes at +0x14/+0x15. The member names are role-inferred.
#pragma once
#ifndef BFME2_PLACEEVENTTRANSLATOR_H
#define BFME2_PLACEEVENTTRANSLATOR_H

#include "Common/GameMessageTranslator.h"
#include "Common/ICoord2D.h"

class PlaceEventTranslator : public GameMessageTranslator
{
public:
	PlaceEventTranslator();																// 0x0043216D
	virtual GameMessageDisposition translateGameMessage(const GameMessage *msg);			// 0x0043218A

	ICoord2D m_downPos;		// +0x04, x and y
	int m_downFrame;		// +0x0C
	int m_upFrame;			// +0x10
	bool m_isDown;			// +0x14
	bool m_isDragging;		// +0x15
};
typedef char PlaceEventTranslatorSize[(sizeof(PlaceEventTranslator) == 0x18) ? 1 : -1];

#endif
