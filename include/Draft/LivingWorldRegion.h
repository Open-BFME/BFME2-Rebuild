// LivingWorldRegion.h: one region of the War of the Ring map. Partly recovered layout.
//
// Target facts (retail RVAs):
//  - Snapshot-derived, SIZE 0x1B4. The parser (0x00210620) allocates 0x1B4 and calls
//    the constructor 0x003F3967 (owner, id, name).
//  - Slots: 0 deleting dtor 0x003F36EC (dtor 0x003F332E), 1 loadPostProcess 0x003F08EE,
//    2 the const name getter 0x003F3418 ("LivingWorldRegion"), 3 DoXfer 0x003F3BFF.
//  - The plot vector is at +0x170 (begin) and +0x174 (end), pointers to plot records
//    (0x003F1053 counts those whose int at +0x20 is zero).
//  - The region's two placement lists sit at +0x180 and +0x18C (0x003F0FA3).
//  - Not recovered: the +0x04 listener base (0x10 bytes), the 0x118-byte definition
//    at +0x14, and everything else; those are unnamed padding below.
#pragma once
#ifndef BFME2_LIVINGWORLDREGION_H
#define BFME2_LIVINGWORLDREGION_H

#include "Common/AsciiString.h"
#include "Common/Snapshot.h"
#include "Common/Vector.h"

class Xfer;

// One plot record as the region holds it: only the int at +0x20 is read so far.
// The class is LivingWorldBuildPlot in the constructor; its layout is not recovered.
struct RegionPlotRecord
{
	unsigned char m_unk0x00[0x20];
	int m_unk0x20;
};

// A 12-byte placement list (begin, end, end of storage). Its element is not recovered.
struct PlacementRange
{
	unsigned char m_unk0x00[0xC];
};

// The argument of 0x003F0FA3: a record whose AsciiString at +0x18 selects the list.
struct PlacementRequest
{
	unsigned char m_unk0x00[0x18];
	AsciiString m_name;					// +0x18
};

class LivingWorldRegion : public Snapshot
{
public:
	virtual ~LivingWorldRegion();				// 0x003F332E (deleting 0x003F36EC)
	virtual void loadPostProcess();				// 0x003F08EE
	virtual char const *GetSnapshotName() const;		// 0x003F3418
	virtual void DoXfer(Xfer *xfer);			// 0x003F3BFF

	int rva003F1053();					// 0x003F1053: plots whose +0x20 is zero
	void rva003F0F48(PlacementRange *list, void *request);	// 0x003F0F48 (body not written)
	void rva003F0FA3(void *request);			// 0x003F0FA3: picks the list by the request's name

private:
	unsigned char m_unk0x04[0x12C - 0x04];			// +0x04 base listener and definition, not recovered
public:
	int m_regionID;					// +0x12C (LivingWorldRegionManager::findPendingBattleByRegionID 0x0020E53D compares it)
	unsigned char m_unk0x130[0x13C - 0x130];		// +0x130 not recovered
	int m_unk0x13C;					// +0x13C (read by LivingWorldPlayer::GetNumRegionsOwned)
private:
	unsigned char m_unk0x140[0x170 - 0x140];		// +0x140 not recovered
	_STL::vector<RegionPlotRecord *> m_plots;		// +0x170
	unsigned char m_unk0x17C[0x180 - 0x17C];		// +0x17C not recovered
	PlacementRange m_placementsA;				// +0x180
	PlacementRange m_placementsB;				// +0x18C
	unsigned char m_unk0x198[0x1B4 - 0x198];		// +0x198 not recovered
};

#endif
