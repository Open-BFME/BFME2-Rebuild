// TransportContain.h: the contain that carries and unloads units. SIZE 0x11C.
//
// Target facts: the constructor 0x00468559 calls OpenContain's 0x004649F8,
// stores a one-slot interface table 0x00C1C780 (__purecall) at +0xFC, then
// its own tables (+0xFC 0x00C47834, whose one slot is createPayloadMember
// 0x00466FE0), zeroes +0x100/+0x104/+0x108 and the byte +0x10C, and builds a
// vector<AsciiString> at +0x110 (vector constructor, then push_back of
// AsciiString). The factory 0x0024B861 allocates 0x11C. xfer 0x00466DBC writes
// its version, OpenContain::xfer, then xferInt(+0x100), xferUnsignedInt(+0x104)
// and xferBool(+0x10C). The primary table 0x00C44278 has 31 slots: OpenContain's
// 27 plus 27 isSpecificRiderFreeToExit 0x004671AC, 28 createPayload 0x004670D6,
// 29 0x0046740C and 30 letRidersUpgradeWeaponSet 0x0046760D.
// WorldBuilder: its asserts name TransportContain::createPayloadMember, so the
// +0xFC interface's only virtual is createPayloadMember; nothing in WB or the
// map names the interface itself.
// PROVISIONAL: TransportPayloadInterface is a descriptive name for that
// interface, chosen so the horde transports can build on it; rename it when
// evidence names it.
#pragma once
#ifndef BFME2_TRANSPORTCONTAIN_H
#define BFME2_TRANSPORTCONTAIN_H

#include <vector>
#include "Common/AsciiString.h"
#include "GameLogic/Module/OpenContain.h"

class ThingTemplate;
class Object;
class PayloadContainView;

// PROVISIONAL name (see above).
class TransportPayloadInterface
{
public:
	virtual Object *createPayloadMember(const ThingTemplate *thingTemplate, PayloadContainView *contain,
		Object *container, const char *name, Int count) = 0;			// 0
};

class TransportContain : public OpenContain, public TransportPayloadInterface	// +0xFC
{
public:
	TransportContain(Thing *thing, const ModuleData *moduleData);		// 0x00468559

	virtual NameKeyType getModuleNameKey() const;						// 0x00467EE1
	virtual UpdateSleepTime update();									// 0x00468749
	virtual void onCapture(Player *oldOwner, Player *newOwner);			// 0x004672B7 (map spells the arguments int)

	virtual Bool vslot14(Int);

	// ContainModuleInterface
	virtual Bool containSlot50() { return true; }
	virtual Int containSlot51() const;
	virtual void onContaining(Object *obj, Bool wasSelected);			// slot 22 of table 0x00844030: 0x0046781F (calls OpenContain's 0x00463097 first)

	// ExitInterface
	virtual Bool isExitBusy() const;									// 0x00466DA9
	const char *name() const;									// 0x00467F26

	// TransportPayloadInterface
	virtual Object *createPayloadMember(const ThingTemplate *thingTemplate, PayloadContainView *contain,
		Object *container, const char *name, Int count);				// 0x00466FE0

	// TransportContain's own primary slots 27..30
protected:
	virtual Bool isSpecificRiderFreeToExit(Object *obj);				// 27, 0x004671AC
public:
	virtual void createPayload();										// 28, 0x004670D6
protected:
	virtual void vslot29();												// 29, 0x0046740C
	virtual void letRidersUpgradeWeaponSet();							// 30, 0x0046760D

	void killRidersWhoAreNotFreeToExit();								// 0x00467F3B
public:
	void UpdateUpgradeCreationTriggers();								// 0x00468045

protected:
	virtual ~TransportContain();
	virtual const char *GetSnapshotName() const;						// 0x00467F26
	virtual void xfer(Xfer *xfer);										// 0x00466DBC

public:
	Int m_unk0x100;														// 0x100 xferInt
	UnsignedInt m_frameExitNotBusy;										// 0x104 xferUnsignedInt; isExitBusy compares the logic frame to it
	Int m_unk0x108;														// 0x108 zeroed, not xferred
	Bool m_unk0x10C;													// 0x10C xferBool
	std::vector<AsciiString> m_unk0x110;								// 0x110
};
// SIZE 0x11C

typedef char TransportContain_off_m_unk0x100[(offsetof(TransportContain, m_unk0x100) == 0x100) ? 1 : -1];
typedef char TransportContain_off_m_unk0x10C[(offsetof(TransportContain, m_unk0x10C) == 0x10C) ? 1 : -1];
typedef char TransportContain_off_m_unk0x110[(offsetof(TransportContain, m_unk0x110) == 0x110) ? 1 : -1];
typedef char TransportContain_size[(sizeof(TransportContain) == 0x11C) ? 1 : -1];

#endif
