// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// VertexFormats.h: the vertex layouts the vertex buffers cast their locked memory to (Zero Hour's dx8fvf.h).
// Target facts: the Copy bodies of DX8VertexBufferClass (0x001398C0..0x0013A510) store 8 dwords per XYZNUV1 vertex, 3 per XYZ,
// 5 per XYZUV1, 6 per XYZN, 9 per XYZNDUV1 and 6 per XYZDUV1, in the member order below. D3D FVF flag values are the ones the
// BFME 2 table at 0x007D2F04 holds.
#pragma once
#ifndef BFME2_VERTEXFORMATS_H
#define BFME2_VERTEXFORMATS_H

// Direct3D flexible vertex format bits (d3d9types.h values; BFME 2 builds its formats from the same bits).
enum {
	BFME_D3DFVF_XYZ = 0x002,
	BFME_D3DFVF_NORMAL = 0x010,
	BFME_D3DFVF_DIFFUSE = 0x040,
	BFME_D3DFVF_TEX1 = 0x100,
	BFME_D3DFVF_TEX2 = 0x200
};

enum {
	DX8_FVF_XYZ = BFME_D3DFVF_XYZ,
	DX8_FVF_XYZN = BFME_D3DFVF_XYZ | BFME_D3DFVF_NORMAL,
	DX8_FVF_XYZNUV1 = BFME_D3DFVF_XYZ | BFME_D3DFVF_NORMAL | BFME_D3DFVF_TEX1,
	DX8_FVF_XYZNUV2 = BFME_D3DFVF_XYZ | BFME_D3DFVF_NORMAL | BFME_D3DFVF_TEX2,
	DX8_FVF_XYZNDUV1 = BFME_D3DFVF_XYZ | BFME_D3DFVF_NORMAL | BFME_D3DFVF_TEX1 | BFME_D3DFVF_DIFFUSE,
	DX8_FVF_XYZNDUV2 = BFME_D3DFVF_XYZ | BFME_D3DFVF_NORMAL | BFME_D3DFVF_TEX2 | BFME_D3DFVF_DIFFUSE,
	DX8_FVF_XYZDUV1 = BFME_D3DFVF_XYZ | BFME_D3DFVF_TEX1 | BFME_D3DFVF_DIFFUSE,
	DX8_FVF_XYZDUV2 = BFME_D3DFVF_XYZ | BFME_D3DFVF_TEX2 | BFME_D3DFVF_DIFFUSE,
	DX8_FVF_XYZUV1 = BFME_D3DFVF_XYZ | BFME_D3DFVF_TEX1,
	DX8_FVF_XYZUV2 = BFME_D3DFVF_XYZ | BFME_D3DFVF_TEX2
};

struct VertexFormatXYZ
{
	float x;
	float y;
	float z;
};

struct VertexFormatXYZNUV1
{
	float x;
	float y;
	float z;
	float nx;
	float ny;
	float nz;
	float u1;
	float v1;
};

struct VertexFormatXYZN
{
	float x;
	float y;
	float z;
	float nx;
	float ny;
	float nz;
};

struct VertexFormatXYZNDUV1
{
	float x;
	float y;
	float z;
	float nx;
	float ny;
	float nz;
	unsigned diffuse;
	float u1;
	float v1;
};

struct VertexFormatXYZNDUV2
{
	float x;
	float y;
	float z;
	float nx;
	float ny;
	float nz;
	unsigned diffuse;
	float u1;
	float v1;
	float u2;
	float v2;
};

struct VertexFormatXYZUV1
{
	float x;
	float y;
	float z;
	float u1;
	float v1;
};

struct VertexFormatXYZDUV1
{
	float x;
	float y;
	float z;
	unsigned diffuse;
	float u1;
	float v1;
};

#endif
