// OrCondition.h: one OR group of a map script's conditions, a list of AND conditions linked
// through Condition::getNext (+0x3C in ScriptCondition.h). Target facts: the vptr at +0x00
// (the virtual destructor is slot 0, 0x003B331E); the first AND condition is at +0x08, the head
// that removeCondition 0x003B2672 and findPreviousCondition 0x003B26C0 walk.
// deleteCondition 0x003B26A1 removes a condition and frees it through its virtual destructor.
#pragma once
#ifndef BFME2_ORCONDITION_H
#define BFME2_ORCONDITION_H

#include "Common/GameType.h"

class Condition;
class DataChunkOutput;
class DataChunkInput;
struct DataChunkInfo;

// Condition's destructor is protected. This view has a virtual destructor of its own, so
// deleteCondition reaches Condition's vtable slot 0 and frees the object the way retail does.
struct ConditionDeleteView
{
	virtual ~ConditionDeleteView() {}
};

class OrCondition
{
public:
	OrCondition() : m_nextOrCondition(0), m_firstAnd(0) {}	// the constructor is inline (Script::duplicate and the parser build it in place)
	OrCondition *duplicate() const;						// 0x003B6D06
	static Bool ParseOrConditionDataChunk(DataChunkInput &file, DataChunkInfo *info, void *userData);	// 0x003B7587

	static void WriteOrConditionDataChunk(DataChunkOutput &chunkWriter, OrCondition *pCondition);	// 0x003B4A3C

	Condition *removeCondition(Condition *cond);		// 0x003B2672
	void deleteCondition(Condition *cond);				// 0x003B26A1
	Condition *findPreviousCondition(Condition *cur);	// 0x003B26C0
	Condition *getFirstAndCondition() const { return m_firstAnd; }
	void setFirstAndCondition(Condition *first) { m_firstAnd = first; }
	OrCondition *getNextOrCondition() const { return m_nextOrCondition; }
	void setNextOrCondition(OrCondition *next) { m_nextOrCondition = next; }

protected:
	virtual ~OrCondition();								// slot 0, 0x003B331E (the map spells it protected: MAE)
	friend class Script;								// Script's destructor and deleteOrCondition free the groups
	OrCondition *m_nextOrCondition;						// +0x04, the next OR group (Script::deleteOrCondition 0x003B258F walks it)
	Condition *m_firstAnd;								// +0x08 (removeCondition 0x003B2672 loads and stores [ecx+8])
};

#endif
