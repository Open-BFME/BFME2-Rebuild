// AssetFactoryBase.h: the SAGE asset system's per-asset loader object.
// Every texture, mesh, shader, animation and render asset is a factory object
// derived from this class; the typed handles (TextureAsset and friends) hold a
// pointer to one. WorldBuilder's assert strings name AssetFactoryBase::
// ~AssetFactoryBase, Delete, LoadNow, ReleaseRef and UnloadNow, the fields
// asset->m_phase and asset->m_refCount, and the factories' PreLoad, Load and
// PostLoad (FXShaderAsset::Factory::PreLoad, MeshAsset::Factory::Load,
// RenderAsset::ModifiedRenderFactory::PostLoad).
//
// Target facts:
// - The constructor 0x0061ED40 clears the 16-bit count at +4 (word store),
//   sets the 8-bit phase at bit 16 to 7 and clears the 3-bit flag field at
//   bit 24 in the same dword, installs vftable 0x0087C584 and zeroes +8, +0xC
//   and +0x10. The destructor (0x0061ED80, deleting form 0x0061EE90, slot 9)
//   frees +0xC with operator delete[].
// - 15 slots. 0..3 and 5..7, 13, 14 are pure. Slot 1 is PreLoad and slot 3
//   PostLoad: the MapPreviewTextureAssetFactory and
//   RenderAsset::ModifiedRenderFactory tables put those names there; the texture
//   factory's slot 3 hands its name to the loaded data's PostLoad. Slot 0
//   returns the asset's name (texture factory 0x00131C40 returns its
//   StringClass's buffer), slot 13 a four-character type id ('TEX' 0x000EF29A),
//   slot 14 the loaded data's memory size (texture 0x00131CDB). Slot 4 is the
//   shared empty `ret 4` body 0x0047A69C.
// - Delete (slot 8, 0x0061EDA0): if slot 10 says loaded, steps the phase to 4,
//   5 and 6 calling slots 5, 6 and 7 in turn, then sets it back to 7; finally
//   `::delete this` (slot 9 with flag 0, then the global operator delete).
// - Slot 10 (0x00130FCE) stamps the access frame (0x0061EEC0 stores the asset
//   manager's incremented frame counter at +0x10) and returns phase == 3.
//   Slot 11 LoadNow (0x0061EE20) asks the asset manager (global 0x009E9C0C) to
//   load this unless the phase is 3; slot 12 UnloadNow (0x0061EE50) to unload
//   it unless the phase is 7.
// - ReleaseRef 0x0061ED10 (non-virtual): if the count is non-zero, decrements
//   the 16-bit count and, when it reaches zero with flag bit 0 (0x1000000) set,
//   tail-calls Delete. Add_Ref 0x000424B6 is `inc word [ecx+4]`.
// Slot names 5..7, 13, 14 and the slot 4 name are role names (the unload
// phases mirror the load ones); no string spells them.
#pragma once
#ifndef BFME2_ASSETFACTORYBASE_H
#define BFME2_ASSETFACTORYBASE_H

class AssetFactoryBase
{
public:
	// m_phase values seen in the binary.
	enum
	{
		PHASE_LOADED = 3,
		PHASE_PRE_UNLOAD = 4,
		PHASE_UNLOAD = 5,
		PHASE_POST_UNLOAD = 6,
		PHASE_UNLOADED = 7
	};

	// m_flags bits.
	enum
	{
		FLAG_DELETABLE = 1		// ReleaseRef deletes the factory at refcount 0
	};

	AssetFactoryBase(void);							// 0x0061ED40

	void Add_Ref(void) { m_refCount++; }					// 0x000424B6
	void ReleaseRef(void);							// 0x0061ED10
	void Touch(void);							// 0x0061EEC0 (role name)

	virtual const char *GetName(void) = 0;					// 0
	virtual void PreLoad(void) = 0;						// 1
	virtual void Load(void) = 0;						// 2
	virtual void PostLoad(void) = 0;					// 3
	virtual void Prioritize(int priority) {}				// 4: 0x0047A69C (role name)
	virtual void PreUnload(void) = 0;					// 5
	virtual void Unload(void) = 0;						// 6
	virtual void PostUnload(void) = 0;					// 7
	virtual void Delete(void);						// 8: 0x0061EDA0
	virtual ~AssetFactoryBase(void);					// 9: 0x0061EE90
	virtual bool IsLoaded(void);						// 10: 0x00130FCE
	virtual void LoadNow(void);						// 11: 0x0061EE20
	virtual void UnloadNow(void);						// 12: 0x0061EE50
	virtual unsigned int GetTypeId(void) = 0;				// 13
	virtual unsigned int GetMemorySize(void) = 0;				// 14

protected:
	unsigned int m_refCount : 16;		// +0x4 bits 0..15
	unsigned int m_phase : 8;		// +0x4 bits 16..23
	unsigned int m_flags : 3;		// +0x4 bits 24..26
	int m_unk08;				// +0x8 zeroed by the constructor; role not yet seen
	char *m_buffer;				// +0xC freed with delete[] by the destructor
	int m_lastAccess;			// +0x10 asset manager frame of the last IsLoaded
};

typedef char AssetFactoryBaseSize[(sizeof(AssetFactoryBase) == 0x14) ? 1 : -1];

#endif
