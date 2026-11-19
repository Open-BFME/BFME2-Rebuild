// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// AABoxRenderObjClass.h: WW3D2's axis-aligned debug box (Zero Hour's boxrobj.h). Target facts: constructor 0x00175BB0 runs
// BoxRenderObjClass's, stores the vftable 0x007D4950 and fills CachedBox at +0x10C (centre = the transform's translation plus
// the local centre, extent = the local extent); Clone 0x00176300 allocates 0x124 bytes. SIZE 0x124. Overridden slots: 2
// Clone, 3 Class_ID (0x1A), 12 Render, 21 Set_Transform, 22 Set_Position, 60 Cast_Ray, 67/68 bounding sphere and box,
// 0x82 update_cached_box.
#pragma once
#ifndef BFME2_AABOXRENDEROBJCLASS_H
#define BFME2_AABOXRENDEROBJCLASS_H

#include "WW3D2/BoxRenderObjClass.h"
#include "WWMath/AABoxClass.h"

class AABoxRenderObjClass : public BoxRenderObjClass
{
public:
	AABoxRenderObjClass();											// 0x00175BB0
	AABoxRenderObjClass(const W3dBoxStruct &def);					// 0x00175C40
	AABoxRenderObjClass(const AABoxRenderObjClass &src);			// 0x001762A0
	AABoxRenderObjClass(const AABoxClass &box);						// 0x00175CD0
	AABoxRenderObjClass &operator=(const AABoxRenderObjClass &that);	// 0x00175DE0

	virtual RenderObjClass *Clone() const;							// slot 2: 0x00176300
	virtual int Class_ID() const { return 0x1A; }					// slot 3: 0x00175750 (CLASSID_AABOX)
	virtual void Render(RenderInfoClass &rinfo);					// slot 12: 0x00175E50
	virtual void Set_Transform(const Matrix3D &m);					// slot 21: 0x00175760
	virtual void Set_Position(const Vector3 &v);					// slot 22: 0x00175780
	virtual bool Cast_Ray(RayCollisionTestClass &raytest);			// slot 60: 0x00175810
	virtual void Get_Obj_Space_Bounding_Sphere(SphereClass &sphere) const;	// slot 67: 0x00176220
	virtual void Get_Obj_Space_Bounding_Box(AABoxClass &box) const;			// slot 68: 0x00175870

	const AABoxClass &Get_Box()
	{
		Validate_Transform();
		update_cached_box();
		return CachedBox;
	}

protected:
	virtual void update_cached_box();								// slot 0x82: 0x001757A0

	AABoxClass CachedBox;											// +0x10C
};
// SIZE 0x124
typedef char AABoxRenderObjClassSize[(sizeof(AABoxRenderObjClass) == 0x124) ? 1 : -1];

#endif
