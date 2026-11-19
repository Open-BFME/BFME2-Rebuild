// ContainModuleSlots.h: the contain module's interface at the slots
// FlammableUpdate reaches. Retail's slot 34 takes a mode (2 when a burn starts
// with contained objects to spare) and slot 70 returns the two-pointer range
// that 0x0036AE51 turns into a list.
#pragma once
#ifndef BFME2_CONTAINMODULESLOTS_H
#define BFME2_CONTAINMODULESLOTS_H

#include "Common/GameType.h"
#include "GameLogic/ContainmentList.h"

// The by-value range slot 70 returns; 0x0036AE51 builds the list from it.
class Rva0036AE51ListView
{
public:
	ContainmentList rva0036AE51();

	void *m_first;								// 0x0
	void *m_last;								// 0x4
};
// SIZE 0x8

class ContainModuleSlots
{
public:
	virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
	virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
	virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
	virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
	virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
	virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
	virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
	virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
	virtual void v32(); virtual void v33();
	virtual void rva34(Int how);				// slot 34
	virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38();
	virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42();
	virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46();
	virtual void v47(); virtual void v48(); virtual void v49(); virtual void v50();
	virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
	virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58();
	virtual void v59(); virtual void v60(); virtual void v61(); virtual void v62();
	virtual void v63(); virtual void v64(); virtual void v65(); virtual void v66();
	virtual void v67(); virtual void v68(); virtual void v69();
	virtual Rva0036AE51ListView rva70();		// slot 70
};

#endif
