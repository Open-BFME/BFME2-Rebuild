#pragma once

#include "GameLogic/SkirmishAI/AITactic.h"

// The three factory-bearing bases have separate retail tables. The name of
// each address-named base remains unresolved.
class AITacticOffensive : public AITactic
{
public:
    AITacticOffensive(const AsciiString &name); // 0x005DC722, table 0x00C767DC
    virtual ~AITacticOffensive();               // 0x005DC73C

    virtual AITactic *create() = 0;             // slot 9
    virtual Bool vslot10() const { return false; } // native 0x0047A699
	// The target is the chooser's AITarget; the ledger spells the parameter void * and the result as a byte.
	unsigned char checkTarget(void *target);
};

class Rva005DCC24 : public AITactic
{
public:
    Rva005DCC24(const AsciiString &name); // 0x005DCC0A, table 0x00C7690C
    virtual ~Rva005DCC24();               // 0x005DCC24

    virtual AITactic *create() = 0;        // slot 9
    virtual Bool vslot10() const { return false; } // native 0x0047A699
};

class Rva005DCBE3 : public AITactic
{
public:
    Rva005DCBE3(const AsciiString &name, Bool flag); // 0x005DCBC7, table 0x00C768E4
    virtual ~Rva005DCBE3();                         // 0x005DCBE3

    virtual AITactic *create() = 0;                  // slot 9 is the last slot
};

class AITacticSiege : public AITacticOffensive
{
public:
    AITacticSiege(const AsciiString &name); // 0x005DC85F, table 0x00C76808
    virtual ~AITacticSiege();               // 0x005DC87B

    virtual void xfer(Xfer *xfer);
    virtual Bool vslot10() const { return true; } // native 0x0050B5C6
	// Whether the owning side has siege weapons that are not busy.
    Bool sideHasIdleSiegeWeapons();
	Bool isWallBreached();	// 0x005DC93B (a team member's object is a breach or gate, and the target is in reach)
	Bool findWallTarget();	// 0x005DC9C8
	Object *rva005DCAE5();	// 0x005DCAE5 (the logic object with the id at +0x58; the map holds it as rva005DCAE5)
	Bool rva005DCAF4();	// 0x005DCAF4 (no wall target object, or it has the flag +0x438 bit 0 set)

protected:
    ObjectID m_unk0x58; // 0x005DC903 transfers this through XferObjectID
};

typedef char AITacticOffensiveSize[(sizeof(AITacticOffensive) == 0x58) ? 1 : -1];
typedef char Rva005DCC24Size[(sizeof(Rva005DCC24) == 0x58) ? 1 : -1];
typedef char Rva005DCBE3Size[(sizeof(Rva005DCBE3) == 0x58) ? 1 : -1];
typedef char AITacticSiegeSize[(sizeof(AITacticSiege) == 0x5C) ? 1 : -1];
