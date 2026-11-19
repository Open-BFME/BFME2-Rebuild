// Null3DObjClass.h: WW3D2's empty render object (nullrobj.h).
// Target facts: allocation 0xE4 (Clone 0x00180F70 news 0xE4); Name[32] at +0xC4
// (Get_Name returns this+0xC4; the constructors strcpy into it). Primary vftable
// 0x007D50D0 replaces only these RenderObjClass slots: 1 (deleting destructor),
// 2 Clone, 3 Class_ID (returns 0x16), 6 Get_Name, 12 Render (empty, ret 4),
// 67/68 the object-space bounding volumes. Both constructors default-construct
// the base (call 0x0013BF00) and then copy the name; operator= copies the name
// before calling RenderObjClass::operator= (0x0013B5F0).
#pragma once
#ifndef BFME2_NULL3DOBJCLASS_H
#define BFME2_NULL3DOBJCLASS_H

#include "WW3D2/RenderObjClass.h"

class Null3DObjClass : public RenderObjClass
{
public:
	Null3DObjClass(const char *name = "NULL");					// 0x00180EB0
	Null3DObjClass(const Null3DObjClass &src);					// 0x00180EF0
	Null3DObjClass &operator=(const Null3DObjClass &that);		// 0x00180F30

	virtual RenderObjClass *Clone() const;						// slot 2: 0x00180F70
	virtual int Class_ID() const;								// slot 3: 0x001874D0
	virtual const char *Get_Name() const;						// slot 6: 0x00174DC0 (folded with BoxRenderObjClass's)
	virtual void Render(RenderInfoClass &rinfo) {}				// slot 12: 0x00180FD0
	virtual void Get_Obj_Space_Bounding_Sphere(SphereClass &sphere) const;	// slot 67: 0x00180FE0
	virtual void Get_Obj_Space_Bounding_Box(AABoxClass &box) const;			// slot 68: 0x00181010

protected:
	char Name[32];												// +0xC4
};
// SIZE 0xE4
typedef char Null3DObjClassSize[(sizeof(Null3DObjClass) == 0xE4) ? 1 : -1];

#endif
