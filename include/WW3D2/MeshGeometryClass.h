// MeshGeometryClass.h: WW3D's shared mesh geometry (vertices, polygons and
// per-polygon data held in ShareBufferClass arrays, plus cached bounds).
//
// Target facts (meshgeometry.cpp):
// - The constructor 0x00168F30 sets NumRefs 1, MultiListObjectClass's table at
//   +8 (ListNode +0xC cleared), then vftable 0x007D43D0 and the secondary
//   0x007D43CC; zeroes +0x10..+0x18, the byte +0x1C and +0x20..+0x5C; sets the
//   box +0x60..+0x74 to (0,0,0)-(1,1,1), the sphere +0x78..+0x84 to centre 0,
//   radius 1, and +0x88 to 0. SIZE 0x8C (MeshModelClass's members follow).
// - vftable 0x007D43D0, 6 slots: Delete_This, the deleting destructor
//   0x0016D790, Load_W3D 0x0016D810, then the protected Compute_Plane_Equations
//   0x0016A850, Compute_Vertex_Normals 0x0016CFC0 and Compute_Bounds 0x0016A9F0.
// - Get_Name 0x0016A6B0 / Get_User_Text 0x0016A780 return the char array of
//   the buffers at +0x10 / +0x14 (or null); Get_Poly_Surface_Type_Array
//   0x0016A000 returns the array of the buffer at +0x58; Get_Bounding_Box
//   0x001692F0 turns the min/max box into centre and half extent;
//   Compute_Vertex_Normals tests polygon count +0x24 and vertex count +0x28
//   and the flag bit 2 at +0x18.
// Element sizes of the buffers at +0x2C..+0x58 come from Compute_Ram_Size
// 0x0016ADC0 (count times 6, 12, 12, 12, 12, 16, 4, 2, 2, 1); the names from
// the accessors that hand each array out.
#pragma once
#ifndef BFME2_MESHGEOMETRYCLASS_H
#define BFME2_MESHGEOMETRYCLASS_H

#include "WWLib/RefCountClass.h"
#include "WWLib/MultiListObjectClass.h"
#include "WWMath/Vector3.h"
#include "WWMath/Vector4.h"
#include "WW3D2/ShareBufferClass.h"
#include "WWMath/Vector3i.h"

class AABoxClass;
class SphereClass;
class PlaneClass;
class ChunkLoadClass;
class AABTreeClass;
class RayCollisionTestClass;
class OBBoxIntersectionTestClass;

class MeshGeometryClass : public RefCountClass, public MultiListObjectClass
{
public:
	MeshGeometryClass(void);								// 0x00168F30
	MeshGeometryClass(const MeshGeometryClass &that);					// 0x0016A230
	virtual ~MeshGeometryClass(void);							// 1: 0x0016CCC0

	MeshGeometryClass &operator=(const MeshGeometryClass &that);				// 0x00168FE0

	virtual bool Load_W3D(ChunkLoadClass &cload);						// 2: 0x0016D810

	const char *Get_Name(void) const;							// 0x0016A6B0
	void Set_Name(const char *newname);							// 0x0016A6C0
	const char *Get_User_Text(void);							// 0x0016A780
	void Set_User_Text(char *usertext);							// 0x0016A790

	int Get_Polygon_Count(void) const { return PolyCount; }
	int Get_Vertex_Count(void) const { return VertexCount; }
	// Flags bit 4: the vertex normals need recomputing (Get_Vertex_Normal_Array
	// 0x0016AB90 tests it before handing the array out).
	// PRELIT_MASK: MeshModelClass::Needs_Vertex_Normals 0x00171850 tests
	// `byte [this+0x19], 0xE0` (the three prelit bits) before asking the
	// material description.
	// DIRTY_BOUNDS: Compute_Bounds 0x0016A9F0 clears bit 0 after recomputing.
	// DIRTY_PLANES: read_triangles 0x0016B020 clears bit 1 once it has stored
	// every polygon's plane.
	enum { SORT_LEVEL_NONE = 0, SORT = 0x00000010, TWO_SIDED = 0x00000100, SKIN = 0x00000400, DIRTY_BOUNDS = 0x01, DIRTY_PLANES = 0x02, DIRTY_VNORMALS = 0x04, ALIGNED = 0x00000200, ORIENTED = 0x00000800, CAST_SHADOW = 0x00001000, PRELIT_MASK = 0xE000 };
	int Get_Flag(int flag) const { return Flags & flag; }
	void Set_Flag(int flag, bool onoff) { if (onoff) Flags |= flag; else Flags &= ~flag; }
	// The sort level is a signed byte (MeshClass::Get_Sort_Level 0x0014A070 movsx).
	int Get_Sort_Level(void) const { return SortLevel; }
	void Set_Sort_Level(int level) { SortLevel = level; }

	// Inline in retail: Vertex_Split_Table's accessors 0x00144330 / 0x00143AC0 read the buffers' arrays directly.
	const Vector3 *Get_Vertex_Array(void) const { return Vertex->Get_Array(); }
	const TriIndex *Get_Polygon_Array(void) const { return Poly->Get_Array(); }

	void Get_Bounding_Box(AABoxClass *set_box);						// 0x001692F0
	void Get_Bounding_Sphere(SphereClass *set_sphere);					// 0x00169370

	unsigned char *Get_Poly_Surface_Type_Array(void);					// 0x0016A000
	unsigned char Get_Poly_Surface_Type(int poly_index) const;				// 0x0016A220

	void Compute_Plane(int poly_index, PlaneClass *set_plane) const;			// 0x0016ACC0
	const Vector3 *Get_Vertex_Normal_Array(bool unk);					// 0x0016AB90
	// Zero Hour's inline (retail expands it in intersect_obbox_brute_force and cast_ray_brute_force): the polygon planes, recomputed
	// through slot 3 when they are dirty.
	const Vector4 *Get_Plane_Array(void)
	{
		Vector4 *planes = get_planes(true);
		if (planes && Get_Flag(DIRTY_PLANES)) {
			Compute_Plane_Equations(planes);
		}
		return planes;
	}
	Vector3 *Get_Buffer40_Array(void);							// 0x0016ABC0
	Vector3 *Get_Buffer44_Array(void);							// 0x0016ABE0
	int Compute_Ram_Size(void);								// 0x0016ADC0
	void Reset_Geometry(int polycount, int vertcount, bool unk);				// 0x0016A320
	void Scale(const Vector3 &sc);								// 0x0016B3F0
	bool Cast_Ray(RayCollisionTestClass &raytest);						// 0x0016D7B0: the culling tree when there is one, else the brute-force cast
	bool Intersect_OBBox(OBBoxIntersectionTestClass &boxtest);				// 0x0016D7E0

protected:
	virtual void Compute_Plane_Equations(Vector4 *peq);					// 3: 0x0016A850
	virtual void Compute_Vertex_Normals(Vector3 *vnorm, bool unk);				// 4: 0x0016CFC0
	virtual void Compute_Bounds(Vector3 *verts);						// 5: 0x0016A9F0

	void Generate_Culling_Tree(void);							// 0x0016AD10
	bool cast_ray_brute_force(RayCollisionTestClass &raytest);				// 0x0016CE00
	bool intersect_obbox_brute_force(OBBoxIntersectionTestClass &boxtest);			// 0x0016CD30
	unsigned long *get_shade_indices(bool create);						// 0x0016A010
	unsigned short *get_bone_links(bool create);						// 0x0016A0C0
	unsigned short *get_buffer54(int count);						// 0x0016A170 (created with the given count)
	Vector3 *get_vert_normals(bool create);							// 0x0016AAE0
	Vector4 *get_planes(bool create);							// 0x0016AC00

	// W3D chunk readers (Load_W3D 0x0016D810 / read_chunks 0x0016D340).
	bool read_vertices(ChunkLoadClass &cload, bool second_set);			// 0x0016AE70
	bool read_vertex_normals(ChunkLoadClass &cload, bool create);			// 0x0016AF40
	bool read_triangles(ChunkLoadClass &cload);					// 0x0016B020
	bool read_user_text(ChunkLoadClass &cload);					// 0x0016B0E0
	bool read_vertex_shade_indices(ChunkLoadClass &cload);				// 0x0016B3C0
	bool read_vertex_influences(ChunkLoadClass &cload);				// 0x0016B1A0
	bool read_aabtree(ChunkLoadClass &cload);					// 0x001693A0
	bool rva0016AF80(ChunkLoadClass &cload);					// 0x0016AF80 (chunk 0x60, MeshModelClass::read_chunks)
	bool rva0016AFD0(ChunkLoadClass &cload);					// 0x0016AFD0 (chunk 0x61)
	bool read_chunks(ChunkLoadClass &cload);					// 0x0016D340

	struct BoneLinkPair
	{
		unsigned short First;
		unsigned short Second;
	};

	ShareBufferClass<char> *MeshName;		// +0x10
	ShareBufferClass<char> *UserText;		// +0x14
	int Flags;					// +0x18
	char SortLevel;					// +0x1C
	unsigned int W3dAttributes;			// +0x20 (read by MeshClass 0x00149960)
	int PolyCount;					// +0x24
	int VertexCount;				// +0x28
	ShareBufferClass<TriIndex> *Poly;		// +0x2C vertex index triples (Compute_Plane)
	ShareBufferClass<Vector3> *Vertex;		// +0x30 vertex positions (Compute_Plane)
	ShareBufferClass<Vector3> *SecondaryVertex;		// +0x34 optional second set of vertex positions
	ShareBufferClass<Vector3> *VertexNorm;		// +0x38 normals read by get_vert_normals(false) 0x0016AAE0
	ShareBufferClass<Vector3> *VertexNormWork;	// +0x3C normals get_vert_normals(true) allocates and zeroes (tag "MeshGeometryClass::VertexNorm")
	ShareBufferClass<Vector3> *VertexTangents;		// +0x40 vertex tangents
	ShareBufferClass<Vector3> *VertexBinormals;		// +0x44 vertex binormals
	ShareBufferClass<Vector4> *PlaneEq;		// +0x48 polygon planes (get_planes 0x0016AC00, 16-byte elements)
	ShareBufferClass<unsigned long> *VertexShadeIdx;	// +0x4C (get_shade_indices 0x0016A010)
	ShareBufferClass<unsigned short> *VertexBoneLink;	// +0x50 (get_bone_links 0x0016A0C0)
	ShareBufferClass<unsigned short> *BoneLinkSpans;	// +0x54 (first bone link and vertex count for each consecutive run)
	ShareBufferClass<unsigned char> *PolySurfaceType;	// +0x58
	RefCountClass *Buffer5C;			// +0x5C
	Vector3 BoundBoxMin;				// +0x60
	Vector3 BoundBoxMax;				// +0x6C
	Vector3 BoundSphereCenter;			// +0x78
	float BoundSphereRadius;			// +0x84
	AABTreeClass *CullTree;				// +0x88
};

typedef char MeshGeometryClassSize[(sizeof(MeshGeometryClass) == 0x8C) ? 1 : -1];

#endif
