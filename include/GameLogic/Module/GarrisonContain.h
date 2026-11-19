// GarrisonContain.h: the GarrisonContain module. SIZE 0x9E0 (ModuleFactory operator new).
// Layout evidence: the constructor 0x00477F06 calls OpenContain first; it stores the
// tables 0x00C461F8 at +0x0, 0x00C43528 at +0xC, 0x00C46560 at +0x10, 0x00C46080 at +0x20, 0x00C46064 at +0x24, 0x00C46F78 at +0x28, 0x00C463D8 at +0x2C, 0x00C46034 at +0x30, 0x00C48B28 at +0x34. Members are what xfer (0x00478260) transfers after OpenContain::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x004783D7 ?rva004783D7@Rva004697E1Contain@@QAEXXZ
//   +0x0 slot 2 -> 0x00478061 ?name@GarrisonContain@@QBEPBDXZ
//   +0x0 slot 5 -> 0x004787B0 ?onObjectCreated@GarrisonContain@@UAEXXZ
//   +0x0 slot 18 -> 0x00479395 ?redeployOccupants@GarrisonContain@@MAEXXZ
//   +0x0 slot 27 -> 0x00479292 ?getObjectGarrisonPointIndex@GarrisonContain@@UAEHW4ObjectID@@@Z
//   +0x10 slot 0 -> 0x00479643 ?update@GarrisonContain@@UAE?AW4UpdateSleepTime@@XZ
//   +0x20 slot 4 -> 0x0050B5C6 ?vslot10@GameWindow@@UAE_NXZ
//   +0x20 slot 7 -> 0x0047840C ?get@Rva0047840CByteChase32Field@@QBEEXZ
//   +0x20 slot 19 -> 0x004780E7 ?rva004780E7@SlaughterHordeContain@@UAEPAVPlayer@@PBV2@@Z
//   +0x20 slot 20 -> 0x00479432 ?recalcApparentControllingPlayer@GarrisonContain@@UAEXXZ
//   +0x20 slot 21 -> 0x004783DC ?setOriginalTeam@Rva004783F4@@UAEXPAVTeam@@@Z
//   +0x20 slot 22 -> 0x00478C2C ?onContaining@GarrisonContain@@UAEXPAVObject@@_N@Z
//   +0x20 slot 23 -> 0x00478D0A ?onRemoving@GarrisonContain@@UAEXPAVObject@@@Z
//   +0x20 slot 38 -> 0x00478699 ?isValidContainerFor@GarrisonContain@@UAE_NPAVObject@@_N1@Z
//   +0x20 slot 42 -> 0x00478231 ?rva00478231@Rva00478231@@QAEX_N@Z
//   +0x20 slot 76 -> 0x00478547 ?calcBestGarrisonPosition@GarrisonContain@@UAE_NPAUCoord3D@@PBU2@@Z
//   +0x20 slot 77 -> 0x004788CE ?attemptBestFirePointPosition@GarrisonContain@@UAE_NPAVObject@@PAVWeapon@@PBUCoord3D@@@Z
//   +0x20 slot 78 -> 0x00478850 ?attemptBestFirePointPosition@GarrisonContain@@UAE_NPAVObject@@PAVWeapon@@0@Z
//   +0x20 slot 100 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x20 slot 102 -> 0x00464120 ?onDelete@OpenContain@@UAEXXZ
//   +0x20 slot 103 -> 0x0050B238 ?SkipBadBlock@Xfer@@UAEXAAVSnapshot@@I@Z
//   +0x20 slot 106 -> 0x0022C4CB ?getUpdatePhase@UpdateModule@@MBE?AW4SleepyUpdatePhase@@XZ
//   +0x20 slot 107 -> 0x00462F75 ?removeFromContainList@OpenContain@@UAEXPAVObject@@@Z
//   +0x20 slot 108 -> 0x005CB9FA ?_bfme_ro_v50@RenderObjClass@@UAE_NH@Z
//   +0x20 slot 110 -> 0x005CB9FF ?vslot24_01@AIUpdateInterface@@UAE_NH@Z
//   +0x20 slot 111 -> 0x00462C72 ?monitorConditionChanges@OpenContain@@MAEXXZ
//   +0x20 slot 113 -> 0x00464EE7 ?rva00464EE7@OpenContain@@UAE_NPAVObject@@@Z
//   +0x20 slot 115 -> 0x0046279C ?doLoadSound@OpenContain@@MAEXXZ
//   +0x20 slot 116 -> 0x0046282B ?doUnloadSound@OpenContain@@MAEXXZ
//   +0x20 slot 118 -> 0x00462E3A ?rva00462E3A@Rva00462E3A@@QAEXPAVObject@@HH@Z
//   +0x2C slot 2 -> 0x0047876A ?onBodyDamageStateChange@GarrisonContain@@UAEXPBVDamageInfo@@W4BodyDamageType@@1@Z
//   +0x2C slot 3 -> 0x00479CF6 ?rva00479CF6@Rva00479CF6@@QAEXH@Z
//   +0x2C slot 4 -> 0x0046261B ?rva0046261B@SlaughterHordeContain@@UAEX_N@Z
//   +0x2C slot 5 -> 0x004647CD ?get@Rva004647CDLeaField@@QBEPAXXZ
//   +0x2C slot 6 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
//   +0x2C slot 7 -> 0x004624E3 ?containReactToTransformChange@OpenContain@@UAEXXZ
//   +0x2C slot 9 -> 0x0047A699 ?IsCRC@Xfer@@UBE_NXZ
//   +0x2C slot 18 -> 0x0046294E ?get@Rva0046294EByteChaseField@@QBEEXZ
//   +0x2C slot 20 -> 0x004639D5 ?rva004639D5@Rva004639D5@@UAEHH@Z
//   +0x2C slot 21 -> 0x00464FC7 ?rva00464FC7@SlaughterHordeContain@@UAE_NPAXH@Z
//   +0x2C slot 22 -> 0x0046322C ?rva0046322C@Rva0046322C@@QAEHXZ
//   +0x2C slot 28 -> 0x0046481D ?rva0050B238@Rva005E62F1@@WCA@AEXPAXI@Z
//   +0x2C slot 29 -> 0x00462504 ?rva00462504@SlaughterHordeContain@@UAEXXZ
//   +0x2C slot 30 -> 0x00479C0C ?rva00479C0C@HordeGarrisonContain@@UAEXPAVObject@@PAXPAUDamageInfoInput00462516@@@Z
//   +0x2C slot 31 -> 0x00479C2A ?rva0050B238@HordeGarrisonContain@@UAEXPAVObject@@_N@Z
//   +0x2C slot 32 -> 0x00462745 ?get@Rva00462745PtrChaseField@@QBEHXZ
//   +0x2C slot 33 -> 0x004647F0 ?Rva004647F0Adjust@@YIPAXPAX@Z
//   +0x2C slot 36 -> 0x00479BF3 ?rva00465011@HordeGarrisonContain@@UAEXPAVObject@@@Z
//   +0x2C slot 37 -> 0x00465078 ?rva00465078@OpenContainActionIface@@UAEXW4CommandSourceType@@@Z
//   +0x2C slot 38 -> 0x0047A69C ?DoXfer@EmissionVelocityInfo@FXParticleSystem@@UAEXAAVXfer@@@Z
//   +0x2C slot 40 -> 0x00463138 ?rva00463138@OpenContainActionIface@@UAEXXZ
//   +0x2C slot 42 -> 0x00479E62 ?isValidContainerFor@HordeGarrisonContain@@UAE_NPAVObject@@_N1@Z
//   +0x2C slot 43 -> 0x00479D6D ?addToContain@HordeGarrisonContain@@UAEXPAVObject@@@Z
//   +0x2C slot 44 -> 0x004638F1 ?addToContainList@OpenContain@@UAEXPAVObject@@@Z
//   +0x2C slot 45 -> 0x00479DCD ?rva00479DCD@Rva00479DCD@@QAEXPAVObject@@H@Z
//   +0x2C slot 47 -> 0x0046274C ?rva0046274C@SlaughterHordeContain@@UAEXPAXH0@Z
//   +0x2C slot 48 -> 0x00462CE1 ?rva00462CE1@SlaughterHordeContain@@UAEPAVBfmeObject872Header@@PAV2@H@Z
//   +0x2C slot 51 -> 0x00462B93 ?rva00462B93@SlaughterHordeContain@@UAE_NPBVRva00462B93Arg@@@Z
//   +0x2C slot 52 -> 0x00462BFF ?rva00462BFF@OpenContainActionIface@@UBEDPBVObject@@0@Z
//   +0x2C slot 57 -> 0x004647D1 ?Rva004647D1Get@@YAMXZ
//   +0x2C slot 59 -> 0x004628AB ?rva004628AB@OpenContainActionIface@@UAE_NHH@Z
//   +0x2C slot 62 -> 0x004631FA ?rva004631FA@SlaughterHordeContain@@UAE_NH@Z
//   +0x2C slot 65 -> 0x00479BDD ?rva004632E0@HordeGarrisonContain@@UAEXPAVObject@@@Z
//   +0x2C slot 67 -> 0x00464810 ?set@Rva00464810ByteSlot@@QAEXE@Z
//   +0x2C slot 68 -> 0x00462735 ?rva00462735@Rva00462735@@QAEXXZ
//   +0x2C slot 69 -> 0x00462CD3 ?get@Rva00462CD3Sub@@QBE_NXZ
//   +0x2C slot 70 -> 0x00463191 ?onContaining@TransportContain@@UAEXPAVObject@@_N@Z
//   +0x2C slot 71 -> 0x004631C9 ?rva004631C9@SlaughterHordeContain@@UAEXXZ
//   +0x2C slot 72 -> 0x004635EC ?rva004635EC@SlaughterHordeContain@@UAEXP6AXPAVObject@@PAX@Z1H@Z
//   +0x2C slot 73 -> 0x00462DE0 ?rva00462DE0@Rva00462DE0@@QAEHPAVRva2225E0Filter@@@Z
//   +0x2C slot 74 -> 0x004647D8 ?rva004647D8@Rva004647D8@@QAEPAXPAX@Z
//   +0x2C slot 75 -> 0x004624EB ?rva004624EB@Rva004624EB@@UAE?AURva00462465@@XZ
//   +0x2C slot 77 -> 0x005C4AFD ?vslot143@AIUpdateInterface@@UBE?AW4CommandSourceType@@XZ
//   +0x2C slot 83 -> 0x00463291 ?rva00463291@OpenContainActionIface@@UAEPAVObject@@PAVRva2225E0Filter@@@Z
//   +0x2C slot 85 -> 0x003F3FCE ?get@Rva003F3FCEDwordField@@QBEHXZ
//   +0x2C slot 86 -> 0x004642D3 ?rva004642D3@SlaughterHordeContain@@UAEXXZ
//   +0x2C slot 87 -> 0x004647FD ?set@Rva004647FDByteSlot@@QAEXE@Z
//   +0x2C slot 88 -> 0x0046333B ?rva0046333B@Rva0046333B@@QAE_NPAH00000@Z
//   +0x2C slot 89 -> 0x00464825 ?get@Rva00464825DwordField@@QBEHXZ
//   +0x2C slot 90 -> 0x00479F29 ?rva004626CA@HordeGarrisonContain@@UAEPBUCoord3D@@XZ
//   +0x2C slot 91 -> 0x00479F62 ?rva004626AF@HordeGarrisonContain@@UAEPBUCoord3D@@XZ
//   +0x2C slot 92 -> 0x00479F9B ?rva00464829@HordeGarrisonContain@@UAEPBUCoord3D@@XZ
//   +0x2C slot 93 -> 0x0046480A ?rva0046480A@Rva0046480A@@QAEHH@Z
//   +0x2C slot 94 -> 0x004625BB ?rva004625BB@SlaughterHordeContain@@UAE_NPAXH@Z
//   +0x2C slot 95 -> 0x004625F0 ?rva004625F0@SlaughterHordeContain@@UAEXPAX@Z
//   +0x2C slot 96 -> 0x00462955 ?rva00462955@OpenContainActionIface@@UAE_NPAVObject@@H@Z
//   +0x2C slot 99 -> 0x00253376 ?getDisabledTypesToProcess@UpdateModule@@UBE?AV?$BitFlags@$0L@@@XZ
//   +0x2C slot 100 -> 0x0044DF8D ?rva0044DF8D@UpdateModule@@UAEXW4UpdateSleepTime@@@Z
//   +0x30 slot 2 -> 0x0047983A ?exitObjectViaDoor@GarrisonContain@@UAEXPAVObject@@W4ExitDoorType@@@Z
//   +0x30 slot 12 -> 0x004628D1 ?rva004628D1@Rva004628D1@@UAEXPAVObject@@HH@Z
// Not established: transfers xfer makes that this view cannot place: slot10 ?, slot4 +0xFC, UnsignedShort at misaligned +0x9DD, UnsignedInt at misaligned +0x9DD, UnsignedInt at misaligned +0x9DD.
#pragma once
#ifndef BFME2_GARRISON_CONTAIN_H
#define BFME2_GARRISON_CONTAIN_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/OpenContain.h"
#include "Common/Coord3D.h"

class Thing;
class ModuleData;
class Xfer;

// OpenContain ends at 0xFC; this class's members follow it.
typedef char GarrisonContain_base_size[(sizeof(OpenContain) == 0xFC) ? 1 : -1];

class Object;
class Weapon;
class GarrisonContain : public OpenContain
{
public:
	GarrisonContain(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual const char *GetSnapshotName() const;
	const char *name() const;						// 0x00478061 (non-virtual, the literal)
	virtual ~GarrisonContain();
	virtual UpdateSleepTime update();	// override: update slot 0 at +0x10 (0x00479643)

	// Overrides of OpenContain's virtuals (the retail tables carry this class's own bodies).
	virtual Bool containSlot50() { return true; }
	virtual Bool calcBestGarrisonPosition(Coord3D *sourcePos, const Coord3D *targetPos);	// 0x00478547
	virtual bool isGarrisonable() const { return true; }				// ContainModuleInterface 4: the shared `mov al,1` 0x0050B5C6 (retail's table overrides OpenContain's `xor al,al`)
	virtual Bool isImmuneToClearBuildingAttacks() const;					// ContainModuleInterface 7, 0x0047840C (module data +0xA1)
	virtual Bool attemptBestFirePointPosition(Object *source, Weapon *weapon, Object *victim);	// ContainModuleInterface 78, 0x00478850
	virtual Bool attemptBestFirePointPosition(Object *source, Weapon *weapon, const Coord3D *targetPos);	// ContainModuleInterface 77, 0x004788CE
	virtual Bool isValidContainerFor(Object *obj, Bool checkCapacity, Bool flag);	// 0x00478699
	virtual void onBodyDamageStateChange(const DamageInfo *damageInfo, BodyDamageType oldState, BodyDamageType newState);	// 0x0047876A
	virtual void onObjectCreated();									// 0x004787B0
	virtual void onRemoving(Object *obj);								// 0x00478D0A
	virtual void recalcApparentControllingPlayer();						// 0x00479432
	virtual void exitObjectViaDoor(Object *exitObj, ExitDoorType exitDoor);	// 0x0047983A
	Bool rva00478629(Object *obj, Bool checkCapacity, Bool flag);			// 0x00478629 (the map's placeholder name; calls OpenContain::isValidContainerFor then tests the body)

	// New virtual of this class: slot 27 of the primary table (OpenContain's own end at 26).
	virtual Int getObjectGarrisonPointIndex(ObjectID id);				// 0x00479292

protected:
	enum { MAX_GARRISON_POINTS = 40 };
	enum { GARRISON_INDEX_INVALID = -1, SEARCH_FOR_REMOVE = -1 };
	enum { MAX_GARRISON_POINT_CONDITIONS = 3 };

	virtual const Player *getApparentControllingPlayer(const Player *observingPlayer) const;	// 0x004780E7 (the map spells it rva004780E7@SlaughterHordeContain; slot 19 of Garrison's ContainModuleInterface table)
	Team *getOriginalTeam();													// 0x004783F4 (map: rva004783F4@Rva004783F4): looks m_originalTeamID up in TheTeamFactory
	virtual void removeAllContained(Bool exposeStealthUnits);					// 0x00478231 (the map spells it Rva00478231::rva00478231(bool); it is slot 42 of Garrison's ContainModuleInterface table)
	void healObjects();															// 0x00478BE0
	void moveObjectsWithMe();													// 0x00478FD7
	virtual void redeployOccupants();										// 0x00479395
	void healSingleObject(Object *obj, Real framesForFullHeal);				// 0x004786F8
	void removeInvalidObjectsFromGarrisonPoints();							// 0x0047933C
	void addValidObjectsToGarrisonPoints();									// 0x0047894C
	void trackTargets();													// 0x00478ADB
	void validateRallyPoint();												// 0x00478141
	void loadGarrisonPoints();												// 0x0047901B
	Int findConditionIndex();					// 0x00477E50 (protected, non-virtual)
	Int findClosestFreeGarrisonPointIndex(Int conditionIndex, const Coord3D *targetPos);	// 0x00478445
	void putObjectAtGarrisonPoint(Object *obj, ObjectID targetID, Int conditionIndex, Int pointIndex);	// 0x00477DA1
	void removeObjectFromGarrisonPoint(Object *obj, Int index);			// 0x00477E82
	void putObjectAtBestGarrisonPoint(Object *obj, Object *target, const Coord3D *targetPos);	// 0x004785A2
	virtual void xfer(Xfer *xfer);
	virtual void loadPostProcess() { OpenContain::loadPostProcess(); }	// retail slot 1 0x004783D7: a jmp to OpenContain's 0x00463C6E

public:
	// One garrison point's occupant (xfer 0x00478260 walks the first 0x28 of these: object id, target id,
	// two unsigned ints, a drawable id). putObjectAtGarrisonPoint 0x00477DA1 and removeObjectFromGarrisonPoint
	// 0x00477E82 write the first four; this version of the game keeps ids, not pointers.
	struct GarrisonPointData
	{
		ObjectID objectID;							// +0x00
		ObjectID targetID;							// +0x04
		UnsignedInt placeFrame;						// +0x08
		UnsignedInt lastEffectFrame;				// +0x0C
		Int effectID;								// +0x10 a DrawableID; xfer passes it to XferDrawableID(Xfer *, int *)
	};

	UnsignedInt m_originalTeamID;					// +0xFC xfer UnsignedInt; checked against TheTeamFactory on load (0x00478260)
	GarrisonPointData m_garrisonPointData[MAX_GARRISON_POINTS];	// +0x100, 0x14 each; xfer 0x00478260's loop
	Int m_garrisonPointsInUse;						// +0x420 xfer Int; putObjectAtGarrisonPoint ++, removeObjectFromGarrisonPoint --
	Coord3D m_garrisonPoint[MAX_GARRISON_POINT_CONDITIONS][MAX_GARRISON_POINTS];	// +0x424; xfer walks 3 x 0x28 Coord3D; index conditionIndex * 40 + point
	Int m_garrisonPointCount[MAX_GARRISON_POINT_CONDITIONS];	// +0x9C4 zeroed by the constructor; findClosestFreeGarrisonPointIndex 0x00478445 reads [conditionIndex]
	Coord3D m_exitRallyPoint;						// +0x9D0 xfer Coord3D (last)
	Bool m_garrisonPointsInitialized;				// +0x9DC xfer Bool
	Bool m_hideGarrisonedStateFromNonallies;		// +0x9DD xfer Bool (first)
	Bool m_rallyValid;								// +0x9DE xfer Bool
public:
	virtual void onContaining(Object *obj, Bool wasSelected);	// 0x00478C2C, ContainModuleInterface slot 22; retail's table has this class's own
};
// SIZE 0x9E0
typedef char GarrisonContain_size[(sizeof(GarrisonContain) == 0x9E0) ? 1 : -1];
typedef char GarrisonContain_off_m_originalTeamID[(offsetof(GarrisonContain, m_originalTeamID) == 0xFC) ? 1 : -1];
typedef char GarrisonContain_off_m_garrisonPointData[(offsetof(GarrisonContain, m_garrisonPointData) == 0x100) ? 1 : -1];
typedef char GarrisonContain_off_m_garrisonPointsInUse[(offsetof(GarrisonContain, m_garrisonPointsInUse) == 0x420) ? 1 : -1];
typedef char GarrisonContain_off_m_garrisonPoint[(offsetof(GarrisonContain, m_garrisonPoint) == 0x424) ? 1 : -1];
typedef char GarrisonContain_off_m_garrisonPointCount[(offsetof(GarrisonContain, m_garrisonPointCount) == 0x9C4) ? 1 : -1];
typedef char GarrisonContain_off_m_exitRallyPoint[(offsetof(GarrisonContain, m_exitRallyPoint) == 0x9D0) ? 1 : -1];
typedef char GarrisonContain_off_m_garrisonPointsInitialized[(offsetof(GarrisonContain, m_garrisonPointsInitialized) == 0x9DC) ? 1 : -1];
typedef char GarrisonContain_off_m_hideGarrisonedStateFromNonallies[(offsetof(GarrisonContain, m_hideGarrisonedStateFromNonallies) == 0x9DD) ? 1 : -1];
typedef char GarrisonContain_off_m_rallyValid[(offsetof(GarrisonContain, m_rallyValid) == 0x9DE) ? 1 : -1];

#endif
