// FreelistPool.h: the game's fixed-cell node pool and the container allocator
// built on it.
// Target facts (retail game.dat):
// - FreelistPool is 0x18 bytes: +0 cells per chunk (0x80), +4 chunk list, +8
//   free-cell head, +0xC chunk allocator 0x0002FFC0 (size, userData), +0x10
//   chunk free 0x0002FFE0 (ptr, userData), +0x14 userData. grow 0x001EAEF9 carves
//   a chunk of count*16+4 bytes into 16-byte cells, aligned to 8; pop
//   0x002393E2 takes the head, growing when empty; 0x001EAF7B frees every chunk.
// - Four pool objects exist (0x00DA60E8, 0x00DB8FEC, 0x00DBA5E0, 0x00DBBD2C),
//   byte-identical in .data, each re-initialized by every unit that uses it
//   (`push -1; push 0; mov ecx,pool; call grow; push dtor; call atexit`, 716
//   units for the first) and destroyed by a per-unit atexit thunk that jumps
//   to 0x001EAF7B: MSVC 7.1's per-unit dynamic initialization of a class
//   template's static data member. One pool per list element type: ObjectID
//   (ArrowStorm's list, erase 0x002ABB20), Object * (HordeContain), Drawable *
//   (DrawableList), TeamPrototype *.
// - The allocator: allocate(n) is `n <= 1 ? pool.pop() : 0` (0x0029E0E6) and
//   deallocate pushes the cell back inline (list::erase 0x002ABB20:
//   `mov edx,[0xDA60F0]; mov [eax],edx; mov [0xDA60F0],eax`).
// FreelistAllocator's own name is not recorded in the binary; FreelistPool's is
// the ledger's (?pop@FreelistPool@@QAEPAXXZ, ?grow@FreelistPool@@AAE_NHH@Z).
#pragma once
#ifndef BFME2_FREELISTPOOL_H
#define BFME2_FREELISTPOOL_H

#include <stddef.h>
#include <new>
#include "Common/GameType.h"

void *FreelistPoolChunkAlloc(Int size, Int userData);	// 0x0002FFC0
void FreelistPoolChunkFree(void *chunk, Int userData);	// 0x0002FFE0

class FreelistPool
{
public:
	FreelistPool()
		: m_cellsPerChunk(0x80), m_chunks(0), m_head(0),
		  m_chunkAlloc(FreelistPoolChunkAlloc), m_chunkFree(FreelistPoolChunkFree), m_userData(0)
	{
		grow(0, -1);
	}
	~FreelistPool() { releaseChunks(); }

	void *pop();												// 0x002393E2
	void push(void *cell) { *(void **)cell = m_head; m_head = cell; }

private:
	bool grow(Int chunk, Int size);								// 0x001EAEF9
	bool releaseChunks();										// 0x001EAF7B

	Int m_cellsPerChunk;										// 0x00
	void *m_chunks;												// 0x04
	void *m_head;												// 0x08
	void *(__cdecl *m_chunkAlloc)(Int size, Int userData);		// 0x0C
	void (__cdecl *m_chunkFree)(void *chunk, Int userData);		// 0x10
	Int m_userData;												// 0x14
};
// SIZE 0x18

// The node allocator a container names as its allocator; STLport rebinds it to
// the node type, so each list element type gets its own pool.
template <class T>
class FreelistAllocator
{
public:
	typedef T value_type;
	typedef T *pointer;
	typedef const T *const_pointer;
	typedef T &reference;
	typedef const T &const_reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	template <class U> struct rebind { typedef FreelistAllocator<U> other; };

	FreelistAllocator() {}
	FreelistAllocator(const FreelistAllocator<T> &) {}
	template <class U> FreelistAllocator(const FreelistAllocator<U> &) {}
	~FreelistAllocator() {}

	pointer allocate(size_type n, const void * = 0) { return n <= 1 ? (pointer)s_pool.pop() : 0; }
	void deallocate(pointer p, size_type) { s_pool.push(p); }
	void construct(pointer p, const T &value) { new (p) T(value); }
	void destroy(pointer p) { p->~T(); }
	size_type max_size() const { return 1; }

	static FreelistPool s_pool;
};

template <class T> FreelistPool FreelistAllocator<T>::s_pool;

template <class T, class U>
inline bool operator==(const FreelistAllocator<T> &, const FreelistAllocator<U> &) { return true; }
template <class T, class U>
inline bool operator!=(const FreelistAllocator<T> &, const FreelistAllocator<U> &) { return false; }

#endif
