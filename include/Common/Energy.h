// Energy.h: the Energy power balance (Common/RTS). Proven so far: getEnergySupplyRatio
// (RVA 0x004DF161) returns 1.0 when the second word is zero, and otherwise the first word
// over the second. The words are named by their offsets only; no row names them yet.
#pragma once
#include "Common/GameType.h"
#include "Common/Snapshot.h"

class Object;
class Player;
class ThingTemplate;
class Xfer;

// The record addPowerBonus (0x004DF25B) and removePowerBonus (0x004DF274) take: the map spells their argument
// `struct Rva004DF207Outer *` (the same type objectEnteringInfluence's old placeholder name carried). Both functions read
// its word at +0x4 as a ThingTemplate pointer and feed that template's EnergyBonus (+0x54C) to rva004DF18D, so it is
// Object-shaped (Thing::m_template is +0x4); the naming lane decides whether it is Object.
struct Rva004DF207Outer
{
	void *m_unk0x00;
	const ThingTemplate *m_template;	// +0x4
};

// The Snapshot base supplies the vptr at +0 (retail vtable slots: destructor, loadPostProcess, GetSnapshotName, xfer); the words start at +4.
class Energy : public Snapshot
{
public:
	// Inline, as Player's constructor 0x002B0F3C builds it at 0x002B0FD9: the table, then +0x4, +0x8, +0xC zeroed (Zero Hour's Energy())
	Energy() : m_unk0x4(0), m_unk0x8(0), m_player(NULL) {}
	Real getEnergySupplyRatio() const;	// 0x004DF161
	void adjustPower(Int amount, Bool add);	// 0x004DF1D8
	void objectEnteringInfluence(Object *);	// 0x004DF207
	void objectLeavingInfluence(Object *);	// 0x004DF231
	void addPowerBonus(Rva004DF207Outer *);	// 0x004DF25B adds the template's EnergyBonus
	void removePowerBonus(Rva004DF207Outer *);	// 0x004DF274 subtracts it
	Bool rva004DF17F() const;	// 0x004DF17F the supply covers the demand (the map spells the class Rva004DF17F)
	void rva004DF18D(Int);	// 0x004DF18D (placeholder helper; positive amounts)
	void rva004DF1AF(Int);	// 0x004DF1AF (placeholder helper; negative amounts)
	Int m_unk0x4;						// +0x4 numerator of the supply ratio (0x004DF161)
	Int m_unk0x8;						// +0x8 denominator of the supply ratio (0x004DF161)
	Player *m_player;					// +0xC receives the brown-out change (0x004DF18D, 0x004DF1AF, each when non-null)

protected:
	// Table 0x007FDC48: deleting dtor, loadPostProcess (the shared empty body 0x000B3FD0), GetSnapshotName 0x002AA3AF, xfer 0x004DF290.
	virtual void loadPostProcess() {}
	virtual const char *GetSnapshotName() const;	// 0x002AA3AF
	virtual void xfer(Xfer *xfer);		// 0x004DF290, the light-CRC gate, then the owning player index
};
