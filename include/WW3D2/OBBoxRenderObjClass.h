// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// OBBoxRenderObjClass.h: WW3D2's oriented debug box (Zero Hour's boxrobj.h), the oriented box render object a model uses as
// its bounding box (class id 0x1B); HLodClass::Get_Obj_Space_Bounding_Box 0x0019C3F0 reads its local centre and extent.
// Target facts: constructor 0x00175F50 runs BoxRenderObjClass's and stores the vftable 0x007D4B60; CachedBox (an OBBoxClass,
// 0x3C bytes) sits at +0x10C, so SIZE 0x148. update_cached_box 0x001758C0 transforms the local centre by the object
// transform into CachedBox.Center, copies the extent and sets the basis from the transform's rotation.
#pragma once
#ifndef BFME2_OBBOXRENDEROBJCLASS_H
#define BFME2_OBBOXRENDEROBJCLASS_H

#include "WW3D2/BoxRenderObjClass.h"
#include "WWMath/OBBoxClass.h"

class OBBoxRenderObjClass : public BoxRenderObjClass
{
public:
	OBBoxRenderObjClass();											// 0x00175F50
	OBBoxRenderObjClass(const W3dBoxStruct &def);					// 0x00175FA0
	OBBoxRenderObjClass(const OBBoxRenderObjClass &src);			// 0x00176360
	OBBoxRenderObjClass(const OBBoxClass &box);						// 0x00176000
	OBBoxRenderObjClass &operator=(const OBBoxRenderObjClass &that);	// 0x001760C0

	virtual RenderObjClass *Clone() const;							// slot 2: 0x001763D0
	virtual int Class_ID() const { return 0x1B; }					// slot 3 (CLASSID_OBBOX)
	virtual void Render(RenderInfoClass &rinfo);					// slot 12: 0x001760F0
	virtual void Set_Transform(const Matrix3D &m);					// slot 21
	virtual void Set_Position(const Vector3 &v);					// slot 22
	virtual bool Cast_Ray(RayCollisionTestClass &raytest);			// slot 60: 0x001759C0
	virtual void Get_Obj_Space_Bounding_Sphere(SphereClass &sphere) const;	// slot 67
	virtual void Get_Obj_Space_Bounding_Box(AABoxClass &box) const;			// slot 68

	OBBoxClass &Get_Box();											// 0x00175A20

protected:
	virtual void update_cached_box();								// slot 0x82: 0x001758C0

	OBBoxClass CachedBox;											// +0x10C
};
// SIZE 0x148
typedef char OBBoxRenderObjClassSize[(sizeof(OBBoxRenderObjClass) == 0x148) ? 1 : -1];

#endif
