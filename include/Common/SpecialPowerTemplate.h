// SpecialPowerTemplate.h: one SpecialPower INI block, an Overridable. SIZE 0x80.
// Target facts: the member offsets are retail's field table 0x009C0E78
// (registered by SpecialPowerStore::parseSpecialPowerDefinition 0x003B15D6;
// map types/fieldparse_entries.csv), and the last entry ends at 0x80.
// Every getter reads through the final override (Zero Hour's getFO):
// getEvaEventToPlayOnSuccess 0x0044EB4B calls 0x00288609 and loads +0x44, and
// SpecialAbilityUpdate::getSpecialPowerType 0x0044EF2C loads +0x1C.
// +0x10 is the block name (not a table entry); +0x14 is the id the store hands out (SpecialPowerStore +0x18 counter).
#pragma once
#ifndef BFME2_SPECIALPOWERTEMPLATE_H
#define BFME2_SPECIALPOWERTEMPLATE_H

#include "Common/GameType.h"
#include "Common/AsciiString.h"
#include "Common/Overridable.h"
#include "Common/Science.h"
#include "Common/Vector.h"

enum SpecialPowerType;
struct FieldParse;

class SpecialPowerTemplate : public Overridable
{
public:
	// Retail's own table 0x00C1ED18 holds only the deleting destructor 0x003B152A.
	SpecialPowerTemplate();											// 0x003B140E (map: ??0Rva003B1101)
	virtual ~SpecialPowerTemplate();								// 0x003B1101 (map: ??1Rva003B1101)
	SpecialPowerTemplate &operator=(const SpecialPowerTemplate &);	// 0x003B1337 (map: ??4Rva003B1101)

	static const FieldParse m_fieldParseTable[];					// RVA 0x009C0E78, parseSpecialPowerDefinition 0x003B15D6 passes it to initFromINI
	// 0x0029FCB4 (map: Rva0029FCB4): returns a copy of the final override's required sciences.
	_STL::vector<ScienceType> rva0029FCB4() const;
	// 0x003B0FAC: stores the block name at +0x10 and the id at +0x14.
	void setNameAndId(const AsciiString &name, Int id);	// 0x003B0FAC (map: Rva003B0FAC)

	const SpecialPowerTemplate *getFO() const { return (const SpecialPowerTemplate *)friend_getFinalOverride(); }

	const AsciiString &getName() const { return getFO()->m_name; }
	SpecialPowerType getSpecialPowerType() const { return getFO()->m_type; }
	bool isSharedNSync() const { return getFO()->m_sharedSyncedTimer; }
	bool hasPublicTimer() const { return getFO()->m_publicTimer; }
	Real getViewObjectRange() const { return getFO()->m_viewObjectRange; }	// +0x50 of the final override (CommandButtonHuntUpdate::scanClosestTarget 0x00495489 reads it through the out-of-line getter)
	Int getLightPointCost() const;									// 0x004210EF
	Int getEvaEventToPlayOnSuccess() const;							// 0x0044EB4B
	const UnsignedByte *getInitiateSound() const;						// 0x004932EF (+0x34 of the final override; decl_queue f11 13:58, ACCEPTED k10)
	const UnsignedByte *getInitiateAtLocationSound() const;			// 0x004932F8 (+0x38 of the final override)
	Real getRadius() const;											// 0x0029B6E2 (+0x54 of the final override)
	Int getViewObjectDuration() const;								// 0x0049330A (+0x4C of the final override)
	AsciiString getPalantirMovie() const;							// 0x00493313 (+0x5C of the final override, copied out; decl_queue f11 14:03, ACCEPTED k10)

	AsciiString m_name;												// 0x10, SpecialPowerModule::getPowerName 0x004934B8 copies it
	UnsignedInt m_id;											// 0x14
	UnsignedInt m_flags;											// 0x18 Flags
	SpecialPowerType m_type;										// 0x1C Enum
	UnsignedInt m_reloadTime;										// 0x20 ReloadTime
	_STL::vector<ScienceType> m_requiredSciences;					// 0x24 RequiredSciences (ctor 0x003B140E builds its _Vector_base)
	Int m_lightPointCost;											// 0x30 LightPointCost
	UnsignedByte m_initiateSound[4];								// 0x34 InitiateSound, AudioEventRTS
	UnsignedByte m_initiateAtLocationSound[4];						// 0x38 InitiateAtLocationSound, AudioEventRTS
	AsciiString m_initiateIntendToDoVoice;							// 0x3C UnitSpecificSoundToUseAsInitiateIntendToDoVoice
	AsciiString m_enterStateInitiateIntendToDoVoice;				// 0x40 UnitSpecificSoundToUseAsEnterStateInitiateIntendToDoVoice
	Int m_evaEventToPlayOnSuccess;									// 0x44 EvaEventToPlayOnSuccess
	UnsignedInt m_detectionTime;									// 0x48 DetectionTime
	UnsignedInt m_viewObjectDuration;								// 0x4C ViewObjectDuration
	Real m_viewObjectRange;											// 0x50 ViewObjectRange
	Real m_radiusCursorRadius;										// 0x54 RadiusCursorRadius
	Bool m_publicTimer;												// 0x58 PublicTimer
	Bool m_sharedSyncedTimer;										// 0x59 SharedSyncedTimer
	AsciiString m_palantirMovie;									// 0x5C PalantirMovie
	UnsignedInt m_objectFilter;										// 0x60 ObjectFilter
	UnsignedByte m_preventActivationConditions[0x10];				// 0x64 PreventActivationConditions
	Real m_maxCastRange;											// 0x74 MaxCastRange
	UnsignedInt m_forbiddenObjectFilter;							// 0x78 ForbiddenObjectFilter
	Real m_forbiddenObjectRange;									// 0x7C ForbiddenObjectRange
};
// SIZE 0x80

#endif
