// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// W3DBufferManager.h: the partial vertex and index buffer allocator (Zero Hour's W3DBufferManager.h, Mark Wilczynski).
// Target facts (retail game.dat): the same members and structs as Zero Hour, with larger size tables: the constructor
// 0x00115DF4 clears 18 x 512 vertex slot heads (0x9000 bytes from +0, the size index is (size >> 5) - 1 and the format
// is shifted by 9, getSlot 0x001163F1), the 18 vertex buffer heads at +0x9000, 4096 empty vertex slots of 0x1C bytes
// at +0x9048, the slot count at +0x25048, 32 empty vertex buffers of 0x1C bytes at +0x2504C, their count at +0x253CC,
// 1024 index slot heads at +0x253D0 (getSlot 0x00116434), the index buffer head at +0x263D0, 4096 empty index slots
// at +0x263D4, their count at +0x423D4, 32 empty index buffers of 0x14 bytes at +0x423D8 and their count at +0x42658.
// SIZE 0x4265C. The constructor zeroes the four struct arrays itself, member by member in declaration order.
#pragma once
#ifndef BFME2_W3DBUFFERMANAGER_H
#define BFME2_W3DBUFFERMANAGER_H

class VertexBufferClass;
class DX8IndexBufferClass;

#define MAX_VB_SIZES 512				// number of different sized VB slots allowed.
#define MIN_SLOT_SIZE 32				// minimum number of vertices allocated per slot (power of 2). See also MIN_SLOT_SIZE_SHIFT.
#define MIN_SLOT_SIZE_SHIFT 5			// used for division by MIN_SLOT_SIZE
#define MAX_VERTEX_BUFFERS_CREATED 32	// maximum number of D3D vertex buffers allowed to create per vertex type.
#define MAX_NUMBER_SLOTS 4096			// maximum number of slots that can be allocated.

#define MAX_IB_SIZES 1024				// number of different sized IB slots allowed.
#define MAX_INDEX_BUFFERS_CREATED 32
#define DEFAULT_INDEX_BUFFER_SIZE 32768

class W3DBufferManager
{
public:
	// List of all possible vertex formats available to the game.
	enum VBM_FVF_TYPES
	{
		VBM_FVF_XYZ,		// position
		VBM_FVF_XYZD,		// position, diffuse
		VBM_FVF_XYZUV,		// position, uv
		VBM_FVF_XYZDUV,		// position, diffuse, uv
		VBM_FVF_XYZUV2,		// position, uv1, uv2
		VBM_FVF_XYZDUV2,	// position, diffuse, uv1, uv2
		VBM_FVF_XYZN,		// position, normal
		VBM_FVF_XYZND,		// position, normal, diffuse
		VBM_FVF_XYZNUV,		// position, normal, uv
		VBM_FVF_XYZNDUV,	// position, normal, diffuse, uv
		VBM_FVF_XYZNUV2,	// position, normal, uv1, uv2
		VBM_FVF_XYZNDUV2,	// position, normal, diffuse, uv1, uv2
		VBM_FVF_XYZRHW,		// transformed position
		VBM_FVF_XYZRHWD,	// transformed position, diffuse
		VBM_FVF_XYZRHWUV,	// transformed position, uv
		VBM_FVF_XYZRHWDUV,	// transformed position, diffuse, uv
		VBM_FVF_XYZRHWUV2,	// transformed position, uv1, uv2
		VBM_FVF_XYZRHWDUV2,	// transformed position, diffuse, uv1, uv2
		MAX_FVF
	};

	struct W3DRenderTask
	{
		W3DRenderTask *m_nextTask;	// next rendering task
	};

	struct W3DVertexBuffer;
	struct W3DIndexBuffer;

	struct W3DVertexBufferSlot
	{
		int m_size;								// +0x00 number of vertices in slot
		int m_start;							// +0x04 index of first vertex within VB
		W3DVertexBuffer *m_VB;					// +0x08 vertex buffer holding this slot.
		W3DVertexBufferSlot *m_prevSameSize;	// +0x0C previous slot of equal size.
		W3DVertexBufferSlot *m_nextSameSize;	// +0x10 next slot of equal size.
		W3DVertexBufferSlot *m_prevSameVB;		// +0x14 previous slot in same VB.
		W3DVertexBufferSlot *m_nextSameVB;		// +0x18 next slot in same VB.
	};

	struct W3DVertexBuffer
	{
		VBM_FVF_TYPES m_format;					// +0x00 format of vertices in this VB.
		W3DVertexBufferSlot *m_usedSlots;		// +0x04 slots inside this vertex buffer being used.
		int m_startFreeIndex;					// +0x08 index of vertex at start of unallocated memory.
		int m_size;								// +0x0C number of vertices allowed in VB.
		W3DVertexBuffer *m_nextVB;				// +0x10 next vertex buffer of same type.
		VertexBufferClass *m_DX8VertexBuffer;	// +0x14 actual DX8 vertex buffer interface
		W3DRenderTask *m_renderTaskList;		// +0x18 used to help app sort its D3D access by VB.
	};

	struct W3DIndexBufferSlot
	{
		int m_size;								// +0x00 number of indices in slot
		int m_start;							// +0x04 index of first index within IB
		W3DIndexBuffer *m_IB;					// +0x08 index buffer holding this slot.
		W3DIndexBufferSlot *m_prevSameSize;		// +0x0C previous slot of equal size.
		W3DIndexBufferSlot *m_nextSameSize;		// +0x10 next slot of equal size.
		W3DIndexBufferSlot *m_prevSameIB;		// +0x14 previous slot in same IB.
		W3DIndexBufferSlot *m_nextSameIB;		// +0x18 next slot in same IB.
	};

	struct W3DIndexBuffer
	{
		W3DIndexBufferSlot *m_usedSlots;		// +0x00 slots inside this index buffer being used.
		int m_startFreeIndex;					// +0x04 index of index at start of unallocated memory.
		int m_size;								// +0x08 number of indices allowed in IB.
		W3DIndexBuffer *m_nextIB;				// +0x0C next index buffer.
		DX8IndexBufferClass *m_DX8IndexBuffer;	// +0x10 actual DX8 index buffer interface
	};

	W3DBufferManager(void);												// 0x00115DF4
	~W3DBufferManager(void);											// 0x001163E7

	W3DVertexBufferSlot *getSlot(VBM_FVF_TYPES fvfType, int size);		// 0x001163F1 return free vertex buffer memory slot.
	W3DIndexBufferSlot *getSlot(int size);								// 0x00116434 return free index buffer memory slot.
	void releaseSlot(W3DVertexBufferSlot *vbSlot);						// 0x00116113 return slot to pool
	void releaseSlot(W3DIndexBufferSlot *vbSlot);						// 0x0011629A return slot to pool
	void freeAllSlots(void);											// 0x00115ECB release all slots to pool.
	void freeAllBuffers(void);											// 0x00115F73 release all vertex buffers to pool.
	void ReleaseResources(void);										// 0x00115FF1 release D3D/W3D resources.
	bool ReAcquireResources(void);										// 0x0011604D reacquire D3D/W3D resources.

	// allows iterating over vertex buffers used by manager. Input of NULL to get first.
	W3DVertexBuffer *getNextVertexBuffer(W3DVertexBuffer *pVb, VBM_FVF_TYPES type)
	{
		if (pVb == 0)
			return m_W3DVertexBuffers[type];
		return pVb->m_nextVB;
	}

protected:
	W3DVertexBufferSlot *m_W3DVertexBufferSlots[MAX_FVF][MAX_VB_SIZES];	// +0x00000 previously allocated slots that are free to fill again.
	W3DVertexBuffer *m_W3DVertexBuffers[MAX_FVF];						// +0x09000 allocated vertex buffers of each type.
	W3DVertexBufferSlot m_W3DVertexBufferEmptySlots[MAX_NUMBER_SLOTS];	// +0x09048 unallocated slot wrappers
	int m_numEmptySlotsAllocated;										// +0x25048
	W3DVertexBuffer m_W3DEmptyVertexBuffers[MAX_VERTEX_BUFFERS_CREATED];	// +0x2504C
	int m_numEmptyVertexBuffersAllocated;								// +0x253CC

	W3DIndexBufferSlot *m_W3DIndexBufferSlots[MAX_IB_SIZES];			// +0x253D0
	W3DIndexBuffer *m_W3DIndexBuffers;									// +0x263D0
	W3DIndexBufferSlot m_W3DIndexBufferEmptySlots[MAX_NUMBER_SLOTS];	// +0x263D4
	int m_numEmptyIndexSlotsAllocated;									// +0x423D4
	W3DIndexBuffer m_W3DEmptyIndexBuffers[MAX_INDEX_BUFFERS_CREATED];	// +0x423D8
	int m_numEmptyIndexBuffersAllocated;								// +0x42658

	W3DVertexBufferSlot *allocateSlotStorage(VBM_FVF_TYPES fvfType, int size);	// 0x0011615B
	W3DIndexBufferSlot *allocateSlotStorage(int size);					// 0x001162BE
};
// SIZE 0x4265C
typedef char W3DBufferManagerSize[(sizeof(W3DBufferManager) == 0x4265C) ? 1 : -1];

extern W3DBufferManager *TheW3DBufferManager;	// singleton

#endif
