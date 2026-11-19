// ThingTemplate.h: SAGE object template, one Object INI block. An Overridable.
// Target facts: the members are retail's field table 0x009BECD8 (191 entries,
// map types/fieldparse_entries.csv): the first member is at 0x10, right after
// Overridable (Overridable.h), Geometry at 0xA0 is the 0x5C GeometryInfo that
// ends where GeometryOther starts (0xFC), KindOf is at 0x108, and the last
// entries reach 0x63C. Entries whose parser has no scalar type keep their INI
// token and their extent to the next entry. Eva-event entries (Rva001DEE74Parse
// stores the iniScanEvaEventID result, 4 bytes) and index-list bytes
// (dup_002EC6E stores one byte) are typed from h05's parse-function catalogue. m_templateID (0x5D8) is not in the
// table: OnActivation 0x004DCC84 keys its per-template map with it. The class
// size is not yet proven beyond 0x640.
#pragma once
#ifndef BFME2_THINGTEMPLATE_H
#define BFME2_THINGTEMPLATE_H

#include <stddef.h>
#include "Common/KindOf.h"
#include "Common/GameType.h"
#include "Common/AsciiString.h"
#include "Common/UnicodeString.h"
#include "Common/Coord3D.h"
#include "Common/Geometry.h"
#include "Common/Overridable.h"
#include "Common/ProductionPrerequisite.h"
#include "Common/ModuleInfo.h"

class AIUpdateModuleData;

class INI;
struct FieldParse;
class WeaponTemplate;
class WeaponTemplateSet;
class ArmorTemplateSet;
#include "Common/BitFlags.h"
class LocomotorTemplate;
typedef std::vector<const LocomotorTemplate *> LocomotorTemplateVector;	// Zero Hour's typedef

// The bit test retail's isKindOf COMDAT calls (0x001E4426, cdecl, (unsigned int *, int));
// the map has only a placeholder name for it, kept here so references resolve.
inline Bool Rva001E4426Test(UnsignedInt *words, Int bit)
{
	return (words[(UnsignedInt)bit >> 5] & (1u << (bit & 31))) != 0;
}

// How the field table is being read (ThingTemplate +0x5F8, a signed byte). parseAddModule
// 0x0033A50D sets 1 around its nested read and parseInheritableModule 0x0033A561 sets 2;
// both throw "Expected oldMode to be MODULEPARSE_NORMAL" when it is not 0.
enum ModuleParseMode
{
	MODULEPARSE_NORMAL = 0,
	MODULEPARSE_ADD_REMOVE_REPLACE = 1,
	MODULEPARSE_INHERITABLE = 2
};

class Image;

class ThingTemplate : public Overridable
{
public:
	ThingTemplate();	// 0x0033E1B8 (decl_queue f01 07:20)
	void copyFrom(const ThingTemplate *other);	// 0x0033EBEB
	void resolveNames();	// 0x0033C2C2
	unsigned short getTemplateID() const { return m_templateID; }
	const AsciiString &getName() const { return m_name; }
	ThingTemplate *friend_getNextTemplate() const { return m_nextTemplate; }
	// 0x0033DCD1: the weapon set chosen for these WeaponSetFlags (BitFlags<104>, 104 names at
	// VA 0x00DBC2C8); WeaponSet::xfer 0x002C8872 looks a loaded set up through it.
	const WeaponTemplateSet *findWeaponTemplateSet(const BitFlags<104> &flags) const;
	// 0x0033DCB8: the armor set chosen for these ArmorSetFlags (BitFlags<21>); the map files it under the placeholder
	// class Rva0033DCB8 with a BitFlags<17> argument (ICF fold). ActiveBody::validateArmorAndDamageFX calls it.
	const ArmorTemplateSet *findArmorTemplateSet(const BitFlags<21> &flags) const;
	// 0x0033CF34: the asset-list walk of this template; the map spells both arguments Int (the first is really
	// an AssetList &). BuffNuggetFXNugget::GetAssetList 0x001E30F5 tail-calls it with its own arguments.
	void GetAssetList(Int assets, Int context);
	// 0x0033BB04: whether the other template is this one or one equivalent to it (the queue
	// walks of ProductionUpdate compare a queued entry's template this way).
	Bool isEquivalentTo(const ThingTemplate *other) const;
	// 0x0033A632: GameLogic's per-name override (findBuildableStatusOverride 0x00245EDF), else
	// the INI Buildable byte (+0x5F2, read with movsx).
	BuildableStatus getBuildable() const;
	// 0x0033B580: resolves the ButtonImage name (+0x78) through TheMappedImageCollection once,
	// caches the image at +0x48C and clears the name.
	const Image *getButtonImage();
	// 0x0033BA46 (map placeholder ?rva0033BA46@ThingTemplate@@QAEPBVImage@@XZ): the portrait image, looked up and cached on
	// first use like getButtonImage 0x0033B580; resolveNames 0x0033C2C2 calls both (Zero Hour's getter name).
	const Image *getSelectedPortraitImage();
	// 0x0033A68A: the last override of this template, or the template itself (through the
	// const friend_getFinalOverride 0x00288609; three ThingFactory-side callers, e.g. 0x00244D56).
	const ThingTemplate *getFinalOverride() const;
	Int calcCostToBuild(const Player *player, Int unused, Int base) const;	// 0x0033A69A (decl_queue f04 04:10, ACCEPTED k06: map ?rva0033A69A, Zero Hour role name)
	Int calcTimeToBuild(const Player *player, Int unused, Int base) const;	// 0x0033AA1F
	// Zero Hour's: the first prerequisite's existing build facility (ProductionPrerequisite 0x004F4C85).
	Int getPrereqCount() const { return m_prereqInfo.size(); }
	const ThingTemplate *getBuildFacilityTemplate(const Player *player) const;	// 0x0033A97F
	AIUpdateModuleData *friend_getAIModuleInfo();	// 0x0033B387
	void validate();	// 0x0033B4CD
	void setCopiedFromDefault();	// 0x0033B539
	// 0x0033A5F4: the five shadow-opacity fields in one call (W3DScriptedModelDraw 0x000C4E23):
	// start +0x5AC, fade-in +0x4F8, peak +0x5B0, fade-out +0x4FC, end +0x5B4.
	void getShadowOpacityValues(UnsignedInt *start, Real *fadeInTime, UnsignedInt *peak, Real *fadeOutTime, UnsignedInt *end) const;
	// 0x115 is byte 0x0D of the KindOf flags (0x108..0x124). CanBeActivatedFor
	// 0x004DD018 reads it as a byte and, when set, takes the attribute test to the
	// object 0x0028C197 slot 68 hands back instead of the emotion's own object.
	// 0x0028A8B5 (no map row yet; role name): clamps *value to the template's [MinVisionBonusPercent +0x4C4,
	// MaxVisionBonusPercent +0x4C0] (Object::getVisionRange 0x0028DDE0 calls it out of line).
	void clampVisionBonusPercent(Real *value) const;
	Bool isKindOf0x115() const { return (((const unsigned char *)m_kindof.m_bits)[0x0D] & 0x20) != 0; }
	// KindOf test, inline in retail: with a constant kind /O1 expands it to one byte test
	// (Thing::setPosition 0x0030AA80: byte +0x108 & 0x10; Object::didEnterOrExit
	// 0x0028D6EB: byte +0x113 & 2), and with a run-time kind it calls the COMDAT
	// 0x000456AC, which calls the bit-test COMDAT 0x001E4426. All 11 calls of 0x000456AC
	// pass a variable (InGameUI::isAnySelectedKindOf 0x0029C0D6, Team 0x0039DC05,
	// ScriptConditions 0x003E64A6, HordeContain 0x0046D2C3, GameLogic::loadAllObjects,
	// ScriptEngine::setPriorityKind, LuaScriptEngine 0x00334D5E, Thing's 0x000456C1).
	// Zero Hour's lookup in the locomotor-set map (+0x3AC, inside the span the header still types as m_unitSpecificFX):
	// NULL when the map is empty or has no entry for the set (AIUpdateInterface::chooseLocomotorSetExplicit 0x00268A37).
	const LocomotorTemplateVector *findLocomotorTemplateVector(Int t) const;	// 0x0033C288
	// The LocomotorSet block's Speed per set (the map at +0x3A0; the global default when it is empty or has no entry):
	// AIUpdateInterface::chooseLocomotorSet 0x00268B20 stores it at +0x1F8 after switching sets. Role name.
	Real getLocomotorSetSpeed(Int t) const;	// 0x0033C259
	Bool isKindOf(KindOfType t) const { return Rva001E4426Test(const_cast<UnsignedInt *>(m_kindof.m_bits), t); }
	Real getRamPower() const;										// 0x004BB968 (out of line; SquishCollide.cpp)
	Real getRamZMult() const;										// 0x004BB96F

	Real m_camouflageDetectionMultiplier;								// 0x010 CamouflageDetectionMultiplier
	UnsignedByte m_formationPreviewDecal[0xC];							// 0x014 FormationPreviewDecal (parsed by Rva0033A49B_ParseDecal)
	UnsignedByte m_formationPreviewItemDecal[0xC];							// 0x020 FormationPreviewItemDecal (parsed by Rva0033A49B_ParseDecal)
	AsciiString m_displayNameLabel;								// 0x02C DisplayName: the INI label (parsed by Rva0033B938_ParseTranslatedLabel)
	UnicodeString m_displayName;								// 0x030 DisplayName: its translation (ProductionPrerequisite::getRequiresList 0x004F4DF2 reads it; UpgradeTemplate keeps the same label/text pair)
	// The translated label pairs (label, then its translation; Rva0033B938_ParseTranslatedLabel fills both);
	// validate 0x0033B4CD copies the plain texts into empty strategic ones.
	AsciiString m_descriptionLabel;								// 0x034 Description
	UnicodeString m_description;								// 0x038
	AsciiString m_descriptionStrategicLabel;								// 0x03C DescriptionStrategic
	UnicodeString m_descriptionStrategic;								// 0x040
	UnsignedByte m_reviveText[0x8];							// 0x044 ReviveText (parsed by Rva0033B938_ParseTranslatedLabel)
	UnsignedByte m_recruitText[0x8];							// 0x04C RecruitText (parsed by Rva0033B938_ParseTranslatedLabel)
	AsciiString m_displayNameStrategicLabel;								// 0x054 DisplayNameStrategic
	UnicodeString m_displayNameStrategic;								// 0x058
	AsciiString m_displayNameInvisibleForEnemy;								// 0x05C DisplayNameInvisibleForEnemy
	AsciiString m_hotkey;								// 0x060 Hotkey
	AsciiString m_name;								// 0x064, the block name: ThingFactory::addTemplate 0x002D04A3 compares it
	AsciiString m_editorName;								// 0x068 EditorName
	AsciiString m_side;								// 0x06C Side
	AsciiString m_commandSet;								// 0x070 CommandSet
	AsciiString m_selectPortrait;								// 0x074 SelectPortrait
	AsciiString m_buttonImage;								// 0x078 ButtonImage
	AsciiString m_upgradeCameo1;								// 0x07C UpgradeCameo1
	AsciiString m_upgradeCameo2;								// 0x080 UpgradeCameo2
	AsciiString m_upgradeCameo3;								// 0x084 UpgradeCameo3
	AsciiString m_upgradeCameo4;								// 0x088 UpgradeCameo4
	AsciiString m_upgradeCameo5;								// 0x08C UpgradeCameo5
	AsciiString m_shadowTexture;								// 0x090 ShadowTexture
	UnsignedByte m_unk0x094[0x8];        // 0x094
	AsciiString m_experienceScalarTable;								// 0x09C ExperienceScalarTable
	GeometryInfo m_geometryInfo;								// 0x0A0 AdditionalGeometry
	UnsignedByte m_geometryOther[0xC];							// 0x0FC GeometryOther (parsed by parseGeometryOther)
	KindOfMaskType m_kindof;								// 0x108 KindOf (BitFlags<218>: ThingTemplate::isKindOfMulti 0x0030A8E3 is `add ecx,0x108; jmp BitFlags<218>::testSetAndClear`)
	UnsignedByte m_unk0x124[0x1C0];        // 0x124
	// The four module lists (ModuleInfo.h; parseModuleName fills them, setCopiedFromDefault 0x0033B539
	// calls ModuleInfo::setCopiedFromDefault 0x0033B18A on each in this order). Zero Hour's names.
	ModuleInfo m_behaviorModuleInfo;							// 0x2E4 Behavior
	ModuleInfo m_drawModuleInfo;							// 0x2F0 Draw
	ModuleInfo m_clientUpdateModuleInfo;							// 0x2FC ClientUpdate
	ModuleInfo m_clientBehaviorModuleInfo;							// 0x308 ClientBehavior (BFME 2)
	UnsignedByte m_flammability[0x10];							// 0x314 Flammability (parsed by parseFlammability)
	// 0x324 Prerequisites: parsePrerequisites 0x0033EBB5 clears it (vector::clear 0x0033EBAA ->
	// erase 0x0033EB77) on an override load and refills it through INI::initFromINIMulti;
	// getBuildFacilityTemplate reads it. Zero Hour's m_prereqInfo.
	std::vector<ProductionPrerequisite> m_prereqInfo;					// 0x324
	std::vector<AsciiString> m_buildVariations;						// 0x330 BuildVariations
	std::vector<AsciiString> m_equivalentTo;							// 0x33C EquivalentTo (isEquivalentTo 0x0033BB04)
	std::vector<AsciiString> m_buildFadeInOnCreateList;	// 0x348 BuildFadeInOnCreateList uses INI::parseAsciiStringVector.
	Real m_buildFadeInOnCreateTime;								// 0x354 BuildFadeInOnCreateTime
	UnsignedByte m_unk0x358[0x30];        // 0x358
	UnsignedByte m_unitSpecificSounds[0xC];							// 0x388 UnitSpecificSounds (parsed by Rva0033DC9CParse)
	UnsignedByte m_unitSpecificFX[0x24];							// 0x394 UnitSpecificFX (parsed by Rva0033DBC9Parse)
	UnsignedByte m_browser[0x34];							// 0x3B8 Browser (parsed by Rva00339E1C_ParseBrowser)
	UnsignedByte m_autoResolveArmor[0xC];							// 0x3EC AutoResolveArmor (parsed by Rva00542BA1Parse)
	UnsignedByte m_autoResolveWeapon[0xC];							// 0x3F8 AutoResolveWeapon (parsed by Rva00542B24Parse)
	UnsignedByte m_worldMapArmoryUpgradesAllowed[0x80];							// 0x404 WorldMapArmoryUpgradesAllowed (parsed by Rva0026F28BParse)
	ThingTemplate *m_nextTemplate;							// 0x484 ThingFactory's template list (addTemplate 0x002D04A3, freeDatabase 0x002D0469)
	UnsignedByte m_unk0x488[0x4];        // 0x488
	const Image *m_buttonImageCache;					// 0x48C getButtonImage 0x0033B580
	UnsignedByte m_autoResolveBody[0x4];							// 0x490 AutoResolveBody (parsed by Rva00418853Parse)
	UnsignedByte m_autoResolveLeadership[0x4];							// 0x494 AutoResolveLeadership (parsed by Rva00418143Parse)
	UnsignedByte m_autoResolveCombatChain[0x4];							// 0x498 AutoResolveCombatChain (parsed by Rva00419182Parse)
	UnsignedByte m_crowdResponseKey[0x4];							// 0x49C CrowdResponseKey (parsed by Rva00415481Parse)
	Real m_fenceWidth;								// 0x4A0 FenceWidth
	Real m_fenceXOffset;								// 0x4A4 FenceXOffset
	Real m_removeTerrainRadius;								// 0x4A8 RemoveTerrainRadius
	Real m_visionRange;								// 0x4AC VisionRange
	Real m_shroudClearingRange;								// 0x4B0 ShroudClearingRange
	Real m_visionSide;								// 0x4B4 VisionSide
	Real m_visionRear;								// 0x4B8 VisionRear
	Real m_visionBonusPercentPerFoot;								// 0x4BC VisionBonusPercentPerFoot
	Real m_maxVisionBonusPercent;								// 0x4C0 MaxVisionBonusPercent
	Real m_minVisionBonusPercent;								// 0x4C4 MinVisionBonusPercent
	Real m_visionBonusTestRadius;								// 0x4C8 VisionBonusTestRadius
	Real m_emotionRange;								// 0x4CC EmotionRange
	Real m_placementViewAngle;								// 0x4D0 PlacementViewAngle
	Real m_factoryExitWidth;								// 0x4D4 FactoryExitWidth
	Real m_factoryExtraBibWidth;								// 0x4D8 FactoryExtraBibWidth
	Real m_buildTime;								// 0x4DC BuildTime
	Real m_scale;								// 0x4E0 Scale
	Real m_instanceScaleFuzziness;								// 0x4E4 InstanceScaleFuzziness
	Real m_shadowSizeX;								// 0x4E8 ShadowSizeX
	Real m_shadowSizeY;								// 0x4EC ShadowSizeY
	Real m_shadowOffsetX;								// 0x4F0 ShadowOffsetX
	Real m_shadowOffsetY;								// 0x4F4 ShadowOffsetY
	Real m_shadowOpacityFadeInTime;								// 0x4F8 ShadowOpacityFadeInTime
	Real m_shadowOpacityFadeOutTime;								// 0x4FC ShadowOpacityFadeOutTime
	Real m_shadowSunAngle;								// 0x500 ShadowSunAngle
	Real m_shadowMaxHeight;								// 0x504 ShadowMaxHeight
	Real m_maxDistanceForEngaged;								// 0x508 MaxDistanceForEngaged
	Real m_minCrushVelocityPercent;								// 0x50C MinCrushVelocityPercent
	Real m_crushDecelerationPercent;								// 0x510 CrushDecelerationPercent
	Real m_crushKnockback;								// 0x514 CrushKnockback
	Real m_crushZFactor;								// 0x518 CrushZFactor
	Real m_threatLevel;								// 0x51C ThreatLevel
	UnsignedByte m_threatBreakdown[0x4];							// 0x520 ThreatBreakdown (parsed by Rva0033A7A2Parse)
	Real m_minZIncreaseForVoiceMoveToHigherGround;								// 0x524 MinZIncreaseForVoiceMoveToHigherGround
	Real m_geometryFrontAngle;								// 0x528 GeometryFrontAngle
	Real m_pathfindDiameter;								// 0x52C PathfindDiameter
	Real m_threatRadius;								// 0x530 ThreatRadius
	Real m_healthBoxScale;								// 0x534 HealthBoxScale
	Real m_healthBoxHeightOffset;								// 0x538 HealthBoxHeightOffset
	Real m_firingArc;								// 0x53C FiringArc
	Real m_campnessValueRadius;								// 0x540 CampnessValueRadius
	UnsignedInt m_visionBonusTestSegments;								// 0x544 VisionBonusTestSegments
	Int m_energyProduction;								// 0x548 EnergyProduction
	Int m_energyBonus;								// 0x54C EnergyBonus
	Int m_displayColor;								// 0x550 DisplayColor
	UnsignedInt m_occlusionDelay;								// 0x554 OcclusionDelay
	UnsignedInt m_voiceAttackChargeTimeout;								// 0x558 VoiceAttackChargeTimeout
	UnsignedInt m_engagedStateTimeout;								// 0x55C EngagedStateTimeout
	Int m_voicePriority;								// 0x560 VoicePriority
	Int m_formationDepth;								// 0x564 FormationDepth
	Int m_formationWidth;								// 0x568 FormationWidth
	UnsignedInt m_slopeLimitIndex;								// 0x56C SlopeLimitIndex
	Int m_bountyValue;								// 0x570 BountyValue
	Int m_evaEventDieOwner;							// 0x574 EvaEventDieOwner (EvaMessage, iniScanEvaEventID)
	Int m_evaEventDieAlly;							// 0x578 EvaEventDieAlly (EvaMessage, iniScanEvaEventID)
	Int m_evaEventDieEnemy;							// 0x57C EvaEventDieEnemy (EvaMessage, iniScanEvaEventID)
	Int m_evaEventDamagedOwner;							// 0x580 EvaEventDamagedOwner (EvaMessage, iniScanEvaEventID)
	Int m_evaEventDamagedFromShroudedSourceOwner;							// 0x584 EvaEventDamagedFromShroudedSourceOwner (EvaMessage, iniScanEvaEventID)
	Int m_evaEventDamagedByFireOwner;							// 0x588 EvaEventDamagedByFireOwner (EvaMessage, iniScanEvaEventID)
	Int m_evaEventSecondDamageFarFromFirstOwner;							// 0x58C EvaEventSecondDamageFarFromFirstOwner (EvaMessage, iniScanEvaEventID)
	Real m_evaEventSecondDamageFarFromFirstScanRange;								// 0x590 EvaEventSecondDamageFarFromFirstScanRange
	UnsignedInt m_evaEventSecondDamageFarFromFirstTimeoutMS;								// 0x594 EvaEventSecondDamageFarFromFirstTimeoutMS
	Int m_evaEventAmbushed;							// 0x598 EvaEventAmbushed (EvaMessage, iniScanEvaEventID)
	Int m_evaEnemyObjectSightedEvent;							// 0x59C EvaEnemyObjectSightedEvent (EvaMessage, iniScanEvaEventID)
	Int m_evaEnemyObjectSightedAfterRespawnEvent;							// 0x5A0 EvaEnemyObjectSightedAfterRespawnEvent (EvaMessage, iniScanEvaEventID)
	Int m_evaOnFirstSightingEventEnemy;							// 0x5A4 EvaOnFirstSightingEventEnemy (EvaMessage, iniScanEvaEventID)
	Int m_evaOnFirstSightingEventNonEnemy;							// 0x5A8 EvaOnFirstSightingEventNonEnemy (EvaMessage, iniScanEvaEventID)
	UnsignedInt m_shadowOpacityStart;								// 0x5AC ShadowOpacityStart
	UnsignedInt m_shadowOpacityPeak;								// 0x5B0 ShadowOpacityPeak
	UnsignedInt m_shadowOpacityEnd;								// 0x5B4 ShadowOpacityEnd
	Int m_supplyOverride;								// 0x5B8 SupplyOverride
	Int m_displayMeleeDamage;								// 0x5BC DisplayMeleeDamage
	Int m_displayRangedDamage;								// 0x5C0 DisplayRangedDamage
	Int m_autoResolveUnitType;								// 0x5C4 AutoResolveUnitType
	Int m_evaEventDetectedEnemy;							// 0x5C8 EvaEventDetectedEnemy (EvaMessage, iniScanEvaEventID)
	Int m_evaEventDetectedAlly;							// 0x5CC EvaEventDetectedAlly (EvaMessage, iniScanEvaEventID)
	Int m_evaEventDetectedOwner;							// 0x5D0 EvaEventDetectedOwner (EvaMessage, iniScanEvaEventID)
	UnsignedInt m_campnessValue;								// 0x5D4 CampnessValue
	UnsignedShort m_templateID;								// 0x5D8 TemplateID
	UnsignedShort m_buildCost;								// 0x5DA BuildCost
	UnsignedShort m_refundValue;								// 0x5DC RefundValue
	UnsignedShort m_threatValue;								// 0x5DE ThreatValue
	UnsignedShort m_maxSimultaneousOfType;								// 0x5E0 MaxSimultaneousOfType
	UnsignedShort m_shadow;								// 0x5E2 Shadow
	Bool m_isPrerequisite;								// 0x5E4 IsPrerequisite
	Bool m_isBridge;								// 0x5E5 IsBridge
	Bool m_isBuildFacility;								// 0x5E6 IsBuildFacility (Team::hasAnyBuildFacility 0x0039E815 tests it; Zero Hour field order)
	Bool m_isTrainable;								// 0x5E7 IsTrainable
	Bool m_isForbidden;								// 0x5E8 IsForbidden
	Bool m_armorCopiedFromDefault;		// 0x5E9 (setCopiedFromDefault 0x0033B539 sets both; Zero Hour's order)
	Bool m_weaponsetsCopiedFromDefault;	// 0x5EA
	Bool m_isGrabbable;								// 0x5EB IsGrabbable
	Bool m_isHarvestable;								// 0x5EC IsHarvestable
	Bool m_shadowOverrideLODVisibility;								// 0x5ED ShadowOverrideLODVisibility
	Bool m_shadowUseHouseColor;								// 0x5EE ShadowUseHouseColor
	Bool m_shadowIsRotatingWithObject;								// 0x5EF ShadowIsRotatingWithObject
	UnsignedByte m_radarPriority;							// 0x5F0 RadarPriority (index list, one byte)
	UnsignedByte m_transportSlotCount;								// 0x5F1 TransportSlotCount
	Char m_buildable;									// 0x5F2 Buildable (index list, one byte; signed: getBuildable 0x0033A632 reads it with movsx)
	UnsignedByte m_buildCompletion;							// 0x5F3 BuildCompletion (index list, one byte)
	UnsignedByte m_editorSorting;							// 0x5F4 EditorSorting (index list, one byte)
	UnsignedByte m_deadCollideSize;							// 0x5F5 DeadCollideSize (index list, one byte)
	UnsignedByte m_thingClass;							// 0x5F6 ThingClass (index list, one byte)
	UnsignedByte m_structureRubbleHeight;								// 0x5F7 StructureRubbleHeight
	Char m_moduleParsingMode;					// 0x5F8 a ModuleParseMode (read with movsx)
	Char m_crusherLevel;								// 0x5F9 CrusherLevel
	Char m_crushableLevel;								// 0x5FA CrushableLevel
	Char m_mountedCrusherLevel;								// 0x5FB MountedCrusherLevel
	Char m_mountedCrushableLevel;								// 0x5FC MountedCrushableLevel
	Bool m_useCrushAttack;								// 0x5FD UseCrushAttack
	Bool m_crushOnlyWhileCharging;								// 0x5FE CrushOnlyWhileCharging
	UnsignedByte m_unk0x5FF[0x1];        // 0x5FF
	const WeaponTemplate * m_crushWeapon;								// 0x600 CrushWeapon
	const WeaponTemplate * m_crushRevengeWeapon;								// 0x604 CrushRevengeWeapon
	Real m_ramPower;								// 0x608 RamPower
	Real m_ramZMult;								// 0x60C RamZMult
	Real m_shockwaveResistance;								// 0x610 ShockwaveResistance
	Bool m_crushAllies;								// 0x614 CrushAllies
	UnsignedByte m_unk0x615[0x3];        // 0x615
	Int m_commandPoints;								// 0x618 CommandPoints
	Int m_commandPointBonus;								// 0x61C CommandPointBonus
	Coord3D m_liveCameraOffset;								// 0x620 LiveCameraOffset
	Real m_liveCameraPitch;								// 0x62C LiveCameraPitch
	Bool m_forceLuaRegistration;								// 0x630 ForceLuaRegistration
	Bool m_showHealthInSelectionDecal;								// 0x631 ShowHealthInSelectionDecal
	Bool m_keepSelectableWhenDead;								// 0x632 KeepSelectableWhenDead
	Bool m_isAutoBuilt;								// 0x633 IsAutoBuilt
	Bool m_canPathThroughGates;								// 0x634 CanPathThroughGates
	Bool m_shouldClearShotsOnIdle;								// 0x635 ShouldClearShotsOnIdle
	UnsignedByte m_unk0x636[0x2];        // 0x636
	Int m_heroSortOrder;								// 0x638 HeroSortOrder
	Int m_selectionPriority;								// 0x63C SelectionPriority

protected:
	static void parseAddModule(INI *ini, void *instance, void *store, const void *userData);			// 0x0033A50D
	static void parseInheritableModule(INI *ini, void *instance, void *store, const void *userData);	// 0x0033A561
	static void parsePrerequisites(INI *ini, void *instance, void *store, const void *userData);		// 0x0033EBB5

private:
	static const FieldParse s_objectFieldParseTable[];	// VA 0x00DBECD8, 191 entries
};

// The fade-in vector has the same 12-byte storage the field table reserves.
typedef char ThingTemplateFadeInListLayout[
	(sizeof(std::vector<AsciiString>) == 0xC
		&& offsetof(ThingTemplate, m_buildFadeInOnCreateList) == 0x348) ? 1 : -1];

#include "Common/Thing.h"
inline Bool Thing::isKindOf(KindOfType t) const { return getTemplate()->isKindOf(t); }

#endif
