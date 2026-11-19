// MetaMap.h: the game-message meta-type table that MetaEvent.cpp fills from INI (SubsystemInterface
// subclass). Target facts: the constructor 0x001DB3E8 runs SubsystemInterface's constructor, stores
// the MetaMap vtable at +0 and clears the word at +0xC, the first word after the base; that word is
// the head of the record list (getMetaMapRec 0x001DB537 walks it through each record's link at +0).
// Its vtable (VA 0x00BDBA7C) inherits the base's slots; init, reset and update are the overrides this
// class declares. Record fields beyond the ones the list code touches are not yet recovered.
#pragma once
#ifndef BFME2_METAMAP_H
#define BFME2_METAMAP_H

#include "Common/SubsystemInterface.h"
#include "Common/MessageStream.h"
#include "Common/UnicodeString.h"

class INI;

// The modifier values a key message is reduced to (HotKeyManager::translate 0x00359556 and
// MetaEventTranslator 0x001DB0B5 build them from the Keyboard.h state masks). Names: Zero Hour's MetaEvent.h.
enum MappableKeyModState { NONE = 0, CTRL = 0x04, SHIFT = 0x10, ALT = 0x40 };

// One row of the built-in meta-type name table: a message's INI name and its type.
struct MetaTypeEntry
{
	const char *name;
	GameMessage::Type type;
};

// One meta-message record: a link, the message type it describes, and the record's
// strings. Size 0x24 (the record allocation in getMetaMapRec).
class MetaMapRec
{
public:
	~MetaMapRec();							// 0x001DB48E, out of line
	MetaMapRec *m_next;						// +0x00 next record in the owner's list
	GameMessage::Type m_type;				// +0x04
	int m_unk0x08;							// +0x08
	int m_unk0x0C;							// +0x0C
	int m_unk0x10;							// +0x10
	int m_unk0x14;							// +0x14
	int m_unk0x18;							// +0x18 set to 6 by the list builder
	UnicodeString m_unk0x1C;				// +0x1C
	UnicodeString m_unk0x20;				// +0x20
};

class MetaMap : public SubsystemInterface
{
public:
	MetaMap();
	virtual ~MetaMap();
	virtual void init();
	virtual void reset();
	virtual void update();
	static void parseMetaMap(INI *ini);	// 0x001DB5A2

protected:
	GameMessage::Type findGameMessageMetaType(const char *name);	// 0x001DB3FE
	MetaMapRec *getMetaMapRec(GameMessage::Type type);				// 0x001DB537

	MetaMapRec *m_recs;						// +0x0C head of the record list (cleared by the constructor)
};

extern MetaMap *TheMetaMap;				// retail global at VA 0x00DFDBF8 (ledger name not recovered)

#endif
