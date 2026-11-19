// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// IndexBufferClass.h: WW3D's index buffers (dx8indexbuffer.cpp, retail 0x001386A0..0x0013917F).
// Class list and member names from Zero Hour's dx8indexbuffer.h; retail differs in these recorded ways:
// - No W3DMPO base (every allocation is the global operator new, 0x0002FDA0 and 0x0002FDE0 for the sorting array).
// - The constructor 0x001386C0 (type, index count) sets NumRefs 1, stores the engine reference count 0 at +0x8, the index count
//   at +0xC and the type at +0x10, installs vftable 0x00BD2EEC and adds the count to the totals at 0x00DF29C8 (indices) and
//   0x00DF29CC (buffers) and twice the count to 0x00DF29D4 (bytes); the deleting destructor is slot 1 (0x001390A0). The count
//   is a dword here (DX8FVFCategoryContainer's fit tests 0x00143C20 and 0x00143A20 read `dword [buffer+0xC]`); Zero Hour's
//   is a short. Add_Engine_Ref 0x00138770 adds one to +0x8. SIZE 0x14.
// - DX8IndexBufferClass (vftable 0x00BD2EF4, SIZE 0x18) keeps the device buffer at +0x14; its constructor 0x00138980 takes the
//   count as an unsigned int (map name ...QAE@IW4UsageType@0@@Z) and holds the device lock (DX8DeviceLock.h) around the
//   creation, which goes through the Direct3D 9 device's CreateIndexBuffer (slot 27, `call [edx+0x6C]`) with the sixth argument
//   NULL, and on failure flushes the mesh cache and retries once.
// - SortingIndexBufferClass (vftable 0x00BD2EFC, SIZE 0x18) keeps its array of unsigned shorts at +0x14.
// - DynamicIBAccessClass is 0xC bytes: type +0, count +4, offset +6, buffer +8. Its WriteLockClass carries the device lock as a
//   third member (both bodies, 0x00138CA0 and 0x00138D80, have an exception frame whose only cleanup is that guard).
// - DX8IndexBufferClass::AppendLockClass and WriteLockClass are Zero Hour's: COMDAT bodies 0x00138790/0x001388C0 (the destructor
//   0x00138840 is shared).
#pragma once
#ifndef BFME2_INDEXBUFFERCLASS_H
#define BFME2_INDEXBUFFERCLASS_H

#include "WWLib/RefCountClass.h"
#include "WW3D2/DX8DeviceLock.h"

class DX8Wrapper;
class SortingRendererClass;
struct IDirect3DIndexBuffer8;
class DX8IndexBufferClass;
class SortingIndexBufferClass;

class IndexBufferClass : public RefCountClass
{
protected:
	virtual ~IndexBufferClass(void);						// 1: 0x001390A0 (deleting), body 0x00138710

public:
	IndexBufferClass(unsigned int type, unsigned int index_count);			// 0x001386C0

	void Copy(unsigned int *indices, unsigned start_index, unsigned index_count);		// 0x001390C0
	void Copy(unsigned short *indices, unsigned start_index, unsigned index_count);	// 0x00139160

	unsigned int Get_Index_Count(void) const { return IndexCount; }
	unsigned int Type(void) const { return BufferType; }
	int Engine_Refs(void) const { return EngineRefs; }
	static unsigned Get_Total_Buffer_Count(void);				// 0x00138740
	static unsigned Get_Total_Allocated_Indices(void);			// 0x00138750
	static unsigned Get_Total_Allocated_Memory(void);			// 0x00138760
	void Add_Engine_Ref(void) const;						// 0x00138770
	void Release_Engine_Ref(void) const;						// 0x00138780

	class WriteLockClass
	{
		IndexBufferClass *index_buffer;
		unsigned short *indices;
		DX8DeviceLock device_lock;
	public:
		WriteLockClass(IndexBufferClass *index_buffer, int flags = 0);		// 0x00138790
		~WriteLockClass();												// 0x00138840

		unsigned short *Get_Index_Array(void) { return indices; }
	};

	class AppendLockClass
	{
		IndexBufferClass *index_buffer;
		unsigned short *indices;
		DX8DeviceLock device_lock;
	public:
		AppendLockClass(IndexBufferClass *index_buffer, unsigned start_index, unsigned index_range, int flags = 0);	// 0x001388C0
		~AppendLockClass();												// 0x00138840

		unsigned short *Get_Index_Array(void) { return indices; }
	};

protected:
	mutable int EngineRefs;				// +0x08
	unsigned int IndexCount;			// +0x0C
	unsigned int BufferType;			// +0x10

	friend class WriteLockClass;
	friend class AppendLockClass;
	friend class DX8IndexBufferClass;
	friend class SortingIndexBufferClass;
};

typedef char IndexBufferClassSize[(sizeof(IndexBufferClass) == 0x14) ? 1 : -1];

class DynamicIBAccessClass
{
	unsigned Type;					// +0x00
	unsigned short IndexCount;		// +0x04
	unsigned short IndexBufferOffset;	// +0x06
	IndexBufferClass *IndexBuffer;	// +0x08

	friend class DX8Wrapper;
	friend class SortingRendererClass;

public:
	DynamicIBAccessClass(unsigned short type, unsigned short index_count);
	~DynamicIBAccessClass();					// 0x00138BE0

	void Allocate_Sorting_Dynamic_Buffer();		// 0x00138F50
	void Allocate_DX8_Dynamic_Buffer();			// 0x00138E00

	unsigned Get_Type() const { return Type; }
	unsigned short Get_Index_Count() const;		// 0x001386A0

	static void _Deinit();						// 0x00138C30
	static void _Reset(bool frame_changed);		// 0x00139070
	static unsigned short Get_Default_Index_Count(void);	// 0x00139090

	class WriteLockClass
	{
		DynamicIBAccessClass *DynamicIBAccess;
		unsigned short *Indices;
		DX8DeviceLock device_lock;
	public:
		WriteLockClass(DynamicIBAccessClass *ib_access);		// 0x00138CA0
		~WriteLockClass();										// 0x00138D80
		unsigned short *Get_Index_Array() { return Indices; }
	};

	friend class WriteLockClass;
};


class DX8IndexBufferClass : public IndexBufferClass
{
public:
	enum UsageType {
		USAGE_DEFAULT = 0,
		USAGE_DYNAMIC = 1,
		USAGE_SOFTWAREPROCESSING = 2,
		USAGE_NPATCHES = 4
	};

	DX8IndexBufferClass(unsigned int index_count, UsageType usage = USAGE_DEFAULT);	// 0x00138980
	virtual ~DX8IndexBufferClass(void);												// 0x00138AA0

	IDirect3DIndexBuffer8 *Get_DX8_Index_Buffer(void) { return index_buffer; }

private:
	IDirect3DIndexBuffer8 *index_buffer;		// +0x14
};

class SortingIndexBufferClass : public IndexBufferClass
{
public:
	SortingIndexBufferClass(unsigned short index_count);	// 0x00138B20
	virtual ~SortingIndexBufferClass(void);					// 0x00138BA0

protected:
	unsigned short *index_buffer;				// +0x14

	friend class DynamicIBAccessClass::WriteLockClass;
};

#endif
