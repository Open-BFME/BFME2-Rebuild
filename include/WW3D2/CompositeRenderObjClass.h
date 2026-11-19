// CompositeRenderObjClass.h: the base of render objects built from
// sub-objects (HLodClass, Animatable3DObjClass); most of its overrides walk
// the sub-objects through slots 28 (count) and 30 (an add-ref'd sub-object).
//
// Target facts:
// - The constructor 0x001A5D30 runs RenderObjClass's default constructor,
//   installs vftable 0x007D6F30 (129 slots) and the secondary 0x007D6F28, then
//   builds two StringClass members, the name at +0xC4 and the base model name
//   at +0xC8. The copy constructor 0x001A6490 also starts from RenderObjClass's
//   default constructor and copies the two names through the source's slots 6
//   and 8 (an empty base model name for a null one).
// - Slot 6 returns +0xC4 (folded body 0x0015E600) and slot 7 assigns it
//   (0x001A6170, a tail jump to StringClass::operator=(const char *)); slot 8
//   0x001A6120 returns +0xC8 or null when it is empty; slot 9 0x001A6180
//   stores "" for a null name.
// - Slots 67 and 68 copy the object-space sphere at +0xCC (0x001A54C0) and box
//   at +0xDC (0x0019BFA0); slot 69 0x001A61A0 recomputes them.
// SIZE 0xF4.
#pragma once
#ifndef BFME2_COMPOSITERENDEROBJCLASS_H
#define BFME2_COMPOSITERENDEROBJCLASS_H

#include "WW3D2/RenderObjClass.h"
#include "WWLib/wwstring.h"

class CompositeRenderObjClass : public RenderObjClass
{
public:
	CompositeRenderObjClass(void);							// 0x001A5D30
	CompositeRenderObjClass(const CompositeRenderObjClass &src);			// 0x001A6490
	CompositeRenderObjClass &operator=(const CompositeRenderObjClass &that);	// 0x001A5E40
	virtual ~CompositeRenderObjClass(void);						// 1: 0x001A5DD0

	virtual const char *Get_Name(void) const { return Name; }			// 6: 0x0015E600
	virtual void Set_Name(const char *name);					// 7: 0x001A6170 (out of line: HLodClass 0x0019F9B0 calls it)
	virtual const char *Get_Base_Model_Name(void) const;				// 8: 0x001A6120
	virtual void Set_Base_Model_Name(const char *name);				// 9: 0x001A6180
	virtual int Get_Num_Polys(void) const;						// 10: 0x001A5EB0
	virtual int Get_Num_Verts(void) const;						// 11: 0x001A5F00
	virtual void Restart(void);							// 14: 0x001A5E70
	virtual void Notify_Added(SceneClass *scene);					// 26: 0x001A5F50
	virtual void Notify_Removed(SceneClass *scene);					// 27: 0x001A5FB0
	virtual bool Cast_Ray(RayCollisionTestClass &raytest);				// 60: 0x001A6000
	virtual bool Intersect_AABox(AABoxIntersectionTestClass &boxtest);		// 61: 0x001A6060
	virtual void Get_Obj_Space_Bounding_Sphere(SphereClass &sphere) const { sphere = ObjSphere; }	// 67: 0x001A54C0
	virtual void Get_Obj_Space_Bounding_Box(AABoxClass &box) const { box = ObjBox; }		// 68: 0x0019BFA0
	virtual void Update_Obj_Space_Bounding_Volumes(void);				// 69: 0x001A61A0
	virtual void Set_User_Data(void *value, bool recursive = false);		// 86: 0x001A60C0

protected:
	StringClass Name;				// +0xC4
	StringClass BaseModelName;			// +0xC8
	SphereClass ObjSphere;				// +0xCC
	AABoxClass ObjBox;				// +0xDC
};

typedef char CompositeRenderObjClassSize[(sizeof(CompositeRenderObjClass) == 0xF4) ? 1 : -1];

#endif
