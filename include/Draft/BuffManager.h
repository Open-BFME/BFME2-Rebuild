// BuffManager.h: the buff table of the game client. SIZE 0x26C.
// Layout, proven by the rows below: the constructor 0x00362DBD writes the vptr at +0, stores its
// int argument at +4 (the drawable), and builds nine BuffInstance (0x44 bytes each) at +8.
// TurnOffBuff 0x003626AD indexes the table by type 1..8 (type 0 is not used).
// Still a Draft: the destructor 0x00362E1C and the registration with the client frame (the
// constructor's last call) are not in this file yet.
#pragma once
#ifndef BFME2_BUFF_MANAGER_H
#define BFME2_BUFF_MANAGER_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Draft/BuffInstance.h"

class Xfer;
class ModuleData;

// One record of the table at 0x009C0138 (map name g_00DC0138): TurnOnBuff 0x003627F9 selects the
// buff's sub type from +4 (1 normal, 2 glow outline); the mask word at +0 is read by 0x0036276F.
struct BuffTypeConfig
{
	unsigned int mask;										// +0x00
	int subtype;											// +0x04
};
extern BuffTypeConfig g_00DC0138[9];

// While BuffManager::DoXfer 0x00362702 transfers its instances this holds the manager's drawable (+4), and
// BuffInstance::DoXfer 0x003628D3 passes it to BuffLogic::addBuff when it restores a glow effect; it is cleared after.
extern void *g_Va00E01E74;

// 0x0036276F recomputes which buff slots are active from the mask table above and pushes the result to each
// instance (0x0036265E). It is BuffManager's own method (TurnOnBuff 0x003627F9 calls it on `this`); the map
// still names it as a placeholder of this class, so the call goes through it until the row is renamed.
class Rva0036276F
{
public:
	void rva0036276F();										// 0x0036276F
};

// 0x00239FE4 is the call that ends the constructor: it registers the new manager with TheGameClient (the
// global at 0x00DFE77C). GameClient.cpp still has it as a placeholder row of this class, so the constructor
// calls it through the placeholder until that row is renamed onto GameClient.
class Rva00239FE4
{
public:
	void rva00239FE4(const ModuleData *buffManager);		// 0x00239FE4
};

class BuffManager
{
public:
	BuffManager(int drawable);								// 0x00362DBD
	virtual ~BuffManager();									// 0x00362E1C, slot 0 (held, as BuffInstance's: retail ends it with Snapshot's table store 0x00BBB554)
	void TurnOffBuff(int type, bool immediate);				// 0x003626AD
	void TurnOnBuff(int type, void *thingTemplate, int duration, int color, float intensity);	// 0x003627F9

protected:
	virtual void LoadPostProcess() {}						// slot 1; the shared empty body 0x000B3FD0 (retail table 0x00C17088 has 4 slots)
	virtual const char *GetSnapshotName() const;			// 0x00362E16 (slot 2)
	virtual void DoXfer(Xfer *xfer);						// 0x00362702 (slot 3)

public:
	void *m_drawable;										// +0x04
	BuffInstance m_buffs[9];								// +0x08
};
// SIZE 0x26C
typedef char BuffManager_size[(sizeof(BuffManager) == 0x26C) ? 1 : -1];

#endif
