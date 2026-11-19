// StaticSortListClass.h: the static sort lists WW3D renders translucent objects through (WW3D::DefaultStaticSortLists,
// WW3D::CurrentStaticSortLists). Evidence: the vtable at DefaultStaticSortListClass (0x00176B00 destructor, 0x00176970
// Add_To_List, 0x001769B0 Render_And_Clear) and WW3D's callers 0x00117C40 (`call [eax+0x4]`, pushes sort_level then the
// object) and 0x00117C60 (`call [eax+0x8]`, pushes the render info). The base has no members of its own that a
// recovered body reads. Names are Zero Hour's.
#pragma once
#ifndef BFME2_STATICSORTLISTCLASS_H
#define BFME2_STATICSORTLISTCLASS_H

#include "WWLib/multilist.h"
#include "WW3D2/RenderObjClass.h"

class RenderInfoClass;

// Retail's w3d_file.h: the deepest static sort level (the array below holds levels 0..MAX_SORT_LEVEL).
const unsigned MAX_SORT_LEVEL = 32;

class StaticSortListClass
{
public:
	StaticSortListClass();
	virtual ~StaticSortListClass();
	virtual void Add_To_List(RenderObjClass *robj, unsigned int sort_level) = 0;			// 1
	virtual void Render_And_Clear(RenderInfoClass &rinfo) = 0;								// 2
};

// The standard implementation (Zero Hour's DefaultStaticSortListClass). Target facts: constructor 0x00176A30 stores the
// vtable (0x00BD4E30), MinSort 1 at +0x04 and MaxSort 0x20 at +0x08, then builds the 33 lists of 0x18 bytes each from +0x0C
// with the EH vector constructor; destructor 0x00176AA0 destroys them, then resets the base vtable (0x00BD4E24).
class DefaultStaticSortListClass : public StaticSortListClass
{
public:
	DefaultStaticSortListClass();										// 0x00176A30
	virtual ~DefaultStaticSortListClass();								// 0x00176AA0 (deleting destructor 0x00176B00)
	virtual void Add_To_List(RenderObjClass *robj, unsigned int sort_level);	// 0x00176970
	virtual void Render_And_Clear(RenderInfoClass &rinfo);				// 0x001769B0

	unsigned int Get_Min_Sort(void) const { return MinSort; }
	unsigned int Get_Max_Sort(void) const { return MaxSort; }
	void Set_Min_Sort(unsigned int value) { MinSort = (value > MAX_SORT_LEVEL) ? MAX_SORT_LEVEL : value; }
	void Set_Max_Sort(unsigned int value) { MaxSort = (value > MAX_SORT_LEVEL) ? MAX_SORT_LEVEL : value; }

private:
	unsigned int MinSort;									// +0x04
	unsigned int MaxSort;									// +0x08
	RefMultiListClass<RenderObjClass> SortLists[MAX_SORT_LEVEL + 1];	// +0x0C, 0x18 bytes each
};

#endif
