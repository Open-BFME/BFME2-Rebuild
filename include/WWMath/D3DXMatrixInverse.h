// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// D3DXMatrixInverse.h: the one D3DX routine WWMath's matrix code calls, declared without the DirectX headers.
// Target facts: Matrix3D::Get_Inverse 0x00713290 and Matrix4::Get_Inverse 0x00715E90 call the statically linked
// _D3DXMatrixInverse@12 (stdcall: output, determinant pointer, input).
#pragma once
#ifndef BFME2_D3DXMATRIXINVERSE_H
#define BFME2_D3DXMATRIXINVERSE_H

struct D3DXMATRIX;
extern "C" D3DXMATRIX *__stdcall D3DXMatrixInverse(D3DXMATRIX *out, float *determinant, const D3DXMATRIX *in);

#endif
