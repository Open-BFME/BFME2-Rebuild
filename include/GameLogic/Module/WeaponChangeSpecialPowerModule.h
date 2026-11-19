// WeaponChangeSpecialPowerModule.h: the WeaponChangeSpecialPowerModule module. SIZE 0x34 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004C3FF8 calls SpecialPowerModule first; it stores the
// tables 0x00C5D1B4 at +0x0, 0x00BEFCF8 at +0xC, 0x00C5D150 at +0x10. Members are what xfer (0x004C8A95) transfers after SpecialPowerModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x004C8A2F ?j_004c8a2f@@YAXXZ
//   +0x0 slot 2 -> 0x004C4022 ?Rva004C4022Get@@YAHXZ
//   +0x10 slot 0 -> 0x0049323D ?eq@Rva002687D0@@QBE_NH@Z
//   +0x10 slot 1 -> 0x004934DE ?isReady@SpecialPowerModule@@UBE_NXZ
//   +0x10 slot 2 -> 0x0049355E ?getPercentReady@SpecialPowerModule@@UBEMXZ
//   +0x10 slot 3 -> 0x00251C08 ?rva00251C08@Rva00251C08@@QAE_NXZ
//   +0x10 slot 4 -> 0x00493AB7 ?rva00493AB7@Rva00493AB7@@QAEIXZ
//   +0x10 slot 5 -> 0x004934B8 ?getPowerName@SpecialPowerModule@@UBE?AVAsciiString@@XZ
//   +0x10 slot 6 -> 0x00493236 ?getSpecialPowerTemplate@SpecialPowerModule@@MBEPBVSpecialPowerTemplate@@XZ
//   +0x10 slot 7 -> 0x004933C2 ?onSpecialPowerCreation@SpecialPowerModule@@UAEXXZ
//   +0x10 slot 8 -> 0x0010670B ?setFont@DisplayString@@UAEXPAVGameFont@@@Z
//   +0x10 slot 9 -> 0x00492FC2 ?pauseCountdown@SpecialPowerModule@@UAEX_N@Z
//   +0x10 slot 10 -> 0x004C40BA ?doSpecialPower@WeaponChangeSpecialPowerModule@@UAEXI@Z
//   +0x10 slot 11 -> 0x004C405D ?rva004C405D@WeaponChangeSpecialPowerModule@@UAEXPAXH@Z
//   +0x10 slot 13 -> 0x004C4069 ?doSpecialPowerUsingWaypoints@WeaponChangeSpecialPowerModule@@UAEXPAXMH@Z
//   +0x10 slot 14 -> 0x00494591 ?markSpecialPowerTriggered@SpecialPowerModule@@UAEXPBUCoord3D@@@Z
//   +0x10 slot 15 -> 0x0049369D ?startPowerRecharge@SpecialPowerModule@@UAEXM@Z
//   +0x10 slot 16 -> 0x00493218 ?rva00493218@InvisibilitySpecialPower@@UBE?AVRva002390CB@@XZ
//   +0x10 slot 17 -> 0x00251C2C ?rva00251C2C@Rva00251C2C@@QAEXXZ
//   +0x10 slot 18 -> 0x0049466D ?rva0049466D@SpecialPowerModule@@UAE_NPBUCoord3D@@@Z
//   +0x10 slot 20 -> 0x0030F2C7 ?getData@NetWrapperCommandMsg@@QAEPAEXZ
//   +0x10 slot 21 -> 0x004932D5 ?get@Rva004932D5ByteChaseField@@QBEEXZ
//   +0x10 slot 22 -> 0x0049329E ?rva0049329E@Rva0049329E@@QAEXXZ
//   +0x10 slot 23 -> 0x004BF893 ?get@Rva004BF893LeaField@@QBEPAXXZ
//   +0x10 slot 30 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x10 slot 34 -> 0x0050B238 ?SkipBadBlock@Xfer@@UAEXAAVSnapshot@@I@Z
//   +0x10 slot 35 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
//   +0x10 slot 37 -> 0x00493805 ?rva00493805@SpecialPowerModule@@UAEXXZ
//   +0x10 slot 38 -> 0x00493EA7 ?ApplyPowerToObject@SpecialPowerModule@@UAEXPAVObject@@HPBV?$BitFlags@$0L@@@@Z
#pragma once
#ifndef BFME2_WEAPON_CHANGE_SPECIAL_POWER_MODULE_H
#define BFME2_WEAPON_CHANGE_SPECIAL_POWER_MODULE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialPowerModule.h"

class Object;
class Thing;
class ModuleData;
class Xfer;

// SpecialPowerModule ends at 0x34; this class's members follow it.
typedef char WeaponChangeSpecialPowerModule_base_size[(sizeof(SpecialPowerModule) == 0x34) ? 1 : -1];

class WeaponChangeSpecialPowerModule : public SpecialPowerModule
{
public:
	WeaponChangeSpecialPowerModule(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual void doSpecialPower(unsigned int commandOptions);	// override: doSpecialPower slot 10 at +0x10 (0x004C40BA)
	virtual void doSpecialPowerAtObject(Object *object, unsigned int commandOptions);	// override: slot 11 at +0x10 (0x004C405D: forwards to doSpecialPower; PlayerUpgrade, SplitHorde and CombineHorde tables share the body)
	virtual void doSpecialPowerAtLocation(const Coord3D *location, unsigned int commandOptions) { doSpecialPower(commandOptions); }	// slot 12 at +0x10: retail 0x004C405D, the same body as slot 11
	virtual void doSpecialPowerUsingWaypoints(const Coord3D *location, const Waypoint *waypoint, unsigned int commandOptions);	// override: doSpecialPowerUsingWaypoints slot 13 at +0x10 (0x004C4069)

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);
	virtual ~WeaponChangeSpecialPowerModule();

public:
protected:
	virtual void loadPostProcess() { SpecialPowerModule::loadPostProcess(); }	// retail slot 0x004C8A2F: a jmp to SpecialPowerModule's, ICF-folded across classes (h05)
};
// SIZE 0x34
typedef char WeaponChangeSpecialPowerModule_size[(sizeof(WeaponChangeSpecialPowerModule) == 0x34) ? 1 : -1];

#endif
