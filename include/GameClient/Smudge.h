// Portions derived from Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
#pragma once
#ifndef BFME2_SMUDGE_H
#define BFME2_SMUDGE_H

#include "Common/GameType.h"
#include "WW3D2/dllist.h"
#include "WWMath/Vector2.h"
#include "WWMath/Vector3.h"

struct Smudge : public DLNodeClass<Smudge>
{
	Smudge();

	Vector3 m_pos;			// +0x0C
	Vector2 m_offset;		// +0x18
	Real m_size;			// +0x20
	Real m_opacity;			// +0x24

	struct smudgeVertex
	{
		Vector3 pos;
		Vector2 uv;
	};

	smudgeVertex m_verts[5];	// +0x28, four corners followed by the interior vertex
};

// The renderer returns used smudges to the shared free list on reset.
struct SmudgeSet : public DLNodeClass<SmudgeSet>
{
public:
	SmudgeSet();
	virtual ~SmudgeSet();

	void reset(void);
	void removeSmudgeFromSet(Smudge &mySmudge);
	Smudge *addSmudgeToSet(void);

	DLListClass<Smudge> &getUsedSmudgeList(void) { return m_usedSmudgeList; }
	Int getUsedSmudgeCount(void) { return m_usedSmudgeCount; }

	static DLListClass<Smudge> m_freeSmudgeList;	// 0x00DBCBF4

private:
	DLListClass<Smudge> m_usedSmudgeList;	// +0x10
	Int m_usedSmudgeCount;			// +0x1C
};

// Constructor 0x002D25BA installs the five entries at table 0x00802A60.
class SmudgeManager
{
public:
	SmudgeManager();
	virtual ~SmudgeManager();

	virtual void init(void) {}
	virtual void reset(void);
	virtual void ReleaseResources(void) {}
	virtual void ReAcquireResources(void) {}

	SmudgeSet *addSmudgeSet(void);
	void removeSmudgeSet(SmudgeSet &mySmudge);

protected:
	enum HardwareSmudgeSupport { SMUDGE_SUPPORT_UNKNOWN, SMUDGE_SUPPORT_NO, SMUDGE_SUPPORT_YES };

	HardwareSmudgeSupport m_hardwareSupportStatus;	// +0x04
	DLListClass<SmudgeSet> m_usedSmudgeSetList;	// +0x08
	DLListClass<SmudgeSet> m_freeSmudgeSetList;	// +0x14
	Int m_smudgeCountLastFrame;			// +0x20
};

typedef char SmudgeSize[(sizeof(Smudge) == 0x8C) ? 1 : -1];
typedef char SmudgeSetSize[(sizeof(SmudgeSet) == 0x20) ? 1 : -1];
typedef char SmudgeManagerSize[(sizeof(SmudgeManager) == 0x24) ? 1 : -1];

#endif
