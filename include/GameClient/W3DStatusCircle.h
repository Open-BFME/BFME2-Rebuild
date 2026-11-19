// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// W3DStatusCircle.h: the full-screen status circle render object (Zero Hour's W3DStatusCircle.h, Mark Wilczynski).
// Target facts (retail game.dat): the constructor 0x0008E54F runs RenderObjClass's constructor, clears +0xC4 (triangle count),
// +0xC8 and +0xD4, stores two floats at +0xCC/+0xD0, the shader bits 0x0010441B at +0xD8 and clears +0xDC, +0xE0 and +0xE4 (the
// vertex material and the circle and screen vertex buffers of Zero Hour's class, whose order BFME 2 keeps from +0xD8 on);
// vftables 0x00BC7850 and 0x00BC784C (+8). The two static members are the diffuse colour 0x00DB45C8 (initial value 255, blue)
// and the update flag 0x00DE207C; setColor 0x0004C41F is Zero Hour's inline: b + (g << 8) + (r << 16).
#pragma once
#ifndef BFME2_W3DSTATUSCIRCLE_H
#define BFME2_W3DSTATUSCIRCLE_H

#include "WW3D2/RenderObjClass.h"
#include "WW3D2/ShaderClass.h"
#include "WWMath/AABoxClass.h"
#include "WWMath/SphereClass.h"

class RayCollisionTestClass;

class W3DStatusCircle : public RenderObjClass
{
public:
	W3DStatusCircle(void);																	// 0x0008E54F
	virtual ~W3DStatusCircle(void);															// slot 1
	virtual RenderObjClass *Clone(void) const;												// slot 2
	virtual int Class_ID(void) const;														// slot 3
	virtual void Render(RenderInfoClass &rinfo);											// slot 12: 0x0008E7A4
	virtual bool Cast_Ray(RayCollisionTestClass &raytest);									// slot 60
	virtual void Get_Obj_Space_Bounding_Sphere(SphereClass &sphere) const;					// slot 67: 0x0008E5B0
	virtual void Get_Obj_Space_Bounding_Box(AABoxClass &aabox) const;						// slot 68: 0x0008E5F4

	static void setColor(int r, int g, int b) { m_needUpdate = true; m_diffuse = (b) + (g << 8) + (r << 16); }	// 0x0004C41F

protected:
	int m_numTriangles;			// +0xC4 dimensions of list
	void *m_indexBuffer;		// +0xC8 DX8 index buffer (Zero Hour's position is after the counters; BFME 2 keeps it here)
	float m_unk0xCC;			// +0xCC set by the constructor next to +0xD0 (a pair of floats; role not recovered)
	float m_unk0xD0;			// +0xD0
	int m_unk0xD4;				// +0xD4 cleared by the constructor
	ShaderClass m_shaderClass;	// +0xD8 shader or rendering state
	void *m_vertexMaterialClass;	// +0xDC
	void *m_vertexBufferCircle;	// +0xE0 collection of vertexes that make the circle.
	void *m_vertexBufferScreen;	// +0xE4 2 triangle quad that covers the screen.
	static int m_diffuse;		// 0x00DB45C8
	static bool m_needUpdate;	// 0x00DE207C
};
// SIZE 0xE8
typedef char W3DStatusCircleSize[(sizeof(W3DStatusCircle) == 0xE8) ? 1 : -1];

#endif
