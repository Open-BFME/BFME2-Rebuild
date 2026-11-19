// ShareBufferClass.h: WW3D's reference-counted array, shared between copies of
// a mesh (vertex arrays, names, per-polygon data).
// Target facts: ShareBufferClass<unsigned int>(int count, const char *msg,
// int alignment) 0x000D1DE2 sets NumRefs 1 and its vftable (two slots,
// RefCountClass's Delete_This and the deleting destructor 0x000D1E6C), stores
// the count at +0x10 and the alignment at +0x14, then allocates with
// operator new[]: count * sizeof(T) bytes into +8 with +0xC the same pointer
// when the alignment is 0, otherwise count * sizeof(T) + alignment bytes with
// +0xC rounded up to the alignment. The destructor 0x000D1E88 frees +8 with
// operator delete[]. MeshGeometryClass::Get_Name 0x0016A6B0 reads the array
// pointer at +0xC. The body reads the members, not the parameters (Zero Hour's
// text): 0x000D1E2D reloads Alignment from +0x14 after operator new[].
#pragma once
#ifndef BFME2_SHAREBUFFERCLASS_H
#define BFME2_SHAREBUFFERCLASS_H

#include <string.h>
#include "WWLib/RefCountClass.h"

template <class T>
class ShareBufferClass : public RefCountClass
{
public:
	ShareBufferClass(int count, const char *msg, int alignment);
	ShareBufferClass(const ShareBufferClass<T> &that);
	virtual ~ShareBufferClass(void);

	T *Get_Array(void) { return Array; }
	void Clear(void) { memset(Array, 0, Count * sizeof(T)); }
	// MatBufferClass::Set_Element 0x0015A060 and Peek_Element 0x0015A0E0 index the allocation at +0x08, not the view at +0x0C.
	T *Get_Raw_Array(void) { return (T *)RawBuffer; }
	int Get_Count(void) { return Count; }

protected:
	char *RawBuffer;		// +0x08 the allocation
	T *Array;			// +0x0C RawBuffer rounded up to Alignment
	int Count;			// +0x10
	int Alignment;			// +0x14
};

template <class T>
ShareBufferClass<T>::ShareBufferClass(int count, const char *msg, int alignment) :
	Count(count),
	Alignment(alignment)
{
	if (Alignment == 0) {
		RawBuffer = (char *)new T[Count];
		Array = (T *)RawBuffer;
	} else {
		RawBuffer = new char[Count * sizeof(T) + Alignment];
		Array = (T *)(((unsigned int)RawBuffer + Alignment - 1) & ~(Alignment - 1));
	}
}

// Copy constructor 0x0015AB50 (the VertexMaterialClass * instantiation) allocates the same way as the constructor and copies the
// elements through the view at +0x0C.
template <class T>
ShareBufferClass<T>::ShareBufferClass(const ShareBufferClass<T> &that) :
	Count(that.Count),
	Alignment(that.Alignment)
{
	if (Alignment == 0) {
		RawBuffer = (char *)new T[Count];
		Array = (T *)RawBuffer;
	} else {
		RawBuffer = new char[Count * sizeof(T) + Alignment];
		Array = (T *)(((unsigned int)RawBuffer + Alignment - 1) & ~(Alignment - 1));
	}
	for (int i = 0; i < Count; i++) {
		Array[i] = that.Array[i];
	}
}

template <class T>
ShareBufferClass<T>::~ShareBufferClass(void)
{
	delete[] RawBuffer;
}

#endif
