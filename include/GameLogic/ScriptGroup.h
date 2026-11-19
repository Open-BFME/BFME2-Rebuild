// ScriptGroup.h: a named group of scripts (and of nested groups) in a map's script list. SIZE 0x10.
// Target facts (code RVAs):
//  - Bases: Snapshot. The vptr is the table 0x00C1F470 (slot 0 the deleting destructor 0x003B4A20,
//    slot 1 loadPostProcess (the shared empty body), slot 2 GetSnapshotName 0x003B40FA, slot 3
//    xfer 0x003B2467); the constructor 0x003B40DF stores the table, zeroes +0x04 and +0x08 and
//    sets the flag bytes (+0x0C true, +0x0D and +0x0E false).
//  - Members: WriteGroupDataChunk 0x003B6975 reads +0x0C (the active flag) and +0x0D (the
//    subroutine flag) with writeByte, then recurses through +0x04 (WriteGroupDataChunk, the
//    nested groups) and +0x08 (WriteScriptDataChunk, the scripts). Both are heads of link lists
//    owned through the ScriptList; the copy constructor 0x003B410E copies them through one
//    helper and then the three flag bytes (+0x0E is cleared, not copied).
//  - ScriptGroup::xfer 0x003B2467 transfers a version and the Bool at +0x0C.
//  Zero Hour's ScriptGroup (Scripts.h) carries the same two flags; BFME 2 moved the script
//  chain and the name out of the group into the ScriptList's tables.
#pragma once
#ifndef BFME2_SCRIPTGROUP_H
#define BFME2_SCRIPTGROUP_H

#include "Common/GameType.h"
#include "Common/Snapshot.h"
#include "GameLogic/ScriptLinks.h"

class DataChunkOutput;
class ScriptList;


class ScriptGroup : public ScriptListBase, public Snapshot
{
public:
	ScriptGroup();					// 0x003B40DF
	ScriptGroup(const ScriptGroup *that);		// 0x003B410E (the copy constructor takes a pointer)
	ScriptGroup *duplicate() const;			// 0x003B4147 (duplicates just this node; Zero Hour's ScriptGroup::duplicate)

	virtual ~ScriptGroup() {}			// slot 0, deleting 0x003B4A20

	friend class ScriptEngine;			// enableScript 0x00204FBB / disableScript 0x0020507F set the active flag
	friend void WriteGroupDataChunk(DataChunkOutput &chunkWriter, ScriptList *list, ScriptGroup *links);	// 0x003B6975

protected:
	virtual void loadPostProcess() {}		// slot 1 (empty)
	virtual const char *GetSnapshotName() const;	// slot 2, 0x003B40FA
	virtual void xfer(Xfer *xfer);			// slot 3, 0x003B2467

	Bool m_isActive;				// +0x0C
	Bool m_isSubroutine;				// +0x0D
	Bool m_hasWarnings;				// +0x0E (runtime)
};
// SIZE 0x10
typedef char ScriptGroupSizeCheck[(sizeof(ScriptGroup) == 0x10) ? 1 : -1];

#endif
