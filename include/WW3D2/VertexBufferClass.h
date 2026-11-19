// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// VertexBufferClass.h: WW3D's vertex buffers (dx8vertexbuffer.cpp, retail 0x00139350..0x0013B090).
// Class list and member names from Zero Hour's dx8vertexbuffer.h; retail differs in these recorded ways:
// - No W3DMPO base (every allocation is the global operator new, 0x0002FDA0 and 0x0002FDE0 for the sorting array).
// - VertexBufferClass (vftable 0x007D2F40: Delete_This 0x0006CE57, deleting destructor 0x0013ABC0) is 0x1C bytes: type +0x08, the
//   vertex count as a word at +0x0C, the engine reference count at +0x10, the FVFInfoClass pointer at +0x14 (allocated 0x60 bytes
//   by the constructor 0x00139350 with `new` and freed by the destructor 0x00139410) and a flag byte at +0x18 that BFME 2 added:
//   the constructor stores (fourth argument != 0), and DynamicVBAccessClass::Allocate_DX8_Dynamic_Buffer overwrites it with the
//   access object's own flag.
// - DX8VertexBufferClass (vftable 0x007D2F50, SIZE 0x20) keeps the device buffer at +0x1C; SortingVertexBufferClass (vftable
//   0x007D2F48, SIZE 0x20) keeps its array of XYZNDUV2 vertices (0x2C bytes each: the constructor 0x00139660 allocates
//   count * 44 bytes through 0x0002FDE0) at +0x1C.
// - The buffer interface is the Direct3D 9 one: Lock is slot 11 (`call [edx+0x2C]`), Unlock slot 12, Release slot 2, and the
//   device creates buffers through slot 26 (`call [eax+0x68]`, seven arguments including a trailing NULL shared handle).
// - DynamicVBAccessClass is 0x18 bytes (FVF descriptor pointer +0, type +4, format index +8, flag +0xC, count +0x10, offset
//   +0x12, buffer +0x14). Retail keeps one dynamic buffer per entry of a fifteen-entry format table (0x007D2F04: the Zero Hour
//   dynamic formats plus 0x52, 0x344 and 0x444): InUse, buffer, offset and size per format, then one sorting array.
#pragma once
#ifndef BFME2_VERTEXBUFFERCLASS_H
#define BFME2_VERTEXBUFFERCLASS_H

#include "WWLib/RefCountClass.h"
#include "WW3D2/DX8DeviceLock.h"
#include "WW3D2/FVFInfoClass.h"

class DX8Wrapper;
class SortingRendererClass;
class Vector2;
class Vector3;
class Vector4;
struct IDirect3DVertexBuffer8;
struct VertexFormatXYZNDUV2;
class DX8VertexBufferClass;
class SortingVertexBufferClass;

class VertexBufferClass : public RefCountClass
{
public:
	VertexBufferClass(unsigned type, unsigned FVF, unsigned short vertex_count, unsigned vertex_size);	// 0x00139350
	virtual ~VertexBufferClass(void);												// 1: 0x0013ABC0 (deleting), body 0x00139410

	const FVFInfoClass &FVF_Info(void) const { return *FVFInfo; }
	unsigned short Get_Vertex_Count(void) const { return VertexCount; }
	unsigned Type(void) const { return BufferType; }
	int Engine_Refs(void) const { return EngineRefs; }
	void Add_Engine_Ref(void) const;						// 0x00139480
	void Release_Engine_Ref(void) const;					// 0x00139490

	static unsigned Get_Total_Buffer_Count(void);			// 0x00139450
	static unsigned Get_Total_Allocated_Vertices(void);		// 0x00139460
	static unsigned Get_Total_Allocated_Memory(void);		// 0x00139470

	class WriteLockClass
	{
		VertexBufferClass *VertexBuffer;
		void *Vertices;
		DX8DeviceLock device_lock;
	public:
		WriteLockClass(VertexBufferClass *vertex_buffer, int flags = 0);		// 0x001394A0
		~WriteLockClass();													// 0x00139530

		void *Get_Vertex_Array(void) { return Vertices; }
	};

	class AppendLockClass
	{
		VertexBufferClass *VertexBuffer;
		void *Vertices;
		DX8DeviceLock device_lock;
	public:
		AppendLockClass(VertexBufferClass *vertex_buffer, unsigned start_index, unsigned index_range, int flags = 0);	// 0x001395B0
		~AppendLockClass();												// 0x00139530

		void *Get_Vertex_Array(void) { return Vertices; }
	};

protected:
	unsigned int BufferType;			// +0x08
	unsigned short VertexCount;			// +0x0C
	mutable int EngineRefs;				// +0x10
	FVFInfoClass *FVFInfo;				// +0x14
	unsigned char DynamicFlag;			// +0x18 (BFME 2 only; see the notes above)

	friend class WriteLockClass;
	friend class AppendLockClass;
	friend class DX8VertexBufferClass;
	friend class SortingVertexBufferClass;
	friend class DynamicVBAccessClass;
};

typedef char VertexBufferClassSize[(sizeof(VertexBufferClass) == 0x1C) ? 1 : -1];

class DX8VertexBufferClass : public VertexBufferClass
{
public:
	enum UsageType {
		USAGE_DEFAULT = 0,
		USAGE_DYNAMIC = 1,
		USAGE_SOFTWAREPROCESSING = 2,
		USAGE_NPATCHES = 4
	};

	DX8VertexBufferClass(unsigned FVF, unsigned short vertex_count, unsigned usage = USAGE_DEFAULT, unsigned vertex_size = 0);	// 0x0013AC00
	DX8VertexBufferClass(const Vector3 *vertices, const Vector3 *normals, const Vector2 *tex_coords, unsigned short vertex_count, unsigned usage = USAGE_DEFAULT);	// 0x0013AC90
	DX8VertexBufferClass(const Vector3 *vertices, const Vector3 *normals, const Vector4 *diffuse, const Vector2 *tex_coords, unsigned short vertex_count, unsigned usage = USAGE_DEFAULT);	// 0x0013AD20
	DX8VertexBufferClass(const Vector3 *vertices, const Vector4 *diffuse, const Vector2 *tex_coords, unsigned short vertex_count, unsigned usage = USAGE_DEFAULT);	// 0x0013ADB0
	DX8VertexBufferClass(const Vector3 *vertices, const Vector2 *tex_coords, unsigned short vertex_count, unsigned usage = USAGE_DEFAULT);	// 0x0013AE40
	virtual ~DX8VertexBufferClass(void);											// 1: 0x0013AC70 (deleting), body 0x00139720

	IDirect3DVertexBuffer8 *Get_DX8_Vertex_Buffer(void) { return VertexBuffer; }

	void Copy(const Vector3 *loc, unsigned first_vertex, unsigned count);														// 0x00139BF0
	void Copy(const Vector3 *loc, const Vector2 *uv, unsigned first_vertex, unsigned count);									// 0x00139DA0
	void Copy(const Vector3 *loc, const Vector3 *norm, unsigned first_vertex, unsigned count);									// 0x00139FE0
	void Copy(const Vector3 *loc, const Vector3 *norm, const Vector2 *uv, unsigned first_vertex, unsigned count);				// 0x001398C0
	void Copy(const Vector3 *loc, const Vector3 *norm, const Vector2 *uv, const Vector4 *diffuse, unsigned first_vertex, unsigned count);	// 0x0013A260
	void Copy(const Vector3 *loc, const Vector2 *uv, const Vector4 *diffuse, unsigned first_vertex, unsigned count);			// 0x0013A510

	void Create(unsigned usage);					// 0x001397B0

protected:
	IDirect3DVertexBuffer8 *VertexBuffer;		// +0x1C
};

class DynamicVBAccessClass
{
	const FVFInfoClass &FVFInfo;		// +0x00 (an entry of the format table at 0x00DF2AA0)
	unsigned Type;						// +0x04
	unsigned FVFIndex;					// +0x08
	unsigned Flag;						// +0x0C
	unsigned short VertexCount;			// +0x10
	unsigned short VertexBufferOffset;	// +0x12
	VertexBufferClass *VertexBuffer;	// +0x14

	friend class DX8Wrapper;
	friend class SortingRendererClass;

public:
	DynamicVBAccessClass(unsigned type, unsigned fvf_index, unsigned short vertex_count, unsigned flag);	// 0x0013B040
	~DynamicVBAccessClass();					// 0x0013A780

	void Allocate_Sorting_Dynamic_Buffer();		// 0x0013A8E0
	void Allocate_DX8_Dynamic_Buffer();			// 0x0013AEC0

	const FVFInfoClass &FVF_Info() const { return FVFInfo; }
	unsigned Get_Type() const { return Type; }
	unsigned short Get_Vertex_Count() const { return VertexCount; }		// 0x002E6AEE

	static void Init();											// 0x0013A7D0
	static void _Deinit();										// 0x0013A840
	static void _Reset(bool frame_changed);						// 0x0013AB70
	static unsigned short Get_Default_Vertex_Count(void);		// 0x0013ABB0

	class WriteLockClass
	{
		DynamicVBAccessClass *DynamicVBAccess;
		void *Vertices;
		DX8DeviceLock device_lock;
	public:
		WriteLockClass(DynamicVBAccessClass *vb_access);		// 0x0013AA00
		~WriteLockClass();										// 0x0013AB00
		VertexFormatXYZNDUV2 *Get_Formatted_Vertex_Array() { return (VertexFormatXYZNDUV2 *)Vertices; }
	};

	friend class WriteLockClass;
};

class SortingVertexBufferClass : public VertexBufferClass
{
	VertexFormatXYZNDUV2 *VertexBuffer;			// +0x1C

	friend class DX8Wrapper;
	friend class SortingRendererClass;
	friend class VertexBufferClass::WriteLockClass;
	friend class VertexBufferClass::AppendLockClass;
	friend class DynamicVBAccessClass::WriteLockClass;

public:
	SortingVertexBufferClass(unsigned short vertex_count);		// 0x00139660
	virtual ~SortingVertexBufferClass(void);					// 1: 0x0013ABE0 (deleting), body 0x001396D0
};

typedef char DynamicVBAccessClassSize[(sizeof(DynamicVBAccessClass) == 0x18) ? 1 : -1];

#endif
