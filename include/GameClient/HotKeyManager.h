// HotKeyManager.h: the table of named hot keys and the translator that feeds it
// key messages. HotKeyManager SIZE 0x30 (SubsystemInterface base 0xC + three maps).
//
// Target facts (retail RVAs; table addresses are VAs). The constructor 0x0035973D
// runs the SubsystemInterface constructor, stores the 14-slot table VA 0x00C15398
// (slot 0 deleting destructor 0x0035978C -> destructor 0x00359290, slot 1 0x003592E6,
// slot 2 SubsystemInterface's 0x001B5384, slot 9 0x003592FA, slot 10 the folded empty
// body) and builds three red-black maps at +0x0C, +0x18 and +0x24 with the
// out-of-line map constructor 0x0033C432. Slot 1 clears the +0x0C and +0x18 maps
// (0x00358F7C); slot 9 clears the +0x0C map only.
//
// The maps hold handles to one referent class, HotKeyAction (below): addHotKey
// 0x00359302 files a handle into the +0x18 map when its Bool argument is true and
// the +0x0C map otherwise, executeHotKey 0x003593E8 looks the lower-cased name up in
// +0x0C then +0x18, and when neither action takes effect the upper-cased name in
// the same order, calling the referent's slots 1, 2 and 3; the +0x24 map is keyed by message type
// (addMessageAction 0x003597A8 assigns through 0x002174A4, removeMessageAction
// 0x003593D8 erases by key; translate 0x00359556 looks the message type up and calls
// the same three slots). The handle is AptRef (Apt/AptRef.h): the pair copy
// 0x00358B43 increments the referent's count at +4 and the old repo's placeholder
// TreeHintRef00217D4C is this handle.
//
// HotKeyTranslator is the message-stream translator that forwards to the manager:
// translateGameMessage 0x00359729 is `TheHotKeyManager ? translate(msg) : 0` as a
// tail jump (global VA 0x00E01E28).
#pragma once
#ifndef BFME2_HOTKEYMANAGER_H
#define BFME2_HOTKEYMANAGER_H

#include "Common/StlMap.h"
#include "Common/SubsystemInterface.h"
#include "Common/AsciiString.h"
#include "Common/GameMessageTranslator.h"
#include "Apt/AptRef.h"

class GameMessage;
class UnicodeString;

// The object a hot key runs. Its virtual order is retail's (0x00C6CEA4 is one
// implementation, InGameToggleStanceCommandButton's): slot 0 the deleting destructor
// (AptRefTarget's), then three Bool(Bool shift) members the manager tries in order.
// The names are role-inferred from executeHotKey's call order and are not evidenced.
class HotKeyAction : public AptRefTarget
{
public:
	virtual bool tryExecute(bool shift) = 0;				// slot 1 (+0x04): tried first
	virtual bool tryExecuteAlternate(bool shift) = 0;		// slot 2 (+0x08): tried when slot 1 fails
	virtual bool afterExecute(bool shift) = 0;				// slot 3 (+0x0C): run once a key took effect; true plays the feedback
};

typedef _STL::map<AsciiString, AptRef<HotKeyAction> > HotKeyMap;					// +0x0C and +0x18
typedef _STL::map<int, AptRef<HotKeyAction> > HotKeyMessageActionMap;				// +0x24

class HotKeyManager : public SubsystemInterface
{
public:
	HotKeyManager();															// 0x0035973D
	virtual ~HotKeyManager();													// 0x00359290
	virtual void init();														// slot 1: 0x003592E6
	virtual void reset();														// slot 9: 0x003592FA
	virtual void update() {}													// slot 10: folded empty body

	bool executeHotKey(const AsciiString &key, bool shift);						// 0x003593E8
	AsciiString searchHotKey(const AsciiString &name);							// 0x00358CCD
	AsciiString searchHotKey(const UnicodeString &name);						// 0x00358B9E
	GameMessageDisposition translate(const GameMessage *message);				// 0x00359556
	void rva00358A53();															// 0x00358A53: feedback when a key took effect and afterExecute asked for it (role-inferred; map name rva00358A53@HotKeyManager)
	void rva00358ACB();															// 0x00358ACB: feedback when a key failed (role-inferred)
	unsigned int removeMessageAction(int messageType);							// 0x003593D8
	void addMessageAction(const AptRef<HotKeyAction> &action, int messageType);	// 0x003597A8

protected:
	HotKeyMap m_hotKeys;														// +0x0C
	HotKeyMap m_secondaryHotKeys;												// +0x18
	HotKeyMessageActionMap m_messageActions;									// +0x24
};
typedef char HotKeyManagerSizeCheck[sizeof(HotKeyManager) == 0x30 ? 1 : -1];

extern HotKeyManager *TheHotKeyManager;											// VA 0x00E01E28

class HotKeyTranslator : public GameMessageTranslator
{
public:
	virtual GameMessageDisposition translateGameMessage(const GameMessage *message);	// 0x00359729
};

#endif
