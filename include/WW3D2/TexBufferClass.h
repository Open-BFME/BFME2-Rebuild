// TexBufferClass.h: the per-polygon texture array of a mesh material description (MeshMatDescClass +0xC8).
//
// Target facts (retail game.dat): a ShareBufferClass<TextureAsset> (the base constructor 0x0015AF30 and copy constructor
// 0x0015AE10 are its instantiations; vftable 0x00BD3CF4 is the class's own, scalar deleting destructor 0x0015D7E0).
// The constructor 0x0015D080 runs the base constructor with a zero alignment, stores the vftable and clears the array
// (count * 4 bytes at +0x0C); the copy constructor 0x0015D0C0 is the base copy plus the vftable; the destructor
// 0x0015D0E0 stores the vftable and tail-jumps to the base destructor 0x0015CC90.
// SIZE 0x18.
#pragma once
#ifndef BFME2_TEXBUFFERCLASS_H
#define BFME2_TEXBUFFERCLASS_H

#include "WW3D2/ShareBufferClass.h"
#include "WW3D2/TextureAsset.h"

// This instantiation owns TextureAsset elements; the array destructor
// at 0x0015CC90 uses their allocation cookie and releases each handle.
template <>
ShareBufferClass<TextureAsset>::~ShareBufferClass(void);

class TexBufferClass : public ShareBufferClass<TextureAsset>
{
public:
	TexBufferClass(int count, const char *msg);		// 0x0015D080
	TexBufferClass(const TexBufferClass &that);		// 0x0015D0C0
	virtual ~TexBufferClass(void);				// 0x0015D0E0
};

#endif
