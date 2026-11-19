// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// BoxRenderObjClass.h: WW3D2's debug box render object (Zero Hour's boxrobj.h), the base of AABoxRenderObjClass and
// OBBoxRenderObjClass. Target facts (retail game.dat): constructor 0x00174C80 runs RenderObjClass's constructor, stores the
// vftable 0x007D4740, zeroes the 32-byte name at +0xC4, sets the colour +0xE4 = (1, 1, 1), the local centre +0xF0 = (0, 0, 0),
// the local extent +0xFC = (1, 1, 1) and the opacity +0x108 = 0.25. SIZE 0x10C. The vtable adds one slot after
// RenderObjClass's, 129 (+0x204, returns 8) and 130 (+0x208) = update_cached_box, pure here. Slot 6 (Get_Name) is out of line at 0x00174DC0 and slot
// 10 (Get_Num_Polys, 12) is a shared stub (0x00182D20). BFME 2's table has no Special_Render, Cast_AABox, Cast_OBBox or
// Intersect_OBBox override: those Zero Hour slots are not in RenderObjClass here.
#pragma once
#ifndef BFME2_BOXRENDEROBJCLASS_H
#define BFME2_BOXRENDEROBJCLASS_H

#include "WW3D2/RenderObjClass.h"
#include "WWMath/Vector3.h"

struct W3dBoxStruct;

class BoxRenderObjClass : public RenderObjClass
{
public:
	BoxRenderObjClass();											// 0x00174C80
	BoxRenderObjClass(const W3dBoxStruct &def);						// 0x00175A40
	BoxRenderObjClass(const BoxRenderObjClass &src);				// 0x00175B50
	BoxRenderObjClass &operator=(const BoxRenderObjClass &that);	// 0x00174D20

	virtual int Get_Num_Polys() const { return 12; }				// slot 10 (shared stub 0x00182D20)
	virtual const char *Get_Name() const { return Name; }			// slot 6: 0x00174DC0
	virtual void Set_Name(const char *name);						// slot 7: 0x00174DD0
	void Set_Color(const Vector3 &color);							// 0x00174DF0
	void Set_Opacity(float opacity) { Opacity = opacity; }

	static void Init();												// 0x00174E20
	static void Shutdown();											// 0x00174EF0

	static void Set_Box_Display_Mask(int mask);						// 0x00174F20
	static int Get_Box_Display_Mask();								// 0x00174F30

	void Set_Local_Center_Extent(const Vector3 &center, const Vector3 &extent)
	{
		ObjSpaceCenter = center;
		ObjSpaceExtent = extent;
		update_cached_box();
	}
	const Vector3 &Get_Local_Center() { return ObjSpaceCenter; }
	const Vector3 &Get_Local_Extent() { return ObjSpaceExtent; }

	virtual int Get_Box_Type() const { return 8; }					// slot 129: shared stub 0x001820A0 (`mov eax, 8`); name: structural

protected:
	virtual void update_cached_box() = 0;							// slot 130
	void render_box(RenderInfoClass &rinfo, const Vector3 &center, const Vector3 &extent);	// 0x00174F40

	static bool IsInitted;											// 0x009F6FCC (Init/Shutdown)
	static int DisplayMask;											// 0x009F6FD0

	char Name[32];													// +0xC4
	Vector3 Color;													// +0xE4
	Vector3 ObjSpaceCenter;											// +0xF0
	Vector3 ObjSpaceExtent;											// +0xFC
	float Opacity;													// +0x108
};
// SIZE 0x10C
typedef char BoxRenderObjClassSize[(sizeof(BoxRenderObjClass) == 0x10C) ? 1 : -1];

#endif
