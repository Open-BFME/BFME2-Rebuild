// TextureBaseClass.h: BFME 2's texture handle.
// BFME 2 replaced Westwood's self-contained texture objects with the SAGE asset
// system (WorldBuilder asserts: AssetFactoryBase::LoadNow/UnloadNow/Delete/
// ReleaseRef, asset->m_phase, asset->m_refCount, TextureAsset::Get/Register).
// A TextureBaseClass is one pointer to the texture's asset factory object.
// Target facts: Peek_D3D_Base_Texture 0x0013270C (51 direct callers, out of
// line) reads this+0, returns 0 when there is no factory, otherwise asks the
// factory's slot 10 whether it is loaded, calls its slot 11 (load now) if not,
// and returns the D3D texture at +8 of the loaded data the factory keeps at
// +0x14. The neighbouring getters 0x00132731..0x0013282C read other fields of
// that loaded data the same way and are declared once their meaning is known.
// The factory's refcount is a 16-bit count at +4 (Add_Ref 0x000424B6
// `inc word [ecx+4]`; release 0x0061ED10).
#pragma once
#ifndef BFME2_TEXTUREBASECLASS_H
#define BFME2_TEXTUREBASECLASS_H

#include "assetmanager/AssetFactoryBase.h"

struct IDirect3DBaseTexture8;

class TextureBaseClass
{
public:
	// Inline in retail: the default constructor 0x00326BE6 clears the factory
	// pointer and the destructor 0x0017098D releases it (the element
	// constructor and destructor MeshMatDescClass's and MaterialInfoClass's
	// texture arrays pass to the vector iterators).
	TextureBaseClass() : m_factory(0) {}
	// The copy adds a reference in place (MeshLoadContextClass::Peek_Texture 0x00188700, MaterialCollectorClass::Peek_Texture 0x0016F4E0).
	TextureBaseClass(const TextureBaseClass &that) : m_factory(that.m_factory) { if (m_factory) m_factory->Add_Ref(); }
	~TextureBaseClass() { if (m_factory) m_factory->ReleaseRef(); }

	void Clear()
	{
		if (m_factory) {
			m_factory->ReleaseRef();
			m_factory = 0;
		}
	}

	void Assign(const TextureBaseClass &that)
	{
		if (that.m_factory) that.m_factory->Add_Ref();
		if (m_factory) m_factory->ReleaseRef();
		m_factory = that.m_factory;
	}

	IDirect3DBaseTexture8 *Peek_D3D_Base_Texture() const;		// 0x0013270C
	bool Has_Factory() const { return m_factory != 0; }		// MeshMatDescClass::Is_Empty 0x0015A6C0 tests the handle word

	// The neighbouring getters over the loaded data (the factory's Impl; 0x00132731..0x0013282C): each loads the texture when it
	// is not loaded, then returns one field (names structural: the Impl offset), or a default for an empty handle.
	int Get_Impl_Unk0C() const;		// 0x00132731 (default 0)
	int Get_Impl_Unk28() const;		// 0x0013275A (default 1)
	int Get_Impl_Unk2C() const;		// 0x00132784
	int Get_Impl_Unk34() const;		// 0x001327D8
	int Get_Impl_Unk38() const;		// 0x00132802
	int Get_Impl_Unk3C() const;		// 0x0013282C

	// Two handles are equal when they share a factory: MaterialInfoClass's
	// texture vector (vftable 0x007D4688) reuses the pointer vectors' operator==
	// 0x00196920 and ID 0x001969C0, and MeshClass's slot 124 0x0014B800 compares
	// the factory words directly.
	bool operator==(const TextureBaseClass &that) const { return m_factory == that.m_factory; }
	bool operator!=(const TextureBaseClass &that) const { return m_factory != that.m_factory; }

	friend class SegLineRendererClass;		// operator= 0x001912F0 counts the reference in place (REF_PTR_SET on the handle's factory)
	friend class StreakRendererClass;		// operator= 0x00743100 counts the reference in place (REF_PTR_SET on the handle's factory)

protected:
	AssetFactoryBase *m_factory;								// +0x0
};

class TextureClass : public TextureBaseClass
{
};

#endif
