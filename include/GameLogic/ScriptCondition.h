// ScriptCondition.h: one map-script condition, the node the script engine evaluates.
// Target facts (code RVAs):
//  - Size 0x50: the parser 0x003B6D92 and duplicate 0x003B5B07 allocate 0x50
//    through global new before calling a Condition constructor.
//  - One vtable, VA 0x00C1F3F8, whose sole slot is the protected scalar
//    deleting destructor 0x003B4A78 (which calls 0x003B4230). Condition has no
//    pool getter slot; the next word, 0x00C1F3FC, is ScriptAction's table.
//  - The constructors 0x003B26DF/0x003B5AC7 write type +0x04, count +0x08 and
//    +0x3C/+0x40/+0x44/+0x48, set bytes +0x4C=1 and +0x4D=0, and clear twelve
//    parameter pointers at +0x0C. The destructor 0x003B4230 deletes those
//    parameters using count +0x08 and walks the +0x3C chain.
//  - Readers: 0x002097C4 tests +0x4C before evaluation, 0x003EC8DF tests +0x4D
//    to invert a condition's result, 0x003EA9B6 reads the type at +0x04 for the
//    dispatcher, and 0x003B6B29/0x003B6B3C test the tail bytes for the
//    disabled/NOT display text.
//  - Rowed out-of-line methods: setConditionType 0x003B4A94, rva003B275A
//    0x003B275A, getUiStrings 0x003B39B2, getUiText 0x003B5BA4,
//    WriteConditionDataChunk 0x003B428C and ParseConditionDataChunk 0x003B6D92,
//    which bounds the type by 0xCA.
// The parameter/custom-data accessors are inlined at their call sites, so they
// live here as inline members. Names follow Zero Hour's Scripts.h where the
// offset and the role agree; the two BFME2 tail bytes keep neutral names.
#pragma once
#ifndef BFME2_SCRIPTCONDITION_H
#define BFME2_SCRIPTCONDITION_H

class AsciiString;
class Parameter;
class DataChunkInput;
class DataChunkOutput;
struct DataChunkInfo;

class Condition
{
protected:
	virtual ~Condition();			// the table's only slot, 0x003B4A78

public:
	enum ConditionType
	{
		CONDITION_FALSE = 0,
		COUNTER = 1,
		FLAG = 2,
		CONDITION_TRUE = 3,
		TIMER_EXPIRED = 4,
		SKIRMISH_SPECIAL_POWER_READY = 0x55,
		NUM_ITEMS = 0xCA
	};

	Condition() throw();			// 0x003B26DF
	Condition(ConditionType type);		// 0x003B5AC7
	Condition *duplicate() const;		// 0x003B5B07
	void setConditionType(ConditionType type);	// 0x003B4A94
	int rva003B275A();			// 0x003B275A
	int getUiStrings(AsciiString *const strings);	// 0x003B39B2
	AsciiString getUiText();		// 0x003B5BA4
	AsciiString getTemplateName() const;	// 0x003B3989 (the UI name of the condition's template; the map's placeholder text@...ConditionPrefix)

	static void WriteConditionDataChunk(DataChunkOutput &writer, Condition *condition);	// 0x003B428C
	static Bool ParseConditionDataChunk(DataChunkInput &file, DataChunkInfo *info, void *userData);	// 0x003B6D92

	ConditionType getConditionType() const { return (ConditionType)m_conditionType; }
	int getType() const { return m_conditionType; }
	Bool isEnabled() const { return m_flag4C; }
	Bool isInverted() const { return m_b4D != 0; }		// 0x003EC8DF
	int getNumParameters() const { return m_numParms; }
	Parameter *getParameter(int index) const
	{
		return (index < m_numParms) ? m_parms[index] : 0;
	}
	Condition *getNext() const { return m_nextCondition; }
	void setNextCondition(Condition *condition) { m_nextCondition = condition; }
	Bool hasWarnings() const { return m_x40 != 0; }
	void setWarnings(Bool warnings) { m_x40 = warnings; }
	int getCustomData() const { return m_x44; }
	void setCustomData(int value) { m_x44 = value; }
	unsigned int getCustomFrame() const { return m_x48; }
	void setCustomFrame(unsigned int frame) { m_x48 = frame; }

	int m_conditionType;			// +0x04
	int m_numParms;				// +0x08
	Parameter *m_parms[12];			// +0x0C
	Condition *m_nextCondition;		// +0x3C
	int m_x40;				// +0x40
	int m_x44;				// +0x44 custom data
	unsigned int m_x48;			// +0x48 custom frame
	Bool m_flag4C;				// +0x4C enabled
	unsigned char m_b4D;			// +0x4D inverted
};
// SIZE 0x50

#endif
