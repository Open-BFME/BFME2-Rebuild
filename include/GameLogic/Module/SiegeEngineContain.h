// SiegeEngineContain.h: the SiegeEngineContain module. SIZE 0x138 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x0047C21E calls TransportContain first; it stores the
// tables 0x00C470F8 at +0x0, 0x00C441B8 at +0xC, 0x00C441A8 at +0x10, 0x00C46F80 at +0x20, 0x00C46064 at +0x24, 0x00C46F78 at +0x28, 0x00C58790 at +0x2C, 0x00C46F48 at +0x30, 0x00C472E4 at +0x34, 0x00C47834 at +0xFC. Members are what xfer (0x0047C322) transfers after TransportContain::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x0047C173 ?LoadPostProcess@SiegeEngineContain@@UAEXXZ
//   +0x0 slot 2 -> 0x0047C012 ?name@SiegeEngineContain@@QBEPBDXZ
//   +0x0 slot 8 -> 0x0047BB6B ?onDelete@SiegeEngineContain@@UAEXXZ
//   +0x0 slot 13 -> 0x0047BD4C ?removeFromContainList@SiegeEngineContain@@UAEXPAVObject@@@Z
//   +0x0 slot 18 -> 0x0047C2F2 ?redeployOccupants@SiegeEngineContain@@QAEXXZ
//   +0x0 slot 27 -> 0x0047B993 ?rva0047B993@Rva0047B993Contain@@QAE_NPAVObject@@@Z
//   +0x0 slot 28 -> 0x0047BBA7 ?createPayload@SiegeEngineContain@@UAEXXZ
//   +0x20 slot 22 -> 0x0047BA10 ?onContaining@Rva0047BA10@@UAEXPAVObject@@_N@Z
//   +0x20 slot 23 -> 0x0047BB3B ?rva0047BB3B@Rva0047BB3BOwner@@QAEXPAVObject@@@Z
//   +0x20 slot 34 -> 0x0047BE23 ?rva0047BE23@Rva0047BDED@@QAEXH@Z
//   +0x20 slot 38 -> 0x0047BA69 ?rva0047BA69@Rva0047BA69@@QAE_NPAVObject@@HH@Z
//   +0x20 slot 40 -> 0x0047C07F ?addToContainList@SiegeEngineContain@@UAEXPAVObject@@@Z
//   +0x20 slot 42 -> 0x0047BDED ?removeAllContained@Rva0047BDED@@QAEX_N@Z
//   +0x20 slot 44 -> 0x0047CAEB ?rva0047CAEB@Rva0047CAEBOwner@@QAEPAVBfmeObject872Header@@PAV2@PAVObject@@@Z
//   +0x20 slot 46 -> 0x0047CB3F ?get@Rva0047CB3FSub@@QBE_NXZ
//   +0x20 slot 53 -> 0x0047BAE7 ?rva0047CADB@SiegeEngineContain@@UAEMXZ
//   +0x20 slot 58 -> 0x0047CDCE ?rva004631FA@SiegeEngineContain@@UAE_NH@Z
//   +0x20 slot 66 -> 0x0047BEF8 ?onContaining@Rva0047BEF8@@UAEXPAVObject@@_N@Z
//   +0x20 slot 67 -> 0x0047BEC9 ?rva0047BEC9@SiegeEngineContain@@QAEXXZ
//   +0x20 slot 68 -> 0x0047BE4C ?iterateContained@Rva0047BE4C@@QAEXP6AXPAVObject@@PAX@Z1H@Z
//   +0x20 slot 71 -> 0x0047C018 ?rva004624EB@SiegeEngineContain@@UAEXAAURva0046247DPair@@@Z
//   +0x20 slot 80 -> 0x0047C033 ?get@Rva0047C033ByteField@@QBEEXZ
//   +0x20 slot 99 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x20 slot 103 -> 0x004672B7 ?onCapture@TransportContain@@UAEXHH@Z
//   +0x20 slot 104 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
//   +0x20 slot 106 -> 0x0022C4CB ?getUpdatePhase@UpdateModule@@MBE?AW4SleepyUpdatePhase@@XZ
//   +0x20 slot 108 -> 0x00466E58 ?rva00466E58@Rva00466E58@@QBEEH@Z
//   +0x20 slot 109 -> 0x0050B238 ?SkipBadBlock@Xfer@@UAEXAAVSnapshot@@I@Z
//   +0x20 slot 110 -> 0x005CB9FF ?vslot24_01@AIUpdateInterface@@UAE_NH@Z
//   +0x20 slot 111 -> 0x00462C72 ?monitorConditionChanges@OpenContain@@MAEXXZ
//   +0x20 slot 113 -> 0x00464EE7 ?rva00464EE7@OpenContain@@UAE_NPAVObject@@@Z
//   +0x20 slot 115 -> 0x0046279C ?doLoadSound@OpenContain@@MAEXXZ
//   +0x20 slot 116 -> 0x0046282B ?doUnloadSound@OpenContain@@MAEXXZ
//   +0x20 slot 118 -> 0x00462E3A ?rva00462E3A@Rva00462E3A@@QAEXPAVObject@@HH@Z
//   +0x20 slot 119 -> 0x00467F3B ?killRidersWhoAreNotFreeToExit@TransportContain@@MAEXXZ
//   +0x30 slot 0 -> 0x0047CA2D ?rva0047CA2D@Rva0047CA2D@@QBE_NXZ
//   +0x30 slot 2 -> 0x0047C4F2 ?exitObjectViaDoor@SiegeEngineContain@@UAEXPAVObject@@W4ExitDoorType@@@Z
//   +0x30 slot 12 -> 0x004641B1 ?onDie@OpenContain@@UAEXPBVDamageInfo@@@Z
//   +0x34 slot 0 -> 0x0058B03E ?loadPostProcess@UpdateModule@@MAEXXZ
//   +0x34 slot 6 -> 0x0047D90C ?rva0047D90C@Rva0047D90C@@QAEXPAVObject@@W4ExitDoorType@@@Z
// Not established: transfers xfer makes that this view cannot place: slot10 ?, slot8 ?, slot8 +0x124, Int ?, Int also read as Bool at +0x124, Int ?.
#pragma once
#ifndef BFME2_SIEGE_ENGINE_CONTAIN_H
#define BFME2_SIEGE_ENGINE_CONTAIN_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/TransportContain.h"
#include "GameLogic/Module/SiegeEngineContainModuleData.h"

class Thing;
class ModuleData;
class Xfer;

// TransportContain ends at 0x11C; this class's members follow it.
typedef char SiegeEngineContain_base_size[(sizeof(TransportContain) == 0x11C) ? 1 : -1];

class SiegeEngineContain : public TransportContain
{
public:
	char const * name() const;	// 0x0047C012
	SiegeEngineContain(Thing *thing, const ModuleData *moduleData);
	virtual Bool containSlot46()
	{
		const SiegeEngineContainModuleData *data =
			static_cast<const SiegeEngineContainModuleData *>(getModuleData());
		return data->m_crewAllowedToFire != false;
	}
	virtual ContainmentListView containSlot71();
	virtual Bool containSlot80() { return m_mineState; }
	virtual NameKeyType getModuleNameKey() const;
	virtual const char *GetSnapshotName() const;
	virtual ~SiegeEngineContain();

protected:
	virtual void xfer(Xfer *xfer);

public:
	ContainedItemsList m_units;
	UnsignedInt m_unitCount;			// Saved count of contained units.
	Bool m_mineState;			// Set when a mine enters; the snapshot transfers one byte.
	std::map<ObjectID, Int> m_redeployAssignments;	// Assignments serialized by unit ID.
	XferContainIDList m_pendingUnitIDs;	// Resolved into m_units after loading.
};
// SIZE 0x138
typedef char SiegeEngineContain_size[(sizeof(SiegeEngineContain) == 0x138) ? 1 : -1];
typedef char SiegeEngineContain_off_m_units[(offsetof(SiegeEngineContain, m_units) == 0x11C) ? 1 : -1];
typedef char SiegeEngineContain_off_m_unitCount[(offsetof(SiegeEngineContain, m_unitCount) == 0x120) ? 1 : -1];
typedef char SiegeEngineContain_off_m_mineState[(offsetof(SiegeEngineContain, m_mineState) == 0x124) ? 1 : -1];

typedef char SiegeEngineContain_off_m_redeployAssignments[(offsetof(SiegeEngineContain, m_redeployAssignments) == 0x128) ? 1 : -1];
typedef char SiegeEngineContain_off_m_pendingUnitIDs[(offsetof(SiegeEngineContain, m_pendingUnitIDs) == 0x134) ? 1 : -1];

#endif
