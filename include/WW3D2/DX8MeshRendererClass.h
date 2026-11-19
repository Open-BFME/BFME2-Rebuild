// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// DX8MeshRendererClass.h: the controller of the DX8 mesh rendering system; it groups mesh fragments into FVF
// containers, texture categories and polygon renderers and draws the visible ones (dx8renderer.cpp).
// Target facts: the constructor 0x00145AA0 sets the lighting byte +0 to 1, clears the camera +4, builds the vector
// of rigid container lists at +8 (SimpleDynVecClass vftable 0x007D34FC, three dwords +0xC/+0x10/+0x14 cleared),
// clears the skin list pointer +0x18, and builds two MultiListClass heads in place: +0x1C (vftable 0x007D34E4, the
// texture categories waiting to be deleted) and +0x34 (vftable 0x007D34E8, the FVF containers waiting to be deleted).
// Zero Hour keeps both delete lists as file statics. There is no decal mesh pointer. SIZE 0x4C.
// Init 0x00144530 allocates the 0x18-byte skin container list with the global operator new when none exists;
// Clear_Pending_Delete_Lists 0x00144580 deletes everything on both delete lists; Shutdown 0x00148010 is Invalidate(true)
// then Clear_Pending_Delete_Lists; Unregister_Mesh_Type 0x001445E0 deletes the polygon renderers of the model.
#pragma once
#ifndef BFME2_DX8MESHRENDERERCLASS_H
#define BFME2_DX8MESHRENDERERCLASS_H

#include "WWLib/multilist.h"
#include "WWLib/simplevec.h"

class CameraClass;
class MeshModelClass;
class DX8TextureCategoryClass;
class DX8FVFCategoryContainer;

typedef MultiListClass<DX8TextureCategoryClass> TextureCategoryList;
typedef MultiListClass<DX8FVFCategoryContainer> FVFCategoryList;

class DX8MeshRendererClass
{
public:
	DX8MeshRendererClass(void);							// 0x00145AA0
	~DX8MeshRendererClass(void);							// 0x00147F60

	void Init(void);								// 0x00144530
	void Shutdown(void);								// 0x00148010
	void Flush(void);								// 0x00148030
	void Clear_Pending_Delete_Lists(void);						// 0x00144580
	void Log_Statistics_String(bool only_visible);					// 0x00145F20
	static void Request_Log_Statistics(void);					// 0x00143D00
	void Register_Mesh_Type(MeshModelClass *mmc);					// 0x00145C30
	void Unregister_Mesh_Type(MeshModelClass *mmc);					// 0x001445E0
	void Set_Camera(CameraClass *cam) { Camera = cam; }
	CameraClass *Peek_Camera(void) { return Camera; }
	void Enable_Lighting(bool enable) { EnableLighting = enable; }
	void Invalidate(bool shutdown = false);						// 0x00145FA0

protected:
	// The categories and containers queue themselves on the delete lists when their last member goes.
	friend class DX8FVFCategoryContainer;
	friend class DX8TextureCategoryClass;

	bool EnableLighting;						// +0x00
	CameraClass *Camera;						// +0x04
	SimpleDynVecClass<FVFCategoryList *> TextureCategoryContainerListsRigid;	// +0x08
	FVFCategoryList *TextureCategoryContainerListSkin;		// +0x18
	TextureCategoryList TextureCategoryDeleteList;			// +0x1C
	FVFCategoryList FVFCategoryContainerDeleteList;			// +0x34
};

// The one renderer (a pointer in retail: Remove_Texture_Category 0x00144C30 tests it before use); data ledger
// ?TheDX8MeshRenderer@@3PAVDX8MeshRendererClass@@A at 0x009F363C.
extern DX8MeshRendererClass *TheDX8MeshRenderer;

typedef char DX8MeshRendererClassSize[(sizeof(DX8MeshRendererClass) == 0x4C) ? 1 : -1];

#endif
