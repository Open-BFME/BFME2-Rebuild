// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// PointGroupClass.h: WW3D2's renderer for groups of points (Zero Hour's pointgr.h; BFME 2 keeps its layout).
// Target facts (retail game.dat): constructor 0x00178E50 stores the table 0x00BD4E94 at +0, nulls the six shared
// arrays at +0x4..+0x18 and the point count +0x1C, clears the frame log2 byte +0x20 and the texture +0x24, copies the
// additive sprite preset into the shader +0x28, clears the mode +0x2C and flags +0x30 and sets the defaults: size
// +0x34 = 0, colour +0x38..+0x40 = (1, 1, 1), alpha +0x44 = 1, orientation +0x48 and frame +0x49 = 0, view-plane
// rectangle +0x4C..+0x58 = 0. The accessors 0x00178FA0..0x00179160 read and write those fields.
#pragma once
#ifndef BFME2_POINTGROUPCLASS_H
#define BFME2_POINTGROUPCLASS_H

#include "WW3D2/ShaderClass.h"
#include "WW3D2/TextureAsset.h"
#include "WWMath/Vector3.h"
#include "WWMath/Vector4.h"

class RenderInfoClass;
template <class T> class ShareBufferClass;

class PointGroupClass
{
public:
	enum PointModeEnum
	{
		TRIS,			// each point is a triangle
		QUADS,			// each point is a quad formed out of two triangles
		SCREENSPACE		// each point is a triangle placed to affect certain pixels
	};

	enum FlagsType
	{
		TRANSFORM		// transform points with the modelview matrix (world-space points)
	};

	PointGroupClass();									// 0x00178E50
	virtual ~PointGroupClass();							// 0x00178ED0

	void Set_Point_Size(float size);					// 0x00178FA0
	void Set_Point_Color(Vector3 color);				// 0x00178FC0
	Vector3 Get_Point_Color();							// 0x00178FF0
	void Set_Point_Orientation(unsigned char orientation);	// 0x00179020
	void Set_Point_Frame(unsigned char frame);			// 0x00179040
	void Set_Flag(FlagsType flag, bool onoff);			// 0x00179060
	int Get_Flag(FlagsType flag);						// 0x00179090
	unsigned char Get_Frame_Row_Column_Count_Log2();	// 0x00179130
	void Set_Frame_Row_Column_Count_Log2(unsigned char frccl2);	// 0x00179140
	int Get_Polygon_Count();							// 0x00179160

	ShaderClass Get_Shader();

protected:
	ShareBufferClass<Vector3> *PointLoc;				// +0x04
	ShareBufferClass<Vector4> *PointDiffuse;			// +0x08
	ShareBufferClass<unsigned int> *APT;				// +0x0C active point table
	ShareBufferClass<float> *PointSize;					// +0x10
	ShareBufferClass<unsigned char> *PointOrientation;	// +0x14
	ShareBufferClass<unsigned char> *PointFrame;		// +0x18
	int PointCount;										// +0x1C
	unsigned char FrameRowColumnCountLog2;				// +0x20 never above 4
	TextureAsset Texture;								// +0x24
	ShaderClass Shader;									// +0x28
	PointModeEnum PointMode;							// +0x2C
	unsigned int Flags;									// +0x30
	float DefaultPointSize;								// +0x34
	Vector3 DefaultPointColor;							// +0x38
	float DefaultPointAlpha;							// +0x44
	unsigned char DefaultPointOrientation;				// +0x48
	unsigned char DefaultPointFrame;					// +0x49
	float VPXMin;										// +0x4C
	float VPYMin;										// +0x50
	float VPXMax;										// +0x54
	float VPYMax;										// +0x58
	bool Billboard;										// +0x5C
};

#endif
