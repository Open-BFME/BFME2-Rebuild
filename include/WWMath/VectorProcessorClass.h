// VectorProcessorClass.h: WWMath's batch vector helpers (WWVegas/WWMath/vp.h).
// Static members only; every body is out of line in WWMath/vp.cpp
// (object O0071CE10, 0x0071CE10-0x0071DDE8), which retail builds with SSE.
// Overload order and spellings follow the map rows of that object.
#pragma once
#ifndef BFME2_VECTORPROCESSORCLASS_H
#define BFME2_VECTORPROCESSORCLASS_H

class Vector2;
class Vector3;
class Vector4;
class Matrix3D;
class Matrix4;

class VectorProcessorClass
{
public:
	static void Transform(Vector3 *dst, const Vector3 *src, const Matrix3D &matrix, const int count);			// 0x0071CE10
	static void Transform(Vector4 *dst, const Vector3 *src, const Matrix4 &matrix, const int count);			// 0x0071CFA0
	static void Copy(Vector2 *dst, const Vector2 *src, const int count);										// 0x0071D3E0
	static void Copy(unsigned int *dst, const unsigned int *src, const int count);								// 0x0071D400
	static void Copy(Vector3 *dst, const Vector3 *src, const int count);										// 0x0071D420
	static void Copy(Vector4 *dst, const Vector4 *src, const int count);										// 0x0071D450
	static void Copy(Vector4 *dst, const Vector3 *src, const float *srca, const int count);					// 0x0071D480
	static void Copy(Vector4 *dst, const Vector3 *src, const float srca, const int count);						// 0x0071D580
	static void Copy(Vector4 *dst, const Vector3 &src, const float *srca, const int count);					// 0x0071D5D0
	static void CopyIndexed(unsigned int *dst, const unsigned int *src, const unsigned int *index, const int count);	// 0x0071D6C0
	static void CopyIndexed(Vector3 *dst, const Vector3 *src, const unsigned int *index, const int count);		// 0x0071D6F0
	static void CopyIndexed(Vector4 *dst, const Vector4 *src, const unsigned int *index, const int count);		// 0x0071D7F0
	static void CopyIndexed(unsigned char *dst, const unsigned char *src, const unsigned int *index, const int count);	// 0x0071D910
	static void CopyIndexed(float *dst, float *src, const unsigned int *index, const int count);				// 0x0071D950
	static void Clamp(Vector4 *dst, const Vector4 *src, const float min, const float max, const int count);	// 0x0071D9F0
	static void Clear(Vector3 *dst, const int count);															// 0x0071DAE0
	static void MinMax(Vector3 *src, Vector3 &min, Vector3 &max, const int count);								// 0x0071DB10
	static void MulAdd(float *dest, float multiplier, float add, const int count);								// 0x0071DC00
	static void DotProduct(float *dst, const Vector3 &a, const Vector3 *b, const int count);					// 0x0071DCA0
	static void CopyIndexed(Vector2 *dst, const Vector2 *src, const unsigned int *index, const int count);
	static void Normalize(Vector3 *dst, const int count);
	static void ClampMin(float *dst, float *src, const float min, const int count);
	static void Power(float *dst, float *src, const float pow, const int count);
};

#endif
