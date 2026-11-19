// cl: /O2 /D_STLP_USE_STATIC_LIB
// stlport46
// bfmealloc_alloconly
// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
#include <math.h>
#include <new>
#include <string.h>
#include "Draft/GridCullSystemClass.h"
#include "Draft/CullableClass.h"
#include "WWMath/CollisionMath.h"
#include "WWMath/FrustumClass.h"
#include "WWMath/OBBoxClass.h"
#include "WWMath/IOGridParametersStruct.h"
#include "WWLib/chunkio.h"
#include "WWLib/mempool_block.h"

// The pool for the cullables' grid links (retail 0x00E1D05C).
DEFINE_AUTO_POOL(GridLinkClass, 256);
// Tier A; object=O00719E60 evidence=W flags=align=A
// Extent is a hypothesis; only Object VERIFIED establishes recovery.
// Edges: left=unknown-rows right=unknown-rows
// The grid's index arithmetic, its object lists and the collection walk.
// CellCount is the three grid dimensions at +0x40; addresses are
// x + CellCount[0]*(y + CellCount[1]*z).
/*
** Current version of the file format
*/
const unsigned long GRID_CURRENT_VERSION = 0x00010000;

/*
** Chunk Id's used by the grid cull system to save itself into a file
*/
enum
{
	GRID_CHUNK_VERSION					= 0x00000001,	// version wrapper, contains 32bit version #
	GRID_CHUNK_PARAMETERS				= 0x00000100,	// parameters for the grid cull system
};

// FUNCTION: BFME2 0x00719E60 size=84 name=?clamp_indices_to_grid@GridCullSystemClass@@IAEXPAH00@Z evidence=N:ledger

__forceinline void GridCullSystemClass::clamp_indices_to_grid(int *x, int *y, int *z)
{
	if (*x < 0)
		*x = 0;
	if (*x >= (int)CellCount[0])
		*x = CellCount[0] - 1;

	if (*y < 0)
		*y = 0;
	if (*y >= (int)CellCount[1])
		*y = CellCount[1] - 1;

	if (*z < 0)
		*z = 0;
	if (*z >= (int)CellCount[2])
		*z = CellCount[2] - 1;
}

// FUNCTION: BFME2 0x00719EC0 size=271 name=?map_point_to_cell@GridCullSystemClass@@IAE_NABVVector3@@AAH11@Z evidence=N:ledger

__forceinline bool GridCullSystemClass::map_point_to_cell(const Vector3 &point, int &x, int &y, int &z)
{
	Vector3 delta(point.X - Origin.X, point.Y - Origin.Y, point.Z - Origin.Z);
	x = (int)floor(delta.X * OOCellDim.X);
	y = (int)floor(delta.Y * OOCellDim.Y);
	z = (int)floor(delta.Z * OOCellDim.Z);

	if (x >= 0 && y >= 0 && z >= 0 &&
		x < (int)CellCount[0] && y < (int)CellCount[1] && z < (int)CellCount[2])
		return true;
	return false;
}

// FUNCTION: BFME2 0x00719FD0 size=23 name=?map_indices_to_address@GridCullSystemClass@@IAEHHHH@Z evidence=N:ledger

int GridCullSystemClass::map_indices_to_address(int x, int y, int z)
{
	return x + y * CellCount[0] + z * CellCount[0] * CellCount[1];
}

// FUNCTION: BFME2 0x00719FF0 size=12 name=?total_cell_count@GridCullSystemClass@@IAEHXZ evidence=N:ledger

int GridCullSystemClass::total_cell_count(void)
{
	return CellCount[2] * CellCount[1] * CellCount[0];
}

// FUNCTION: BFME2 0x0071A000 size=665 name=?init_volume@GridCullSystemClass@@IAEXABVVector3@@0PAUVolumeStruct@1@@Z evidence=N:ledger

__forceinline void GridCullSystemClass::init_volume(const Vector3 &bound_min, const Vector3 &bound_max, VolumeStruct *volume)
{
	Vector3 grid_min(bound_min.X, bound_min.Y, bound_min.Z);
	grid_min.X -= MaxObjExtent;
	grid_min.Y -= MaxObjExtent;
	grid_min.Z -= MaxObjExtent;

	Vector3 grid_max(bound_max.X, bound_max.Y, bound_max.Z);
	grid_max.X += MaxObjExtent;
	grid_max.Y += MaxObjExtent;
	grid_max.Z += MaxObjExtent;

	map_point_to_cell(grid_min, volume->Min[0], volume->Min[1], volume->Min[2]);
	map_point_to_cell(grid_max, volume->Max[0], volume->Max[1], volume->Max[2]);
	clamp_indices_to_grid(&volume->Min[0], &volume->Min[1], &volume->Min[2]);
	clamp_indices_to_grid(&volume->Max[0], &volume->Max[1], &volume->Max[2]);
	volume->Max[0]++;
	volume->Max[1]++;
	volume->Max[2]++;
}

// FUNCTION: BFME2 0x0071A2A0 size=704 name=?init_volume@GridCullSystemClass@@IAEXABVAABoxClass@@PAUVolumeStruct@1@@Z evidence=N:ledger
__forceinline void GridCullSystemClass::init_volume(const AABoxClass &box, VolumeStruct *volume)
{
	init_volume(box.Center - box.Extent, box.Center + box.Extent, volume);
}

// FUNCTION: BFME2 0x0071A560 size=730 name=?init_volume@GridCullSystemClass@@IAEXABVOBBoxClass@@PAUVolumeStruct@1@@Z evidence=N:ledger
__forceinline void GridCullSystemClass::init_volume(const OBBoxClass &box, VolumeStruct *volume)
{
	Vector3 aaextent;
	box.Compute_Axis_Aligned_Extent(&aaextent);
	init_volume(box.Center - aaextent, box.Center + aaextent, volume);
}

// FUNCTION: BFME2 0x0071A840 size=694 name=?init_volume@GridCullSystemClass@@IAEXABVFrustumClass@@PAUVolumeStruct@1@@Z evidence=N:ledger
__forceinline void GridCullSystemClass::init_volume(const FrustumClass &frustum, VolumeStruct *volume)
{
	init_volume(frustum.Get_Bound_Min(), frustum.Get_Bound_Max(), volume);
}

// FUNCTION: BFME2 0x0071AB00 size=38 name=?Is_Empty@VolumeStruct@GridCullSystemClass@@QBE_NXZ evidence=N:ledger

bool GridCullSystemClass::VolumeStruct::Is_Empty(void) const
{
	return Max[0] - Min[0] <= 0 || Max[1] - Min[1] <= 0 || Max[2] - Min[2] <= 0;
}

// FUNCTION: BFME2 0x0071AB30 size=124 name=?Overlap_Test@CollisionMath@@SA?AW4OverlapType@1@ABVAABoxClass@@ABVVector3@@@Z evidence=N:ledger
CollisionMath::OverlapType CollisionMath::Overlap_Test(const AABoxClass &box, const Vector3 &point)
{
	if (WWMath::Fabs(point.X - box.Center.X) > box.Extent.X) return POS;
	if (WWMath::Fabs(point.Y - box.Center.Y) > box.Extent.Y) return POS;
	if (WWMath::Fabs(point.Z - box.Center.Z) > box.Extent.Z) return POS;
	return NEG;
}

// FUNCTION: BFME2 0x0071ABB0 size=262 name=?Overlap_Test@CollisionMath@@SA?AW4OverlapType@1@ABVAABoxClass@@0@Z evidence=N:ledger
CollisionMath::OverlapType CollisionMath::Overlap_Test(const AABoxClass &box, const AABoxClass &box2)
{
	Vector3 dc(box2.Center.X - box.Center.X, box2.Center.Y - box.Center.Y, box2.Center.Z - box.Center.Z);

	if (box.Extent.X + box2.Extent.X < WWMath::Fabs(dc.X)) return POS;
	if (box.Extent.Y + box2.Extent.Y < WWMath::Fabs(dc.Y)) return POS;
	if (box.Extent.Z + box2.Extent.Z < WWMath::Fabs(dc.Z)) return POS;

	if ((dc.X + box2.Extent.X <= box.Extent.X) &&
		(dc.Y + box2.Extent.Y <= box.Extent.Y) &&
		(dc.Z + box2.Extent.Z <= box.Extent.Z) &&
		(dc.X - box2.Extent.X >= -box.Extent.X) &&
		(dc.Y - box2.Extent.Y >= -box.Extent.Y) &&
		(dc.Z - box2.Extent.Z >= -box.Extent.Z))
	{
		return NEG;
	}
	return BOTH;
}

// FUNCTION: BFME2 0x0071ACC0 size=7 name=?get_next_object@@YAPAVCullableClass@@PAV1@@Z evidence=N:ledger

static CullableClass *get_next_object(CullableClass *obj)
{
	return ((GridLinkClass *)obj->CullLink)->Next;
}

// FUNCTION: BFME2 0x0071AD00 size=40 name=??1GridCullSystemClass@@UAE@XZ evidence=N:ledger

GridCullSystemClass::~GridCullSystemClass(void)
{
	if (Cells != NULL) {
		delete Cells;
		Cells = NULL;
	}
}

// FUNCTION: BFME2 0x0071AD30 size=236 name=?Save@GridCullSystemClass@@UAEXAAVChunkSaveClass@@@Z evidence=N:ledger

void GridCullSystemClass::Save(ChunkSaveClass &csave)
{
	/*
	** write the version chunk
	*/
	unsigned long version = GRID_CURRENT_VERSION;
	csave.Begin_Chunk(GRID_CHUNK_VERSION);
	csave.Write(&version, sizeof(version));
	csave.End_Chunk();

	/*
	** write the grid parameters
	*/
	IOGridParametersStruct params;
	memset(&params, 0, sizeof(params));
	params.CellCount[0] = CellCount[0];
	params.CellCount[1] = CellCount[1];
	params.CellCount[2] = CellCount[2];
	params.CellDim.X = CellDim.X;
	params.CellDim.Y = CellDim.Y;
	params.CellDim.Z = CellDim.Z;
	params.MaxObjExtent = MaxObjExtent;
	params.MinCellSize.X = MinCellSize.X;
	params.MinCellSize.Y = MinCellSize.Y;
	params.MinCellSize.Z = MinCellSize.Z;
	params.Origin.X = Origin.X;
	params.Origin.Y = Origin.Y;
	params.Origin.Z = Origin.Z;

	csave.Begin_Chunk(GRID_CHUNK_PARAMETERS);
	csave.Write(&params, sizeof(params));
	csave.End_Chunk();
}

// FUNCTION: BFME2 0x0071AE20 size=30 name=?Reset_Statistics@GridCullSystemClass@@QAEXXZ evidence=N:ledger

void GridCullSystemClass::Reset_Statistics(void)
{
	// number of (virtual) nodes = 2n-1
	Stats.NodeCount = 2 * (CellCount[0] * CellCount[1] * CellCount[2]) - 1;
	Stats.NodesAccepted = 0;
	Stats.NodesTriviallyAccepted = 0;
	Stats.NodesRejected = 0;
}

// FUNCTION: BFME2 0x0071AE40 size=4 name=?get@Rva0071AE40LeaField@@QBEPAXXZ evidence=?:placeholder
const GridCullSystemClass::Statistics &GridCullSystemClass::Get_Statistics(void)
{
	return Stats;
}

// FUNCTION: BFME2 0x0071AE50 size=42 name=?link_object_to_list@GridCullSystemClass@@IAEXPAPAVCullableClass@@PAV2@@Z evidence=N:ledger

void GridCullSystemClass::link_object_to_list(CullableClass **head, CullableClass *obj)
{
	GridLinkClass *link = (GridLinkClass *)obj->CullLink;

	link->Next = *head;
	link->Prev = NULL;

	if (link->Next != NULL)
	{
		GridLinkClass *next_link = (GridLinkClass *)link->Next->CullLink;
		next_link->Prev = obj;
	}

	*head = obj;
}

// FUNCTION: BFME2 0x0071AE80 size=65 name=?unlink_object_from_list@GridCullSystemClass@@IAEXPAPAVCullableClass@@PAV2@@Z evidence=N:ledger

void GridCullSystemClass::unlink_object_from_list(CullableClass **head, CullableClass *obj)
{
	GridLinkClass *link = (GridLinkClass *)obj->CullLink;

	if (obj == *head)
		*head = link->Next;

	if (link->Prev)
	{
		GridLinkClass *prev_link = (GridLinkClass *)link->Prev->CullLink;
		prev_link->Next = link->Next;
	}

	if (link->Next)
	{
		GridLinkClass *next_link = (GridLinkClass *)link->Next->CullLink;
		next_link->Prev = link->Prev;
	}

	link->Prev = NULL;
	link->Next = NULL;
}

// FUNCTION: BFME2 0x0071AFA0 size=144 name=?Contains@AABoxClass@@QBE_NABVVector3@@@Z evidence=N:ledger

bool AABoxClass::Contains(const Vector3 &point) const
{
	return CollisionMath::Overlap_Test(*this, point) == CollisionMath::INSIDE;
}

// FUNCTION: BFME2 0x0071B030 size=318 name=?map_point_to_address@GridCullSystemClass@@IAE_NABVVector3@@AAH@Z evidence=N:ledger

__forceinline bool GridCullSystemClass::map_point_to_address(const Vector3 &point, int &address)
{
	int ix, iy, iz;
	if (map_point_to_cell(point, ix, iy, iz))
	{
		address = map_indices_to_address(ix, iy, iz);
		return true;
	}

	address = -1;
	return false;
}

// FUNCTION: BFME2 0x0071B190 size=112 name=?link_object@GridCullSystemClass@@IAEXPAVCullableClass@@H@Z evidence=N:ledger

void GridCullSystemClass::link_object(CullableClass *obj, int address)
{
	GridLinkClass *link = (GridLinkClass *)obj->CullLink;

	const AABoxClass &box = obj->CullBox;
	if ((box.Extent.X > MaxObjExtent) ||
		(box.Extent.Y > MaxObjExtent) ||
		(box.Extent.Z > MaxObjExtent) ||
		(address == -1))
	{
		link->GridAddress = -1;
		link_object_to_list(&NoGridList, obj);
	}
	else
	{
		link->GridAddress = address;
		link_object_to_list(&(Cells[address]), obj);
	}
}

// FUNCTION: BFME2 0x0071B200 size=89 name=?unlink_object@GridCullSystemClass@@IAEXPAVCullableClass@@@Z evidence=N:ledger

void GridCullSystemClass::unlink_object(CullableClass *obj)
{
	GridLinkClass *link = (GridLinkClass *)obj->CullLink;

	if (link->GridAddress == -1)
		unlink_object_from_list(&NoGridList, obj);
	else
		unlink_object_from_list(&(Cells[link->GridAddress]), obj);
}

// FUNCTION: BFME2 0x0071B260 size=157 name=?collect_objects_in_leaf@GridCullSystemClass@@IAEXABVVector3@@PAVCullableClass@@@Z evidence=N:ledger

void GridCullSystemClass::collect_objects_in_leaf(Vector3 const &point, CullableClass *head)
{
	if (head != NULL)
	{
		for (GridListIterator it(head); !it.Is_Done(); it.Next())
		{
			CullableClass *obj = it.Peek_Obj();
			if (obj->CullBox.Contains(point) == true)
				Add_To_Collection(obj);
		}
	}
}

// FUNCTION: BFME2 0x0071B300 size=188 name=?collect_objects_in_leaf@GridCullSystemClass@@IAEXABVAABoxClass@@PAVCullableClass@@@Z evidence=N:ledger
void GridCullSystemClass::collect_objects_in_leaf(const AABoxClass &box, CullableClass *head)
{
	if (head != NULL)
	{
		for (GridListIterator it(head); !it.Is_Done(); it.Next())
		{
			CullableClass *obj = it.Peek_Obj();
			if (CollisionMath::Overlap_Test(box, obj->CullBox) != CollisionMath::OUTSIDE)
				Add_To_Collection(obj);
		}
	}
}

// FUNCTION: BFME2 0x0071B3C0 size=63 name=?collect_objects_in_leaf@GridCullSystemClass@@IAEXABVOBBoxClass@@PAVCullableClass@@@Z evidence=N:ledger
void GridCullSystemClass::collect_objects_in_leaf(const OBBoxClass &box, CullableClass *head)
{
	if (head != NULL)
	{
		for (GridListIterator it(head); !it.Is_Done(); it.Next())
		{
			CullableClass *obj = it.Peek_Obj();
			if (CollisionMath::Overlap_Test(box, obj->CullBox) != CollisionMath::OUTSIDE)
				Add_To_Collection(obj);
		}
	}
}

// FUNCTION: BFME2 0x0071B400 size=63 name=?collect_objects_in_leaf@GridCullSystemClass@@IAEXABVFrustumClass@@PAVCullableClass@@@Z evidence=N:ledger
void GridCullSystemClass::collect_objects_in_leaf(const FrustumClass &frustum, CullableClass *head)
{
	if (head != NULL)
	{
		for (GridListIterator it(head); !it.Is_Done(); it.Next())
		{
			CullableClass *obj = it.Peek_Obj();
			if (CollisionMath::Overlap_Test(frustum, obj->CullBox) != CollisionMath::OUTSIDE)
				Add_To_Collection(obj);
		}
	}
}

// FUNCTION: BFME2 0x0071B4A0 size=886 name=?Collect_Objects@GridCullSystemClass@@UAEXABVVector3@@@Z evidence=N:ledger

void GridCullSystemClass::Collect_Objects(const Vector3 &point)
{
	VolumeStruct volume;
	init_volume(point, point, &volume);
	if (!volume.Is_Empty())
	{
		int delta_x = volume.Max[0] - volume.Min[0];
		int address = map_indices_to_address(volume.Min[0], volume.Min[1], volume.Min[2]);
		for (int z = volume.Min[2]; z < volume.Max[2]; z++)
		{
			for (int y = volume.Min[1]; y < volume.Max[1]; y++)
			{
				for (int x = volume.Min[0]; x < volume.Max[0]; x++)
				{
					collect_objects_in_leaf(point, Cells[address]);
					address++;
				}
				address -= delta_x;
				address += CellCount[0];
			}
			address = map_indices_to_address(volume.Min[0], volume.Min[1], z + 1);
		}
	}
	collect_objects_in_leaf(point, NoGridList);
}

// FUNCTION: BFME2 0x0071B820 size=924 name=?Collect_Objects@GridCullSystemClass@@UAEXABVAABoxClass@@@Z evidence=N:ledger
void GridCullSystemClass::Collect_Objects(const AABoxClass &box)
{
	VolumeStruct volume;
	init_volume(box, &volume);
	if (!volume.Is_Empty())
	{
		int delta_x = volume.Max[0] - volume.Min[0];
		int address = map_indices_to_address(volume.Min[0], volume.Min[1], volume.Min[2]);
		for (int z = volume.Min[2]; z < volume.Max[2]; z++)
		{
			for (int y = volume.Min[1]; y < volume.Max[1]; y++)
			{
				for (int x = volume.Min[0]; x < volume.Max[0]; x++)
				{
					collect_objects_in_leaf(box, Cells[address]);
					address++;
				}
				address -= delta_x;
				address += CellCount[0];
			}
			address = map_indices_to_address(volume.Min[0], volume.Min[1], z + 1);
		}
	}
	collect_objects_in_leaf(box, NoGridList);
}

// FUNCTION: BFME2 0x0071BBC0 size=995 name=?Collect_Objects@GridCullSystemClass@@UAEXABVOBBoxClass@@@Z evidence=N:ledger
void GridCullSystemClass::Collect_Objects(const OBBoxClass &box)
{
	VolumeStruct volume;
	init_volume(box, &volume);
	if (!volume.Is_Empty())
	{
		int delta_x = volume.Max[0] - volume.Min[0];
		int address = map_indices_to_address(volume.Min[0], volume.Min[1], volume.Min[2]);
		for (int z = volume.Min[2]; z < volume.Max[2]; z++)
		{
			for (int y = volume.Min[1]; y < volume.Max[1]; y++)
			{
				for (int x = volume.Min[0]; x < volume.Max[0]; x++)
				{
					collect_objects_in_leaf(box, Cells[address]);
					address++;
				}
				address -= delta_x;
				address += CellCount[0];
			}
			address = map_indices_to_address(volume.Min[0], volume.Min[1], z + 1);
		}
	}
	collect_objects_in_leaf(box, NoGridList);
}

// FUNCTION: BFME2 0x0071BFB0 size=961 name=?Collect_Objects@GridCullSystemClass@@UAEXABVFrustumClass@@@Z evidence=N:ledger
void GridCullSystemClass::Collect_Objects(const FrustumClass &frustum)
{
	VolumeStruct volume;
	init_volume(frustum, &volume);
	if (!volume.Is_Empty())
	{
		int delta_x = volume.Max[0] - volume.Min[0];
		int address = map_indices_to_address(volume.Min[0], volume.Min[1], volume.Min[2]);
		for (int z = volume.Min[2]; z < volume.Max[2]; z++)
		{
			for (int y = volume.Min[1]; y < volume.Max[1]; y++)
			{
				for (int x = volume.Min[0]; x < volume.Max[0]; x++)
				{
					collect_objects_in_leaf(frustum, Cells[address]);
					address++;
				}
				address -= delta_x;
				address += CellCount[0];
			}
			address = map_indices_to_address(volume.Min[0], volume.Min[1], z + 1);
		}
	}
	collect_objects_in_leaf(frustum, NoGridList);
}

// FUNCTION: BFME2 0x0071C380 size=363 name=?Collect_And_Unlink_All@GridCullSystemClass@@IAEXXZ evidence=N:ledger

void GridCullSystemClass::Collect_And_Unlink_All(void)
{
	Reset_Collection();

	for (int k = 0; k < (int)CellCount[2]; k++)
	{
		for (int j = 0; j < (int)CellCount[1]; j++)
		{
			for (int i = 0; i < (int)CellCount[0]; i++)
			{
				CullableClass *obj = Cells[map_indices_to_address(i, j, k)];
				while (obj)
				{
					CullableClass *nextobj = get_next_object(obj);

					unlink_object(obj);
					Add_To_Collection(obj);

					obj = nextobj;
				}
			}
		}
	}

	CullableClass *obj = NoGridList;
	while (obj)
	{
		CullableClass *nextobj = get_next_object(obj);

		unlink_object(obj);
		Add_To_Collection(obj);

		obj = nextobj;
	}
}

// FUNCTION: BFME2 0x0071C4F0 size=327 name=?Update_Culling@GridCullSystemClass@@UAEXPAVCullableClass@@@Z evidence=N:ledger

void GridCullSystemClass::Update_Culling(CullableClass *obj)
{
	int address;
	GridLinkClass *link = (GridLinkClass *)obj->CullLink;
	map_point_to_address(obj->CullBox.Center, address);

	if (address != link->GridAddress)
	{
		unlink_object(obj);
		link_object(obj, address);
	}
}

// FUNCTION: BFME2 0x0071C640 size=63 name=?Remove_Object_Internal@GridCullSystemClass@@IAEXPAVCullableClass@@@Z evidence=N:ledger

void GridCullSystemClass::Remove_Object_Internal(CullableClass *obj)
{
	GridLinkClass *link = (GridLinkClass *)obj->CullLink;

	unlink_object(obj);
	link->Set_Culling_System(NULL);
	delete link;
	obj->CullLink = NULL;

	ObjCount--;
	obj->Release_Ref();
}

// FUNCTION: BFME2 0x0071C680 size=323 name=?link_object@GridCullSystemClass@@IAEXPAVCullableClass@@@Z evidence=N:ledger

void GridCullSystemClass::link_object(CullableClass *obj)
{
	int address;
	map_point_to_address(obj->CullBox.Center, address);

	link_object(obj, address);
}

// FUNCTION: BFME2 0x0071C7D0 size=555 name=?Re_Partition@GridCullSystemClass@@UBEXABVVector3@@0M@Z evidence=N:ledger

void GridCullSystemClass::Re_Partition(const Vector3 &input_min, const Vector3 &input_max, float objdim)
{
	/*
	** grab all objects into the collection list and unlink them from the grid
	*/
	Reset_Collection();
	Collect_And_Unlink_All();

	/*
	** Do a sanity check on the input parameters
	*/
	Vector3 min = input_min;
	Vector3 max = input_max;

	if (max.X - min.X < 1.0f) {
		max.X += MinCellSize.X;
		min.X -= MinCellSize.X;
	}
	if (max.Y - min.Y < 1.0f) {
		max.Y += MinCellSize.Y;
		min.Y -= MinCellSize.Y;
	}
	if (max.Z - min.Z < 1.0f) {
		max.Z += MinCellSize.Z;
		min.Z -= MinCellSize.Z;
	}

	/*
	** Compute the grid parameters
	*/
	Origin = min;
	Vector3 world_dim = max - min;
	MaxObjExtent = objdim;

	/*
	** how many cells should we use on each dimension?
	*/
	CellCount[0] = CellCount[1] = CellCount[2] = 1;
	CellDim = world_dim;
	bool done = false;

	while (!done) {

		/*
		** find biggest dimension
		*/
		int bigdim = 0;
		if (CellDim[1]/MinCellSize[1] > CellDim[bigdim]/MinCellSize[bigdim]) bigdim = 1;
		if (CellDim[2]/MinCellSize[2] > CellDim[bigdim]/MinCellSize[bigdim]) bigdim = 2;

		/*
		** split dimension in two if possible
		*/
		if (CellDim[bigdim] >= 2.0f * MinCellSize[bigdim]) {
			CellDim[bigdim] /= 2.0f;
			CellCount[bigdim] *= 2;
		}

		/*
		** check termination conditions
		*/
		if (total_cell_count() >= TerminationCellCount) {
			done = true;
		}

		if (	(CellDim[0] < 2.0*MinCellSize[0]) &&
				(CellDim[1] < 2.0*MinCellSize[1]) &&
				(CellDim[2] < 2.0*MinCellSize[2]) ) {
			done = true;
		}
	}

	OOCellDim.X = 1.0f / CellDim.X;
	OOCellDim.Y = 1.0f / CellDim.Y;
	OOCellDim.Z = 1.0f / CellDim.Z;

	if (Cells != NULL) {
		delete[] Cells;
	}
	Cells = new CullableClass * [total_cell_count()];
	memset(&(Cells[0]), 0, total_cell_count() * sizeof(CullableClass *));

	/*
	** iterate the collection list and re-insert all objects into the grid
	*/
	CullableClass * obj;
	for (	obj = Get_First_Collected_Object_Internal();
			obj != NULL;
			obj = Get_Next_Collected_Object_Internal(obj))
	{
		link_object(obj);
	}
}

// FUNCTION: BFME2 0x0071CA00 size=387 name=?Load@GridCullSystemClass@@UAEXAAVChunkLoadClass@@@Z evidence=N:ledger

void GridCullSystemClass::Load(ChunkLoadClass &cload)
{
	/*
	** read the version chunk
	*/
	unsigned long version;
	cload.Open_Chunk();
	cload.Read(&version, sizeof(version));
	cload.Close_Chunk();

	/*
	** read the parameters chunk
	*/
	IOGridParametersStruct params;
	memset(&params, 0, sizeof(params));
	cload.Open_Chunk();
	cload.Read(&params, sizeof(params));
	cload.Close_Chunk();

	/*
	** unlink all objects
	*/
	Collect_And_Unlink_All();

	/*
	** partition the grid according to the loaded parameters
	*/
	CellCount[0]		= params.CellCount[0];
	CellCount[1]		= params.CellCount[1];
	CellCount[2]		= params.CellCount[2];
	CellDim.X			= params.CellDim.X;
	CellDim.Y			= params.CellDim.Y;
	CellDim.Z			= params.CellDim.Z;
	MaxObjExtent		= params.MaxObjExtent;
	MinCellSize.X		= params.MinCellSize.X;
	MinCellSize.Y		= params.MinCellSize.Y;
	MinCellSize.Z		= params.MinCellSize.Z;
	Origin.X				= params.Origin.X;
	Origin.Y				= params.Origin.Y;
	Origin.Z				= params.Origin.Z;

	OOCellDim.X = 1.0f / CellDim.X;
	OOCellDim.Y = 1.0f / CellDim.Y;
	OOCellDim.Z = 1.0f / CellDim.Z;

	if (Cells != NULL) {
		delete [] Cells;
		Cells = NULL;
	}

	Cells = new CullableClass * [total_cell_count()];
	memset(&(Cells[0]), 0, total_cell_count() * sizeof(CullableClass *));

	/*
	** re-link the objects in
	*/
	CullableClass * obj;
	for (	obj = Get_First_Collected_Object_Internal();
			obj != NULL;
			obj = Get_Next_Collected_Object_Internal(obj))
	{
		link_object(obj);
	}
}

// FUNCTION: BFME2 0x0071CB90 size=77 name=?Add_Object_Internal@GridCullSystemClass@@IAEXPAVCullableClass@@@Z evidence=N:ledger

void GridCullSystemClass::Add_Object_Internal(CullableClass *obj)
{
	GridLinkClass *link = new GridLinkClass(this);
	obj->CullLink = link;
	link_object(obj);

	ObjCount++;
	obj->Add_Ref();
}

// FUNCTION: BFME2 0x0071CBE0 size=260 name=??0GridCullSystemClass@@QAE@XZ evidence=N:ledger

GridCullSystemClass::GridCullSystemClass(void) :
	MinCellSize(10, 10, 10),
	MaxObjExtent(15),
	Origin(-100, -100, -100),
	CellDim(10, 10, 10),
	Cells(NULL),
	NoGridList(NULL),
	ObjCount(0),
	TerminationCellCount(0x4000)
{
	CellCount[0] = CellCount[1] = CellCount[2] = 0;
	Re_Partition(Vector3(-100, -100, -100), Vector3(100, 100, 100), 15);
	Reset_Statistics();
}
