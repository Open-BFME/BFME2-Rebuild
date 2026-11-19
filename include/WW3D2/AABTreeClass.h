// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// AABTreeClass.h: a mesh's axis-aligned bounding box culling tree (aabtree.cpp, retail 0x00197B40..0x00198800).
// Target facts: SIZE 0x1C; the default constructor 0x00197B40 stores NumRefs = 1, the vftable 0x007D5F00 (Delete_This, the
// deleting destructor 0x00198170) and zeroes +0x08..+0x18; Compute_Ram_Size 0x00197CE0 is `(PolyCount + NodeCount * 8) * 4 + 0x1C`
// (CullNodeStruct is 0x20 bytes). Class layout, member names and the CullNodeStruct accessors from Zero Hour's aabtree.h;
// retail has no W3DMPO base (the deleting destructor calls the global operator delete 0x0002FD60). Zero Hour's APT generators and
// Cast_AABox/Cast_OBBox/semi-infinite-ray members are not in retail's map (dead-stripped) and are not declared.
#pragma once
#ifndef BFME2_AABTREECLASS_H
#define BFME2_AABTREECLASS_H

#include "WWLib/RefCountClass.h"
#include "WWMath/Vector3.h"
#include "WW3D2/AABTreeBuilderClass.h"

#define AABTREE_LEAF_FLAG 0x80000000

class ChunkLoadClass;
class MeshClass;
class MeshGeometryClass;
class RayCollisionTestClass;
class OBBoxIntersectionTestClass;

class AABTreeClass : public RefCountClass
{
public:
	AABTreeClass(void);													// 0x00197B40
	AABTreeClass(AABTreeBuilderClass *builder);							// 0x00197E90
	AABTreeClass(const AABTreeClass &that);								// 0x00198190
	~AABTreeClass(void);												// 0x00197F30 (the deleting destructor is 0x00198170)

	void Load_W3D(ChunkLoadClass &cload);								// 0x00198090
	void Scale(float scale);											// 0x00197BB0

	int Get_Node_Count(void) { return NodeCount; }
	int Get_Poly_Count(void) { return PolyCount; }
	int Compute_Ram_Size(void);											// 0x00197CE0: inline in Zero Hour; retail's copy is this row

	// Inline in Zero Hour and in retail: MeshGeometryClass::Cast_Ray 0x0016D7B0 and Intersect_OBBox 0x0016D7E0 pass Nodes[0] to the
	// recursive tests themselves (0x00198630, 0x00198730); 0x00168C20 is Intersect_OBBox's out-of-line copy.
	bool Cast_Ray(RayCollisionTestClass &raytest) { return Cast_Ray_Recursive(&(Nodes[0]), raytest); }
	bool Intersect_OBBox(OBBoxIntersectionTestClass &boxtest) { return Intersect_OBBox_Recursive(&(Nodes[0]), boxtest); }
	void Set_Mesh(MeshGeometryClass *mesh);							// out of line in retail: the folded store body (0x004FA930 / 0x004FC2A0)

private:
	AABTreeClass &operator=(const AABTreeClass &that);					// 0x00197F80
	void Read_Poly_Indices(ChunkLoadClass &cload);						// 0x00197D00
	void Read_Nodes(ChunkLoadClass &cload);								// 0x00197D20

	void Build_Tree_Recursive(AABTreeBuilderClass::CullNodeStruct *node, int &curpolyindex);	// 0x00197C30
	void Reset(void);													// 0x00197B70
	void Update_Bounding_Boxes(void);
	void Update_Min_Max(int index, Vector3 &min, Vector3 &max);			// 0x00198580

	// The culling tree is built from an array of these (Zero Hour's remark): the extents of an axis-aligned box, and either
	// two child indices or a polygon range, told apart by the MSB of FrontOrPoly0.
	struct CullNodeStruct
	{
		Vector3 Min;
		Vector3 Max;

		unsigned long FrontOrPoly0;
		unsigned long BackOrPolyCount;

		bool Is_Leaf(void) { return (FrontOrPoly0 & AABTREE_LEAF_FLAG) != 0; }
		int Get_Back_Child(void) { return BackOrPolyCount; }
		int Get_Front_Child(void) { return FrontOrPoly0; }
		int Get_Poly0(void) { return FrontOrPoly0 & ~AABTREE_LEAF_FLAG; }
		int Get_Poly_Count(void) { return BackOrPolyCount; }

		void Set_Front_Child(unsigned long index) { FrontOrPoly0 = index; }
		void Set_Back_Child(unsigned long index) { BackOrPolyCount = index; }
		void Set_Poly0(unsigned long index) { FrontOrPoly0 = (index | AABTREE_LEAF_FLAG); }
		void Set_Poly_Count(unsigned long count) { BackOrPolyCount = count; }
	};

	bool Cast_Ray_Recursive(CullNodeStruct *node, RayCollisionTestClass &raytest);						// 0x00198630
	bool Intersect_OBBox_Recursive(CullNodeStruct *node, OBBoxIntersectionTestClass &boxtest);			// 0x00198730
	bool Cast_Ray_To_Polys(CullNodeStruct *node, RayCollisionTestClass &raytest);						// 0x001981F0
	bool Intersect_OBBox_With_Polys(CullNodeStruct *node, OBBoxIntersectionTestClass &boxtest);			// 0x001983D0
	void Update_Bounding_Boxes_Recursive(CullNodeStruct *node);											// 0x00198800

	int NodeCount;						// +0x08 number of nodes in the tree
	CullNodeStruct *Nodes;				// +0x0C array of nodes
	int PolyCount;						// +0x10 number of polygons in the parent mesh (and of indices in PolyIndices)
	unsigned long *PolyIndices;			// +0x14 linear array of polygon indices the nodes index into
	MeshGeometryClass *Mesh;			// +0x18 the parent mesh (not counted: the tree is a member of the mesh)

	friend class MeshClass;
	friend class MeshGeometryClass;
	friend class AABTreeBuilderClass;
};

#endif
