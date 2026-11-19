// ScriptConditions.h: the script condition evaluator and its interface.
// Target facts:
//  - ScriptConditions SIZE 0xC: ScriptEngine::init allocates 0xC (0x00205B38)
//    and its ctor 0x003E5513 only calls SubsystemInterface() and stores its
//    table 0x00C35B38 (18 slots), so ScriptConditionsInterface adds no data.
//  - ScriptConditionsInterface's own table (deleting dtor 0x003E3BDF) marks
//    slots 1, 9, 10 and 14..17 __purecall. ScriptConditions fills slot 1 and
//    slot 10 with the shared empty bodies, slot 9 with reset 0x003E3C66 and
//    14..16 with evaluateCondition 0x003EC8D4,
//    evaluateSkirmishCommandButtonIsReady 0x003E73E7 and evaluateTeamIsContained
//    0x003E70AC. Slot 17 (0x003E4921, Parameter * -> bool) has no recovered
//    name; it is declared under its address so the table has retail's 18 slots.
//  - The evaluate* bodies are protected non-virtual members (the map's IAE).
// Zero Hour keeps both classes in this header.
#pragma once
#ifndef BFME2_SCRIPTCONDITIONS_H
#define BFME2_SCRIPTCONDITIONS_H

#include "Common/GameType.h"
#include "Common/SubsystemInterface.h"

class AsciiString;
class Condition;
class Parameter;

class ScriptConditionsInterface : public SubsystemInterface
{
public:
	virtual ~ScriptConditionsInterface() {}	// user-declared: ~ScriptConditions 0x003E5525 stores this table (0x00835AE0) before calling ~SubsystemInterface 0x001B4E74
	virtual Bool evaluateCondition(Condition *pCondition) = 0;	// slot 14
	virtual Bool evaluateSkirmishCommandButtonIsReady(Parameter *pSkirmishPlayerParm, Parameter *pTeamParm, Parameter *pCommandButtonParm, Bool allReady) = 0;	// slot 15
	virtual Bool evaluateTeamIsContained(Parameter *pTeamParm, Bool allContained) = 0;	// slot 16
	virtual Bool rva003E4921(Parameter *pParm) = 0;	// slot 17
};

class ScriptConditions : public ScriptConditionsInterface
{
public:
	ScriptConditions();			// 0x003E5513
	virtual ~ScriptConditions();		// 0x003E5525

	virtual void init() { reset(); }	// slot 1, 0x000D20D6: `mov eax,[ecx]; jmp [eax+0x24]`, a virtual tail call to reset (slot 9)
	virtual void reset();			// slot 9, 0x003E3C66
	virtual void update() {}		// slot 10, the shared empty body 0x000B3FD0

	virtual Bool evaluateCondition(Condition *pCondition);	// slot 14, 0x003EC8D4
	virtual Bool rva003E4921(Parameter *pParm);	// slot 17, 0x003E4921

protected:	// the map spells slots 15 and 16 protected (MAE)
	virtual Bool evaluateSkirmishCommandButtonIsReady(Parameter *pSkirmishPlayerParm, Parameter *pTeamParm, Parameter *pCommandButtonParm, Bool allReady);	// slot 15, 0x003E73E7
	virtual Bool evaluateTeamIsContained(Parameter *pTeamParm, Bool allContained);	// slot 16, 0x003E70AC

	Bool evaluatePlayerHasNOrFewerBuildings(Parameter *pBuildingCountParm, Parameter *pSideParm);	// 0x003E4132
	Bool evaluatePlayerIsInPlanningMode(Parameter *pSideParm);	// 0x003E54B4
	Bool evaluateIsDestroyed(Parameter *pTeamParm);	// 0x003E5DE9

	Bool evaluateNamedUnitDestroyed(Parameter *pUnitParm);	// 0x003E3DF7
	Bool evaluateNamedUnitExists(Parameter *pUnitParm);	// 0x003E3E2F
	Bool evaluateNamedUnitDying(Parameter *pUnitParm);	// 0x003E3E54
	Bool evaluateNamedUnitTotallyDead(Parameter *pUnitParm);	// 0x003E3E89
	Int rva003E40AF(Int v);	// 0x003E40AF
	Bool evaluateNamedOwnedByPlayer(Parameter *pUnitParm, Parameter *pPlayerParm);	// 0x003E40D2
	Bool evaluateVideoHasCompleted(Parameter *pVideoParm);	// 0x003E41E3
	Bool evaluateSpeechHasCompleted(Parameter *pSpeechParm);	// 0x003E41FB
	Bool evaluateAudioHasCompleted(Parameter *pAudioParm);	// 0x003E4213
	Bool evaluateNamedCreated(Parameter *pUnitParm);	// 0x003E3F41
	unsigned char evaluateTeamCreated(Parameter *pTeamParm);	// 0x003E6803
	Bool evaluateTeamHasObjectStatus(Parameter *pTeamParm, Parameter *pObjectStatus, Bool entireTeam);	// 0x003E712A
	Bool evaluateTeamOwnedByPlayer(Parameter *pTeamParm, Parameter *pPlayerParm);	// 0x003E6B57
	Bool evaluateSkirmishSupplySourceAttacked(Parameter *pPlayerParm);	// 0x003E4AE4
	Bool evaluateUnitHasObjectStatus(Parameter *unitParm, Parameter *statusParm);	// 0x003E5AE9
	Bool evaluateNamedExitedArea(Parameter *pUnitParm, Parameter *pTriggerParm);	// 0x003E6D60
	Bool evaluateMusicHasCompleted(Parameter *musicParm, Parameter *intParm);	// 0x003E7AFC
	Bool evaluateTeamCachedFloatCompare(Parameter *teamParm, Parameter *comparison, Parameter *threshold, Parameter *input);	// 0x003E7E54
	Bool rva003E859E(Parameter *teamParm, Parameter *stateParm);	// 0x003E859E
	Bool evaluateTeamStateIs(Parameter *pTeamParm, Parameter *pStateParm);	// 0x003E9471
	Bool evaluateTeamStateIsNot(Parameter *pTeamParm, Parameter *pStateParm);	// 0x003E94E5
	Bool evaluateSkirmishPlayerIsFaction(Parameter *pSkirmishPlayerParm, Parameter *pFactionParm);	// 0x003E9E62
	Bool evaluatePlayerSpecialPowerFromUnitTriggered(Parameter *pPlayerParm, Parameter *pSpecialPowerParm, Parameter *pUnitParm);	// 0x003E422B
	Bool evaluatePlayerSpecialPowerFromUnitMidway(Parameter *pPlayerParm, Parameter *pSpecialPowerParm, Parameter *pUnitParm);	// 0x003E42A1
	Bool evaluatePlayerSpecialPowerFromUnitComplete(Parameter *pPlayerParm, Parameter *pSpecialPowerParm, Parameter *pUnitParm);	// 0x003E4317
	Bool evaluateUpgradeFromUnitComplete(Parameter *pPlayerParm, Parameter *pUpgradeParm, Parameter *pUnitParm);	// 0x003E438D
	Bool evaluatePlayerDestroyedNOrMoreBuildings(Parameter *playerParm, Parameter *countParm, Parameter *opponentParm);	// 0x003E4810
	Bool evaluateSkirmishStartPosition(Parameter *pSkirmishPlayerParm, Parameter *pStartNdx);	// 0x003E4B2A
	Bool evaluateNamedAttackedByPlayer(Parameter *pUnitParm, Parameter *pPlayerParm);	// 0x003E96F5
	Bool evaluateNamedAttackedByType(Parameter *pUnitParm, Parameter *pTypeParm);	// 0x003E9556
	Bool evaluateTeamAttackedByType(Parameter *pTeamParm, Parameter *pTypeParm);	// 0x003E95FF
	Bool evaluateTeamAttackedByPlayer(Parameter *pTeamParm, Parameter *pPlayerParm);	// 0x003E6784
	Bool evaluateTeamDiscovered(Parameter *pTeamParm, Parameter *pPlayerParm);	// 0x003E6AA9
	Bool evaluateTeamCanPathToNamed(Parameter *pTeamParm, Parameter *pUnitParm);	// 0x003E8535
	Bool rva003E5105(Parameter *parameter, Parameter *b);	// 0x003E5105
	Bool evaluateSkirmishPlayerHasPrereqsToBuild(Parameter *pSkirmishPlayerParm, Parameter *pObjectTypeParm);	// 0x003E49B8
	Bool evaluateNamedDestroyedByType(Parameter *pUnitParm, Parameter *pTypeParm);	// 0x003EA076
	Bool evaluatePlayerLostObjectType(Parameter *pPlayerParm, Parameter *pTypeParm);	// 0x003E9EB0
	Bool evaluateHasCommandPointsToBuildUnit(Parameter *pPlayerParm, Parameter *pTypeParm);	// 0x003E81E8
	Bool evaluateTeamInsideAreaPartially(Parameter *pTeamParm, Parameter *pTriggerParm, Parameter *pTypeParm);	// 0x003E5E7D
	Bool evaluateTeamInsideAreaEntirely(Parameter *pTeamParm, Parameter *pTriggerParm, Parameter *pTypeParm);	// 0x003E66EA
	Bool evaluateNamedInsideArea(Parameter *pUnitParm, Parameter *pTriggerParm);	// 0x003E5EFF
	Bool evaluateNamedOutsideArea(Parameter *pUnitParm, Parameter *pTriggerParm);	// 0x003E66D5
	Bool evaluateTeamOutsideAreaEntirely(Parameter *pTeamParm, Parameter *pTriggerParm, Parameter *pTypeParm);	// 0x003E674C
	Bool evaluateTeamEnteredAreaEntirely(Parameter *pTeamParm, Parameter *pTriggerParm, Parameter *pTypeParm);	// 0x003E6DAA
	Bool evaluateTeamEnteredAreaPartially(Parameter *pTeamParm, Parameter *pTriggerParm, Parameter *pTypeParm);	// 0x003E6E0C
	Bool evaluateTeamExitedAreaEntirely(Parameter *pTeamParm, Parameter *pTriggerParm, Parameter *pTypeParm);	// 0x003E6E6E
	Bool evaluateTeamExitedAreaPartially(Parameter *pTeamParm, Parameter *pTriggerParm, Parameter *pTypeParm);	// 0x003E704A
	Bool evaluateSkirmishNamedAreaExists(Parameter *pPlayerParm, Parameter *pTriggerParm);	// 0x003E77FF
	Bool rva003E4D93(Parameter *pUnitParm, Parameter *pComparisonParm, Parameter *pValueParm, Parameter *pRangeParm);	// 0x003E4D93
	Bool evaluatePlayerHasHeroesAtRankLevel(Parameter *pPlayerParm, Parameter *pCountParm, Parameter *pRankParm);	// 0x003E85E0 (role name: counts the player's hero units, KINDOF_HERO, whose veterancy level is at least the rank; map placeholder rva003E85E0)
	Bool evaluateAllDestroyed(Parameter *pPlayerParm);	// 0x003E3C89
	Bool evaluateAllBuildFacilitiesDestroyed(Parameter *pPlayerParm);	// 0x003E3D10
	Bool evaluateConditionHelper(Condition *pCondition);	// 0x003EA9AF
	Bool evaluateDistanceBetweenObjects(Condition *pCondition);	// 0x003E4C8D
	Bool evaluateSkirmishSupplySourceSafe(Condition *pCondition, Parameter *pPlayerParm, Parameter *pValueParm);	// 0x003E4A63
	Bool evaluateCounterCounter(Condition *pCondition);	// 0x003E7CBA
	Bool evaluateSkirmishPlayerHasUnitsInArea(Condition *pCondition, Parameter *pSkirmishPlayerParm, Parameter *pTriggerParm);	// 0x003E782D
	Bool evaluateSkirmishPlayerIsOutsideArea(Condition *pCondition, Parameter *pSkirmishPlayerParm, Parameter *pTriggerParm);	// 0x003E79AF
	Bool evaluateTeamCountCompare(Parameter *pTeamParm, Parameter *pCountParm, Parameter *pKindParm);	// 0x003E5E2F
	Bool evaluatePlayerHasCredits(Parameter *pValueParm, Parameter *pOpParm, Parameter *pPlayerParm);	// 0x003E3F59
	Bool evaluateBuildingEntered(Parameter *pPlayerParm, Parameter *pUnitParm);	// 0x003E3FFE

	// The other named condition rows of ScriptConditions.cpp, with the signature
	// and access their decorated map names state (IAE protected, QAE public).
	// Rows under placeholder names or placeholder parameter types are not declared.
	Bool evaluateNamedUnitStance(Parameter *parm1, Parameter *parm2);	// 0x003E5267 (map placeholder rva003E5267: StancesBehavior current stance against the parameter)
	Bool evaluateNamedGateIsOpen(Parameter *parm1);	// 0x003E3EC3 (map placeholder rva003E3EC3: GateOpenAndCloseBehavior primary slot 6)
	Bool evaluateBridgeBroken(Parameter *parm1);	// 0x003E3D95
	Bool evaluateBridgeRepaired(Parameter *parm1);	// 0x003E3DC6
	Bool evaluateIsBuildingEmpty(Parameter *parm1);	// 0x003E4079
	Bool evaluatePlayerHasPower(Parameter *parm1);	// 0x003E417D
	Bool evaluateScienceAcquired(Parameter *parm1, Parameter *parm2);	// 0x003E4403
	Bool evaluateCanPurchaseScience(Parameter *parm1, Parameter *parm2);	// 0x003E446F
	Bool evaluateSciencePurchasePoints(Parameter *parm1, Parameter *parm2);	// 0x003E44D1
	Bool evaluateNamedBaseUnpackableForPlayer(Parameter *parm1, Parameter *parm2);	// 0x003E4565
	Bool evaluatePlayerHasComparisonPercentPower(Parameter *parm1, Parameter *parm2, Parameter *parm3);	// 0x003E46B4
	Bool evaluatePlayerHasComparisonValueExcessPower(Parameter *parm1, Parameter *parm2, Parameter *parm3);	// 0x003E477F
	Bool evaluateUnitHasEmptied(Parameter *parm1);	// 0x003E4860
	Bool evaluateSkirmishPlayerHasBeenAttackedByPlayer(Parameter *parm1, Parameter *parm2);	// 0x003E4BFA
	Bool evaluatePlayerCompareLightPoints(Parameter *parm1, Parameter *parm2, Parameter *parm3);	// 0x003E4F79
	Bool evaluateCanBuildObjectTypeAtBase(Parameter *parm1, Parameter *parm2, Parameter *parm3);	// 0x003E5017
	Bool evaluateNamedCanPathToWaypoint(Parameter *parm1, Parameter *parm2);	// 0x003E50B4
	Bool evaluateHasEvaEventPlayedInLastNSeconds(const AsciiString &name, Real value);	// 0x003E543A
	Bool evaluateUnitHealth(Parameter *parm1, Parameter *parm2, Parameter *parm3);	// 0x003E5587
	Bool evaluatePlayerHasNOrFewerFactionBuildings(Parameter *parm1, Parameter *parm2);	// 0x003E586B
	Bool evaluatePlayerHasNOrFewerBases(Parameter *parm1, Parameter *parm2);	// 0x003E58F0
	Bool evaluateCanBuildAtBase(Parameter *parm1, Parameter *parm2);	// 0x003E5B11
	Bool evaluatePlayerHasKilledKindOfUnits(Parameter *parm1, Parameter *parm2, Parameter *parm3);	// 0x003E5C04
	Bool evaluatePlayerHasNumberUnitsDistanceFromObject(Parameter *parm1, Parameter *parm2, Parameter *parm3, Parameter *parm4, Parameter *parm5);	// 0x003E609F
	Bool evaluatePlayerHasUnitTypeInArea(Condition *pCondition, Parameter *parm1, Parameter *parm2, Parameter *parm3, Parameter *parm4, Parameter *parm5, Parameter *parm6, Bool flag);	// 0x003E61A4
	Bool evaluatePlayerHasUnitKindInArea(Condition *pCondition, Parameter *parm1, Parameter *parm2, Parameter *parm3, Parameter *parm4, Parameter *parm5);	// 0x003E64A6
	Bool evaluateNamedDiscovered(Parameter *parm1, Parameter *parm2);	// 0x003E6A2B
	Bool evaluateNamedEnteredArea(Parameter *parm1, Parameter *parm2);	// 0x003E6D0A
	Bool evaluateSkirmishValueInArea(Condition *pCondition, Parameter *parm1, Parameter *parm2, Parameter *parm3, Parameter *parm4);	// 0x003E719C
	Bool evaluateSkirmishUnownedFactionUnitComparison(Parameter *parm1, Parameter *parm2, Parameter *parm3);	// 0x003E749E
	Bool evaluateSkirmishPlayerHasComparisonGarrisoned(Parameter *parm1, Parameter *parm2, Parameter *parm3);	// 0x003E7597
	Bool evaluateSkirmishPlayerHasComparisonCapturedUnits(Parameter *parm1, Parameter *parm2, Parameter *parm3);	// 0x003E76E4
	Bool evaluateSkirmishPlayerHasDiscoveredPlayer(Parameter *parm1, Parameter *parm2);	// 0x003E79F5
	Bool evaluateDistanceBetweenTeams(Condition *pCondition);	// 0x003E7B56
	Bool evaluateCounterSeconds(Condition *pCondition);	// 0x003E7D83
	Bool evaluateTeamIsLedByUnit(Parameter *parm1, Parameter *parm2);	// 0x003E803D
	Bool evaluateHasCommandPointsToBuildTeam(Parameter *parm1, Parameter *parm2);	// 0x003E80DA
	Bool evaluatePlayerHasKilledTypeUnits(Parameter *parm1, Parameter *parm2, Parameter *parm3);	// 0x003E82E2
	Bool evaluateTeamCanPathToWaypoint(Parameter *parm1, Parameter *parm2);	// 0x003E84C5
	Bool evaluateUnitHasToggledWeapon(Parameter *parm1, Parameter *parm2);	// 0x003E89A9
	Bool evaluateAnyUnitsUsingBloodthirsty(Parameter *parm1);	// 0x003E8AA9
	Bool evaluateSkirmishSpecialPowerIsReady(Parameter *parm1, Parameter *parm2);	// 0x003E9062
	Bool evaluateSkirmishCanFireSpecialPowerOnTeam(Parameter *parm1, Parameter *parm2, Parameter *parm3);	// 0x003E91F6
	Bool evaluateGameModeActive(Parameter *parm1);	// 0x003E92B2
	Bool evaluateNamedUnitRankLevel(Parameter *parm1, Parameter *parm2);	// 0x003E92DF
	Bool evaluateHasUnits(Parameter *parm1);	// 0x003E9373
	Bool evaluateBuiltByPlayer(Condition *pCondition, Parameter *parm1, Parameter *parm2);	// 0x003E977E
	Bool evaluateNamedReachedWaypointsEnd(Parameter *parm1, Parameter *parm2);	// 0x003E992C
	Bool evaluateTeamReachedWaypointsEnd(Parameter *parm1, Parameter *parm2);	// 0x003E9A1F
	Bool evaluateNamedSelected(Condition *pCondition, Parameter *parm1);	// 0x003E9B6A
	Bool evaluatePlayerUnitCondition(Condition *pCondition, Parameter *parm1, Parameter *parm2, Parameter *parm3, Parameter *parm4);	// 0x003E9C3B
	Bool evaluateHasDelayedCarryoverUnitOfType(const AsciiString &name, Parameter *parm1);	// 0x003EA87C

public:
	Bool evaluateEnemySighted(const AsciiString &name, Parameter *parm1, Parameter *parm2);	// 0x003E56C5
	Bool evaluateTypeSighted(const AsciiString &name, Parameter *parm1, Parameter *parm2);	// 0x003EA3AB
	Bool evaluateSkirmishSuppliesWithinDistancePerimeter(Parameter *parm1, Parameter *parm2, Parameter *parm3, Parameter *parm4);	// 0x003EA566
	Bool evaluateIsNumOfUnitsBelongingToPlayerNearEvaEventLastPlayedLocationComparisonInt(Parameter *parm1, Real value, const AsciiString *name, Int value3, Int value4);	// 0x003EA764
protected:
	Bool evaluateNamedUnitExperienceLevelCompare(Parameter *pUnitParm, Parameter *pComparisonParm, Parameter *pLevelParm);	// 0x003E514F
protected:
  Bool evaluatePlayerIsAtRankLevelLimit(Parameter *pPlayerParm);	// 0x003E4519
  Bool evaluatePlayerRankLevelComparison(Parameter *pPlayerParm, Parameter *pComparisonParm, Parameter *pValueParm);	// 0x003E52EB
protected:
  Bool evaluateNumPlayersInGame(Parameter *pComparisonParm, Parameter *pValueParm);	// 0x003E53C8
};
// SIZE 0xC
typedef char ScriptConditionsSize[(sizeof(ScriptConditions) == 0xC) ? 1 : -1];

#endif
