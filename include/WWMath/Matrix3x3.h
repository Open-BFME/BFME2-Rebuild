// Target3FBF50/3FC4C2: three12-byte Vector3 rows, Rotate_Z overloads.
// Mathematical class spelling follows the existing matched map and BFME1
// sphereobj/GeneralsMD matrix3.h donor; original target class-name proof is limited.
#pragma once
#include "WWMath/Vector3.h"
class Matrix3x3
{
public:
    void Rotate_Z(float sine, float cosine);
    void Rotate_Z(float angle);
    // Row access; OBBoxClass::Compute_Axis_Aligned_Extent 0x00719D10 reads Basis[i][j].
    __forceinline Vector3 &operator[](int i) { return Row[i]; }
    __forceinline const Vector3 &operator[](int i) const { return Row[i]; }
private:
    Vector3 Row[3];
};
typedef char Matrix3x3SizeCheck[sizeof(Matrix3x3)==36 ? 1 : -1];
