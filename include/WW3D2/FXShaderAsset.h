// FXShaderAsset.h: BFME 2's effect-shader asset handle and its factory.
// Target facts:
// - FXShaderAsset::Register 0x001516C1 has TextureAsset::Register's shape:
//   unless the name is null or already known (0x0061F0D0), it allocates a
//   0x1C-byte factory, runs the constructor 0x001515CA(name) and hands it to
//   0x0061EF90.
// - FXShaderAsset::Factory 0x001515CA: AssetFactoryBase's constructor, the
//   Impl pointer at +0x14 cleared, vftable 0x007D3A6C, an AsciiString name at
//   +0x18 (AsciiString(const char *) 0x00037BA0). SIZE 0x1C.
// - vftable 0x007D3A6C, 15 slots: GetName 0x00151733, PreLoad 0x001518B2
//   (WorldBuilder: FXShaderAsset::Factory::PreLoad), Load the empty
//   0x000B3FD0, PostLoad 0x00151CD8, base 4, empty 5 and 6, PostUnload
//   0x001517B5, base Delete, deleting destructor 0x00151717, IsLoaded
//   0x00151415, base LoadNow/UnloadNow, GetTypeId 'FXSH' 0x0015162C,
//   GetMemorySize 0x0015140A.
#pragma once
#ifndef BFME2_FXSHADERASSET_H
#define BFME2_FXSHADERASSET_H

#include "assetmanager/AssetFactoryBase.h"
#include "Common/AsciiString.h"

class FXShaderAsset
{
public:
	// The loaded effect: 0x28 bytes (PreLoad 0x001518B2 allocates it and runs the constructor 0x00151866; PostUnload 0x001517B5
	// calls Unload 0x00151744 and deletes it through slot 0). Only the word at +4 is read outside the unit: IsLoaded 0x00151415
	// treats a non-zero value as loaded. The rest is not recovered.
	class Impl
	{
	public:
		Impl(void);							// 0x00151866
		virtual ~Impl(void);						// 0
		void Unload(void);						// 0x00151744
		void Load(const char *name);					// 0x00151915
		void *m_loaded;							// +0x04
		unsigned char m_unrecovered[0x20];				// +0x08..+0x27
	};

	class Factory : public AssetFactoryBase
	{
	public:
		Factory(const char *name);						// 0x001515CA
		virtual ~Factory(void);							// 9: 0x00151717

		virtual const char *GetName(void);					// 0: 0x00151733
		virtual void PreLoad(void);						// 1: 0x001518B2
		virtual void Load(void) {}						// 2: 0x000B3FD0
		virtual void PostLoad(void);						// 3: 0x00151CD8
		virtual void PreUnload(void) {}						// 5: 0x000B3FD0
		virtual void Unload(void) {}						// 6: 0x000B3FD0
		virtual void PostUnload(void);						// 7: 0x001517B5
		virtual bool IsLoaded(void);						// 10: 0x00151415
		virtual unsigned int GetTypeId(void) { return 0x46585348; }		// 13: 0x0015162C ('FXSH')
		virtual unsigned int GetMemorySize(void);				// 14: 0x0015140A

		Impl *m_impl;					// +0x14
		AsciiString m_name;				// +0x18
	};

	static void Register(const char *name);						// 0x001516C1
};

typedef char FXShaderAssetImplSize[(sizeof(FXShaderAsset::Impl) == 0x28) ? 1 : -1];
typedef char FXShaderAssetFactorySize[(sizeof(FXShaderAsset::Factory) == 0x1C) ? 1 : -1];

#endif
