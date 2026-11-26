// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// AggregateDefClass.h: WW3D2's aggregate definition (Zero Hour's agg_def.h): a base model name plus the subobjects attached
// to its bones, with loader and saver. Target facts (retail game.dat): constructor 0x001A3ED0 stores the vftable
// (0x007D6C70), clears the version +4, builds the subobject vector at +8 (a DynamicVectorClass of pointers: vftable
// 0x007D6C3C +8, array +0xC, maximum +0x10, owned flags +0x14/+0x15, count +0x18, growth step +0x1C = 10), clears
// W3dAggregateInfoStruct +0x20 (0x24 bytes) and W3dAggregateMiscInfo +0x44 (0x14 bytes, OriginalClassID = 0x19) and the
// name +0x58. SIZE 0x5C. The loaders and savers return bool (mangled _N). The table has no Load_Assets or
// Create_Render_Object slot (BFME 2 dropped them); slot 15 is Is_Object_In_List.
#pragma once
#ifndef BFME2_AGGREGATEDEFCLASS_H
#define BFME2_AGGREGATEDEFCLASS_H

#include "WW3D2/w3d_file.h"
#include "WWLib/vector.h"

class ChunkLoadClass;
class ChunkSaveClass;
class RenderObjClass;

// The asset manager's render-object factory (retail 0x00136175), called by name by the creation members.
RenderObjClass *Create_Render_Obj(const char *asset_name);

class AggregateDefClass
{
public:
	AggregateDefClass();											// 0x001A3ED0
	AggregateDefClass(RenderObjClass &base_model);					// 0x001A3BE0
	AggregateDefClass(const AggregateDefClass &src);				// 0x001A3F60
	virtual ~AggregateDefClass();									// slot 0: 0x001A3C90

	const AggregateDefClass &operator=(const AggregateDefClass &src);	// 0x001A3D10

	virtual bool Load_W3D(ChunkLoadClass &chunk_load);				// slot 1: 0x001A3350
	virtual bool Save_W3D(ChunkSaveClass &chunk_save);				// slot 2: 0x001A35A0

	const char *Get_Name() const { return m_pName; }
	int Compute_Save_Size() const;													// 0x001A3930 (header, name and subobject records)
	void Set_Name(const char *pname);
	RenderObjClass *Create();
	AggregateDefClass *Clone() const { return new AggregateDefClass(*this); }

	unsigned long Class_ID() const { return m_MiscInfo.OriginalClassID; }

	void Initialize(RenderObjClass &base_model);

protected:
	virtual bool Read_Header(ChunkLoadClass &chunk_load);			// slot 3: 0x001A3400
	virtual bool Read_Info(ChunkLoadClass &chunk_load);				// slot 4: 0x001A3460
	virtual bool Read_Subobject(ChunkLoadClass &chunk_load);		// slot 5: 0x001A34D0
	virtual bool Read_Class_Info(ChunkLoadClass &chunk_load);		// slot 6: 0x001A3560

	virtual bool Save_Header(ChunkSaveClass &chunk_save);			// slot 7: 0x001A35F0
	virtual bool Save_Info(ChunkSaveClass &chunk_save);				// slot 8: 0x001A3960
	virtual bool Save_Subobject(ChunkSaveClass &chunk_save, W3dAggregateSubobjectStruct *psubobject);	// slot 9: 0x001A3670
	virtual bool Save_Class_Info(ChunkSaveClass &chunk_save);		// slot 10: 0x001A3690

	virtual void Attach_Subobjects(RenderObjClass &base_model);		// slot 11: 0x001A3880
	virtual RenderObjClass *Find_Subobject(RenderObjClass &model, const char mesh_path[MESH_PATH_ENTRIES][MESH_PATH_ENTRY_LEN], const char bone_path[MESH_PATH_ENTRIES][MESH_PATH_ENTRY_LEN]);	// slot 12: 0x001A3190
	virtual void Free_Subobject_List();								// slot 13: 0x001A3830
	virtual void Add_Subobject(const W3dAggregateSubobjectStruct &subobj_info);	// slot 14: 0x001A3E20
	virtual bool Is_Object_In_List(const char *passet_name, DynamicVectorClass<RenderObjClass *> &node_list);	// slot 15: 0x001A38E0
	virtual void Build_Subobject_List(RenderObjClass &original_model, RenderObjClass &model);	// slot 16: 0x001A4010

private:
	unsigned long m_Version;										// +0x04
	DynamicVectorClass<W3dAggregateSubobjectStruct *> m_SubobjectList;	// +0x08
	W3dAggregateInfoStruct m_Info;									// +0x20
	W3dAggregateMiscInfo m_MiscInfo;								// +0x44
	char *m_pName;													// +0x58
};
// SIZE 0x5C
typedef char AggregateDefClassSize[(sizeof(AggregateDefClass) == 0x5C) ? 1 : -1];

#endif
