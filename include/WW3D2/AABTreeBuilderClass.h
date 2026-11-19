// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// AABTreeBuilderClass.h: WW3D2's axis-aligned bounding box tree builder (aabtreebuilder.cpp, retail 0x00198B70..0x00199E30).
// Class list, member order and nested structures from Zero Hour's aabtreebuilder.h. Retail's Build_AABTree takes
// Vector3i16 triangles (map ?Build_AABTree@AABTreeBuilderClass@@QAEXHPAVVector3i16@@HPAVVector3@@@Z, 0x00199CD0) and Vector3i
// triangles (0x00199E30). The tree class (AABTreeClass.h) reads the builder's nodes (friend), so the node structure is private.
#pragma once
#ifndef BFME2_AABTREEBUILDERCLASS_H
#define BFME2_AABTREEBUILDERCLASS_H

#include <float.h>
#include <stddef.h>
#include "WWMath/AAPlaneClass.h"
#include "WWMath/Vector3.h"
#include "WWMath/Vector3i.h"

class AABTreeClass;
class ChunkSaveClass;
struct W3dMeshAABTreeNode;

class AABTreeBuilderClass
{
public:
	AABTreeBuilderClass(void);
	~AABTreeBuilderClass(void);

	void Build_AABTree(int polycount, TriIndex *polys, int vertcount, Vector3 *verts);		// 0x00199CD0
	void Build_AABTree(int polycount, Vector3i *polys, int vertcount, Vector3 *verts);		// 0x00199E30
	void Export(ChunkSaveClass &csave);														// 0x00199680

	int Node_Count(void);																	// 0x00199640
	int Poly_Count(void);																	// 0x00144010 (out of line; the identical getter bodies are one folded copy)

	enum
	{
		MIN_POLYS_PER_NODE = 4,
		SMALL_VERTEX = -100000,
		BIG_VERTEX = 100000
	};

private:
	// The node used while building; it is much more wasteful than AABTreeClass's streamlined node (Zero Hour's remark).
	struct CullNodeStruct
	{
		CullNodeStruct(void) : Index(0), Min(0, 0, 0), Max(0, 0, 0), Front(NULL), Back(NULL), PolyCount(0), PolyIndices(NULL) {}
		~CullNodeStruct(void)
		{
			if (Front) {
				delete Front;
			}
			if (Back) {
				delete Back;
			}
			if (PolyIndices) {
				delete[] PolyIndices;
			}
		}

		int Index;
		Vector3 Min;
		Vector3 Max;
		CullNodeStruct *Front;
		CullNodeStruct *Back;
		int PolyCount;
		int *PolyIndices;
	};

	// The outcome of evaluating one partition.
	struct SplitChoiceStruct
	{
		SplitChoiceStruct(void) :
			Cost(FLT_MAX),
			FrontCount(0),
			BackCount(0),
			BMin(BIG_VERTEX, BIG_VERTEX, BIG_VERTEX),
			BMax(SMALL_VERTEX, SMALL_VERTEX, SMALL_VERTEX),
			FMin(BIG_VERTEX, BIG_VERTEX, BIG_VERTEX),
			FMax(SMALL_VERTEX, SMALL_VERTEX, SMALL_VERTEX),
			Plane(AAPlaneClass::XNORMAL, 0)
		{
		}

		float Cost;
		int FrontCount;
		int BackCount;
		Vector3 BMin;
		Vector3 BMax;
		Vector3 FMin;
		Vector3 FMax;
		AAPlaneClass Plane;
	};

	struct SplitArraysStruct
	{
		SplitArraysStruct(void) : FrontCount(0), BackCount(0), FrontPolys(NULL), BackPolys(NULL) {}

		int FrontCount;
		int BackCount;
		int *FrontPolys;
		int *BackPolys;
	};

	enum OverlapType
	{
		POS = 0x01,
		NEG = 0x02,
		ON = 0x04,
		BOTH = 0x08,
		OUTSIDE = POS,
		INSIDE = NEG,
		OVERLAPPED = BOTH,
		FRONT = POS,
		BACK = NEG,
	};

	void Reset();																			// 0x00198B70
	void Build_Tree(CullNodeStruct *node, int polycount, int *polyindices);				// 0x00199AE0
	SplitChoiceStruct Select_Splitting_Plane(int polycount, int *polyindices);				// 0x00199810
	SplitChoiceStruct Compute_Plane_Score(int polycont, int *polyindices, const AAPlaneClass &plane);	// 0x00199280
	void Split_Polys(int polycount, int *polyindices, const SplitChoiceStruct &sc, SplitArraysStruct *arrays);	// 0x00198D40
	OverlapType Which_Side(const AAPlaneClass &plane, int poly_index);						// 0x00198C60
	void Compute_Bounding_Box(CullNodeStruct *node);										// 0x001994E0
	int Assign_Index(CullNodeStruct *node, int index);										// 0x00198E00
	int Node_Count_Recursive(CullNodeStruct *node, int curcount);							// 0x00198E40
	void Update_Min(int poly_index, Vector3 &set_min);										// 0x00198E80
	void Update_Max(int poly_index, Vector3 &set_max);										// 0x00198F60
	void Update_Min_Max(int poly_index, Vector3 &set_min, Vector3 &set_max);				// 0x00199040

	void Build_W3D_AABTree_Recursive(CullNodeStruct *node, W3dMeshAABTreeNode *w3dnodes, unsigned long *poly_indices, int &cur_node, int &cur_poly);	// 0x001991B0

	CullNodeStruct *Root;				// +0x00
	int CurPolyIndex;					// +0x04
	int PolyCount;						// +0x08
	TriIndex *Polys;					// +0x0C
	int VertCount;						// +0x10
	Vector3 *Verts;						// +0x14

	friend class AABTreeClass;
};

#endif
