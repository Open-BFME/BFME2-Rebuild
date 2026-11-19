// HordeSiegeEngineContain.h: the HordeSiegeEngineContain module. SIZE 0x144 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x0047D247 calls HordeTransportContain first; it stores the
// tables 0x00C474A0 at +0x0, 0x00C441B8 at +0xC, 0x00C45EA8 at +0x10, 0x00C47328 at +0x20, 0x00C46064 at +0x24, 0x00C45D2C at +0x28, 0x00C58790 at +0x2C, 0x00C472F4 at +0x30, 0x00C472E4 at +0x34, 0x00C47834 at +0xFC. Members are what xfer (0x0047D36A) transfers after HordeTransportContain::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x0047D19C ?LoadPostProcess@HordeSiegeEngineContain@@UAEXXZ
//   +0x0 slot 2 -> 0x0047D081 ?name@HordeSiegeEngineContain@@QBEPBDXZ
//   +0x0 slot 8 -> 0x0047CB4D ?onDelete@HordeSiegeEngineContain@@UAEXXZ
//   +0x0 slot 13 -> 0x0047CD0D ?removeFromContainList@HordeSiegeEngineContain@@UAEXPAVObject@@@Z
//   +0x0 slot 18 -> 0x0047D33A ?rva0047D33A@HordeSiegeEngineContain@@QAEXXZ
//   +0x0 slot 27 -> 0x0047CA1A ?isSpecificRiderFreeToExit@HordeSiegeEngineContain@@MAE_NPAVObject@@@Z
//   +0x0 slot 28 -> 0x0047CB89 ?createPayload@HordeSiegeEngineContain@@UAEXXZ
//   +0x20 slot 22 -> 0x0047CA15 ?j_0047ca15@@YAXXZ
//   +0x20 slot 23 -> 0x0047BB3B ?rva0047BB3B@Rva0047BB3BOwner@@QAEXPAVObject@@@Z
//   +0x20 slot 34 -> 0x0047CE0C ?removeAndApplyForce@Rva0047CE0CInterface@@QAEXH@Z
//   +0x20 slot 38 -> 0x0047CA4D ?rva0047CA4D@Rva0047CA4D@@QAE_NPAVObject@@HH@Z
//   +0x20 slot 40 -> 0x0047D0E7 ?addToContainList@HordeSiegeEngineContain@@UAEXPAVObject@@@Z
//   +0x20 slot 42 -> 0x0047CD98 ?removeAllContained@Rva0047CD98@@UAEX_N@Z
//   +0x20 slot 44 -> 0x0047CAEB ?rva0047CAEB@Rva0047CAEBOwner@@QAEPAVBfmeObject872Header@@PAV2@PAVObject@@@Z
//   +0x20 slot 46 -> 0x0047CB3F ?get@Rva0047CB3FSub@@QBE_NXZ
//   +0x20 slot 53 -> 0x0047CADB ?rva0047CADB@HordeSiegeEngineContain@@UAEMXZ
//   +0x20 slot 58 -> 0x0047CDCE ?rva004631FA@SiegeEngineContain@@UAE_NH@Z
//   +0x20 slot 66 -> 0x0047CF67 ?onContaining@HordeSiegeEngineContain@@UAEXPAVObject@@_N@Z
//   +0x20 slot 67 -> 0x0047CF38 ?rva0047CF38@HordeSiegeEngineContain@@QAEXXZ
//   +0x20 slot 68 -> 0x0047CEBB ?iterateContained@Rva0047CEBB@@QAEXP6AXPAVObject@@PAX@Z1H@Z
//   +0x20 slot 71 -> 0x0047D087 ?rva004624EB@HordeSiegeEngineContain@@UAEXAAURva0046247DPair@@@Z
//   +0x20 slot 80 -> 0x004F0307 ?get@Rva004F0307ByteField@@QBEEXZ
//   +0x20 slot 99 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x20 slot 103 -> 0x004672B7 ?onCapture@TransportContain@@UAEXHH@Z
//   +0x20 slot 104 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
//   +0x20 slot 106 -> 0x0022C4CB ?getUpdatePhase@UpdateModule@@MBE?AW4SleepyUpdatePhase@@XZ
//   +0x20 slot 108 -> 0x00466E58 ?rva00466E58@Rva00466E58@@QBEEH@Z
//   +0x20 slot 109 -> 0x00477BA2 ?rva00477BA2@HordeTransportContain@@QAEXPAVOldContain@@PAVObject@@@Z
//   +0x20 slot 110 -> 0x005CB9FF ?vslot24_01@AIUpdateInterface@@UAE_NH@Z
//   +0x20 slot 111 -> 0x00462C72 ?monitorConditionChanges@OpenContain@@MAEXXZ
//   +0x20 slot 113 -> 0x00464EE7 ?rva00464EE7@OpenContain@@UAE_NPAVObject@@@Z
//   +0x20 slot 115 -> 0x0046279C ?doLoadSound@OpenContain@@MAEXXZ
//   +0x20 slot 116 -> 0x0046282B ?doUnloadSound@OpenContain@@MAEXXZ
//   +0x20 slot 118 -> 0x00462E3A ?rva00462E3A@Rva00462E3A@@QAEXPAVObject@@HH@Z
//   +0x20 slot 119 -> 0x00467F3B ?killRidersWhoAreNotFreeToExit@TransportContain@@MAEXXZ
//   +0x30 slot 0 -> 0x0047CA2D ?rva0047CA2D@Rva0047CA2D@@QBE_NXZ
//   +0x30 slot 2 -> 0x0047D90C ?rva0047D90C@Rva0047D90C@@QAEXPAVObject@@W4ExitDoorType@@@Z
//   +0x34 slot 0 -> 0x0058B03E ?loadPostProcess@UpdateModule@@MAEXXZ
// Not established: transfers xfer makes that this view cannot place: slot10 ?, slot8 ?, slot8 +0x130, Int ?, Int also read as Bool at +0x130, Int ?.
#pragma once
#ifndef BFME2_HORDE_SIEGE_ENGINE_CONTAIN_H
#define BFME2_HORDE_SIEGE_ENGINE_CONTAIN_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/HordeTransportContain.h"
#include "GameLogic/Module/HordeSiegeEngineContainModuleData.h"

class Thing;
class ModuleData;
class Xfer;

// HordeTransportContain ends at 0x128; this class's members follow it.
typedef char HordeSiegeEngineContain_base_size[(sizeof(HordeTransportContain) == 0x128) ? 1 : -1];

class HordeSiegeEngineContain : public HordeTransportContain
{
public:
	HordeSiegeEngineContain(Thing *thing, const ModuleData *moduleData);
	virtual Bool containSlot46()
	{
		const HordeSiegeEngineContainModuleData *data =
			static_cast<const HordeSiegeEngineContainModuleData *>(getModuleData());
		return data->m_crewAllowedToFire != false;
	}
	virtual ContainmentListView containSlot71();
	virtual Bool containSlot80() { return m_mineState; }
	virtual NameKeyType getModuleNameKey() const;
	virtual const char *GetSnapshotName() const;
	const char *name() const;						// 0x0047D081 (non-virtual, the literal)
	virtual void removeAllContained(Bool exposeStealthUnits);	// 0x0047CD98: ContainModuleInterface slot 42
	virtual void iterateContained(ContainIterateFunc func, void *userData, UnsignedInt flags);	// 0x0047CEBB: slot 68
	virtual ~HordeSiegeEngineContain();

protected:
	virtual void xfer(Xfer *xfer);

public:
	ContainedItemsList m_units;                   // +0x128: removeAllContained 0x0047CD98, iterateContained 0x0047CEBB and 0x0047CF38 walk it (the list header is the only member there)
	UnsignedInt m_unitCount;			// Saved count of contained units.
	Bool m_mineState;			// Set when a mine enters; the snapshot transfers one byte.
	std::map<ObjectID, Int> m_redeployAssignments;	// Assignments serialized by unit ID.
	XferContainIDList m_pendingUnitIDs;	// Resolved into m_units after loading.
};
// SIZE 0x144
typedef char HordeSiegeEngineContain_size[(sizeof(HordeSiegeEngineContain) == 0x144) ? 1 : -1];
typedef char HordeSiegeEngineContain_off_m_unitCount[(offsetof(HordeSiegeEngineContain, m_unitCount) == 0x12C) ? 1 : -1];
typedef char HordeSiegeEngineContain_off_m_mineState[(offsetof(HordeSiegeEngineContain, m_mineState) == 0x130) ? 1 : -1];

typedef char HordeSiegeEngineContain_off_m_redeployAssignments[(offsetof(HordeSiegeEngineContain, m_redeployAssignments) == 0x134) ? 1 : -1];
typedef char HordeSiegeEngineContain_off_m_pendingUnitIDs[(offsetof(HordeSiegeEngineContain, m_pendingUnitIDs) == 0x140) ? 1 : -1];

#endif
