// Generated from the ledger; map snapshot c591f7146f88c8a8248ebb8ae22f0c978c22aed463db64e05ef57ff0dfae99cc
// object=G00098CC0 name-evidence=invented
// Extents and edges are hypotheses; byte verification alone does not establish original TU boundaries.
// FUNCTION: BFME2 0x00098CC0 size=202 name=?Rotate_Z@Matrix3D@@QAEXM@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
#include "WWMath/Matrix3D.h"
#include <math.h>

inline void Matrix3D::Rotate_Z(float s, float c)
{
	for (int i = 0; i < 3; ++i) {
		float tmp1 = Row[i][0];
		float tmp2 = Row[i][1];
		Row[i][0] = c * tmp1 + s * tmp2;
		Row[i][1] = c * tmp2 - s * tmp1;
	}
}

void Matrix3D::Rotate_Z(float theta)
{
	Rotate_Z(sinf(theta), cosf(theta));
}
