// EmotionTrackerUpdateEntry.h: one INI-authored emotion. SIZE 0x190 (IniParseAddEmotion 0x004B2087 allocates 0x190;
// rva004B0EBC 0x004B0EBC reads +0x18C and the parser stores +0x18D).
// Target facts: the fields are the offsets OnActivation 0x004DCC84, OnDeactivation
// 0x004DD2C0, CanBeActivatedFor 0x004DD018 and the per-frame step 0x004DCEC4 read
// through their own private views in the old repo (Rva004DC902AsciiField,
// Rva004DC8E7AsciiField, EmotionEntry); those views agree on every offset below.
// The two AsciiString getters are out of line: 0x004DC8E7 hands back the attribute
// modifier name (+0x3C) and 0x004DC902 the Lua event name (+0x188).
#pragma once
#ifndef BFME2_EMOTIONTRACKERUPDATEENTRY_H
#define BFME2_EMOTIONTRACKERUPDATEENTRY_H

#include "Common/AsciiString.h"
#include "Common/GameType.h"

class FXList;
class INI;

class EmotionTrackerUpdateEntry
{
public:
	EmotionTrackerUpdateEntry();			// 0x004DC920 (out of line, 205 B)
	void rva004DC788(const EmotionTrackerUpdateEntry &other);	// 0x004DC788 (map ?rva004DC788@Rva004DC920@@QAEXABV1@@Z, 246 B): copies another entry
	void rva004DCAD2(INI *ini);			// 0x004DCAD2 (map ?rva004DCAD2@Rva004DCAD2@@QAEXPAVINI@@@Z): parses an OVERRIDE block into the entry
	~EmotionTrackerUpdateEntry();			// 0x004DC9ED (out of line: EmotionTrackerUpdateModuleData's destructor 0x004B1285 calls it before the delete)
	AsciiString getAttributeModifierName() const;
	AsciiString getEventName() const;
	// The per-frame step reads the start delay through an inline getter (old repo
	// EmotionNugget.cpp, 0x004DCEC4): that spelling gives retail's load order.
	int getModifierDelay() const { return m_attributeDelay; }

	AsciiString m_name;							// 0x00 the emotion's name (IniParseAddEmotion 0x004B2087 stores the token through StringBase::set)
	int m_type;									// 0x04 3 = ends with its source object, 4..7 = attribute kinds
	Bool m_08;									// 0x08 idle-only
	Bool m_09;									// 0x09 busy-only
	UnsignedInt m_duration;						// 0x0C
	int m_nextDelay;							// 0x10
	int m_targetDelay;							// 0x14
	int m_templateDelay;						// 0x18
	int m_1c;									// 0x1C
	int m_20;									// 0x20
	int m_24;									// 0x24
	int m_28;									// 0x28
	Bool m_2c;									// 0x2C
	Bool m_2d;									// 0x2D
	const FXList *m_activationFX;				// 0x30
	const FXList *m_updateFX;					// 0x34
	const FXList *m_deactivationFX;				// 0x38
	AsciiString m_attributeModifier;			// 0x3C
	int m_attributeDelay;						// 0x40
	Bool m_flag44;								// 0x44
	int m_strength;								// 0x48
	int m_emotionState;							// 0x4C
	int m_aiDelay;								// 0x50
	int m_conditions54[19];						// 0x54 second argument on activation
	int m_conditionsA0[19];						// 0xA0 first argument on deactivation
	int m_conditionsEC[19];						// 0xEC first argument on activation
	int m_conditions138[19];					// 0x138 second argument on deactivation
	Bool m_allowStateChange;					// 0x184
	AsciiString m_eventName;					// 0x188
	Bool m_18c;									// 0x18C read by EmotionTrackerUpdate::rva004B0EBC
	Bool m_18d;									// 0x18D set by IniParseAddEmotion after it parses a block whose first token is the string at 0x00C56770
};
// SIZE 0x190

#endif
