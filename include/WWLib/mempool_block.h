// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// mempool_block.h: ObjectPoolClass<T, BLOCK_SIZE>::Allocate_Object_Memory (Zero Hour's mempool.h). Retail's block comes
// from `_STL::allocator<char>::allocate(n, 0)` (0x000307F0: `push 0; push 0x1404; call` in 0x00610680 and 0x0071AED0), the
// game's allocator, so a unit that defines a pool includes this header and carries `// bfmealloc_alloconly` and
// `/D_CRTIMP=` on its cl line.
#pragma once
#ifndef BFME2_MEMPOOL_BLOCK_H
#define BFME2_MEMPOOL_BLOCK_H

#include <memory>
#include "WWLib/mempool.h"

template <class T, int BLOCK_SIZE>
T *ObjectPoolClass<T, BLOCK_SIZE>::Allocate_Object_Memory(void)
{
	FastCriticalSectionClass::LockClass lock(ObjectPoolCS);

	if (FreeListHead == 0) {

		// No free objects, allocate another block
		unsigned int *tmp_block_head = BlockListHead;
		BlockListHead = (unsigned int *)_STL::allocator<char>::allocate(sizeof(T) * BLOCK_SIZE + sizeof(unsigned int *), 0);
		// Link this block into the block list
		*(void **)BlockListHead = tmp_block_head;

		// Link the objects in the block into the free object list
		FreeListHead = (T *)(BlockListHead + 1);
		for (int i = 0; i < BLOCK_SIZE; i++) {
			*(T **)(&(FreeListHead[i])) = &(FreeListHead[i + 1]);	// link up the elements
		}
		*(T **)(&(FreeListHead[BLOCK_SIZE - 1])) = 0;				// Mark the end

		FreeObjectCount += BLOCK_SIZE;
		TotalObjectCount += BLOCK_SIZE;
	}

	T *obj = FreeListHead;						// Get the next free object
	FreeListHead = *(T **)(FreeListHead);		// Bump the Head
	FreeObjectCount--;

	return obj;
}

#endif
