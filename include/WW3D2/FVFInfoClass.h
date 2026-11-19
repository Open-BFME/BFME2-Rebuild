// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// Describes the byte offsets and stride of a Direct3D flexible vertex format.
// The vertex can append an additional basis and per-element extension data.
#pragma once
#ifndef BFME2_FVFINFOCLASS_H
#define BFME2_FVFINFOCLASS_H

class StringClass;

class FVFInfoClass
{
public:
	FVFInfoClass(void) {}														// the file-static format table is raw memory until Init
	FVFInfoClass(unsigned fvf, unsigned vertex_size, bool additional_basis, unsigned extension_count);
	void Initialize(unsigned fvf, unsigned vertex_size, bool additional_basis, unsigned extension_count);

	unsigned Get_FVF(void) const { return FVF; }
	unsigned Get_FVF_Size(void) const { return FVFSize; }
	void Get_FVF_Name(StringClass &fvfname) const;

protected:
	unsigned FVF;                  // +0x00
	bool AdditionalBasis;          // +0x04
	unsigned ExtensionCount;       // +0x08
	unsigned FVFSize;              // +0x0C
	unsigned PositionOffset;       // +0x10
	unsigned ExtensionOffset;      // +0x14
	unsigned NormalOffset;         // +0x18
	unsigned ExtensionEnd;         // +0x1C
	unsigned BlendOffset;          // +0x20
	unsigned TexCoordOffset[8];    // +0x24
	unsigned DiffuseOffset;        // +0x44
	unsigned SpecularOffset;       // +0x48
	unsigned BasisOffset;          // +0x4C
	unsigned SecondBasisOffset;    // +0x50
	unsigned BasisEnd;             // +0x54
	unsigned ExtensionDataOffset;  // +0x58
	unsigned FormatIndex;          // +0x5C
};

typedef char FVFInfoClassSize[(sizeof(FVFInfoClass) == 0x60) ? 1 : -1];

#endif
