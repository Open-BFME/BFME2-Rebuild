// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// D3DXVec3CatmullRom.h: the D3DX routine Drawable::getPosition calls, declared without the DirectX headers.
// Target facts: Drawable::getPosition 0x002763E6 calls the import thunk 0x0062AFC8 (map name D3DXVec3CatmullRom,
// evidence: jmp [IAT 0x007BA9DC] d3dx9_27.dll!D3DXVec3CatmullRom) with the output vector, four control points and
// the interpolation parameter (stdcall: output, v0, v1, v2, v3, s).
#pragma once
#ifndef BFME2_D3DXVEC3CATMULLROM_H
#define BFME2_D3DXVEC3CATMULLROM_H

struct D3DXVECTOR3;
extern "C" D3DXVECTOR3 *__stdcall D3DXVec3CatmullRom(D3DXVECTOR3 *out, const D3DXVECTOR3 *v0, const D3DXVECTOR3 *v1,
	const D3DXVECTOR3 *v2, const D3DXVECTOR3 *v3, float s);

#endif
