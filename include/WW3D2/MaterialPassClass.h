// MaterialPassClass.h: an extra rendering pass (textures, shader and vertex
// material) applied on top of a mesh, e.g. the shroud and glow passes.
//
// Target facts:
// - The constructor 0x0013EDD0 sets NumRefs 1 and vftable 0x007D330C, builds
//   eight texture handles at +8 (vector constructor with the handle's
//   constructor and destructor), clears +0x28, +0x2C and +0x34 and sets the
//   byte +0x30. The deleting destructor 0x0013EF80 calls the destructor
//   0x0013EE40. SIZE 0x38.
// - vftable: Delete_This, the deleting destructor, Install_Materials
//   0x0013EFA0, then the empty 0x0069E440, which W3DShroudMaterialPassClass
//   overrides as UnInstall_Materials 0x000729EB.
// - MatPassTaskClass's constructor 0x001438C0 adds a reference to the pass.
// Names of +0x28..+0x34 are role names (shader bits, vertex material, a flag).
#pragma once
#ifndef BFME2_MATERIALPASSCLASS_H
#define BFME2_MATERIALPASSCLASS_H

#include "WWLib/RefCountClass.h"
#include "WW3D2/TextureAsset.h"
#include "WW3D2/ShaderClass.h"

class VertexMaterialClass;

class MaterialPassClass : public RefCountClass
{
public:
	enum { MAX_TEX_STAGES = 8 };

	MaterialPassClass(void);						// 0x0013EDD0
	virtual ~MaterialPassClass(void);					// 1: 0x0013EE40

	void Set_Shader(ShaderClass shader);					// 0x0013EEF0
	void Set_Material(VertexMaterialClass *mat);				// 0x0030D3AD

	virtual void Install_Materials(void) const;				// 2: 0x0013EFA0
	virtual void UnInstall_Materials(void) const {}				// 3: 0x0069E440 (empty)

protected:
	TextureAsset Texture[MAX_TEX_STAGES];		// +0x08
	ShaderClass Shader;				// +0x28 cleared by the constructor
	VertexMaterialClass *Material;			// +0x2C
	bool Flag30;					// +0x30 true after construction; role not seen yet
	int Field34;					// +0x34 cleared by the constructor
};

typedef char MaterialPassClassSize[(sizeof(MaterialPassClass) == 0x38) ? 1 : -1];

#endif
