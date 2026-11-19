// DRAFT (h08): ExperienceLevel, one entry of an experience-level list. SIZE 0x108.
// No seeded row compiles against it yet (ExperienceLevelSystem.cpp holds only the parser
// 0x0028A294); compile-checked only.
//
// Target facts:
// - Size: AddExperienceLevel 0x0028A1AA and CreateNewExpLevel 0x0028A473 copy levels by value
//   into STLport lists whose node payload the map calls BfmePod264 (264 = 0x108 bytes); the copy
//   constructor is 0x00289CEB and the virtual destructor 0x002894A2.
// - Base: Overridable (GetLevelRank 0x0028879C and GetRequiredExperience 0x002887B8 follow
//   +0x04 to the final override before reading); operator< 0x00288621 compares +0x18.
// - Fields from the field table 0x007FBAC0: RequiredExperience +0x18, ExperienceAward +0x1C,
//   ExperienceAwardOwnGuysDie +0x20, TargetNames +0x24 and AttributeModifiers +0x30 (string
//   vectors), LevelUpOCL +0x48, SelectionDecal +0xA4, ShowLevelUpTint +0xD8, LevelUpTintColor
//   +0xDC (RGB), LevelUpTintPre/Post/SustainColorTime +0xE8/+0xEC/+0xF0, LevelUpTintFrequency
//   +0xF4, LevelUpTintAmplitude +0xF8, Rank +0xFC, InformUpdateModule +0x100, SinglePlayerOnly
//   +0x101, MultiPlayerOnly +0x102, EmotionType +0x104 (a bit set). Upgrades, LevelUpFx and
//   ModelConditionState are parsed by offset-0 class callbacks, so their storage (+0x10..+0x17,
//   +0x3C..+0x47, +0x4C..+0xA3) is not placed by the table.
#pragma once
#ifndef BFME2_EXPERIENCELEVEL_H
#define BFME2_EXPERIENCELEVEL_H

#include <stddef.h>
#include <vector>
#include "Common/GameType.h"
#include "Common/AsciiString.h"
#include "Common/Overridable.h"
#include "Common/RGBColor.h"

class ObjectCreationList;

class ExperienceLevel : public Overridable
{
public:
	ExperienceLevel(const ExperienceLevel &that);						// 0x00289CEB
	virtual ~ExperienceLevel();											// 0x002894A2
	Bool operator<(const ExperienceLevel &that) const;					// 0x00288621

	UnsignedByte m_unk0x10[0x8];										// 0x10 not placed by the table
	Int m_requiredExperience;											// 0x18
	Int m_experienceAward;												// 0x1C
	Int m_experienceAwardOwnGuysDie;									// 0x20
	std::vector<AsciiString> m_targetNames;								// 0x24
	std::vector<AsciiString> m_attributeModifiers;						// 0x30
	UnsignedByte m_unk0x3C[0xC];										// 0x3C not placed by the table
	const ObjectCreationList *m_levelUpOCL;								// 0x48
	UnsignedByte m_unk0x4C[0x58];										// 0x4C not placed by the table
	UnsignedByte m_selectionDecal[0x34];								// 0xA4 SelectionDecal (decal record, type unresolved)
	Bool m_showLevelUpTint;												// 0xD8
	RGBColor m_levelUpTintColor;										// 0xDC
	Int m_levelUpTintPreColorTime;										// 0xE8
	Int m_levelUpTintPostColorTime;										// 0xEC
	Int m_levelUpTintSustainColorTime;									// 0xF0
	Real m_levelUpTintFrequency;										// 0xF4
	Real m_levelUpTintAmplitude;										// 0xF8
	Int m_rank;															// 0xFC
	Bool m_informUpdateModule;											// 0x100
	Bool m_singlePlayerOnly;											// 0x101
	Bool m_multiPlayerOnly;												// 0x102
	UnsignedInt m_emotionType;											// 0x104 EmotionType bits
};
// SIZE 0x108
typedef char ExperienceLevel_off_m_requiredExperience[(offsetof(ExperienceLevel, m_requiredExperience) == 0x18) ? 1 : -1];
typedef char ExperienceLevel_off_m_targetNames[(offsetof(ExperienceLevel, m_targetNames) == 0x24) ? 1 : -1];
typedef char ExperienceLevel_off_m_levelUpOCL[(offsetof(ExperienceLevel, m_levelUpOCL) == 0x48) ? 1 : -1];
typedef char ExperienceLevel_off_m_showLevelUpTint[(offsetof(ExperienceLevel, m_showLevelUpTint) == 0xD8) ? 1 : -1];
typedef char ExperienceLevel_off_m_rank[(offsetof(ExperienceLevel, m_rank) == 0xFC) ? 1 : -1];
typedef char ExperienceLevel_size[(sizeof(ExperienceLevel) == 0x108) ? 1 : -1];

#endif
