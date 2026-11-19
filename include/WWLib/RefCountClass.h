// RefCountClass.h: WWLib's reference counter (WWVegas/WWLib/refcount.h).
// Target facts: vftable 0x007C650C has two slots, Delete_This 0x0006CE57 then the
// scalar deleting destructor 0x00072899, so Delete_This is declared before the
// destructor; 106 vftables reuse the one Delete_This body, i.e. it is the inline
// header definition. NumRefs is at +4: Release_Ref 0x005D1A7D is
// `dec [ecx+4]; jne; jmp [vftable+0]`.
// Release_Ref is an inline member (a header definition, like Delete_This). The /O2 library
// units expand it, so hanim.cpp and gridcull.cpp carry the decrement in place; the one
// out-of-line copy at 0x005D1A7D is a COMDAT from a size-optimised game unit, and its seven
// direct callers (the strategic checklist UI) reach it by call. The compiler emits that COMDAT
// whenever a unit uses the function, so refcount.cpp can own the row.
// Delete_This 0x0006CE57 calls slot 1 with flag 0 and then the global operator
// delete on the returned pointer, which is MSVC's code for `::delete this` (a
// plain `delete this` passes flag 1 to the deleting destructor).
#pragma once
#ifndef BFME2_REFCOUNTCLASS_H
#define BFME2_REFCOUNTCLASS_H

#include <assert.h>

class RefCountClass
{
public:
	RefCountClass(void) : NumRefs(1) {}
	RefCountClass(const RefCountClass &) : NumRefs(1) {}

	void Add_Ref(void) { NumRefs++; }
	void Release_Ref(void) { NumRefs--; if (NumRefs == 0) Delete_This(); }	// 0x005D1A7D

	int Num_Refs(void) { return NumRefs; }

	virtual void Delete_This(void) { ::delete this; }			// slot 0: 0x0006CE57
	static int Total_Refs(void) { return TotalRefs; }

protected:
	virtual ~RefCountClass(void) { assert(NumRefs == 0); }		// slot 1: ??_G 0x00072899

private:
	int NumRefs;	// +0x04
	static int TotalRefs;
};
// SIZE 0x08

#endif
