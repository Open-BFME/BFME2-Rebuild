// MatPassTaskClass.h: one queued material pass for a mesh (dx8renderer.cpp).
// Target facts: the constructor 0x001438C0 stores the pass at +0 and the mesh
// at +4, clears the next link at +8 and adds a reference to both; the
// destructor 0x001438F0 releases both. SIZE 0xC.
#pragma once
#ifndef BFME2_MATPASSTASKCLASS_H
#define BFME2_MATPASSTASKCLASS_H

#include "WWLib/mempool.h"

class MaterialPassClass;
class MeshClass;

class MatPassTaskClass : public AutoPoolClass<MatPassTaskClass, 256>
{
public:
	MatPassTaskClass(MaterialPassClass *pass, MeshClass *mesh);		// 0x001438C0
	~MatPassTaskClass(void);						// 0x001438F0

	MaterialPassClass *Peek_Material_Pass(void) { return Pass; }
	MeshClass *Peek_Mesh(void) { return Mesh; }
	MatPassTaskClass *Get_Next_Visible(void) { return NextVisible; }
	void Set_Next_Visible(MatPassTaskClass *task) { NextVisible = task; }

protected:
	MaterialPassClass *Pass;		// +0x0
	MeshClass *Mesh;			// +0x4
	MatPassTaskClass *NextVisible;		// +0x8
};

typedef char MatPassTaskClassSize[(sizeof(MatPassTaskClass) == 0xC) ? 1 : -1];

#endif
