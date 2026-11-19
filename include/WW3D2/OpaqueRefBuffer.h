// OpaqueRefBuffer.h: the per-polygon array of referenced objects of a mesh material description (MeshMatDescClass +0x108).
//
// Target facts (retail game.dat): the same shape as MatBufferClass over RefCountClass pointers: a
// ShareBufferClass<RefCountClass *> (vftable 0x00BD3CCC is its own, scalar deleting destructor 0x0015D060) that owns a
// reference to each element. The copy constructor 0x0015B390 runs the base copy, stores the vftable and adds a reference to
// each non-null element; the destructor 0x0015B3D0 releases and clears each one before the base frees the allocation. The
// elements are indexed through the allocation at +0x08 (MeshMatDescClass::Set_FXShader 0x0015D300).
// SIZE 0x18.
#pragma once
#ifndef BFME2_OPAQUEREFBUFFER_H
#define BFME2_OPAQUEREFBUFFER_H

#include "WW3D2/ShareBufferClass.h"

class OpaqueRefBuffer : public ShareBufferClass<RefCountClass *>
{
public:
	OpaqueRefBuffer(int count, const char *msg) : ShareBufferClass<RefCountClass *>(count, msg, 0)	// 0x0015B230
	{
		memset(Get_Raw_Array(), 0, count * sizeof(RefCountClass *));
	}
	OpaqueRefBuffer(const OpaqueRefBuffer &that);			// 0x0015B390
	virtual ~OpaqueRefBuffer(void);					// 0x0015B3D0

	RefCountClass *Peek_Element(int index) { return Get_Raw_Array()[index]; }
	RefCountClass *Get_Element(int index)
	{
		if (Get_Raw_Array()[index] != NULL) {
			Get_Raw_Array()[index]->Add_Ref();
		}
		return Get_Raw_Array()[index];
	}
	void Set_Element(int index, RefCountClass *object)
	{
		if (object != NULL) {
			object->Add_Ref();
		}
		if (Get_Raw_Array()[index] != NULL) {
			Get_Raw_Array()[index]->Release_Ref();
		}
		Get_Raw_Array()[index] = object;
	}
};

#endif
