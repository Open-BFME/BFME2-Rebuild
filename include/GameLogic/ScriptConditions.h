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
	virtual ~ScriptConditions();

	virtual void init() { reset(); }	// slot 1, 0x000D20D6: `mov eax,[ecx]; jmp [eax+0x24]`, a virtual tail call to reset (slot 9)
	virtual void reset();			// slot 9, 0x003E3C66
	virtual void update() {}		// slot 10, the shared empty body 0x000B3FD0

	virtual Bool evaluateCondition(Condition *pCondition);	// slot 14, 0x003EC8D4
	virtual Bool rva003E4921(Parameter *pParm);	// slot 17, 0x003E4921

protected:	// the map spells slots 15 and 16 protected (MAE)
	virtual Bool evaluateSkirmishCommandButtonIsReady(Parameter *pSkirmishPlayerParm, Parameter *pTeamParm, Parameter *pCommandButtonParm, Bool allReady);	// slot 15, 0x003E73E7
	virtual Bool evaluateTeamIsContained(Parameter *pTeamParm, Bool allContained);	// slot 16, 0x003E70AC

	Bool evaluatePlayerHasNOrFewerBuildings(Parameter *pBuildingCountParm, Parameter *pSideParm);
	Bool evaluatePlayerIsInPlanningMode(Parameter *pSideParm);
	Bool evaluateIsDestroyed(Parameter *pTeamParm);

	Bool evaluateNamedUnitDestroyed(Parameter *pUnitParm);
	Bool evaluateNamedUnitExists(Parameter *pUnitParm);
	Bool evaluateNamedUnitDying(Parameter *pUnitParm);
	Bool evaluateNamedUnitTotallyDead(Parameter *pUnitParm);
	Int rva003E40AF(Int v);
	Bool evaluateNamedOwnedByPlayer(Parameter *pUnitParm, Parameter *pPlayerParm);
	Bool evaluateVideoHasCompleted(Parameter *pVideoParm);
	Bool evaluateSpeechHasCompleted(Parameter *pSpeechParm);
	Bool evaluateAudioHasCompleted(Parameter *pAudioParm);
	Bool evaluateNamedCreated(Parameter *pUnitParm);
	unsigned char evaluateTeamCreated(Parameter *pTeamParm);
	Bool evaluateTeamHasObjectStatus(Parameter *pTeamParm, Parameter *pObjectStatus, Bool entireTeam);
	Bool evaluateTeamOwnedByPlayer(Parameter *pTeamParm, Parameter *pPlayerParm);
	Bool evaluateSkirmishSupplySourceAttacked(Parameter *pPlayerParm);
	Bool evaluateUnitHasObjectStatus(Parameter *unitParm, Parameter *statusParm);
	Bool evaluateNamedExitedArea(Parameter *pUnitParm, Parameter *pTriggerParm);
	Bool evaluateMusicHasCompleted(Parameter *musicParm, Parameter *intParm);
	Bool evaluateTeamCachedFloatCompare(Parameter *teamParm, Parameter *comparison, Parameter *threshold, Parameter *input);
	Bool rva003E859E(Parameter *teamParm, Parameter *stateParm);
	Bool evaluateTeamStateIs(Parameter *pTeamParm, Parameter *pStateParm);
	Bool evaluateTeamStateIsNot(Parameter *pTeamParm, Parameter *pStateParm);
	Bool evaluateSkirmishPlayerIsFaction(Parameter *pSkirmishPlayerParm, Parameter *pFactionParm);
	Bool evaluatePlayerSpecialPowerFromUnitTriggered(Parameter *pPlayerParm, Parameter *pSpecialPowerParm, Parameter *pUnitParm);
	Bool evaluatePlayerSpecialPowerFromUnitMidway(Parameter *pPlayerParm, Parameter *pSpecialPowerParm, Parameter *pUnitParm);
	Bool evaluatePlayerSpecialPowerFromUnitComplete(Parameter *pPlayerParm, Parameter *pSpecialPowerParm, Parameter *pUnitParm);
	Bool evaluateUpgradeFromUnitComplete(Parameter *pPlayerParm, Parameter *pUpgradeParm, Parameter *pUnitParm);
	Bool evaluatePlayerDestroyedNOrMoreBuildings(Parameter *playerParm, Parameter *countParm, Parameter *opponentParm);
	Bool evaluateSkirmishStartPosition(Parameter *pSkirmishPlayerParm, Parameter *pStartNdx);
	Bool evaluateNamedAttackedByPlayer(Parameter *pUnitParm, Parameter *pPlayerParm);
	Bool evaluateNamedAttackedByType(Parameter *pUnitParm, Parameter *pTypeParm);
	Bool evaluateTeamAttackedByType(Parameter *pTeamParm, Parameter *pTypeParm);
	Bool evaluateTeamAttackedByPlayer(Parameter *pTeamParm, Parameter *pPlayerParm);
	Bool evaluateTeamDiscovered(Parameter *pTeamParm, Parameter *pPlayerParm);
	Bool evaluateTeamCanPathToNamed(Parameter *pTeamParm, Parameter *pUnitParm);
	Bool rva003E5105(Parameter *parameter, Parameter *b);
	Bool evaluateSkirmishPlayerHasPrereqsToBuild(Parameter *pSkirmishPlayerParm, Parameter *pObjectTypeParm);
	Bool evaluateNamedDestroyedByType(Parameter *pUnitParm, Parameter *pTypeParm);
	Bool evaluatePlayerLostObjectType(Parameter *pPlayerParm, Parameter *pTypeParm);
	Bool evaluateHasCommandPointsToBuildUnit(Parameter *pPlayerParm, Parameter *pTypeParm);
	Bool evaluateTeamInsideAreaPartially(Parameter *pTeamParm, Parameter *pTriggerParm, Parameter *pTypeParm);
	Bool evaluateTeamInsideAreaEntirely(Parameter *pTeamParm, Parameter *pTriggerParm, Parameter *pTypeParm);
	Bool evaluateNamedInsideArea(Parameter *pUnitParm, Parameter *pTriggerParm);
	Bool evaluateNamedOutsideArea(Parameter *pUnitParm, Parameter *pTriggerParm);
	Bool evaluateTeamOutsideAreaEntirely(Parameter *pTeamParm, Parameter *pTriggerParm, Parameter *pTypeParm);
	Bool evaluateTeamEnteredAreaEntirely(Parameter *pTeamParm, Parameter *pTriggerParm, Parameter *pTypeParm);
	Bool evaluateTeamEnteredAreaPartially(Parameter *pTeamParm, Parameter *pTriggerParm, Parameter *pTypeParm);
	Bool evaluateTeamExitedAreaEntirely(Parameter *pTeamParm, Parameter *pTriggerParm, Parameter *pTypeParm);
	Bool evaluateTeamExitedAreaPartially(Parameter *pTeamParm, Parameter *pTriggerParm, Parameter *pTypeParm);
	Bool evaluateSkirmishNamedAreaExists(Parameter *pPlayerParm, Parameter *pTriggerParm);
	Bool rva003E4D93(Parameter *pUnitParm, Parameter *pComparisonParm, Parameter *pValueParm, Parameter *pRangeParm);
	Bool evaluatePlayerHasHeroesAtRankLevel(Parameter *pPlayerParm, Parameter *pCountParm, Parameter *pRankParm);	// 0x003E85E0 (role name: counts the player's hero units, KINDOF_HERO, whose veterancy level is at least the rank; map placeholder rva003E85E0)
	Bool evaluateAllDestroyed(Parameter *pPlayerParm);
	Bool evaluateAllBuildFacilitiesDestroyed(Parameter *pPlayerParm);
	Bool evaluateConditionHelper(Condition *pCondition);
	Bool evaluateDistanceBetweenObjects(Condition *pCondition);
	Bool evaluateSkirmishSupplySourceSafe(Condition *pCondition, Parameter *pPlayerParm, Parameter *pValueParm);
	Bool evaluateCounterCounter(Condition *pCondition);
	Bool evaluateSkirmishPlayerHasUnitsInArea(Condition *pCondition, Parameter *pSkirmishPlayerParm, Parameter *pTriggerParm);
	Bool evaluateSkirmishPlayerIsOutsideArea(Condition *pCondition, Parameter *pSkirmishPlayerParm, Parameter *pTriggerParm);
	Bool evaluateTeamCountCompare(Parameter *pTeamParm, Parameter *pCountParm, Parameter *pKindParm);
	Bool evaluatePlayerHasCredits(Parameter *pValueParm, Parameter *pOpParm, Parameter *pPlayerParm);
	Bool evaluateBuildingEntered(Parameter *pPlayerParm, Parameter *pUnitParm);

	// The other named condition rows of ScriptConditions.cpp, with the signature
	// and access their decorated map names state (IAE protected, QAE public).
	// Rows under placeholder names or placeholder parameter types are not declared.
	Bool evaluateNamedUnitStance(Parameter *parm1, Parameter *parm2);	// 0x003E5267 (map placeholder rva003E5267: StancesBehavior current stance against the parameter)
	Bool evaluateNamedGateIsOpen(Parameter *parm1);	// 0x003E3EC3 (map placeholder rva003E3EC3: GateOpenAndCloseBehavior primary slot 6)
	Bool evaluateBridgeBroken(Parameter *parm1);
	Bool evaluateBridgeRepaired(Parameter *parm1);
	Bool evaluateIsBuildingEmpty(Parameter *parm1);
	Bool evaluatePlayerHasPower(Parameter *parm1);
	Bool evaluateScienceAcquired(Parameter *parm1, Parameter *parm2);
	Bool evaluateCanPurchaseScience(Parameter *parm1, Parameter *parm2);
	Bool evaluateSciencePurchasePoints(Parameter *parm1, Parameter *parm2);
	Bool evaluateNamedBaseUnpackableForPlayer(Parameter *parm1, Parameter *parm2);
	Bool evaluatePlayerHasComparisonPercentPower(Parameter *parm1, Parameter *parm2, Parameter *parm3);
	Bool evaluatePlayerHasComparisonValueExcessPower(Parameter *parm1, Parameter *parm2, Parameter *parm3);
	Bool evaluateUnitHasEmptied(Parameter *parm1);
	Bool evaluateSkirmishPlayerHasBeenAttackedByPlayer(Parameter *parm1, Parameter *parm2);
	Bool evaluatePlayerCompareLightPoints(Parameter *parm1, Parameter *parm2, Parameter *parm3);
	Bool evaluateCanBuildObjectTypeAtBase(Parameter *parm1, Parameter *parm2, Parameter *parm3);
	Bool evaluateNamedCanPathToWaypoint(Parameter *parm1, Parameter *parm2);
	Bool evaluateHasEvaEventPlayedInLastNSeconds(const AsciiString &name, Real value);
	Bool evaluateUnitHealth(Parameter *parm1, Parameter *parm2, Parameter *parm3);
	Bool evaluatePlayerHasNOrFewerFactionBuildings(Parameter *parm1, Parameter *parm2);
	Bool evaluatePlayerHasNOrFewerBases(Parameter *parm1, Parameter *parm2);
	Bool evaluateCanBuildAtBase(Parameter *parm1, Parameter *parm2);
	Bool evaluatePlayerHasKilledKindOfUnits(Parameter *parm1, Parameter *parm2, Parameter *parm3);
	Bool evaluatePlayerHasNumberUnitsDistanceFromObject(Parameter *parm1, Parameter *parm2, Parameter *parm3, Parameter *parm4, Parameter *parm5);
	Bool evaluatePlayerHasUnitTypeInArea(Condition *pCondition, Parameter *parm1, Parameter *parm2, Parameter *parm3, Parameter *parm4, Parameter *parm5, Parameter *parm6, Bool flag);
	Bool evaluatePlayerHasUnitKindInArea(Condition *pCondition, Parameter *parm1, Parameter *parm2, Parameter *parm3, Parameter *parm4, Parameter *parm5);
	Bool evaluateNamedDiscovered(Parameter *parm1, Parameter *parm2);
	Bool evaluateNamedEnteredArea(Parameter *parm1, Parameter *parm2);
	Bool evaluateSkirmishValueInArea(Condition *pCondition, Parameter *parm1, Parameter *parm2, Parameter *parm3, Parameter *parm4);
	Bool evaluateSkirmishUnownedFactionUnitComparison(Parameter *parm1, Parameter *parm2, Parameter *parm3);
	Bool evaluateSkirmishPlayerHasComparisonGarrisoned(Parameter *parm1, Parameter *parm2, Parameter *parm3);
	Bool evaluateSkirmishPlayerHasComparisonCapturedUnits(Parameter *parm1, Parameter *parm2, Parameter *parm3);
	Bool evaluateSkirmishPlayerHasDiscoveredPlayer(Parameter *parm1, Parameter *parm2);
	Bool evaluateDistanceBetweenTeams(Condition *pCondition);
	Bool evaluateCounterSeconds(Condition *pCondition);
	Bool evaluateTeamIsLedByUnit(Parameter *parm1, Parameter *parm2);
	Bool evaluateHasCommandPointsToBuildTeam(Parameter *parm1, Parameter *parm2);
	Bool evaluatePlayerHasKilledTypeUnits(Parameter *parm1, Parameter *parm2, Parameter *parm3);
	Bool evaluateTeamCanPathToWaypoint(Parameter *parm1, Parameter *parm2);
	Bool evaluateUnitHasToggledWeapon(Parameter *parm1, Parameter *parm2);
	Bool evaluateAnyUnitsUsingBloodthirsty(Parameter *parm1);
	Bool evaluateSkirmishSpecialPowerIsReady(Parameter *parm1, Parameter *parm2);
	Bool evaluateSkirmishCanFireSpecialPowerOnTeam(Parameter *parm1, Parameter *parm2, Parameter *parm3);
	Bool evaluateGameModeActive(Parameter *parm1);
	Bool evaluateNamedUnitRankLevel(Parameter *parm1, Parameter *parm2);
	Bool evaluateHasUnits(Parameter *parm1);
	Bool evaluateBuiltByPlayer(Condition *pCondition, Parameter *parm1, Parameter *parm2);
	Bool evaluateNamedReachedWaypointsEnd(Parameter *parm1, Parameter *parm2);
	Bool evaluateTeamReachedWaypointsEnd(Parameter *parm1, Parameter *parm2);
	Bool evaluateNamedSelected(Condition *pCondition, Parameter *parm1);
	Bool evaluatePlayerUnitCondition(Condition *pCondition, Parameter *parm1, Parameter *parm2, Parameter *parm3, Parameter *parm4);
	Bool evaluateHasDelayedCarryoverUnitOfType(const AsciiString &name, Parameter *parm1);

public:
	Bool evaluateEnemySighted(const AsciiString &name, Parameter *parm1, Parameter *parm2);
	Bool evaluateTypeSighted(const AsciiString &name, Parameter *parm1, Parameter *parm2);
	Bool evaluateSkirmishSuppliesWithinDistancePerimeter(Parameter *parm1, Parameter *parm2, Parameter *parm3, Parameter *parm4);
	Bool evaluateIsNumOfUnitsBelongingToPlayerNearEvaEventLastPlayedLocationComparisonInt(Parameter *parm1, Real value, const AsciiString *name, Int value3, Int value4);
protected:
	Bool evaluateNamedUnitExperienceLevelCompare(Parameter *pUnitParm, Parameter *pComparisonParm, Parameter *pLevelParm);	// 0x003E514F
protected:
  Bool evaluatePlayerIsAtRankLevelLimit(Parameter *pPlayerParm);	// 0x003E4519
  Bool evaluatePlayerRankLevelComparison(Parameter *pPlayerParm, Parameter *pComparisonParm, Parameter *pValueParm);	// 0x003E52EB
protected:
  Bool evaluateNumPlayersInGame(Parameter *pComparisonParm, Parameter *pValueParm);
  Bool evaluatePlayerHasNumberObjectsWithModelCondition(Parameter *pPlayerParm, Parameter *pModelConditionParm, Parameter *pComparisonParm, Parameter *pValueParm);
};
// SIZE 0xC
typedef char ScriptConditionsSize[(sizeof(ScriptConditions) == 0xC) ? 1 : -1];

#endif
