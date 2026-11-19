// CullableClass.h: an object a CullSystemClass can collect.
//
// Target facts (retail cullsys.cpp):
//  - The constructor 0x007244C0 stores the reference count at +4, the table at
//    +0, the cull link at +8, the collected link at +0x24 and the box from
//    +0xC, so the class derives from RefCountClass (vptr + NumRefs) with
//    CullLink, CullBox and NextCollected after it.
//  - Set_Culling_System 0x00724520 and Get_Culling_System 0x00724540 both go
//    through CullLink at +8; Get_Next_Collected_Object_Internal 0x00724570
//    reads a neighbour's NextCollected at +0x24.
#pragma once
#ifndef BFME2_CULLABLECLASS_H
#define BFME2_CULLABLECLASS_H

#include "WWLib/RefCountClass.h"
#include "Draft/AABoxClass.h"

class CullLinkClass;
class CullSystemClass;

class CullableClass : public RefCountClass
{
public:
	CullableClass(void);
	virtual ~CullableClass(void);

	void Set_Cull_Box(const AABoxClass &box, bool just_loaded);
	void Set_Culling_System(CullSystemClass *sys);
	CullSystemClass *Get_Culling_System(void) const;

	CullLinkClass *CullLink;		// +0x08
	AABoxClass CullBox;			// +0x0C
	CullableClass *NextCollected;		// +0x24
};

#endif
