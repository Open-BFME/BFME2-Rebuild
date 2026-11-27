#pragma once

#include "Common/AsciiString.h"
#include "Common/Coord2D.h"
#include "Common/Science.h"
#include "Common/Snapshot.h"
#include "Common/Vector.h"
#include "GameLogic/ArmySummaryEntry.h"

class ArmySummaryListener;

// Native copies increment entry +0xB0 and release the +0xAC base.
typedef AptRef<ArmySummaryEntry> ArmySummaryEntryRef;

// Snapshot table 0x008394F0. The 0x68-byte array stride is used by
// LivingWorldBattle's copy at 0x003F5A06.
class ArmySummary : public Snapshot
{
public:
	ArmySummary();	// 0x0040E3EE
	ArmySummary(const ArmySummary &other);	// 0x0040E96D
	ArmySummary &operator=(const ArmySummary &other);	// 0x0040EAED
	virtual ~ArmySummary();
	virtual const char *GetSnapshotName() const;

	// The reinforcement and engagement timers (the names are the ledger's placeholders).
	void rva0040C985(Int frame);			// 0x0040C985: start engaging at the frame, due after the reinforcement delay
	Real rva0040C9A4();						// 0x0040C9A4: engagement progress, 0..1
	Int rva0040C9F4();						// 0x0040C9F4: frames left until the engagement ends
	void rva0040CA09();						// 0x0040CA09: engaged -> finished once the end frame is reached
	Int rva0040CA24();						// 0x0040CA24: the night variant of +0x24 when it is night
	void rva0040CA3A(Int seconds);			// 0x0040CA3A: sets the reinforcement delay in logic frames and resets the state

protected:
	virtual void loadPostProcess() {}	// slot 1, empty 0x000B3FD0
	virtual void xfer(Xfer *xfer);

private:
	struct IndexedEntry
	{
		Int id;	// 0x0040CB3A searches this key
		ArmySummaryEntryRef entry;	// 0x0040CBD7 copies this reference
	};

	_STL::vector<ArmySummaryListener *> m_listeners;	// +0x04, notified by 0x0040D8B8/0x0040D8D6
	Int m_listenerIndex;	// +0x10, 0x0040D487 saves and advances the active callback index
	Bool m_unk0x14;	// xfer transfers a Bool
	AsciiString m_armyName;	// +0x18, GetArmyNameByID 0x0040D34C
	Int m_livingWorldArmyID;	// +0x1C, XferLivingWorldArmyID 0x00318D1E
	AsciiString m_armyBanner;	// +0x20, GetArmyBannerByID 0x0040D366
	Int m_unk0x24;
	Int m_unk0x28;
	Int m_reinforcementState;	// +0x2C, XferReinforcementState 0x0040C96D
	Int m_reinforcementDelayFrames;	// +0x30, added to current frame in 0x0040C985
	Int m_engageEndFrame;	// +0x34, 0x0040C9F4 computes frames remaining
	Int m_engageStartFrame;	// +0x38, 0x0040C9A4 uses both frames for engage progress
	Int m_nextEntryID;	// +0x3C, incremented by AddArmyEntry 0x0040ECCF
	_STL::vector<IndexedEntry> m_entries;	// +0x40, 0x0040CBD7 reads indexed entries
	_STL::vector<ScienceType> m_sciences;	// +0x4C, typed transfer 0x0040E19D
	Coord2D m_unk0x58;	// xfer 0x0040EE89 uses the Coord2D slot
	Int m_unk0x60;
	AsciiString m_unk0x64;	// transferred as AsciiString by 0x0040EE89
};

typedef char ArmySummary_size[(sizeof(ArmySummary) == 0x68) ? 1 : -1];
