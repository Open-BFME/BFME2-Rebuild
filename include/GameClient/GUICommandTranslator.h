// GUICommandTranslator.h: the translator that turns GUI command-button clicks into game messages
// (GameClient/MessageStream/GUICommandTranslator.cpp). Target facts: the constructor 0x0043132D
// stores the class's own vtable (VA 0x00C3C968) at +0 and zeroes the members from +4 to +0x38 with
// no base constructor call, so the class has no base. Slot 0 is translateGameMessage (0x004314B1).
// The member types follow the widths of the constructor's stores; the names are not yet recovered.
#pragma once
#ifndef BFME2_GUICOMMANDTRANSLATOR_H
#define BFME2_GUICOMMANDTRANSLATOR_H

#include "Common/GameType.h"

class GameMessage;
enum GameMessageDisposition;

class GUICommandTranslator
{
public:
	GUICommandTranslator();							// 0x0043132D
	virtual GameMessageDisposition translateGameMessage(const GameMessage *message);	// slot 0, 0x004314B1
	virtual ~GUICommandTranslator();				// slot 4

	UnsignedInt m_unk0x04;							// +0x04
	UnsignedInt m_unk0x08;							// +0x08
	UnsignedInt m_unk0x0C;							// +0x0C
	UnsignedInt m_unk0x10;							// +0x10
	UnsignedByte m_unk0x14;							// +0x14
	UnsignedInt m_unk0x18;							// +0x18
	UnsignedInt m_unk0x1C;							// +0x1C
	UnsignedInt m_unk0x20;							// +0x20
	UnsignedInt m_unk0x24;							// +0x24
	UnsignedInt m_unk0x28;							// +0x28
	UnsignedInt m_unk0x2C;							// +0x2C
	UnsignedByte m_unk0x30;							// +0x30
	UnsignedInt m_unk0x34;							// +0x34
	UnsignedInt m_unk0x38;							// +0x38
};
// SIZE 0x3C
typedef char GUICommandTranslatorSize[sizeof(GUICommandTranslator) == 0x3C ? 1 : -1];

#endif
