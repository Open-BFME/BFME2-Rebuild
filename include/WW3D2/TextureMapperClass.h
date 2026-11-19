// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// TextureMapperClass.h: the base of WW3D's texture-coordinate mappers that a
// VertexMaterialClass holds per texture stage (mapper.cpp).
//
// Target facts:
// - vftable 0x007D3198, 10 slots: Delete_This, the deleting destructor
//   0x00184150 (it calls RefCountClass's destructor 0x00724510, so the class
//   adds no destructor body), 2 0x0065CE90 (`return 0`), 3 __purecall,
//   4 0x0073B660 (`return false`), 5 __purecall, 6 0x0069E440 (empty),
//   7 0x0073B660 (`return false`), 8 0x00182150 (`return false`, one argument),
//   9 __purecall. 23 mapper vftables derive from it; each returns its own
//   constant from slot 2 (0x001820A0 returns 8, 0x001820B0 9), the time-varying
//   mappers return true from slot 4 (0x001826C0), the environment mappers true
//   from slot 7.
// - The constructor 0x00182120 sets NumRefs and the vftable and stores the stage
//   at +8, clamped to 1 when it is 2 or more; the copy constructor 0x00182080
//   copies +8. SIZE 0xC.
// Slot names follow the class family's design (mapper id, clone, time variance,
// apply to a UV array, reset, normals, texture matrix); slot 8 is a BFME
// addition whose role is not recovered and keeps the old views' name.
#pragma once
#ifndef BFME2_TEXTUREMAPPERCLASS_H
#define BFME2_TEXTUREMAPPERCLASS_H

#include "WWLib/RefCountClass.h"

class Matrix4;
class FXShaderParameterList;

class TextureMapperClass : public RefCountClass
{
public:
	// Zero Hour's ids; retail's slot 2 bodies return them (scale 8 at 0x001820A0, grid 9 at 0x001820B0).
	enum
	{
		MAPPER_ID_UNKNOWN,
		MAPPER_ID_LINEAR_OFFSET,
		MAPPER_ID_CLASSIC_ENVIRONMENT,
		MAPPER_ID_ENVIRONMENT,
		MAPPER_ID_SCREEN,
		MAPPER_ID_ANIMATING_1D,
		MAPPER_ID_AXIAL,
		MAPPER_ID_SILHOUETTE,
		MAPPER_ID_SCALE,
		MAPPER_ID_GRID,
		MAPPER_ID_ROTATE,
		MAPPER_ID_SINE_LINEAR_OFFSET,
		MAPPER_ID_STEP_LINEAR_OFFSET,
		MAPPER_ID_ZIGZAG_LINEAR_OFFSET,
		MAPPER_ID_WS_CLASSIC_ENVIRONMENT,
		MAPPER_ID_WS_ENVIRONMENT,
		MAPPER_ID_GRID_CLASSIC_ENVIRONMENT,
		MAPPER_ID_GRID_ENVIRONMENT,
		MAPPER_ID_RANDOM,
		MAPPER_ID_EDGE,
		MAPPER_ID_BUMPENV,
		MAPPER_ID_GRID_WS_CLASSIC_ENVIRONMENT,
		MAPPER_ID_GRID_WS_ENVIRONMENT
	};

	TextureMapperClass(unsigned int stage = 0);						// 0x00182120
	TextureMapperClass(const TextureMapperClass &src) : Stage(src.Stage) {}					// 0x00182080

	virtual int Mapper_ID(void) const { return 0; }						// 2
	virtual TextureMapperClass *Clone(void) const = 0;					// 3
	virtual bool Is_Time_Variant(void) { return false; }					// 4
	virtual void Apply(int uv_array_index) = 0;						// 5
	virtual void Reset(void) {}								// 6
	virtual bool Needs_Normals(void) { return false; }					// 7
	// Slot 8: converts the mapper to FXShader parameters (appends them to the list; true when it did). Retail's base is
	// `xor al,al; ret 4` (0x00182150); the Scale, LinearOffset and Grid families override it (0x00187690, 0x00187850, 0x00187AD0).
	virtual bool Convert_To_FXShader(FXShaderParameterList *parameters) { return false; }	// 8
	virtual void Calculate_Texture_Matrix(Matrix4 &tex_matrix) = 0;				// 9

	unsigned int Get_Stage(void) const { return Stage; }

protected:
	unsigned int Stage;		// +0x08
};

typedef char TextureMapperClassSize[(sizeof(TextureMapperClass) == 0xC) ? 1 : -1];

#endif
