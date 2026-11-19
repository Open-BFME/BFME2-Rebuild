// TextureAsset.h: BFME 2's texture handle and its asset factory.
// A TextureAsset is a TextureBaseClass handle (one AssetFactoryBase pointer);
// the factory owns a TextureAsset::Impl once PreLoad has run, and the Impl owns
// the Direct3D texture.
//
// Target facts:
// - TextureAsset::SetQualityLevel 0x00132C9D stores its argument at +0x38 of the
//   handle's factory when there is one.
// - TextureAsset::Factory: TextureAsset::Register 0x00132BD2 allocates 0x3C
//   bytes and runs the constructor 0x001320D0(name): AssetFactoryBase's
//   constructor, vftable 0x007D25D8, Impl pointer +0x14 = 0, a StringClass
//   name at +0x18 (StringClass(const char *, bool) 0x000F0ED1), a 0x14-byte
//   member at +0x1C built by 0x0013EA00(0), then +0x30, +0x34, +0x38 = 0. Its
//   PreLoad (slot 1, 0x00131C63) allocates the 0x58-byte Impl, copies +0x30,
//   +0x34 and +0x38 into the Impl's +0x44, +0x4C and +0x48, and calls
//   Impl::PreLoad(name) 0x00131259. Slot 10 (0x00131CF2) is "loaded" as soon as
//   the Impl holds a D3D texture, else AssetFactoryBase's test.
// - TextureAsset::Impl: constructor 0x0013107A installs vftable 0x007D25C8
//   (three slots: 0x001311AA, the deleting destructor 0x00131E52, the empty
//   0x000B3FD0), clears the byte +4 and +8..+0x24, sets +0x28..+0x3C to 1,
//   +0x40 to 0x7FFFFFFF, +0x44..+0x4C to 0, +0x50 to 2 and +0x54 to 0. The
//   destructor body 0x001310CF releases the D3D texture at +8 (IUnknown slot 2).
//   TextureBaseClass::Peek_D3D_Base_Texture 0x0013270C returns that +8; the
//   getter 0x00132731 returns +0xC. Impl::PreLoad 0x00131259 swaps the name's
//   extension for .dds, .tga, then .jpg, keeping the first GetGameFile hit at
//   +0x10; a .jpg also opens the matching .png at +0x1C; both files are marked
//   delete-on-close.
// Names of the Impl's virtual slots 0 and 2 and of most Impl fields are not
// recovered; the ones below are role names.
#pragma once
#ifndef BFME2_TEXTUREASSET_H
#define BFME2_TEXTUREASSET_H

#include "WW3D2/TextureBaseClass.h"
#include "assetmanager/AssetFactoryBase.h"
#include "WWLib/wwstring.h"

struct IDirect3DBaseTexture8;
class File;

class TextureAsset : public TextureBaseClass
{
public:
	class Impl
	{
	public:
		Impl(void);								// 0x0013107A
		virtual void Apply(unsigned int stage);				// 0: 0x001311AA (role name)
		virtual ~Impl(void);							// 1: 0x00131E52 / 0x001310CF
		virtual void Release_Device_Resources(void) {}				// 2: 0x000B3FD0 (role name)

		void PreLoad(const char *name);						// 0x00131259
		void Load(void);							// 0x0013132D (the file contents; role name)
		unsigned int GetMemorySize(void);					// 0x00131B1F (the texture's memory size; role name)
		void PostLoad(const char *name);					// 0x00131E6E

		bool m_unk04;					// +0x04
		IDirect3DBaseTexture8 *D3DTexture;		// +0x08
		int m_unk0C;					// +0x0C (getter 0x00132731)
		File *m_file;					// +0x10 the image file PreLoad found
		char *m_fileData;				// +0x14 its contents (Load: readEntireAndClose)
		int m_fileSize;					// +0x18 its size (Load: size)
		File *m_alphaFile;				// +0x1C the .png PreLoad pairs with a .jpg
		char *m_alphaData;				// +0x20
		int m_alphaSize;				// +0x24
		int m_unk28[6];					// +0x28..+0x3C set to 1
		int m_unk40;					// +0x40 0x7FFFFFFF
		int m_unk44;					// +0x44 from the factory's +0x30
		int QualityLevel;				// +0x48 from the factory's +0x38
		int m_unk4C;					// +0x4C from the factory's +0x34
		int m_unk50;					// +0x50 2
		int m_unk54;					// +0x54
	};

	// vftable 0x007D25D8 (18 slots): GetName 0x00131C40, PreLoad 0x00131C63,
	// Load 0x00131CD3 (Impl::Load 0x0013132D: each file's size and contents),
	// PostLoad 0x00132129, base 4, empty 5 and 6 (0x000B3FD0), PostUnload
	// 0x00133380 (drops the Impl), base Delete, deleting destructor 0x001320B4,
	// IsLoaded 0x00131CF2, base LoadNow/UnloadNow, GetTypeId 'TEX' 0x000EF29A,
	// GetMemorySize 0x00131CDB, then three of its own: 15 0x005CB9FF (false;
	// RecolorFactoryDecal's is TextureRecolorView::tryDynamicRecolor 0x001326EA
	// in the map), 16 0x00131CEA (forwards to the +0x1C member), 17 0x0047A699
	// (false). Names of 16 and 17 are role names.
	class Factory : public AssetFactoryBase
	{
	public:
		Factory(const char *name);						// 0x001320D0
		virtual ~Factory(void);							// 9: 0x001320B4 / 0x00131BE5

		virtual const char *GetName(void);					// 0: 0x00131C40
		virtual void PreLoad(void);						// 1: 0x00131C63
		virtual void Load(void);						// 2: 0x00131CD3
		virtual void PostLoad(void);						// 3: 0x00132129
		virtual void PreUnload(void) {}						// 5: 0x000B3FD0
		virtual void Unload(void) {}						// 6: 0x000B3FD0
		virtual void PostUnload(void);						// 7: 0x00133380
		virtual bool IsLoaded(void);						// 10: 0x00131CF2
		virtual unsigned int GetTypeId(void) { return 0x544558; }		// 13: 0x000EF29A ('TEX')
		virtual unsigned int GetMemorySize(void);				// 14: 0x00131CDB
		virtual bool TryDynamicRecolor(void *recolor) { return false; }	// 15: 0x005CB9FF
		virtual void Forward_To_Member(void);					// 16: 0x00131CEA (role name)
		virtual bool Is_Recolor(void) { return false; }				// 17: 0x0047A699 (role name)

		Impl *m_impl;					// +0x14
		StringClass m_name;				// +0x18
		unsigned char m_unk1C[0x14];			// +0x1C a 0x14-byte object (constructor 0x0013EA00)
		int m_unk30;					// +0x30 copied to Impl +0x44
		int m_unk34;					// +0x34 copied to Impl +0x4C
		int m_qualityLevel;				// +0x38 copied to Impl +0x48
	};

	void SetFactoryField30(int value);						// 0x00132C7D (the factory's +0x30, copied to the Impl's +0x44; role name)
	void SetFactoryField34(int value);						// 0x00132C8D (the factory's +0x34, copied to the Impl's +0x4C; role name)
	void SetQualityLevel(int level);						// 0x00132C9D
	void SetQualityEnabled(bool enabled);						// 0x00132FE9 (forwards 1 or 0 to SetQualityLevel; role name)
	static void Register(const char *name);						// 0x00132BD2
};

typedef char TextureAssetImplSize[(sizeof(TextureAsset::Impl) == 0x58) ? 1 : -1];
typedef char TextureAssetFactorySize[(sizeof(TextureAsset::Factory) == 0x3C) ? 1 : -1];

#endif
