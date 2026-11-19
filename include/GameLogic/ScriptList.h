// ScriptList.h: a side's list of map scripts and script groups. SIZE 0x4C.
// Target facts (code RVAs):
//  - Layout, from the constructor 0x003B7720, the destructor 0x003B774B and the swap 0x003B58DF:
//    +0x00 the vptr (table 0x00C1FAAC: slot 0 the deleting destructor 0x003B7D90, slot 1
//    loadPostProcess, slot 2 GetSnapshotName 0x003B7745, slot 3 xfer 0x003B4716);
//    +0x04 a base object of two words (ScriptListBase: the constructor zeroes both words before it
//    stores the table; the destructor 0x003B4071 runs after the two sets below; swap 0x003B33FA
//    exchanges them), then two sets of 0x20 bytes each at +0x0C and +0x2C (each has its own
//    constructor 0x003B761E, destructors 0x003B872A and 0x003B8802 and swap 0x003B56A5).
//    The destructor restores Snapshot's table 0x00BBB554 last.
//  - WriteScriptListDataChunk 0x003B73C5 walks the two words of the base: +0x08 is the script
//    link list (WriteScriptDataChunk 0x003B5994), +0x04 the group link list (WriteGroupDataChunk
//    0x003B6975). clear 0x003B7812 swaps with a freshly constructed list and destroys it.
//  - Size 0x4C: SidesInfo (SidesList.cpp) holds one by value at +0x08 of its 0x60 bytes.
//  The two sets are ScriptTable<ScriptGroup> (+0x0C) and ScriptTable<Script> (+0x2C): see ScriptTable.h.
//    Which is which: captureGroup 0x003B40A1 reads the table at +0x0C and ~ScriptGroup runs in its node
//    destructor 0x003B448C; captureScript 0x003B40B6 reads +0x2C and the node destructor is ~Script.
#pragma once
#ifndef BFME2_SCRIPTLIST_H
#define BFME2_SCRIPTLIST_H

#include "Common/GameType.h"
#include "Common/Snapshot.h"
#include "GameLogic/ScriptTable.h"

class DataChunkOutput;
class Script;
class ScriptGroup;
class ScriptList;

// The chunk writers walk a link chain and read the list's tables.  The map still spells their chain argument as the old private
// view did (Script * / ScriptGroup *); the argument really is the first link (ScriptLink * / ScriptGroupLink *), cast at the entry
// and at the callers while the map still records those parameter types.
void WriteScriptDataChunk(DataChunkOutput &chunkWriter, ScriptList *list, Script *links);	// 0x003B5994
void WriteGroupDataChunk(DataChunkOutput &chunkWriter, ScriptList *list, ScriptGroup *links);	// 0x003B6975

class ScriptList : public Snapshot, public ScriptListBase
{
public:
	ScriptList();								// 0x003B7720
	ScriptList(const ScriptList &that);			// 0x003B88DA
	virtual ~ScriptList();						// slot 0, deleting 0x003B7D90, non-deleting 0x003B774B

	ScriptList *duplicate() const;				// 0x003B8945
	void clear();								// 0x003B7812
	void swap(ScriptList *other);			// 0x003B58DF
	void WriteScriptListDataChunk(DataChunkOutput &chunkWriter);		// 0x003B73C5
	static void WriteScriptsDataChunk(DataChunkOutput &chunkWriter, ScriptList *scriptLists[], Int numLists);	// 0x003B73F6

	void flagAll();								// 0x003B40CB: flags every live entry of both tables
	void commit();								// 0x003B693A: prunes stale links, then flattens and unflags both tables
	// 0x003B23DE / 0x003B23F7: link `node` into a chain after `after`, or at the front of the head's chain when `after` is null.
	void addGroupRef(ScriptListBase *head, ScriptGroupLink *node, ScriptGroupLink *after);	// Retail branches on 'after' here. The current code generation selects with cmove.
	void addScriptRef(ScriptListBase *head, ScriptLink *node, ScriptLink *after);
	void pruneStaleLinks(ScriptListBase *links);				// 0x003B48E4: drops the links whose entry has moved on (the others restart at depth 1), groups recursively
	void pruneSkippedLinks(ScriptListBase *links);				// 0x003B4982: drops the links skipGroup / skipScript reject (stale or flagged), groups recursively
	void deleteScriptRef(void *head, ScriptLink *link);		// 0x003B7389: unlinks the link from the head's script chain, pops its version and frees it
	void deleteGroupRef(void *head, ScriptGroupLink *link);	// 0x003B779C: the same for a group link, after deleting everything the group holds
	// 0x003B7DAC / 0x003B84C9: move a chain of links from `source` into this list (not written yet; takeGroups walks a deque)
	void takeGroups(ScriptList *source, ScriptGroupLink **sourceHead, ScriptGroupLink **destHead);
	void takeScripts(ScriptList *source, ScriptLink **sourceHead, ScriptLink **destHead);
	void doTakeGroup(ScriptList *source, ScriptGroupLink **link, ScriptGroupLink **dest);	// 0x003B89C9 (pending register allocation match).
	void takeGroup(ScriptList *source, ScriptGroupLink **head, ScriptGroupLink *wanted, ScriptGroupLink **first, ScriptGroupLink **after);	// 0x003B8A9B
	void cleanup();							// 0x003B7362: prunes the skipped links, then settles both tables

	friend class ScriptEngine;		// walkNamed 0x0020A775 reads the script table's entry names
	friend void WriteScriptDataChunk(DataChunkOutput &chunkWriter, ScriptList *list, Script *links);	// 0x003B5994
	friend void WriteGroupDataChunk(DataChunkOutput &chunkWriter, ScriptList *list, ScriptGroup *links);	// 0x003B6975

	ScriptGroup *findGroup(const AsciiString &name);			// 0x003B68E8 (the map spells it rva003B68E8 returning void *)
	Script *findScript(const AsciiString &name);			// 0x003B6911
	ScriptGroup *captureGroup(const ScriptGroupLink *link);			// 0x003B40A1
	Script *captureScript(const ScriptLink *link);			// 0x003B40B6
	ScriptGroup *currentGroup(const ScriptGroupLink *link) const;		// 0x003B4826
	Bool isGroupLinkCurrent(const ScriptGroupLink *link) const;		// 0x003B483D
	Bool skipGroup(const ScriptGroupLink *link);				// 0x003B485A
	Script *currentScript(const ScriptLink *link) const;		// 0x003B4885
	Bool isScriptLinkCurrent(const ScriptLink *link) const;		// 0x003B489C
	Bool skipScript(const ScriptLink *link);				// 0x003B48B9

protected:
	virtual void loadPostProcess() {}			// slot 1 (empty)
	virtual const char *GetSnapshotName() const;	// slot 2, 0x003B7745
	virtual void xfer(Xfer *xfer);				// slot 3, 0x003B4716

	ScriptTable<ScriptGroup> m_groups;			// +0x0C the group table (constructor 0x003B761E)
	ScriptTable<Script> m_scripts;				// +0x2C the script table
};
// SIZE 0x4C
typedef char ScriptListSize[(sizeof(ScriptList) == 0x4C) ? 1 : -1];

#endif
