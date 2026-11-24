// Script action execution and the subsystem state shared by every action.
#pragma once
#include <stddef.h>
#include "Common/SubsystemInterface.h"
class AsciiString;
class Parameter;
class Object;
class SpecialPowerTemplate;
struct Coord3D;
struct RGBColor;
class ScriptAction;
class Xfer;
// The base of ScriptActions: its own table is 0x0081FD98 (slot 0 the out-of-line destructor 0x003BA6B4,
// slots 1/9/10 and 14..17 pure), over SubsystemInterface. ScriptActions' ctor 0x003BD56C stores its own table
// 0x00C1FE48 after SubsystemInterface() and its destructor 0x003BD582 restores this base's table 0x00C1FD98.
// Slots 14..17 are ScriptActions' own virtuals (0x003CA4BE, 0x003BA8C4, 0x003BCC37, 0x003C35F7).
class ScriptActionsInterface : public SubsystemInterface {
public:
  ScriptActionsInterface() {}
  virtual ~ScriptActionsInterface() {}	// slot 0, 0x003BA6B4
  virtual void executeAction(ScriptAction *) = 0;	// slot 14
  virtual void setSuppressNewWindows(bool) = 0;	// slot 15 (map: setSuppressNewWindows@ScriptActionsRva003BA8C4)
  virtual void rva003BCC37(bool) = 0;	// slot 16
  virtual void rva003C35F7(int, int) = 0;	// slot 17
};
class ScriptActions : public ScriptActionsInterface {
public:
  void doRotateCameraToAngle(Real degrees, Real sec, Real easeIn, Real easeOut);	// 0x003BAF1D
  void doMoveCameraAlongWaypointPath(const AsciiString &pathLabel, Real sec, Real cameraStutterSec, Real easeIn, Real easeOut, Real unk, Int mode);	// 0x003BB0C7
  void doRadarCreateEvent(Coord3D *position, void *eventType);	// 0x003BB76E
  void doStopPlayingSound(const AsciiString &soundName);	// 0x003BC510
  ScriptActions();
  virtual ~ScriptActions();	// slot 0, 0x003BD582 (deleting 0x003BE5AD)
  virtual void init() { reset(); }
  virtual void reset();	// slot 9, 0x003BA8AC
  virtual void update() {}	// slot 10 (the shared empty body 0x000B3FD0)
  virtual void executeAction(ScriptAction *);	// slot 14, 0x003CA4BE
  virtual void setSuppressNewWindows(bool);	// slot 15, 0x003BA8C4
  virtual void rva003BCC37(bool);	// slot 16, 0x003BCC37
  virtual void rva003C35F7(int, int);	// slot 17, 0x003C35F7
  // Non-virtual action methods (public, QAE); the comment is the retail RVA.
  void doPlayerHunt(const AsciiString &name);
  void doRevealMapEntire(const AsciiString &name);
  void doShroudMapEntire(const AsciiString &name);
  void doPlayerDisableBaseConstruction(const AsciiString &name);
  void doPlayerDisableUnitConstruction(const AsciiString &name);
  void doPlayerEnableBaseConstruction(const AsciiString &name);
  void doPlayerEnableUnitConstruction(const AsciiString &name);
  void doPlayerGarrisonAllBuildings(const AsciiString &name);
  void doPlayerExitAllBuildings(const AsciiString &name);
  void doNamedDelete(Parameter *unit);
  void doNamedKill(Parameter *unit);
  void doEnableInput();
  void doRadarRefresh();
  void doTeamHunt(const AsciiString &team);
  void doTeamKill(const AsciiString &team);
  void doNamedSetStoppingDistance(Parameter *unit, float stoppingDistance);
  void doNamedDamage(const Parameter &unit, int amount);
  void doNamedSetSpecialPowerCountdown(Parameter *unit, const AsciiString &specialPower, int seconds);
  void doNamedAddSpecialPowerCountdown(Parameter *unit, const AsciiString &specialPower, int seconds);
  void doTeamStopSequentialScript(const AsciiString &team);
  void doUnitStopSequentialScript(Parameter *unit);
  void doSetMoney(const AsciiString &name, int amount);
  void doGiveMoney(const AsciiString &name, int amount);
  void doDisableSpecialPowerDisplay();
  void doEnableSpecialPowerDisplay();
  void doNamedHideSpecialPowerDisplay(Parameter *unit);
  void doNamedShowSpecialPowerDisplay(Parameter *unit);
  void doMoviePlayRadar(const AsciiString &movieName);
  void doAudioSetVolume(int whichToAffect, float newVolumeLevel);
  void doDamageTeamMembers(const AsciiString &team, float amount);
  void doModifyBuildableStatus(const AsciiString &objectType, int buildableStatus);
  void doRotateCamera(float rotations, float sec, float easeIn, float easeOut);
  void doStopCameraFollowUnit();
  void doFreezeTime();
  void doUnfreezeTime();
protected:
  void doNamedCustomColor(Parameter *unit, int color);
  void doTeamExitAll(const AsciiString &team);
  void doUnitForceEmotion(Parameter *unit, int index, float value);
  void doTeamForceEmotion(Parameter *team, int index, float value);
  void doTeamAttackArea(const AsciiString &team, const AsciiString &area);
  void doTeamAttackTeam(const AsciiString &team, const AsciiString &victim);
  void doSetTeamState(const AsciiString &team, const AsciiString &state);
  void updateTeamSetAttitude(const AsciiString &team, Int attitude);
  void doSetUnitReference(const AsciiString &reference, Parameter *unit, bool byName);
  void doTeamIdleForFramecount(const AsciiString &team, int framecount, bool seconds);
  void doTeamSetRepulsor(const AsciiString &team, bool repulsor);
  void doDisplayText(const AsciiString &displayText);
  void doTeamSpinForFramecount(const AsciiString &team, int framecount);
  void doIdleAllPlayerUnits(const AsciiString &name);
  void doSetStoppingDistance(const AsciiString &team, float stoppingDistance);
  void doBorderSwitch(int borderToUse);
  void doResumeSupplyTruckingForIdleUnits(const AsciiString &name);
  void doSetUnitReferenceToHomeBaseOfPlayer(const AsciiString &playerName, const AsciiString &unitReference, bool flag);
  void doSetCounterToPlayerOwnershipOfUnitsWithModelCondition(const AsciiString &playerName, const AsciiString &kindName, const AsciiString &counterName);
  void doUnitStandGround(Parameter *unit, Parameter *standGround);	// (IAE; the unit, and for a horde each member)
  void doTeamStandGround(Parameter *team, Parameter *standGround);
public:
  void deleteAllUnmanned();	// 0x003C1FAD (map placeholder ?Rva003C1FADDestroy@ScriptActions@@QAEXXZ; Zero Hour's name and body: destroys every object disabled DISABLED_UNMANNED)
  bool m_suppressNewWindows;
public:
  void doSetDrawIconUIMode(Bool);	// 0x003BCA7B
  void doSoundEnableAll();	// 0x003BC4FF
  void doSoundRestoreAllVolumeOverrides();	// 0x003BC566
  void doSoundClearEnvelopeExclusions();	// 0x003BA9E5
  void doClearReverbRoomType();	// 0x003BB641
  void doClearAudioVolumeAreas();	// 0x003BB654
  void doSetOcclusionMode(Bool);	// 0x003BCA68
  void doSetDynamicLODMode(Bool);	// 0x003BCA8E
  void doEnableScoring(Bool);	// 0x003BB700
public:
  void doEnableAudioEvent(const AsciiString &);	// 0x003BC4D1
  void doDisableAudioEvent(const AsciiString &);	// 0x003BC4E8
  void doRemoveAudioEventVolumeOverride(const AsciiString &);	// 0x003BC54F
  void doExcludeAudioEventFromVolumeEnvelope(const AsciiString &);	// 0x003BA9AF
  void doIncludeAudioEventInVolumeEnvelope(const AsciiString &);	// 0x003BA9CA
  void doAmbientSoundsPause(Bool);	// 0x003BBC61
public:
  void doAudioSetVolumeEnvelope(Real, Real, Real, Real, Real);	// 0x003BA96F
  void doPlayerGrantScience(const AsciiString &, const AsciiString &);	// 0x003BC6D9
  void doPlayerPurchaseScience(const AsciiString &, const AsciiString &);	// 0x003BC72F
public:
  void doSetAudioVolumeForArea(const AsciiString &, Real);	// 0x003C004D
  void doRemoveAudioVolumeForArea(const AsciiString &);	// 0x003C0091
protected:
public:
  void doTeamGuardObject(const AsciiString &, Parameter *);	// 0x003BF7AC
public:
  void doNamedHunt(Parameter *);	// 0x003C8A95
  void doUnitExitBuilding(Parameter *);	// 0x003C90B1
public:
  void doNamedGuardNamed(Parameter *, Parameter *);	// 0x003C89D1
protected:
  void doTeamSetObjectStatus(Parameter *, ObjectStatusTypes, Bool);	// 0x003C38EF
public:
  void doTeamExitAllBuildings(const AsciiString &);	// 0x003C8E2B
protected:
  void doTeamAvailableForRecruitment(const AsciiString &, Bool);
protected:
  void doResetMusicScripting(Bool);
protected:
  void doTeamSetRecruitableFlags(const AsciiString &, Int);	// 0x003C0F09
public:
  void doNamedApplyAttackPrioritySet(Parameter *, const AsciiString &);	// 0x003BB1FF
public:
  void doTeamApplyAttackPrioritySet(const AsciiString &, const AsciiString &);	// 0x003BEDD1
public:
  void doTeamUseCommandButtonAbility(const AsciiString &, const AsciiString &);	// 0x003C075B
  void doTeamUseCommandButtonAbilityOnNamed(const AsciiString &teamName, const AsciiString &abilityName, const AsciiString &targetName);	// 0x003C07C1 (TEAM_USE_COMMANDBUTTON_ABILITY_ON_NAMED)
  void doTeamUseCommandButtonAbilityAtWaypoint(const AsciiString &teamName, const AsciiString &abilityName, const AsciiString &waypointName);	// 0x003C0849 (TEAM_USE_COMMANDBUTTON_ABILITY_AT_WAYPOINT)
public:
  void doDisplayEffect4(Real a, Real b, Real c, Real d);	// 0x003BA9FA
  void doDisplayEffect1(Real a);	// 0x003BAA2D
  void doNamedSetAttitude(Parameter *unit, Int attitude);	// 0x003BB274
  void doSoundOverrideVolume(const AsciiString &soundEventName, Real newVolume);	// 0x003BC524
public:
  void doTeamIncreasePriority(const AsciiString &teamName);	// 0x003BFAF4
  void doTeamIncreasePriorityBy(const AsciiString &teamName, Int amount);	// 0x003BFB8C
  void doTeamDecreasePriority(const AsciiString &teamName);	// 0x003BFC27
  void doTeamDecreasePriorityBy(const AsciiString &teamName, Int amount);	// 0x003BFCBF
public:
  void doSetCounterToPlayersMoney(Parameter *pPlayerParm, Parameter *pCounterParm);	// 0x003C2BC0
  void doSetCounterToUnitExperience(Parameter *pUnitParm, Parameter *pCounterParm);	// 0x003C2C8F
protected:
  void doCounterMathCounter(Parameter *, Parameter *, Parameter *);
  void doCounterMathValue(Parameter *, Parameter *, Parameter *);
public:
  void doPlayerSetScienceAvailability(const AsciiString &playerName, const AsciiString &scienceName, const AsciiString &availability);	// 0x003BC785
  void doPlayerGrantUpgrade(const AsciiString &playerName, const AsciiString &upgradeName);	// 0x003BB1A4
public:
  void doSetGameLogicFlag9F(Bool value);	// 0x003BC8F0
  void doSetGameLogicTimerSeconds(Real seconds);	// 0x003BCAA1
public:
  void doNamedSetHeld(Parameter *unit, Bool held);	// 0x003BBF23
protected:
  void doTeamSetOverrideRelationToTeam(const AsciiString &, const AsciiString &, Object *);
public:
  void doTeamSetOverrideRelationToPlayer(const AsciiString &, const AsciiString &, Object *);
public:
  void doTeamUseCommandButtonOnNamed(const AsciiString &teamName, const AsciiString &commandButtonName, Parameter *unit);	// 0x003C1020
  void doTeamGuardTeam(const AsciiString &teamName, const AsciiString &targetTeamName);	// 0x003C2A29
protected:
  void changeObjectPanelFlagForSingleObject(Object *obj, const AsciiString &flagToChange, Bool newVal);	// 0x003C5C69
  void doAffectObjectPanelFlagsUnit(Parameter *unit, const AsciiString &flagName, Bool enable);
public:
  void doSetCounterToPlayerCommandPointCap(Parameter *pPlayerParm, Parameter *pCounterParm);	// 0x003C2D90
protected:
  void doTeamGuard(const AsciiString &);
protected:
  void doTeamGuardInTunnelNetwork(const AsciiString &);
  void doTeamGuardForFramecount(const AsciiString &, Int, Bool);
protected:
protected:
public:
  void doTeamWanderInPlace(const AsciiString &teamName);	// 0x003C8B62
public:
  void doPlayerSellEverything(const AsciiString &playerName);
  void doPlayerKill(const AsciiString &playerName);
  void doScreenShake(Int intensity);
  void doNamedFlash(Parameter *unit, Int timeInSeconds, RGBColor *color);	// 0x003BB5B7 (NAMED_FLASH, TEAM_FLASH and NAMED_FLASH_WHITE share it)
protected:
  void doCameoFlash(const AsciiString &buttonName, Int timeInSeconds);
  void doTeamEnterNamed(const AsciiString &teamName, const AsciiString &unitDestName);
public:
  void doTeamGuardPosition(const AsciiString &teamName, const AsciiString &waypointName);	// 0x003BF71E (TEAM_GUARD_POSITION)
  void doTeamGuardArea(const AsciiString &teamName, const AsciiString &areaName, const AsciiString &waypointName);	// 0x003BF813 (TEAM_GUARD_AREA)
protected:
  void doTeamFollowWaypoints(const AsciiString &teamName, const AsciiString &waypointPathLabel, Bool asTeam, Bool inFormation);
  void doTeamAttackMoveFollowWaypoints(const AsciiString &teamName, const AsciiString &waypointPathLabel, Bool asTeam, Bool inFormation);
public:
  void doTeamWander(const AsciiString &teamName, const AsciiString &pathLabel);	// 0x003C8ABF (TEAM_WANDER)
  void doTeamPanic(const AsciiString &teamName, const AsciiString &pathLabel);	// 0x003C8BCB (TEAM_PANIC)
  void doMoviePlayFullScreen(const AsciiString &movieName, Bool unused);	// 0x003BFFEC (MOVIE_PLAY_FULLSCREEN)
protected:
  void doNamedEnableCameraFading(Parameter *unit, Bool enable);
  void doTeamRemoveOverrideRelationToTeam(const AsciiString &, const AsciiString &);
  void doTeamRemoveAllOverrideRelations(const AsciiString &);
public:
  void doSetCounterToPlayerCommandPointsInUse(Parameter *pPlayerParm, Parameter *pCounterParm);	// 0x003C2CD8
protected:
  void doNamedExitAll(Parameter *unit);
  void doNamedGuard(Parameter *unit);
  void doNamedFaceNamed(Parameter *unit, const AsciiString &targetName);
  void doNamedFaceWaypoint(Parameter *unit, const AsciiString &waypointName);
  void doNamedAttackArea(Parameter *unit, const AsciiString &areaName);
  void doNamedAttackAreaForSeconds(Parameter *unit, const AsciiString &areaName, Int seconds);
  void doNamedAttackTeam(Parameter *unit, const AsciiString &teamName);
  void doNamedEnterNamed(Parameter *unit, const AsciiString &transportName);
  void doUnitGuardForFramecount(Parameter *unit, Int framecount, Bool seconds);
  void doUnitIdleForFramecount(Parameter *unit, Int framecount, Bool seconds);
  void doNamedSetRepulsor(Parameter *unit, Bool repulsor);
  void doNamedFollowWaypoints(Parameter *unit, const AsciiString &pathLabel);
  void doUnitGuardPosition(Parameter *unit, const AsciiString &waypointName);
public:
  void doModCameraLookToward(const AsciiString *waypointName);	// 0x003C4D5C (CAMERA_MOD_LOOK_TOWARD)
  void doPlayerDisableFactories(const AsciiString &playerName, const AsciiString &objectList);	// (Player::setObjectsEnabled(list, false) on every player of the mask)
  void doPlayerEnableFactories(const AsciiString &playerName, const AsciiString &objectList);
  void doTeamGrantUpgrade(const AsciiString &teamName, const AsciiString &upgradeName);	// 0x003C2121 (role name; executeAction case 403 passes the team and upgrade strings; Team::rva0039DB31 applies the upgrade to the members; map placeholder Rva003C2121Grant)
protected:
  void doTeamEnableStealth(const AsciiString &teamName, Bool enable);	// (script status bit 8 on every member, set to !enable)
  void doTeamSetScriptStatus20(const AsciiString &teamName, Bool value);	// 0x003C05DE (map placeholder rva003C05DE; script status bit 0x20 on every member; role name)
  void doNamedEnableStealth(Parameter *unit, Bool enable);	// (script status bit 8, set to !enable)
  void doNamedSetScriptStatus20(Parameter *unit, Bool value);	// 0x003BBD61 (map placeholder rva003BBD61; script status bit 0x20; role name)
public:
  void doNamedAddExperience(Parameter *unit, Int amount);	// 0x003BC903 (role name; ExperienceTracker::rva0039B315 grants the experience; map placeholder Rva003BC903Do)
  void doTeleportToWaypoint(Parameter *unit, const AsciiString &waypointName);	// 0x003BD116 (Zero Hour's name; map placeholder Rva003BD116Set; Object::teleportTo(waypoint location, false))
  void doNamedSetWeaponSetFlag17(Parameter *unit, Parameter *enabledParm);	// 0x003BD032 (role name: sets or clears weapon set flag 0x17 on the named unit by the parameter's int; map placeholder Rva003BD032Set)
  void doNamedIdle(Parameter *unit);	// 0x003C91D1 (role name: releases the temporary weapon lock and orders aiIdle; map placeholder Rva003C91D1Idle)
  void doTeamTeleportToWaypoint(Parameter *teamParm, const AsciiString &waypointName);	// 0x003C353B (role name; teleports every member to the waypoint; map placeholder Rva003C353BDo)
  void doSetCounterToPlayersLightPoints(Parameter *pPlayerParm, Parameter *pCounterParm);	// 0x003C2C28 (role name; the counter becomes the sum of Player::getLightPoints over the mask; map placeholder Rva003C2C28Do)
  void doSetCounterToPlayerCommandPointsAvailable(Parameter *pPlayerParm, Parameter *pCounterParm);	// 0x003C2D34 (role name: the counter becomes Command::rva002A7548(1), the cap less the points in use; sibling of doSetCounterToPlayerCommandPointsInUse 0x003C2CD8; map placeholder Rva003C2D34Do)
  void doSetCounterToTeamCommandPoints(Parameter *pTeamParm, Parameter *pCounterParm);	// 0x003C2DEA (role name: the counter becomes the sum of the members' template CommandPoints (+0x618); map placeholder Rva003C2DEADo)
  void doTeamHarvestAtWaypoint(const AsciiString &teamName, const AsciiString &waypointName);	// 0x003BF903 (role name: AIGroup::groupHarvest at the waypoint's location for the team; map placeholder Rva003BF903Do)
  void doTeamSetFlagByName(const AsciiString &teamName, const AsciiString &flagName, Bool value);	// 0x003BECCD (role name: forwards to Team::rva003A38E9(name, value), which looks the name up in the team's table at +0x48; map placeholder Rva003BECCDSet)
  void doModCameraFinalLookToward(const AsciiString &waypointName);	// 0x003C4DC2 (CAMERA_MOD_FINAL_LOOK_TOWARD)
  void doUndoRevealMapAtWaypointPermanent(const AsciiString &lookName);	// 0x003BCA43 (map placeholder Rva003BCA43Do; Zero Hour's name, body undoNamedMapReveal then the removal 0x00357D52)
  void doPlayerRepairStructure(const AsciiString &playerName, const AsciiString &structureName);	// 0x003BF96F (map placeholder Rva003BF96FDo; Zero Hour's name, body Player::repairStructure slot 7)
  void doDisplayCounter(AsciiString counterName, const AsciiString &counterText);	// 0x003C5405 (map placeholder Rva003C5405Do; Zero Hour's name; the name is qualified with the script scope, resolveName 0x002046C0)
  void doDisplayCountdownTimer(AsciiString timerName, const AsciiString &timerText);	// 0x003C549D (map placeholder Rva003C549DDo; Zero Hour's name)
  void doExitSpecificBuilding(const AsciiString &buildingName);	// 0x003C8C6E (map placeholder Rva003C8C6EDo; Zero Hour's name, body aiEvacuate or removeAllContained)
public:
  void doExitMap();	// 0x003BD108
  void doNamedStopSpecialPower(Parameter *unit);	// 0x003BCD1B
  void doNamedFireSpecialPower2DAtNamed(Parameter *unit, Parameter *target);	// 0x003BCCC6
  void doNamedSetExperienceTrackerValue(Parameter *unit, Int value);	// 0x003BC96F
  void doPlayerAddSkillPoints(const AsciiString &player, Int value);	// 0x003BC7EF
  void doWaterChangeHeight(Parameter *waterName, Real height);
  void doWaterChangeHeightOverTime(Parameter *waterName, Real newHeight, Real time, Real damage);
public:
  void doRevealMapAtWaypoint(const AsciiString &waypointName, Real radius, const AsciiString &playerName, const AsciiString &revealName);	// 0x003BC9E0
  void doRevealMapAtWaypointPermanent(const AsciiString &waypointName, const AsciiString &playerName, const AsciiString &revealName);	// 0x003BCA12
  void doCameraShakeAtWaypoint(const AsciiString &waypointName, Real power, Real duration, Real radius);	// 0x003BCAF6
  void bfmeGoFGE(void *unit, void *status, Bool set);
public:
  void doNamedStopSpecialPowerCountdown(Parameter *unit, const AsciiString &specialPower, void *stop);
  void doNamedFireSpecialPowerAtWaypoint(Parameter *unit, const AsciiString &specialPower, void *waypoint);
  void doNamedFireSpecialPowerAtNamed(Parameter *unit, const AsciiString &specialPower, const AsciiString &target);
protected:
  void doPlayerSetSpecialPowerCountdown(const AsciiString &player, const AsciiString &specialPower, Int seconds);
public:
  void doFireSpecialPowerAtWaypoint(const AsciiString &player, const AsciiString &specialPower, const AsciiString &waypoint);
public:
  Bool FireSpecialPowerAtPosition(const AsciiString &player, const SpecialPowerTemplate *power, const Coord3D *position);
public:
  void doRotateCameraStopAtEnd(Real rotations, Real sec, Real easeIn, Real easeOut);	// 0x003BAF95
  void doRotateCameraTowardObject(Parameter *unit, Real sec, Real holdSec, Real easeIn, Real easeOut, Real extra);	// 0x003BAFE8
  void doRotateCameraTowardWaypoint(const AsciiString &waypointName, Real sec, Real easeIn, Real easeOut, Bool reverseRotation);	// 0x003BB05F
public:
  void doSetWarehouseValue(const AsciiString &warehouseName, Int cashValue);
protected:
  void doSetCaveIndex(const AsciiString &caveName, Int caveIndex);
protected:
  void doNamedSetTeam(Parameter *unit, const AsciiString &teamName);
  void doNamedBuildStructureAtWaypoint(Parameter *unit, const AsciiString &structure, Real angle, const AsciiString &waypoint);
public:
  void doNamedCloseGate(Parameter *unit);	// 0x003BCD45
  void doNamedOpenGate(Parameter *unit);	// 0x003BCDD7
public:
  void doNamedSetDualWeaponFlag(const AsciiString &unitName, Bool flag);	// 0x003C3175
  void doTeamSetDualWeaponFlag(const AsciiString &teamName, Bool flag);	// 0x003C30D9
  void doNamedGuardAreaFromWaypoint(Parameter *unit, const AsciiString &areaName, const AsciiString &waypointName);	// 0x003C8A15
  void doNamedSetDrawableState(Parameter *unit, Bool state);	// 0x003C36C0
  void doTeamSetDrawableState(Parameter *team, Bool state);	// 0x003C36FE
protected:
  void doTeamRecruitUnitsOfType(const AsciiString &teamName, Int count, const AsciiString &thingName);	// 0x003C2166
  void doTeamSetMemberFlag(Parameter *team, Parameter *unit, Bool value);	// 0x003C207C
public:
  void doAffectSkillPointsModifier(const AsciiString &playerName, Real newModifier);	// 0x003BCBC7
public:
  void doTeamDelete(const AsciiString &teamName, Int ignoreDead);
  void doViewFilterFade(Bool enable, Int frames);	// 0x003BBE91
  void doCameraEnableSlaveMode(const AsciiString &thingTemplateName, const AsciiString &boneName);	// 0x003BCADA
  void doCameraDisableSlaveMode();	// 0x003BCAE8
public:
  void doCameraSetDefault(Real pitch, Real angle, Real maxHeight);
  void doBlackWhiteMode(Bool startBWMode, Int frames);
public:
  void doZoomCamera(Real zoom, Real sec, Real easeIn, Real easeOut);
  void doModCameraMoveToSelection();	// 0x003BAE04 (Zero Hour's name: the camera's final move to the average position of the selected drawables)
  void doPitchCameraVariant(Real pitch, Real sec, Real easeIn, Real easeOut);	// 0x003BABCB (View slot 60; role name)
  void doCameraNudge(Parameter *unused);	// 0x003BACCA (two View::scrollBy calls with +-0.0001; role name)
  void doSetupCamera(const AsciiString &waypointName, Real zoom, Real pitch, const AsciiString &lookAtWaypointName);	// 0x003BAD15 (Zero Hour's name)
  void doCameraMotionBlurJump(const AsciiString &waypointName, Bool saturate);	// 0x003BB7FE (Zero Hour's name)
  void doCameraMotionBlur(Bool zoomIn, Bool saturate);	// 0x003BB898 (Zero Hour's name)
  void doSetBorderShroud(Bool setting);
  void doLetterBoxMode(Bool immediate);
  void doCameraSetOffset2D(Real x, Real y);	// 0x003BCC08 (View slot 111 with a Coord2D; role name)
  void doTacticalViewFlag1(Bool flag);	// 0x003BCFE7 (View slot 147 forwarder; role name)
  void doTacticalViewFlag2(Bool flag);	// 0x003BCFF5 (slot 148)
  void doTacticalViewFlag3(Bool flag);	// 0x003BD003 (slot 149)
  void doTacticalViewFlag4(Bool flag);	// 0x003BD011 (slot 150)
  void doLivingWorldSetFlag();	// 0x003BCC94 (LivingWorld slot 10 with true; role name)
public:
  void doResetCamera(const AsciiString &waypointName, Real sec, Real easeIn, Real easeOut);	// 0x003C4E28
  void doPlayerRemoveOverrideRelationToTeam(const AsciiString& playerName, const AsciiString& otherTeam);
  void doTeamRemoveOverrideRelationToPlayer(const AsciiString& teamName, const AsciiString& otherPlayer);
protected:
  void doNamedAttackFollowWaypoints(Parameter *unit, const AsciiString &pathLabel);
  void doNamedFollowWaypointsExact(Parameter *unit, const AsciiString &pathLabel);
  void doTeamFollowWaypointsExact(const AsciiString& teamName, Int waypointPathLabel, Bool asTeam);
public:
  void bfmeLinkObjectsA70(void *oldOwnerName, void *newOwnerName);	// (map ledger name; hands the first player's assets to the second)
public:
  void doDisableCountdownTimerDisplay();
  void doEnableCountdownTimerDisplay();
protected:
  void doTeamAttackNamed(const AsciiString &teamName, const AsciiString &unitName);
public:
  void doSetControlBarOverride(const AsciiString &buttonName, const AsciiString &objectType, Int slot);	// 0x003BCB7A
  void doTeamGuardSupplyCenter(const AsciiString *teamName, Int minSupplies);	// 0x003C1F6C
  void doPlayerSetOverrideRelationToTeam(const AsciiString& playerName, const AsciiString& otherTeam, Object *relation);	// (the map's third parameter type; the value is a Relationship)
public:
  void excludePlayerFromScoreScreen(const AsciiString &playerName);	// 0x003BB6C0
  void doObjectTypeListMaintenance(const AsciiString &objectList, const AsciiString &objectType, Bool addObject);	// 0x003BC9D5
protected:
  void doNamedFireWeaponFollowingWaypointPath(const AsciiString &unit, const AsciiString &waypointPath);
protected:
  void doAffectObjectPanelFlagsTeam(const AsciiString& teamName, const AsciiString& flagName, Bool enable);
protected:
  void doTeamFaceNamed(const AsciiString &teamName, const AsciiString &faceUnitName);
  void doTeamFaceWaypoint(const AsciiString &teamName, const AsciiString &faceWaypointName);
public:
  void doChooseVictimAlwaysUsesNormal(Bool enable);	// 0x003BCC6E
protected:
  void doPlayerForceEmotion(Parameter *playerParm, Int emotion, Real duration);
protected:
  void doUnitSetScriptCondition(const AsciiString &unit, Int kind, Bool set);	// 0x003C0D6A (role name; the map spells it rva003C0D6A)	// 0x003C0D6A
protected:
  void doTeamSetScriptCondition(const AsciiString &teamName, Int kind, Bool set);	// 0x003C0D90 (role name; the map spells it rva003C0D90)	// 0x003C0D90
public:
  void doFireSpecialPowerAtTeamPosition(const AsciiString &playerName, const AsciiString &powerName, const AsciiString &teamName);	// 0x003C4145 (role name; the map spells it rva003C4145: the team's position, then FireSpecialPowerAtPosition)	// 0x003C4145
public:
  void doPitchCameraDegrees(Real degrees, Real sec, Real easeIn, Real easeOut);	// 0x003BAC6D (role name; the map spells it rva003BAC6D: View slot 62 pitchCamera with the angle converted from degrees; the ledger name doPitchCamera belongs to 0x003BAC1C, which calls zoomCamera)	// 0x003BAC6D
public:
  void doPitchCamera(Real zoom, Real sec, Real easeIn, Real easeOut);	// (ledger name; body is zoomCamera, slot 61)	// 0x003BAC1C
protected:
  void doSetTeamReference(const AsciiString &reference, Parameter *teamParm, bool byName);	// 0x003C20CA (role name; the map spells it rva003C20CA: the team counterpart of doSetUnitReference, IAE)	// 0x003C20CA
protected:
  void doEnablePlanningMode(unsigned char enable);	// // 0x003BD424
  void doFlashSpellStoreButton(Int seconds);	// // 0x003BD42F
  void doFlashObjectivesButton(Int seconds);	// // 0x003BD444
  void doFlashPlanningModeButton(Int seconds);	// // 0x003BD459
  void doHeroSelectButtonFlash(Int slot, Int seconds);	// // 0x003BD270
  void doSelectBuilderButtonFlash(Int seconds);	// // 0x003BD28D
public:
  void doEvaEnabledDisabled(unsigned char setEnabled);	// 0x003BCA5D (Zero Hour's name; the map spells it Rva003BCA5DSet)	// 0x003BCA5D
public:
  void doObjectRadarCreateEvent(Parameter *unitParm, void *eventType);
  void doTeamRadarCreateEvent(const AsciiString *teamName, void *eventType);
public:
  void doMilitaryCaption(Int briefing, Real duration);
public:
  void doBuildUpgrade(const AsciiString &playerName, const AsciiString &upgradeName);	// 0x003BB163
public:
  void doSetPlayerTemplateBuildable(Parameter *playerParameter, const AsciiString *templateName, Bool buildable);	// 0x003BD062
public:
  void doRevealMapEntirePermanently(Bool reveal, const AsciiString &playerName);	// 0x003BB9C6
  void doPlayerRelatesPlayer(const AsciiString &sourcePlayerName, Int relationship, const AsciiString &targetPlayerName);	// 0x003BB713
protected:
  void doTeamGarrisonSpecificBuilding(const AsciiString &teamName, const AsciiString &buildingName, Bool instant);
protected:
  void putUnitInContain(Object *unit, ContainModuleInterface *contain, Bool flag);
public:
  void doMoveCameraTo(const AsciiString &waypointName, Real sec, Real cameraStutterSec, Real easeIn, Real easeOut);	// 0x003BAAE4
public:
  void doTeamFlash(const AsciiString *teamName, Int timeInSeconds, RGBColor *color);	// 0x003BFF50
protected:
  void doUnitStartSequentialScript(Parameter *unitParameter, const AsciiString &scriptName, Int timesToLoop);
protected:
  void doTeamStartSequentialScript(const AsciiString &teamName, const AsciiString &scriptName, Int timesToLoop);
public:
  void doSetCounterToUnitsDestroyed(Parameter *playerParameter, Parameter *templateParameter, Parameter *counterParameter);	// 0x003C2E61
protected:
  void doPlaySoundEffect(const AsciiString &sound);
  void doSoundPlayFromNamed(const AsciiString &sound, const AsciiString &unitName);
  void doSpeechPlay(const AsciiString &speechName, Bool allowOverlap);
protected:
  void doForceObjectSelection(const AsciiString& teamName, const AsciiString& objectType, Bool centerInView, const AsciiString& audioToPlay);
protected:
public:
  void doSetCounterToUnitsDestroyedOfKind(Parameter *playerParameter, Parameter *kindOfParameter, Parameter *counterParameter);	// 0x003C2EC9
public:
  void doPlayerSelectUnit(Parameter *playerParameter, Parameter *unitParameter);	// 0x003BD19C
public:
  void doCameraFollowNamed(Parameter *unitParameter, Bool snapToUnit, Real play);	// 0x003BC1B2
  void doCameraTetherNamed(Parameter *unitParameter, Bool snapToUnit, Real play);	// 0x003BAA46
protected:
  void doPlaySoundEffectAtTeam(const AsciiString &sound, const AsciiString &teamName);
protected:
  void doPlaySoundEffectAt(const AsciiString &sound, const AsciiString &waypointName);
public:
  void doSetReverbRoomType(const AsciiString &roomType);	// 0x003BB62C
public:
  void doRevealMapAtWaypoint(const AsciiString &waypointName, Real radius, const AsciiString &playerName);	// 0x003BB8ED: MAP_REVEAL_AT_WAYPOINT (retail action template 93; Zero Hour doRevealMapAtWaypoint, same reveal-then-undo)
protected:
  void doRevealMapInTrigger(const AsciiString &triggerName, const AsciiString &playerName);	// 0x003C013C: MAP_REVEAL_IN_TRIGGER (retail action template 552; no Zero Hour function, name = the action in do-spelling)
public:
  void doTeamSetHealth(Parameter *teamParameter, Parameter *healthParameter);	// 0x003C301D: TEAM_SET_HEALTH (retail action template 423; no Zero Hour function, name = the action in do-spelling); the clamp is WWLib min/max
public:
  void doTeamStop(const AsciiString &teamName, Bool shouldDisband);	// 0x003C08CC: TEAM_STOP / TEAM_STOP_AND_DISBAND (retail action templates 178 and 179; Zero Hour doTeamStop)
protected:
  void doMergeTeamIntoTeam(const AsciiString &teamSrcName, const AsciiString &teamDestName);
public:
  void doNamedUseCommandButtonOnNearestEnemyBuilding(Parameter *unitParameter, const AsciiString &commandButtonName);	// 0x003BDE5D: NAMED_USE_COMMANDBUTTON_ON_NEAREST_ENEMY_BUILDING (retail action template 427; no Zero Hour function, the named-unit twin of the TEAM_ ones)
public:
  void doNamedUseCommandButtonOnNearestGarrisonedBuilding(Parameter *unitParameter, const AsciiString &commandButtonName);	// 0x003BDB03: NAMED_USE_COMMANDBUTTON_ON_NEAREST_GARRISONED_BUILDING (retail action template 425; Zero Hour has the TEAM_ twin doTeamUseCommandButtonOnNearestGarrisonedBuilding)
public:
  void doNamedUseCommandButtonOnNearestEnemyBuildingClass(Parameter *unitParameter, const AsciiString &commandButtonName, Int kindOfBit);	// 0x003BDFA5: NAMED_USE_COMMANDBUTTON_ON_NEAREST_ENEMY_BUILDING_CLASS (retail action template 428; Zero Hour has the TEAM_ twin doTeamUseCommandButtonOnNearestBuildingClass)
public:
protected:
  void doTeamCaptureNearestUnownedFactionUnit(const AsciiString &teamName);
protected:
  void doMoveUnitTowardsNearest(const AsciiString &unitName, const AsciiString &objectType, AsciiString triggerName);
protected:
  void doMoveTeamTowardsNearest(const AsciiString &teamName, const AsciiString &objectType, AsciiString triggerName);
public:
  void doDestroyAllContained(Parameter *unitParameter, Int damageType);
};
