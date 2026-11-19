// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// RenderInfoClass.h: the per-frame render state a scene hands to render objects (WWVegas/WW3D2/rinfo.h).
// Layout facts (retail game.dat): the constructor 0x00142EE0 stores the camera reference at +0, zeroes six floats
// +4..+0x18, sets three floats +0x1C..+0x24 to 1.0, zeroes +0x28/+0x2C, the 32-entry material pass array +0x30..+0xAF,
// the pass count +0xB0 and the rejected count +0xB4, the override flag stack +0xB8 (32 entries) with its level +0x138
// and three pointers +0x13C/+0x140/+0x144 (a stack of reference-counted rendering methods that Pop_Rendering_Method
// 0x00142DF0 pops: it releases the last entry) ; the destructor 0x00142FE0 pops every material pass and then destroys
// that vector. Push_Material_Pass 0x00142BA0 refuses at 31 entries (counting the refusal in +0xB4).
// Zero Hour's members: Camera, fog_scale, fog_start, fog_end, alphaOverride, materialPassAlphaOverride,
// materialPassEmissiveOverride, light_environment, Texture_Projector: retail has three more floats after fog_end
// (names structural: unknown_10/14/18), so the BFME 2 order is fog_scale..unknown_18, then the three overrides.
// SIZE 0x148.
#pragma once
#ifndef BFME2_RENDERINFOCLASS_H
#define BFME2_RENDERINFOCLASS_H

#include "WWLib/RefCountClass.h"

class CameraClass;
class MaterialPassClass;
class LightEnvironmentClass;
class TexProjectClass;

class RenderInfoClass
{
public:
	RenderInfoClass(CameraClass &cam);							// 0x00142EE0
	~RenderInfoClass(void);										// 0x00142FE0

	enum RINFO_OVERRIDE_FLAGS {
		RINFO_OVERRIDE_DEFAULT = 0x0000,
		RINFO_OVERRIDE_FORCE_TWO_SIDED = 0x0001,
		RINFO_OVERRIDE_FORCE_SORTING = 0x0002,
		RINFO_OVERRIDE_ADDITIONAL_PASSES_ONLY = 0x0004,
		RINFO_OVERRIDE_SHADOW_RENDERING = 0x0008
	};

	enum { MAX_ADDITIONAL_MATERIAL_PASSES = 32, MAX_OVERRIDE_FLAG_LEVEL = 32 };

	void Push_Material_Pass(MaterialPassClass *matpass);		// 0x00142BA0
	void Pop_Material_Pass(void);								// 0x00142BE0
	int Additional_Pass_Count(void);							// 0x00142C20
	MaterialPassClass *Peek_Additional_Pass(int i);				// 0x00142C30
	void Push_Override_Flags(RINFO_OVERRIDE_FLAGS flg);			// 0x00142C50
	void Pop_Override_Flags(void);								// 0x00142C70
	RINFO_OVERRIDE_FLAGS &Current_Override_Flags(void);			// 0x00142C80
	void Pop_Rendering_Method(void);							// 0x00142DF0

	CameraClass &Camera;										// +0x00
	float fog_scale;											// +0x04
	float fog_start;											// +0x08
	float fog_end;												// +0x0C
	float unknown_10;											// +0x10 (structural name)
	float unknown_14;											// +0x14
	float unknown_18;											// +0x18
	float alphaOverride;										// +0x1C
	float materialPassAlphaOverride;							// +0x20
	float materialPassEmissiveOverride;							// +0x24
	LightEnvironmentClass *light_environment;					// +0x28
	TexProjectClass *Texture_Projector;							// +0x2C

protected:
	MaterialPassClass *AdditionalMaterialPassArray[MAX_ADDITIONAL_MATERIAL_PASSES];	// +0x30
	unsigned AdditionalMaterialPassCount;						// +0xB0
	unsigned RejectedMaterialPasses;							// +0xB4
	RINFO_OVERRIDE_FLAGS OverrideFlag[MAX_OVERRIDE_FLAG_LEVEL];	// +0xB8
	unsigned OverrideFlagLevel;									// +0x138
	// +0x13C: begin, end, capacity of a vector of reference-counted rendering methods (an STLport vector in retail).
	RefCountClass **RenderingMethodsFirst;						// +0x13C
	RefCountClass **RenderingMethodsLast;						// +0x140
	RefCountClass **RenderingMethodsEnd;						// +0x144
};

typedef char RenderInfoClassSize[(sizeof(RenderInfoClass) == 0x148) ? 1 : -1];

#endif
