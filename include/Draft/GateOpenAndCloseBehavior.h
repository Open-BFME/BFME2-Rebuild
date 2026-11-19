// GateOpenAndCloseBehavior.h: the gate that opens and closes its door. Promoted from the Draft.
// Layout: a 4-byte primary interface at +0 (vtable 0x00C50178, slots below), then UpdateModule
// at +4 (0x20 bytes, so m_object is at +0xC and the interface at +0x14). Members start at +0x24.
// Proven by the rows written against it: open 0x004991CB and close 0x0049920B use slots 6 and 10
// through the primary and the members +0x28, +0x30, +0x34 and +0x3C; rva00498BAF 0x00498BAF reads
// +0x2C; changeWantOpenCount 0x00498831 and getControllingPlayer 0x004989D7 use +0x40 and +0xC.
// Still unnamed: primary slots 0, 2, 3, 4 and 5, and the class's constructor, destructor, xfer and
// update rows (they wait on the primary's vtable identity and the module-data layout).
#pragma once
#ifndef BFME2_GATE_OPEN_AND_CLOSE_BEHAVIOR_H
#define BFME2_GATE_OPEN_AND_CLOSE_BEHAVIOR_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"

class Xfer;
class Coord3D;
class Object;
class Player;
class Thing;
class ModuleData;
struct Rva00498DF3Arg;

// The gate's primary interface, 4 bytes at +0 of the gate. Slots 1 and 6..10 are named by the rows
// that call or implement them; the others are unidentified and stay gaps.
class GatePrimary
{
public:
	virtual void gap0() = 0;							// slot 0, not identified
	virtual bool rva00498BAF(Object *object) = 0;		// slot 1, 0x00498BAF
	virtual void gap2() = 0;							// slot 2, not identified
	virtual void gap3() = 0;							// slot 3, not identified
	virtual void gap4() = 0;							// slot 4, not identified
	virtual void gap5() = 0;							// slot 5, not identified
	virtual bool slot6() const = 0;						// slot 6 (+0x18), open and close test it
	virtual void open() = 0;							// slot 7 (+0x1C), 0x004991CB
	virtual void close() = 0;							// slot 8 (+0x20), 0x0049920B
	virtual void rva00498806() = 0;						// slot 9 (+0x24)
	virtual unsigned char slot10() const = 0;			// slot 10 (+0x28), open and close test it
};

class GateOpenAndCloseBehavior : public GatePrimary, public UpdateModule
{
public:
	GateOpenAndCloseBehavior(Thing *thing, const ModuleData *moduleData);	// 0x0049889C
	virtual ~GateOpenAndCloseBehavior();				// 0x00498798
	virtual NameKeyType getModuleNameKey() const;		// 0x00498857
	virtual void onObjectCreated();						// 0x0049936A
	virtual UpdateSleepTime update();					// 0x0049947A
	virtual bool rva00498BAF(Object *object);			// slot 1, 0x00498BAF
	virtual bool slot6() const;							// slot 6
	virtual void open();								// slot 7, 0x004991CB
	virtual void close();								// slot 8, 0x0049920B
	virtual void rva00498806();							// slot 9
	virtual unsigned char slot10() const;				// slot 10

	void changeWantOpenCount(int delta);				// 0x00498831
	bool rva00498980();									// 0x00498980 (threshold for the state)
	Player *getControllingPlayer() const;				// 0x004989D7
	Relationship getRelationship(const Player *player) const;	// 0x004989DF
	Real getM34() const;							// 0x004987FA
	Real getM38() const;							// 0x004987FE
	Bool getM30() const;							// 0x00498802
	void moveUnitAwayFromGate(Object *unit, Real offset);	// 0x00498DF3
	void pushUnitAwayIfBlocking(Object *unit, Int, Int);	// 0x00498F46
	Bool selectLinkedObject();							// 0x00498CB1: selects the linked object (m_linkedObjectId at +0x24) for the local player
	void resetProgress();								// 0x0049881D: level 0, stamped with the logic frame (m_34, m_stateFrame)
	const Coord3D *getPosition() const;					// 0x004989EE: the object's position (m_object at +0xC, Thing::m_cachedPos at +0x38)
	const char *name() const;							// 0x004987F4

protected:
	virtual void xfer(Xfer *xfer);						// 0x00498BCB

private:
	void rva004989F5();
	void setOpenCloseState(int state);					// 0x00498AB2
	void rva00498FAA();									// 0x00498FAA
	void rva0049924B(bool flag);
	void rva004992D5(bool a, bool b);

public:
	ObjectID m_linkedObjectId;							// +0x24
	Int m_state;										// +0x28 (the state setOpenCloseState 0x00498AB2 stores)
	Int m_2C;											// +0x2C (rva00498BAF tests it for 1)
	Bool m_30;											// +0x30 (cleared by open and close)
	Real m_34;											// +0x34 (zeroed by open and close)
	Real m_38;											// +0x38
	UnsignedInt m_stateFrame;							// +0x3C (set to the frame by open and close)
	Int m_request;										// +0x40 (changeWantOpenCount 0x00498831)
	Int m_soundHandle;									// +0x44 (AudioHandle: xfer 0x00498BCB passes it to TheAudio's slot 88)
	Bool m_soundPlayed;									// +0x48
};
// 0x00498725: the gate module of an object, found by name key in its behavior list.
GateOpenAndCloseBehavior *findGateOpenAndCloseBehavior(Object *object);
// 0x00498B8B: whether the gate may push this object aside (exists, not KindOf bit 2, ground layer).
unsigned char isPushableByGate(Object *object);
// The primary is a bare vptr: 4 bytes.
typedef char GatePrimary_size[(sizeof(GatePrimary) == 4) ? 1 : -1];

#endif
