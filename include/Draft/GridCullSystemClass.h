// GridCullSystemClass.h: the uniform-grid culling system.
//
// Target facts (retail gridcull.cpp and its constructor 0x0071CBE0):
//  - MinCellSize at +0x08, MaxObjExtent at +0x14, TerminationCellCount at
//    +0x18 (0x4000), Origin at +0x1C, CellDim at +0x28, Cells at +0x34,
//    NoGridList at +0x38, ObjCount at +0x3C, CellCount at +0x40 and the
//    statistics at +0x58 (NodeCount, NodesAccepted, NodesTriviallyAccepted,
//    NodesRejected).
//  - total_cell_count 0x00719FF0 is CellCount[2]*CellCount[1]*CellCount[0];
//    map_indices_to_address 0x00719FD0 is x + CellCount[0]*(y + CellCount[1]*z).
//  - VolumeStruct::Is_Empty 0x0071AB00 compares Max[i] at +0xC.. with Min[i].
//  - A cullable's link is a GridLinkClass: CullLinkClass's system pointer at +4,
//    then GridAddress +0x08, Prev +0x0C and Next +0x10.
#pragma once
#ifndef BFME2_GRIDCULLSYSTEMCLASS_H
#define BFME2_GRIDCULLSYSTEMCLASS_H

#include "Draft/CullSystemClass.h"
#include "Draft/CullLinkClass.h"
#include "Draft/CullableClass.h"
#include "WWMath/Vector3.h"
#include "WWLib/mempool.h"

class AABoxClass;
class GridCullSystemClass;
class ChunkLoadClass;
class ChunkSaveClass;
class CullableClass;
class FrustumClass;
class OBBoxClass;

class GridLinkClass : public CullLinkClass, public AutoPoolClass<GridLinkClass, 256>
{
public:
	// Zero Hour's inline constructor: retail's Add_Object_Internal 0x0071CB90 allocates from the pool at 0x00E1D05C
	// (block 0x1404 = 0x14 * 256 + 4) and stores vftable 0x00CEFCAC, the system at +4, -1 at +8, zero at +0xC/+0x10.
	GridLinkClass(GridCullSystemClass *system) : CullLinkClass((CullSystemClass *)system), GridAddress(-1), Prev(0), Next(0) {}
	virtual ~GridLinkClass(void);

	int GridAddress;		// +0x08
	CullableClass *Prev;		// +0x0C
	CullableClass *Next;		// +0x10
};

// Walks one grid cell's (or the no-grid) list through each link's Next; the
// leaf collectors 0x0071B260..0x0071B400 test the current object before every
// step, as Next does.
class GridListIterator
{
public:
	GridListIterator(CullableClass *head) { First(head); }

	void First(CullableClass *head) { Head = head; CurObj = head; }
	void First(void) { CurObj = Head; }
	void Next(void) { if (CurObj) CurObj = ((GridLinkClass *)CurObj->CullLink)->Next; }
	bool Is_Done(void) { return CurObj == NULL; }
	CullableClass *Peek_Obj(void) { return CurObj; }

private:
	CullableClass *Head;
	CullableClass *CurObj;
};

class GridCullSystemClass : public CullSystemClass
{
public:
	struct VolumeStruct
	{
		int Min[3];		// +0x00
		int Max[3];		// +0x0C

		bool Is_Empty(void) const;
	};

	struct Statistics
	{
		unsigned int NodeCount;			// +0x00
		unsigned int NodesAccepted;		// +0x04
		unsigned int NodesTriviallyAccepted;	// +0x08
		unsigned int NodesRejected;		// +0x0C
	};

	GridCullSystemClass(void);
	virtual ~GridCullSystemClass(void);
	virtual void Collect_Objects(FrustumClass const &);
	virtual void Collect_Objects(OBBoxClass const &);
	virtual void Collect_Objects(AABoxClass const &);
	virtual void Collect_Objects(Vector3 const &);
	virtual void Update_Culling(CullableClass *);
	virtual void Re_Partition(Vector3 const &, Vector3 const &, float);
	virtual void Load(ChunkLoadClass &);
	virtual void Save(ChunkSaveClass &);
	virtual int Get_Object_Count(void) const;

protected:
	int map_indices_to_address(int x, int y, int z);
	bool map_point_to_cell(Vector3 const &, int &, int &, int &);
	bool map_point_to_address(Vector3 const &, int &);
	void clamp_indices_to_grid(int *x, int *y, int *z);
	int total_cell_count(void);
	void init_volume(const Vector3 &, const Vector3 &, VolumeStruct *);
	void init_volume(const AABoxClass &, VolumeStruct *);
	void init_volume(const OBBoxClass &, VolumeStruct *);
	void init_volume(const FrustumClass &, VolumeStruct *);

	void Add_Object_Internal(CullableClass *obj);
	void Remove_Object_Internal(CullableClass *obj);
	void link_object(CullableClass *obj);
	void link_object(CullableClass *obj, int address);
	void unlink_object(CullableClass *obj);
	void link_object_to_list(CullableClass **head, CullableClass *obj);
	void unlink_object_from_list(CullableClass **head, CullableClass *obj);
	void collect_objects_in_leaf(Vector3 const &, CullableClass *head);
	void collect_objects_in_leaf(AABoxClass const &, CullableClass *head);
	void collect_objects_in_leaf(OBBoxClass const &, CullableClass *head);
	void collect_objects_in_leaf(FrustumClass const &, CullableClass *head);
	void Collect_And_Unlink_All(void);

public:
	void Reset_Statistics(void);
	const Statistics &Get_Statistics(void);		// 0x0071AE40 (the map has a placeholder row here; Zero Hour's Get_Statistics)

	Vector3 MinCellSize;		// +0x08
	Real MaxObjExtent;		// +0x14
	int TerminationCellCount;	// +0x18
	Vector3 Origin;			// +0x1C
	Vector3 CellDim;		// +0x28
	Vector3 OOCellDim;		// +0x34 one over CellDim (read by map_point_to_address 0x0071B030)
	unsigned int CellCount[3];	// +0x40
	CullableClass **Cells;		// +0x4C
	CullableClass *NoGridList;	// +0x50
	int ObjCount;			// +0x54
	Statistics Stats;		// +0x58
};

#endif
