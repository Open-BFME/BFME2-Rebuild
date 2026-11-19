// RiderChangeContain.h: the RiderChangeContain module. SIZE 0x148 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x0047E268 calls SiegeEngineContain first; it stores the
// tables 0x00C47A00 at +0x0, 0x00C441B8 at +0xC, 0x00C479F0 at +0x10, 0x00C47878 at +0x20, 0x00C46064 at +0x24, 0x00C46F78 at +0x28, 0x00C58790 at +0x2C, 0x00C47848 at +0x30, 0x00C47838 at +0x34, 0x00C47834 at +0xFC. Members are what xfer (0x0047E357) transfers after SiegeEngineContain::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x0047E38F ?rva0047E38F@Rva0047E38F@@QAEXXZ
//   +0x0 slot 2 -> 0x0047E325 ?name@RiderChangeContain@@QBEPBDXZ
//   +0x0 slot 9 -> 0x0047E33F ?rva0047E33F@Rva0047E33F@@UAEXHH@Z
//   +0x0 slot 25 -> 0x0047E330 ?rva0047E330@Rva0047E330@@QAEXXZ
//   +0x0 slot 27 -> 0x0047E335 ?j_0047e335@@YAXXZ
//   +0x0 slot 28 -> 0x0047E32B ?rva0047E32B@Rva0047E32B@@QAEXXZ
//   +0x0 slot 31 -> 0x0006E01D ?get@Rva0006E01DDwordField@@QBEHXZ
//   +0x0 slot 32 -> 0x0047E344 ?getContainerPipsToShow@RiderChangeContain@@UAE_NAAH0@Z
//   +0x10 slot 0 -> 0x0047E59E ?rva047E59E@Rva047E59E@@QAEXXZ
//   +0x20 slot 9 -> 0x0050B5C6 ?vslot10@GameWindow@@UAE_NXZ
//   +0x20 slot 10 -> 0x0047EA58 ?rva0047EA58@RiderChangeContain@@UAEPAVObject@@XZ
//   +0x20 slot 22 -> 0x0047E5D4 ?onContaining@RiderChangeContain@@UAEXPAVObject@@_N@Z
//   +0x20 slot 23 -> 0x0047E7CF ?onRemoving@RiderChangeContain@@UAEXPAVObject@@@Z
//   +0x20 slot 24 -> 0x0047E39A ?rva0047E33F@Rva0047E33F@@WCA@AEXHH@Z
//   +0x20 slot 38 -> 0x0047E520 ?isValidContainerFor@RiderChangeContain@@UAE_NPAVObject@@_N1@Z
//   +0x20 slot 72 -> 0x0047EA26 ?friend_getRider@RiderChangeContain@@UBEPBVObject@@XZ
//   +0x20 slot 95 -> 0x00253376 ?getDisabledTypesToProcess@UpdateModule@@UBE?AV?$BitFlags@$0L@@@XZ
//   +0x20 slot 96 -> 0x0044DF8D ?rva0044DF8D@UpdateModule@@UAEXW4UpdateSleepTime@@@Z
//   +0x30 slot 0 -> 0x0047A699 ?IsCRC@Xfer@@UBE_NXZ
//   +0x30 slot 1 -> 0x0047E33A ?j_0047e33a@@YAXXZ
//   +0x30 slot 12 -> 0x0046261B ?rva0046261B@SlaughterHordeContain@@UAEX_N@Z
//   +0x30 slot 13 -> 0x004647CD ?get@Rva004647CDLeaField@@QBEPAXXZ
//   +0x30 slot 14 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
//   +0x30 slot 15 -> 0x004624E3 ?containReactToTransformChange@OpenContain@@UAEXXZ
//   +0x30 slot 26 -> 0x0046294E ?get@Rva0046294EByteChaseField@@QBEEXZ
//   +0x30 slot 27 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x30 slot 28 -> 0x004639D5 ?rva004639D5@Rva004639D5@@UAEHH@Z
//   +0x30 slot 29 -> 0x00464FC7 ?rva00464FC7@SlaughterHordeContain@@UAE_NPAXH@Z
//   +0x30 slot 30 -> 0x0046322C ?rva0046322C@Rva0046322C@@QAEHXZ
//   +0x30 slot 31 -> 0x005DAA6C ?Remove_Sub_Object@RenderObjClass@@UAEHPAV1@@Z
//   +0x30 slot 33 -> 0x0047A69C ?DoXfer@EmissionVelocityInfo@FXParticleSystem@@UAEXAAVXfer@@@Z
//   +0x30 slot 37 -> 0x00462504 ?rva00462504@SlaughterHordeContain@@UAEXXZ
//   +0x30 slot 38 -> 0x00462516 ?rva00462516@SlaughterHordeContain@@QAEXPAX0PAUDamageInfoInput00462516@@@Z
//   +0x30 slot 39 -> 0x00468657 ?rva00468657@Rva00468657@@QAEXPAVObject@@_N@Z
//   +0x30 slot 40 -> 0x0047E4EE ?getContainMax@RiderChangeContain@@UBEHXZ
//   +0x30 slot 41 -> 0x004647F0 ?Rva004647F0Adjust@@YIPAXPAX@Z
//   +0x30 slot 44 -> 0x00465011 ?rva00465011@OpenContainActionIface@@UAEXPAVObject@@@Z
//   +0x30 slot 45 -> 0x00465078 ?rva00465078@OpenContainActionIface@@UAEXW4CommandSourceType@@@Z
//   +0x30 slot 46 -> 0x0047BE23 ?rva0047BE23@Rva0047BDED@@QAEXH@Z
//   +0x30 slot 48 -> 0x00463138 ?rva00463138@OpenContainActionIface@@UAEXXZ
//   +0x30 slot 51 -> 0x00464830 ?addToContain@OpenContain@@UAEXPAVObject@@@Z
//   +0x30 slot 52 -> 0x0047C07F ?addToContainList@SiegeEngineContain@@UAEXPAVObject@@@Z
//   +0x30 slot 53 -> 0x00463509 ?removeFromContain@Rva00463509@@QAEXPAVObject@@_N@Z
//   +0x30 slot 54 -> 0x0047BDED ?removeAllContained@Rva0047BDED@@QAEX_N@Z
//   +0x30 slot 55 -> 0x0046274C ?rva0046274C@SlaughterHordeContain@@UAEXPAXH0@Z
//   +0x30 slot 56 -> 0x0047CAEB ?rva0047CAEB@Rva0047CAEBOwner@@QAEPAVBfmeObject872Header@@PAV2@PAVObject@@@Z
//   +0x30 slot 57 -> 0x00462B4A ?rva00462B4A@Rva00462B4A@@QAE_NXZ
//   +0x30 slot 58 -> 0x0047CB3F ?get@Rva0047CB3FSub@@QBE_NXZ
//   +0x30 slot 59 -> 0x00462B93 ?rva00462B93@SlaughterHordeContain@@UAE_NPBVRva00462B93Arg@@@Z
//   +0x30 slot 60 -> 0x00462BFF ?rva00462BFF@OpenContainActionIface@@UBEDPBVObject@@0@Z
//   +0x30 slot 63 -> 0x00467F2C ?get@Rva00467F2CDwordField@@QBEHXZ
//   +0x30 slot 65 -> 0x0047BAE7 ?rva0047CADB@SiegeEngineContain@@UAEMXZ
//   +0x30 slot 67 -> 0x004628AB ?rva004628AB@OpenContainActionIface@@UAE_NHH@Z
//   +0x30 slot 69 -> 0x005CB9FF ?vslot24_01@AIUpdateInterface@@UAE_NH@Z
//   +0x30 slot 70 -> 0x0047CDCE ?rva004631FA@SiegeEngineContain@@UAE_NH@Z
//   +0x30 slot 71 -> 0x0046731A ?rva0046731A@Rva0046731A@@QAEXPAVObject@@@Z
//   +0x30 slot 72 -> 0x004673C0 ?rva004673C0@TransportContain@@UAEXXZ
//   +0x30 slot 73 -> 0x004632E0 ?getClosestRider@OpenContain@@UAEPAVObject@@PBUCoord3D@@@Z
//   +0x30 slot 75 -> 0x00464810 ?set@Rva00464810ByteSlot@@QAEXE@Z
//   +0x30 slot 76 -> 0x00462735 ?rva00462735@Rva00462735@@QAEXXZ
//   +0x30 slot 77 -> 0x00462CD3 ?get@Rva00462CD3Sub@@QBE_NXZ
//   +0x30 slot 78 -> 0x0047BEF8 ?onContaining@Rva0047BEF8@@UAEXPAVObject@@_N@Z
//   +0x30 slot 79 -> 0x0047BEC9 ?rva0047BEC9@SiegeEngineContain@@QAEXXZ
//   +0x30 slot 80 -> 0x0047BE4C ?iterateContained@Rva0047BE4C@@QAEXP6AXPAVObject@@PAX@Z1H@Z
//   +0x30 slot 81 -> 0x00462DE0 ?rva00462DE0@Rva00462DE0@@QAEHPAVRva2225E0Filter@@@Z
//   +0x30 slot 82 -> 0x004647D8 ?rva004647D8@Rva004647D8@@QAEPAXPAX@Z
//   +0x30 slot 83 -> 0x0047C018 ?rva004624EB@SiegeEngineContain@@UAEXAAURva0046247DPair@@@Z
//   +0x30 slot 85 -> 0x005C4AFD ?vslot143@AIUpdateInterface@@UBE?AW4CommandSourceType@@XZ
//   +0x30 slot 88 -> 0x0050B5F1 ?slot11Command@ChangeStanceGroupOrder@@UAE_NPAHPAUGroupOrderCommand@@@Z
//   +0x30 slot 89 -> 0x0041BA5C ?rva0041BA5C@Rva0041BA5C@@QAE_NHHH@Z
//   +0x30 slot 91 -> 0x00463291 ?rva00463291@OpenContainActionIface@@UAEPAVObject@@PAVRva2225E0Filter@@@Z
//   +0x30 slot 92 -> 0x0047C033 ?get@Rva0047C033ByteField@@QBEEXZ
//   +0x30 slot 93 -> 0x003F3FCE ?get@Rva003F3FCEDwordField@@QBEHXZ
//   +0x30 slot 94 -> 0x004642D3 ?rva004642D3@SlaughterHordeContain@@UAEXXZ
//   +0x30 slot 95 -> 0x004647FD ?set@Rva004647FDByteSlot@@QAEXE@Z
//   +0x30 slot 96 -> 0x0046333B ?rva0046333B@Rva0046333B@@QAE_NPAH00000@Z
//   +0x30 slot 97 -> 0x00464825 ?get@Rva00464825DwordField@@QBEHXZ
//   +0x30 slot 98 -> 0x004626CA ?rva004626CA@OpenContain@@UAEPBUCoord3D@@XZ
//   +0x30 slot 99 -> 0x004626AF ?rva004626AF@OpenContainActionIface@@UAEPBUCoord3D@@XZ
//   +0x30 slot 100 -> 0x00464829 ?get@Rva00464829AddDwordField@@QBEHXZ
//   +0x30 slot 101 -> 0x0046480A ?rva0046480A@Rva0046480A@@QAEHH@Z
//   +0x30 slot 102 -> 0x004625BB ?rva004625BB@SlaughterHordeContain@@UAE_NPAXH@Z
//   +0x30 slot 103 -> 0x004625F0 ?rva004625F0@SlaughterHordeContain@@UAEXPAX@Z
//   +0x30 slot 104 -> 0x00462955 ?rva00462955@OpenContainActionIface@@UAE_NPAVObject@@H@Z
//   +0x30 slot 105 -> 0x0050B238 ?SkipBadBlock@Xfer@@UAEXAAVSnapshot@@I@Z
//   +0x34 slot 0 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x34 slot 6 -> 0x0047C4F2 ?exitObjectViaDoor@SiegeEngineContain@@UAEXPAVObject@@W4ExitDoorType@@@Z
#pragma once
#ifndef BFME2_RIDER_CHANGE_CONTAIN_H
#define BFME2_RIDER_CHANGE_CONTAIN_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SiegeEngineContain.h"

class Thing;
class ModuleData;
class Xfer;

// SiegeEngineContain ends at 0x138; this class's members follow it.
typedef char RiderChangeContain_base_size[(sizeof(SiegeEngineContain) == 0x138) ? 1 : -1];

class RiderChangeContain : public SiegeEngineContain
{
public:
	RiderChangeContain(Thing *thing, const ModuleData *moduleData);
	virtual Bool containSlot09() { return true; }
	virtual NameKeyType getModuleNameKey() const;
	virtual const char *GetSnapshotName() const;
	const char *name() const;						// 0x0047E325 (non-virtual, the literal)
	virtual Int getContainMax() const;				// 0x0047E4EE: the module data's Slots, 0 without module data
	// Retail slot 32 (+0x0) -> 0x0047E344: clears both out-parameters and returns false.
	virtual Bool getContainerPipsToShow(Int &numPips, Int &numMax);

protected:
	virtual void xfer(Xfer *xfer);

public:
	Int m_unk0x138;                                // +0x138; xfer Int
	UnsignedInt m_unk0x13C;                        // +0x13C; xfer UnsignedInt
	Int m_unk0x140;                                // +0x140; constructor store
	Bool m_unk0x144;                               // +0x144; constructor store
};
// SIZE 0x148
typedef char RiderChangeContain_size[(sizeof(RiderChangeContain) == 0x148) ? 1 : -1];
typedef char RiderChangeContain_off_m_unk0x138[(offsetof(RiderChangeContain, m_unk0x138) == 0x138) ? 1 : -1];
typedef char RiderChangeContain_off_m_unk0x13C[(offsetof(RiderChangeContain, m_unk0x13C) == 0x13C) ? 1 : -1];
typedef char RiderChangeContain_off_m_unk0x140[(offsetof(RiderChangeContain, m_unk0x140) == 0x140) ? 1 : -1];
typedef char RiderChangeContain_off_m_unk0x144[(offsetof(RiderChangeContain, m_unk0x144) == 0x144) ? 1 : -1];

#endif
