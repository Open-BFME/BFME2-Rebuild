// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// mempool.h: WWLib's ObjectPoolClass and AutoPoolClass (Zero Hour's mempool.h, release form: its asserts are gone).
// Target facts (retail game.dat): the pool is FreeListHead +0x00, BlockListHead +0x04, FreeObjectCount +0x08,
// TotalObjectCount +0x0C and a FastCriticalSectionClass lock word +0x10 (SIZE 0x14), which is the layout of the COMDAT
// pairs ObjectPoolClass<MultiListNodeClass, 256>::Allocate_Object_Memory 0x00610680 (201 B, with an unwind funclet for
// the lock guard) and the 38-byte free 0x00065964 shape; a block is `sizeof(T) * BLOCK_SIZE + 4` bytes from
// `_STL::allocator<char>::allocate(n, 0)` (0x000307F0, `push 0; push 0x1404; call`) with the previous block's address in its first word. AutoPoolClass<MultiListNodeClass, 256>::operator new
// 0x00610770 and operator delete 0x00610780 forward to the static Allocator at 0x00E07C04.
#pragma once
#ifndef BFME2_MEMPOOL_H
#define BFME2_MEMPOOL_H

#include <new>
#include <stddef.h>
#include "WWLib/mutex.h"

template <class T, int BLOCK_SIZE = 64>
class ObjectPoolClass
{
public:
	// No constructor and no destructor: retail never initialises or tears down the pool at 0x00E07C04 (no code
	// outside the three pool users references the address), so the object is plain zeroed .bss.

	T *Allocate_Object(void);
	void Free_Object(T *obj);

	T *Allocate_Object_Memory(void);
	void Free_Object_Memory(T *obj);

protected:
	T *FreeListHead;
	unsigned int *BlockListHead;
	int FreeObjectCount;
	int TotalObjectCount;
	FastCriticalSectionClass ObjectPoolCS;
};

template <class T, int BLOCK_SIZE = 64>
class AutoPoolClass
{
public:
	static void *operator new(size_t size);
	static void operator delete(void *memory);

private:
	// not implemented
	static void *operator new[](size_t size);
	static void operator delete[](void *memory);

	// This must be statically defined by the user
	static ObjectPoolClass<T, BLOCK_SIZE> Allocator;
};

// DEFINE_AUTO_POOL(T, BLOCKSIZE): put this in the .cpp file of the class.
#define DEFINE_AUTO_POOL(T, BLOCKSIZE) \
ObjectPoolClass<T, BLOCKSIZE> AutoPoolClass<T, BLOCKSIZE>::Allocator;

template <class T, int BLOCK_SIZE>
T *ObjectPoolClass<T, BLOCK_SIZE>::Allocate_Object(void)
{
	// allocate memory for the object
	T *obj = Allocate_Object_Memory();

	// construct the object in-place
	return new (obj) T;
}

template <class T, int BLOCK_SIZE>
void ObjectPoolClass<T, BLOCK_SIZE>::Free_Object(T *obj)
{
	// destruct the object
	obj->T::~T();

	// release the memory
	Free_Object_Memory(obj);
}

// Allocate_Object_Memory is defined in WWLib/mempool_block.h: it needs the game allocator through STLport, which only
// the units that define a pool include.

template <class T, int BLOCK_SIZE>
void ObjectPoolClass<T, BLOCK_SIZE>::Free_Object_Memory(T *obj)
{
	FastCriticalSectionClass::LockClass lock(ObjectPoolCS);

	*(T **)(obj) = FreeListHead;		// Link to the Head
	FreeListHead = obj;					// Set the Head
	FreeObjectCount++;
}

template <class T, int BLOCK_SIZE>
void *AutoPoolClass<T, BLOCK_SIZE>::operator new(size_t size)
{
	return (void *)(Allocator.Allocate_Object_Memory());
}

template <class T, int BLOCK_SIZE>
void AutoPoolClass<T, BLOCK_SIZE>::operator delete(void *memory)
{
	if (memory == 0) return;
	Allocator.Free_Object_Memory((T *)memory);
}

#endif
