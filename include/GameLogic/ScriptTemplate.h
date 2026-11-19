// ScriptTemplate.h: the table entry that describes one script condition or action type
// (its UI name, its UI strings and its parameter types). Zero Hour's Scripts.h declares
// the same classes under the same names; BFME 2's layout differs by one word at the front.
// Target facts (code RVAs):
//  - SIZE 0x80: ScriptEngine's constructor 0x00208343 builds an array of 0x257 records of
//    0x80 bytes each, and getActionTemplate 0x00203926 / getConditionTemplate 0x00203941
//    index it.
//  - +0x04 is the name string (Condition::text 0x003B3989 and ScriptAction::text 0x003B3AB9
//    copy an AsciiString from template +0x04).
//  - Template::getUiStrings 0x003B276B copies [+0x18, +0x18 + 4 * count) where the count is
//    the word at +0x14 and returns it. Template::getParameterType 0x003B27A3 bounds an index
//    by the word at +0x48 and reads the parameter type from +0x4C + 4 * index.
//  - Condition::rva003B275A 0x003B275A and ScriptAction::rva003B2832 0x003B2832 return the
//    first word of their template.
#pragma once
#ifndef BFME2_SCRIPTTEMPLATE_H
#define BFME2_SCRIPTTEMPLATE_H

#include "Common/GameType.h"
#include "Common/AsciiString.h"
#include "GameLogic/Scripts.h"

class Template
{
public:
	Int getUiStrings(AsciiString strings[MAX_PARMS]) const;				// 0x003B276B
	Parameter::ParameterType getParameterType(Int ndx) const;			// 0x003B27A3

	Int m_unk0x00;						// +0x00 (the first word Condition::rva003B275A 0x003B275A returns)
	AsciiString m_uiName;				// +0x04 (Condition::text 0x003B3989)
	AsciiString m_uiName2;				// +0x08 (ScriptEngine::addActionTemplateInfo 0x00205A2B / addConditionTemplateInfo 0x00205A8D copy it; the Template constructor 0x003B39C7 clears it inline)
	AsciiString m_internalName;			// +0x0C (ScriptEngine::initTemplateNameKeys 0x00204D87 turns it into the key at +0x10)
	NameKeyType m_internalNameKey;		// +0x10 (WriteConditionDataChunk 0x003B428C and WriteActionDataChunk 0x003B3DFF write it as the name key)
	Int m_numUiStrings;					// +0x14
	AsciiString m_uiStrings[MAX_PARMS];	// +0x18
	Int m_numParameters;				// +0x48
	Parameter::ParameterType m_parameters[MAX_PARMS];	// +0x4C
	AsciiString m_helpText;				// +0x7C (the same two functions copy it; the constructor clears it inline)
};
// SIZE 0x80

class ConditionTemplate : public Template {};
class ActionTemplate : public Template {};

#endif
