// Line3DClass.h: WW3D2's box-shaped line render object (line3d.h).
// Target facts: allocation 0x144 (Clone 0x00166E00 news 0x144). The
// constructor 0x001673D0 gives the layout: Length +0xC4 (sqrt of the endpoint
// distance), Width +0xC8, Shader +0xCC, eight corner vertices +0xD0..+0x12F,
// Color +0x130 whose W (+0x13C) is the opacity (Re_Color 0x00167330 rewrites
// X/Y/Z and keeps W), SortLevel (signed char) +0x140. Overridden slots of
// RenderObjClass: 1, 2 Clone, 3 Class_ID (returns 6), 10 Get_Num_Polys (shared
// body), 12 Render, 67/68 bounding volumes, 91/92 Scale, 94/95 sort level.
#pragma once
#ifndef BFME2_LINE3DCLASS_H
#define BFME2_LINE3DCLASS_H

#include "WW3D2/RenderObjClass.h"
#include "WW3D2/ShaderClass.h"
#include "WWMath/Vector3.h"
#include "WWMath/Vector4.h"

class Line3DClass : public RenderObjClass
{
public:
	Line3DClass(const Vector3 &start, const Vector3 &end, float width, float r, float g, float b, float opacity = 1.0f);	// 0x001673D0
	Line3DClass(const Line3DClass &src);						// 0x00166A70
	Line3DClass &operator=(const Line3DClass &that);			// 0x00166C40

	virtual RenderObjClass *Clone() const;						// slot 2: 0x00166E00
	virtual int Class_ID() const;								// slot 3: 0x00166C10
	virtual int Get_Num_Polys() const;							// slot 10: 0x00182D20 (shared body)
	virtual void Render(RenderInfoClass &rinfo);				// slot 12: 0x00167660
	virtual void Get_Obj_Space_Bounding_Sphere(SphereClass &sphere) const;	// slot 67: 0x001672C0
	virtual void Get_Obj_Space_Bounding_Box(AABoxClass &box) const;			// slot 68: 0x001672F0
	virtual void Scale(float scale);							// slot 92: 0x00166E60
	virtual void Scale(float scalex, float scaley, float scalez);	// slot 91: 0x00167090
	virtual int Get_Sort_Level() const;							// slot 94: 0x00166C30
	virtual void Set_Sort_Level(int level);						// slot 95: 0x00166C20

	void Reset(const Vector3 &new_start, const Vector3 &new_end);					// 0x00167B60
	void Reset(const Vector3 &new_start, const Vector3 &new_end, float new_width);	// 0x00167CA0
	void Re_Color(float r, float g, float b);					// 0x00167330
	void Set_Opacity(float opacity);							// 0x00167370

protected:
	float Length;					// +0xC4
	float Width;					// +0xC8
	ShaderClass Shader;				// +0xCC
	Vector3 vert[8];				// +0xD0
	Vector4 Color;					// +0x130 (W is the opacity)
	signed char SortLevel;			// +0x140
};
// SIZE 0x144
typedef char Line3DClassSize[(sizeof(Line3DClass) == 0x144) ? 1 : -1];

#endif
