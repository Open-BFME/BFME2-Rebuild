// LuaScriptEngine.h: the script engine the game's Lua bindings run inside.
// Target facts (retail game.dat): TheLuaScriptEngine at 0x00E01DBC.
// Constructor 0x003379FD calls SubsystemInterface 0x001B4E63, installs table
// 0x00C0E390 at +0 and clears +0x0C/+0x10/+0x9C, byte +0xAC, dword +0xD4, byte
// +0xD8 and dword +0xDC; allocation 0x0022F558 asks for 0xE0 bytes, so the
// class ends at 0xE0. Destructor 0x00337AA8 restores the same table, closes the
// two lua states through lua_close 0x0074DF60 and calls the parent destructor.
// +0x14 holds seventeen eight-byte EventFlagEntry records (constructor
// 0x00332AEF, InitializeInternalEvents 0x00332B7A, ProcessInternalEvent
// 0x00332C7A), ending exactly at +0x9C. Four twelve-byte vector headers sit at
// +0xA0/+0xB0/+0xBC/+0xC8 (constructors through 0x00211E58).
// The callbacks reach the current-drawable context at +0x9C (LuaDrawableLink)
// and the two states at +0x0C/+0x10.
#pragma once
#ifndef BFME2_LUASCRIPTENGINE_H
#define BFME2_LUASCRIPTENGINE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/AsciiString.h"
#include "Common/SubsystemInterface.h"

class Drawable;
class Object;
class Xfer;
struct lua_State;

class DelayedLuaEventList
{
public:
	DelayedLuaEventList();
	~DelayedLuaEventList();

	unsigned char m_unk0x00[0x4C];				// 0x00
};
// SIZE 0x4C

// The current-drawable context LuaScriptEngine+0x9C exposes to the callbacks.
struct LuaDrawableLink
{
	AsciiString m_previousAnimationState;		// 0x00
	AsciiString m_transitionAnimState;			// 0x04
	AsciiString m_previousAnimation;			// 0x08
	Drawable *m_drawable;						// 0x0C
	float m_prevAnimFraction;					// 0x10
	unsigned char m_allowToContinue;			// 0x14
	unsigned char m_pad15[3];
};
// SIZE 0x18

// One of the seventeen internal-event records at LuaScriptEngine+0x14.
struct EventFlagEntry
{
	void setKeyFromName(const char *name);		// 0x003316BA

	int m_key;									// 0x00
	unsigned char m_flag;						// 0x04
	unsigned char m_pad05[3];
};
// SIZE 0x08

class LuaScriptEngine : public SubsystemInterface
{
public:
	LuaScriptEngine();											// 0x003379FD
	virtual ~LuaScriptEngine();									// 0x00337AA8
	virtual void init();										// slot 1, 0x00335D5B
	virtual void reset();										// slot 9, 0x00337C8C
	virtual void update();										// slot 10
	virtual void LoadPostProcess() {}							// slot 14, the shared empty body 0x000B3FD0 (retail's table 0x0080E390 has 17 slots; the three Snapshot-shaped virtuals are 14..16)
	virtual const char *GetSnapshotName() const;				// slot 15, 0x00337AA2
	virtual void DoXfer(Xfer *xfer);							// slot 16, 0x00306A9B

	void UnRegisterObject(Object *obj);							// 0x00333F37 (Object::setTeam 0x00298AE4 unregisters, then registers again)
	void *findEvent(NameKeyType key);							// 0x00333918
	void DispatchEvent(Int *event, Object *object, void *list);	// 0x00334634 (the ledger spells it ?DispatchEvent@LuaScriptEngine@@QAEXPAHPAVObject@@PAX@Z)
	void DispatchEvent(Int eventIndex, Object *object, void *list);	// 0x003360D2: DispatchEvent(&m_events[eventIndex], ...)
	void ExecuteChunk(char *buffer, int size, const AsciiString &chunk);		// 0x003340C1
	void executeChunk(char *buffer, int size, const AsciiString &chunk);		// 0x003340FA (the map's second spelling): ExecuteChunk over the shared state +0x10
	void RegisterObject(Object *obj);										// 0x00333E5B (WorldBuilder name; AIUpdateInterface::loadPostProcess 0x002686A4 calls it on TheLuaScriptEngine)
	// 0x00337DEF (the map's placeholder name; it returns a 20-byte record of the sorted list at +0xB0, lower_bound 0x00336283,
	// whose first member is its AsciiString name): AIUpdateInterface::xfer 0x00267EDD looks its +0x220 record up again by name.
	void *rva00337DEF(const AsciiString &name);
	void LoadScripts(const char *filename);									// 0x003345BE
	static int ObjectModifyUpgrade(lua_State *state, bool grant);				// 0x003354D6

	lua_State *m_luaState;						// 0x0C
	lua_State *m_luaStateShared;				// 0x10
	EventFlagEntry m_events[17];				// 0x14
	LuaDrawableLink *m_drawable;				// 0x9C
	unsigned char m_unk0xA0[0x0C];				// 0xA0 vector
	Bool m_unk0xAC;								// 0xAC
	unsigned char m_pad0xAD[3];
	unsigned char m_unk0xB0[0x0C];				// 0xB0 vector
	unsigned char m_unk0xBC[0x0C];				// 0xBC vector
	unsigned char m_unk0xC8[0x0C];				// 0xC8 vector
	Int m_unk0xD4;								// 0xD4
	Bool m_unk0xD8;								// 0xD8
	unsigned char m_pad0xD9[3];
	Int m_unk0xDC;								// 0xDC
};
typedef char LuaScriptEngineSize[sizeof(LuaScriptEngine) == 0xE0 ? 1 : -1];
typedef char LuaDrawableLinkSize[sizeof(LuaDrawableLink) == 0x18 ? 1 : -1];
typedef char EventFlagEntrySize[sizeof(EventFlagEntry) == 0x08 ? 1 : -1];

extern LuaScriptEngine *TheLuaScriptEngine;

#endif
