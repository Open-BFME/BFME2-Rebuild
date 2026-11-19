// MatBufferClass.h: the per-vertex vertex-material array of a mesh material description (MeshMatDescClass +0xE8).
//
// Target facts (retail game.dat): a ShareBufferClass<VertexMaterialClass *> (vftable 0x00BD3C84) that owns a reference to
// each element. Its own vftable 0x00BD3C9C has RefCountClass's Delete_This and the scalar deleting destructor
// 0x0015D040. The copy constructor 0x0015B2C0 runs the base copy 0x0015AB50, stores the vftable and adds a reference to each
// non-null element; the destructor 0x0015B300 releases and clears each one before the base destructor frees the allocation.
// Set_Element 0x0015A060 adds the new reference first, releases the old one and stores; Peek_Element 0x0015A0E0 reads the
// allocation at +0x08 (MeshMatDescClass::Peek_Material 0x0015A8E0 expands it).
// SIZE 0x18.
#pragma once
#ifndef BFME2_MATBUFFERCLASS_H
#define BFME2_MATBUFFERCLASS_H

#include "WW3D2/ShareBufferClass.h"
#include "WW3D2/VertexMaterialClass.h"

class VertexMaterialClass;

class MatBufferClass : public ShareBufferClass<VertexMaterialClass *>
{
public:
	MatBufferClass(int count, const char *msg) : ShareBufferClass<VertexMaterialClass *>(count, msg, 0)	// 0x0015B1A0
	{
		memset(Get_Raw_Array(), 0, count * sizeof(VertexMaterialClass *));
	}
	MatBufferClass(const MatBufferClass &that);			// 0x0015B2C0
	virtual ~MatBufferClass(void);					// 0x0015B300

	void Set_Element(int index, VertexMaterialClass *vmat);	// 0x0015A060
	VertexMaterialClass *Peek_Element(int index);			// 0x0015A0E0 (out of line in retail)
	VertexMaterialClass *Get_Element(int index)
	{
		if (Get_Raw_Array()[index] != NULL) {
			Get_Raw_Array()[index]->Add_Ref();
		}
		return Get_Raw_Array()[index];
	}
};

#endif
