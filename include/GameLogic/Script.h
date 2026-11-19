// Script.h: one map script (a named condition/action rule). SIZE 0x54.
// Target facts (code RVAs):
//  - Size 0x54: Script::duplicate 0x003B7550 allocates 0x54 before the copy constructor
//    0x003B7448.
//  - Bases: Snapshot. The vptr is the table 0x00C1F424 (slot 0 the deleting destructor
//    0x003B41F8, slot 1 loadPostProcess (the shared empty body 0x000B3FD0), slot 2
//    GetSnapshotName 0x003B3662, slot 3 xfer 0x003B2540); the destructor 0x003B35B1 ends by
//    restoring Snapshot's table 0x00BBB554.
//  - Members, from the copy constructor 0x003B7448, updateFrom 0x003B3668, the destructor
//    0x003B35B1, the chunk writer WriteScriptDataChunk 0x003B5994 and the reader parse
//    0x003B79F1:
//      +0x04/+0x08/+0x0C three strings (parse reads them in this order, the writer writes
//      them after the list-owned name),
//      +0x10 a 16-byte record (ScriptSubRecord, read when the chunk version is 3 or more),
//      +0x20 an Int (read when the version is 2 or more), +0x24 a 4-byte mask (the writer's
//      WriteScriptPlayerMask 0x003B372D turns its seven bits into names; read when the
//      version is 4 or more), +0x28..+0x2D six bytes (the writer's order is +0x28, +0x29,
//      +0x2B, +0x2C, +0x2D, +0x2A), +0x30 the OR-condition list head, +0x34 two action list
//      heads (true, false), +0x3C UnsignedInt, +0x40 Bool, +0x41 Bool, +0x44 a string,
//      +0x48/+0x4C Real, +0x50 Int.
//    The copy constructor clears +0x3C, +0x41, +0x44, +0x48, +0x4C and +0x50 and copies the rest.
//  - Script::xfer 0x003B2540 transfers a version, the Bool at +0x40 and, from version 2,
//    the UnsignedInt at +0x3C.
//  The names follow Zero Hour's Script (Scripts.h): the order of the six flags matches its
//  writer (isActive, isOneShot, easy, normal, hard, isSubroutine), and the trailing runtime
//  fields (frame, warnings, team name, times, count) follow its runtime block.
#pragma once
#ifndef BFME2_SCRIPT_H
#define BFME2_SCRIPT_H

#include "Common/GameType.h"
#include "Common/AsciiString.h"
#include "Common/Snapshot.h"

class AssetList;
struct AssetLoadMode;
class OrCondition;
class ScriptAction;
template <class T> class ScriptTable;
template <class T> struct ScriptTableNode;
class DataChunkInput;
class DataChunkOutput;
class ScriptList;
struct ScriptLink;
struct DataChunkInfo;

// The 16-byte record at Script +0x10. Copied by 0x003B2498 (operator=), cleared by
// 0x003B24C7 and 0x003B24DB, written by WriteScriptSubRecord 0x003B24F2 as byte, byte,
// Int, byte, string, and read by 0x003B3536.
struct ScriptSubRecord
{
	Bool m_flag0;				// +0x00
	Bool m_flag1;				// +0x01
	Int m_value;				// +0x04
	Bool m_flag8;				// +0x08
	AsciiString m_text;			// +0x0C

	void read(DataChunkInput &file, unsigned short version);	// 0x003B3536
	ScriptSubRecord &operator=(const ScriptSubRecord &that);	// 0x003B2498 (copies the five fields; the text through set)
	void setPrimary(Bool on);	// 0x003B24C7: stores flag 0; switching it off also clears the value and flag 1
	void setSecondary(Bool on);	// 0x003B24DB: switching it on sets flag 0, off clears the value; flag 1 follows
};
// SIZE 0x10

void WriteScriptSubRecord(DataChunkOutput &chunkWriter, const ScriptSubRecord *record);	// 0x003B24F2

// The 4-byte phase mask at Script +0x24 (its writer WriteScriptPlayerMask 0x003B372D names seven bits from the
// table at 0x00DD263C). Its copy constructor is out of line in retail (0x006CF0F0), so it is declared here and
// not defined.
class ScriptPhaseMask
{
public:
	ScriptPhaseMask() {}
	ScriptPhaseMask(const ScriptPhaseMask &that) throw();	// 0x002CF0F0

	// 0x003B44EE: applies one token of a mask list (NONE, +NAME, -NAME or NAME) against the seven phase names; the
	// two flags record that a bare name and a signed name were seen, and mixing them throws an INIException (the
	// same worker as BitFlags<101>'s 0x003339CE, one copy per instantiation).
	Bool applyToken(const char *token, Bool *sawName, Bool *sawSign);
	// 0x003B5624: applies every token of a space-separated list until one is NONE.
	void parse(AsciiString text);

	UnsignedInt m_bits[1];				// +0x00 (seven bits)
};

class Script : public Snapshot
{
protected:
	virtual ~Script();					// slot 0, deleting 0x003B41F8, non-deleting 0x003B35B1 (the ledger spells it protected, MAE)
	virtual void loadPostProcess();			// slot 1 (empty; the shared body 0x000B3FD0)
	virtual const char *GetSnapshotName() const;	// slot 2, 0x003B3662
	virtual void xfer(Xfer *xfer);			// slot 3, 0x003B2540

public:
	Script() throw();					// 0x003B417E (retail's default constructor, no row in the map yet: ScriptList::xfer 0x003B4716 builds the empty script it reads broken entries into)
	Script(const Script &that);			// 0x003B7448
	Script *duplicate() const;			// 0x003B7550
	void updateFrom(Script *source);		// 0x003B3668
	OrCondition *findPreviousOrCondition(OrCondition *curOr) const;	// 0x003B260F
	void deleteAction(Int list, ScriptAction *action);	// 0x003B25CD: list 0 is the true list, 1 the false list (Zero Hour's deleteAction/deleteFalseAction)
	void deleteOrCondition(OrCondition *cond);	// 0x003B258F: unlinks the OR group from the list at +0x30 and frees it
	Bool parse(DataChunkInput &file, unsigned short version);	// 0x003B79F1
	AsciiString getUiText();			// 0x003B6AB2
	void GetAssetList(AssetList *assets, AssetLoadMode *mode);	// 0x003B3836 (the things the actions name: units, objects, the unit lines of a named team template)
	AsciiString getConditionTeamName() const;	// 0x002041E1 (Zero Hour's accessor; +0x44)
	void setConditionTeamName(AsciiString teamName);	// 0x002041FC (Zero Hour's setter)
	void setPhaseMaskWord(const Int *word);		// 0x003B2534 (the map's placeholder rva003B2534: stores the word at +0x24)

	friend void WriteScriptDataChunk(DataChunkOutput &chunkWriter, ScriptList *list, Script *links);	// 0x003B5994
	friend class TeamPrototype;			// ~TeamPrototype 0x003A33CD deletes the production condition script and the generic scripts
	friend class ScriptAction;			// ParseActionDataChunk 0x003B65BF / ParseActionFalseDataChunk 0x003B65F9 link into m_actions
	friend class OrCondition;			// ParseOrConditionDataChunk 0x003B7587 links into m_condition
	friend class ScriptEngine;			// removeSequentialScriptsOf 0x00205140 tests the record's first flag
	friend struct ScriptTableNode<Script>;	// the node's destructor is out of line (0x003B3F5E)
	friend class ScriptTable<Script>;		// the version nodes of a ScriptList's script table destroy it (0x003B3F5E: add 4, jmp 0x003B35B1)

protected:
	AsciiString m_scriptName;			// +0x04
	AsciiString m_comment;				// +0x08
	AsciiString m_conditionComment;			// +0x0C
	ScriptSubRecord m_record;			// +0x10
	Int m_delayEvaluationSeconds;			// +0x20
	ScriptPhaseMask m_phaseMask;			// +0x24 (WriteScriptPlayerMask 0x003B372D)
	Bool m_isActive;				// +0x28
	Bool m_isOneShot;				// +0x29
	Bool m_isSubroutine;				// +0x2A
	Bool m_easy;					// +0x2B
	Bool m_normal;					// +0x2C
	Bool m_hard;					// +0x2D
	OrCondition *m_condition;			// +0x30, the head of the OR-condition list
	ScriptAction *m_actions[2];			// +0x34 the true and false action lists
	UnsignedInt m_frameToEvaluateAt;		// +0x3C
	Bool m_activeNow;				// +0x40 (the Bool Script::xfer transfers; parse stores the read isActive here too)
	Bool m_hasWarnings;				// +0x41
	AsciiString m_conditionTeamName;		// +0x44
	Real m_conditionTime;				// +0x48
	Real m_curTime;					// +0x4C
	Int m_conditionExecutedCount;			// +0x50
};
// SIZE 0x54
typedef char ScriptSizeCheck[(sizeof(Script) == 0x54) ? 1 : -1];

#endif
