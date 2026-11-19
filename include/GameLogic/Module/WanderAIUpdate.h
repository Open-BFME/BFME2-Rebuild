// WanderAIUpdate.h: the WanderAIUpdate module. SIZE 0x3E4 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004A931F calls AIUpdateInterface first; it stores the
// tables 0x00C53D30 at +0x0, 0x00BFA3C0 at +0xC, 0x00C53D24 at +0x10, 0x00C47B88 at +0x20, 0x00C52F88 at +0x24. Members are what xfer (0x0049060D) transfers after AIUpdateInterface::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x0049A7EE ?loadPostProcess@WorkerAIUpdate@@MAEXXZ
//   +0x0 slot 2 -> 0x004A9357 ?Rva004A9357Get@@YAHXZ
//   +0x10 slot 0 -> 0x004A9424 ?update@WanderAIUpdate@@UAE?AW4UpdateSleepTime@@XZ
//   +0x10 slot 8 -> 0x002625C8 ?onObjectCreated@AIUpdateInterface@@UAEXXZ
//   +0x10 slot 9 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x10 slot 12 -> 0x0050B238 ?SkipBadBlock@Xfer@@UAEXAAVSnapshot@@I@Z
//   +0x10 slot 13 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
//   +0x10 slot 16 -> 0x0026B487 ?privateMoveToPosition@AIUpdateInterface@@MAEXPBUCoord3D@@MW4CommandSourceType@@@Z
//   +0x10 slot 17 -> 0x0026C04E ?privateAttackMoveToPositionAndOrientation@AIUpdateInterface@@MAEXPBUCoord3D@@MHW4CommandSourceType@@@Z
//   +0x10 slot 18 -> 0x0026B637 ?bfmePrivateCommand01@AIUpdateInterface@@MAEXPAVObject@@W4CommandSourceType@@@Z
//   +0x10 slot 19 -> 0x00267CFA ?privateMoveToPositionSA@AIUpdateInterface@@MAEXPBUCoord3D@@W4CommandSourceType@@@Z
//   +0x10 slot 20 -> 0x0026B567 ?privateMoveToPositionAmphibious@AIUpdateInterface@@MAEXPBUCoord3D@@HW4CommandSourceType@@@Z
//   +0x10 slot 21 -> 0x00264558 ?bfmePrivateCommand48@AIUpdateInterface@@MAEXPAVObject@@W4CommandSourceType@@@Z
//   +0x10 slot 22 -> 0x0026B6B2 ?privateMoveToAndEvacuate@AIUpdateInterface@@MAEXPBUCoord3D@@W4CommandSourceType@@@Z
//   +0x10 slot 23 -> 0x0026B720 ?privateMoveToAndEvacuateAndExit@AIUpdateInterface@@MAEXPBUCoord3D@@W4CommandSourceType@@@Z
//   +0x10 slot 24 -> 0x0026B78E ?bfmePrivateCommand38@AIUpdateInterface@@MAEXPBUCoord3D@@W4CommandSourceType@@@Z
//   +0x10 slot 25 -> 0x0026B7FC ?bfmePrivateCommand41@AIUpdateInterface@@MAEXPBUCoord3D@@W4CommandSourceType@@@Z
//   +0x10 slot 26 -> 0x0026D56C ?bfmePrivateCommand42@AIUpdateInterface@@MAEXPAVObject@@W4CommandSourceType@@@Z
//   +0x10 slot 27 -> 0x0026D5FB ?privateIdle@AIUpdateInterface@@MAEXW4CommandSourceType@@@Z
//   +0x10 slot 28 -> 0x00267DAD ?privateTightenToPosition@AIUpdateInterface@@MAEXPBUCoord3D@@W4CommandSourceType@@@Z
//   +0x10 slot 29 -> 0x0026B8E5 ?privateFollowWaypointPath@AIUpdateInterface@@MAEXPBVWaypoint@@W4CommandSourceType@@@Z
//   +0x10 slot 30 -> 0x0026B9E2 ?privateFollowWaypointPathAsTeam@AIUpdateInterface@@MAEXPBVWaypoint@@W4CommandSourceType@@@Z
//   +0x10 slot 31 -> 0x0026B987 ?privateFollowWaypointPathExact@AIUpdateInterface@@MAEXPBVWaypoint@@W4CommandSourceType@@@Z
//   +0x10 slot 32 -> 0x0026BA33 ?privateFollowWaypointPathAsTeamExact@AIUpdateInterface@@MAEXPBVWaypoint@@W4CommandSourceType@@@Z
//   +0x10 slot 33 -> 0x0026BA84 ?privateFollowPath@AIUpdateInterface@@MAEXPBVRva0035149F@@PAVObject@@W4CommandSourceType@@_N@Z
//   +0x10 slot 34 -> 0x0026BB5A ?bfmePrivateCommand24@AIUpdateInterface@@MAEXPBVRva0035149F@@PAVObject@@MW4CommandSourceType@@@Z
//   +0x10 slot 35 -> 0x0026BC16 ?bfmePrivateCommand25@AIUpdateInterface@@MAEXPBVRva0035149F@@PAVObject@@MW4CommandSourceType@@@Z
//   +0x10 slot 36 -> 0x0026E074 ?privateFollowPathAppend@AIUpdateInterface@@MAEXPBUCoord3D@@W4CommandSourceType@@@Z
//   +0x10 slot 37 -> 0x0026BCD2 ?privateAttackObject@AIUpdateInterface@@MAEXPAVObject@@HW4CommandSourceType@@@Z
//   +0x10 slot 38 -> 0x0026BE86 ?bfmePrivateCommand39@AIUpdateInterface@@MAEXPAVObject@@W4CommandSourceType@@@Z
//   +0x10 slot 41 -> 0x0026BEE6 ?privateForceAttackObject@AIUpdateInterface@@MAEXPAVObject@@HW4CommandSourceType@@@Z
//   +0x10 slot 42 -> 0x0026BFD9 ?privateAttackTeam@AIUpdateInterface@@MAEXPBVTeam@@HW4CommandSourceType@@@Z
//   +0x10 slot 43 -> 0x0026DB1A ?privateAttackPosition@AIUpdateInterface@@MAEXPBUCoord3D@@HW4CommandSourceType@@@Z
//   +0x10 slot 44 -> 0x0026C0D6 ?bfmePrivateCommand49@AIUpdateInterface@@MAEXPAVObject@@W4CommandSourceType@@@Z
//   +0x10 slot 45 -> 0x0026C150 ?privateAttackFollowWaypointPath@AIUpdateInterface@@MAEXPBVWaypoint@@H_NW4CommandSourceType@@@Z
//   +0x10 slot 46 -> 0x002646E9 ?privateHunt@AIUpdateInterface@@MAEXW4CommandSourceType@@@Z
//   +0x10 slot 49 -> 0x0026DD89 ?privateGetHealed@AIUpdateInterface@@MAEXPAVObject@@W4CommandSourceType@@@Z
//   +0x10 slot 50 -> 0x0026DDBB ?privateGetRepaired@AIUpdateInterface@@MAEXPAVObject@@W4CommandSourceType@@@Z
//   +0x10 slot 51 -> 0x00264775 ?privateEnter@AIUpdateInterface@@MAEXPAVObject@@W4CommandSourceType@@@Z
//   +0x10 slot 52 -> 0x002647EF ?bfmePrivateCommand3D@AIUpdateInterface@@MAEXPAVObject@@W4CommandSourceType@@@Z
//   +0x10 slot 53 -> 0x0026472F ?privateMoveToObject@AIUpdateInterface@@MAEXPAVObject@@W4CommandSourceType@@@Z
//   +0x10 slot 54 -> 0x0026481B ?privateDock@AIUpdateInterface@@MAEXPAVObject@@W4CommandSourceType@@@Z
//   +0x10 slot 55 -> 0x00264853 ?privateHarvest@AIUpdateInterface@@MAEXPBUCoord3D@@W4CommandSourceType@@@Z
//   +0x10 slot 56 -> 0x0026488B ?privateExitInstantly@AIUpdateInterface@@MAEXPAVObject@@W4CommandSourceType@@@Z
//   +0x10 slot 57 -> 0x002648F1 ?bfmePrivateCommand4B@AIUpdateInterface@@MAEXPAVObject@@PBUCoord3D@@W4CommandSourceType@@@Z
//   +0x10 slot 58 -> 0x0026492A ?bfmePrivateCommand4D@AIUpdateInterface@@MAEXPAVObject@@PBVRva0035149F@@W4CommandSourceType@@@Z
//   +0x10 slot 59 -> 0x00264963 ?bfmePrivateCommand3E@AIUpdateInterface@@MAEXPAVObject@@W4CommandSourceType@@@Z
//   +0x10 slot 60 -> 0x002649A3 ?bfmePrivateCommand4A@AIUpdateInterface@@MAEXPAVObject@@PBUCoord3D@@W4CommandSourceType@@@Z
//   +0x10 slot 61 -> 0x002649DC ?bfmePrivateCommand4C@AIUpdateInterface@@MAEXPAVObject@@PBVRva0035149F@@W4CommandSourceType@@@Z
//   +0x10 slot 62 -> 0x00264A15 ?bfmePrivateCommand54@AIUpdateInterface@@MAEXPAVObject@@PBUCoord3D@@W4CommandSourceType@@@Z
//   +0x10 slot 63 -> 0x0026DDED ?rva0026DDED@AIUpdateInterface@@UAEXHW4CommandSourceType@@@Z
//   +0x10 slot 64 -> 0x00262F5E ?privateGoProne@AIUpdateInterface@@MAEXPBVDamageInfo@@W4CommandSourceType@@@Z
//   +0x10 slot 65 -> 0x00264CBB ?privateGuardTunnelNetwork@AIUpdateInterface@@MAEXHW4CommandSourceType@@@Z
//   +0x10 slot 66 -> 0x00264BC2 ?privateGuardPosition@AIUpdateInterface@@MAEXPBUCoord3D@@W4GuardMode@@W4CommandSourceType@@@Z
//   +0x10 slot 67 -> 0x00264D0E ?privateGuardObject@AIUpdateInterface@@MAEXPAVObject@@W4GuardMode@@W4CommandSourceType@@@Z
//   +0x10 slot 68 -> 0x00264D81 ?bfmePrivateCommand20@AIUpdateInterface@@MAEXPBURva00264D81Arg@@W4GuardMode@@W4CommandSourceType@@@Z
//   +0x10 slot 69 -> 0x00267DF1 ?privateGuardRetaliate@AIUpdateInterface@@MAEXPAVObject@@PBUCoord3D@@HW4CommandSourceType@@@Z
//   +0x10 slot 70 -> 0x00264DF8 ?privateGuardAreaFromPosition@AIUpdateInterface@@MAEXPBVPolygonTrigger@@W4GuardMode@@W4CommandSourceType@@PBUCoord3D@@@Z
//   +0x10 slot 71 -> 0x0026C1CE ?privateAttackArea@AIUpdateInterface@@MAEXPBVPolygonTrigger@@W4CommandSourceType@@@Z
//   +0x10 slot 72 -> 0x002645B9 ?privateFaceObject@AIUpdateInterface@@MAEXPAVObject@@W4CommandSourceType@@@Z
//   +0x10 slot 73 -> 0x00267D65 ?privateFacePosition@AIUpdateInterface@@MAEXPBUCoord3D@@W4CommandSourceType@@@Z
//   +0x10 slot 74 -> 0x00262EFF ?rva00262EFF@AIUpdateInterface@@UAEXPAX00@Z
//   +0x10 slot 75 -> 0x0026DE77 ?privateCommandButton@AIUpdateInterface@@MAEXPBVCommandButton@@W4CommandSourceType@@@Z
//   +0x10 slot 76 -> 0x00267E8E ?privateCommandButtonPosition@AIUpdateInterface@@MAEXPBVCommandButton@@PBUCoord3D@@W4CommandSourceType@@@Z
//   +0x10 slot 77 -> 0x000D1407 ?Set_Animation@RenderObjClass@@UAEXPAVHAnimClass@@MH@Z
//   +0x10 slot 78 -> 0x00264A4E ?bfmeGoCSG@AIUpdateInterface@@UAEXPAX0@Z
//   +0x10 slot 79 -> 0x00264A86 ?bfmeGoCSA@AIUpdateInterface@@UAEXPAX@Z
//   +0x10 slot 80 -> 0x00264AB2 ?bfmePrivateCommand53@AIUpdateInterface@@MAEXW4CommandSourceType@@@Z
//   +0x10 slot 81 -> 0x00264ADE ?bfmeGoCSH@AIUpdateInterface@@UAEXPAX0@Z
//   +0x10 slot 82 -> 0x00264B16 ?bfmeGoCSD@AIUpdateInterface@@UAEXPAX0@Z
//   +0x10 slot 83 -> 0x00264B86 ?bfmePrivateCommand31@AIUpdateInterface@@MAEXHW4CommandSourceType@@@Z
//   +0x10 slot 84 -> 0x0026D765 ?privateMoveAwayFromUnit@AIUpdateInterface@@MAEXPAVObject@@PBUCoord3D@@W4CommandSourceType@@@Z
//   +0x10 slot 85 -> 0x0026B86A ?bfmePrivateCommand3C@AIUpdateInterface@@MAEXPAVObject@@W4CommandSourceType@@@Z
//   +0x10 slot 86 -> 0x00264B4E ?bfmeGoCSE@AIUpdateInterface@@UAEXPAX0@Z
//   +0x10 slot 87 -> 0x002630FD ?rva002630FD@AIUpdateInterface@@MAEXH@Z
//   +0x10 slot 88 -> 0x00263133 ?bfmeGoCSF@AIUpdateInterface@@UAEXPAX0@Z
//   +0x10 slot 89 -> 0x0026316B ?rva0026316B@AIUpdateInterface@@MAEXPAXH@Z
//   +0x10 slot 90 -> 0x002631A6 ?rva002631A6@AIUpdateInterface@@MAEXMH@Z
//   +0x10 slot 93 -> 0x0047A699 ?IsCRC@Xfer@@UBE_NXZ
//   +0x10 slot 94 -> 0x00262B39 ?isBusy@AIUpdateInterface@@UBE_NXZ
//   +0x10 slot 106 -> 0x0026D478 ?joinTeam@AIUpdateInterface@@UAEXXZ
//   +0x10 slot 107 -> 0x0026E988 ?rva0026E988@AIUpdateInterface@@UAEHXZ
//   +0x10 slot 108 -> 0x0026E999 ?rva0026E999@AIUpdateInterface@@UAEHXZ
//   +0x10 slot 109 -> 0x005DAA6C ?Remove_Sub_Object@RenderObjClass@@UAEHPAV1@@Z
//   +0x10 slot 110 -> 0x0050B5C6 ?vslot10@GameWindow@@UAE_NXZ
//   +0x10 slot 113 -> 0x002645FF ?rva002645FF@Rva002645FF@@QAE_NXZ
//   +0x10 slot 114 -> 0x00262D8F ?rva00262D8F@AIUpdateInterface@@UBE_NXZ
//   +0x10 slot 115 -> 0x00264656 ?isClearingMines@AIUpdateInterface@@UBE_NXZ
//   +0x10 slot 116 -> 0x00262DA3 ?rva00262DA3@AIUpdateInterface@@UBE_NXZ
//   +0x10 slot 117 -> 0x00262DAB ?rva00262DAB@AIUpdateInterface@@UBE_NXZ
//   +0x10 slot 118 -> 0x00262DBF ?rva00262DBF@AIUpdateInterface@@UBE_NXZ
//   +0x10 slot 119 -> 0x00262D7B ?rva00262D7B@AIUpdateInterface@@UBE_NXZ
#pragma once
#ifndef BFME2_WANDER_A_I_UPDATE_H
#define BFME2_WANDER_A_I_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/AIUpdate.h"

class Thing;
class ModuleData;
class Xfer;

// AIUpdateInterface ends at 0x3E4; this class's members follow it.
typedef char WanderAIUpdate_base_size[(sizeof(AIUpdateInterface) == 0x3E4) ? 1 : -1];

class WanderAIUpdate : public AIUpdateInterface
{
public:
	WanderAIUpdate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual ~WanderAIUpdate();
	virtual UpdateSleepTime update();	// override: update slot 0 at +0x10 (0x004A9424)

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);
	virtual AIStateMachine *makeStateMachine();		// 0x004A93C9

public:
protected:
	virtual void loadPostProcess() { AIUpdateInterface::loadPostProcess(); }	// retail slot 0x0049A7EE: a jmp to AIUpdateInterface's, ICF-folded across classes (h05)
};
// SIZE 0x3E4
typedef char WanderAIUpdate_size[(sizeof(WanderAIUpdate) == 0x3E4) ? 1 : -1];

#endif
