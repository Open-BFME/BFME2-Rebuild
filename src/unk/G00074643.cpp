// Generated from the ledger; map snapshot c591f7146f88c8a8248ebb8ae22f0c978c22aed463db64e05ef57ff0dfae99cc
// object=G00074643 name-evidence=invented
// Extents and edges are hypotheses; byte verification alone does not establish original TU boundaries.
#include "WWMath/Matrix3D.h"
// FUNCTION: BFME2 0x00074643 size=133 name=?Scale@Matrix3D@@QAEXM@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
void Matrix3D::Scale(float scale)
{
	Row[0][0] *= scale;
	Row[1][0] *= scale;
	Row[2][0] *= scale;
	Row[0][1] *= scale;
	Row[1][1] *= scale;
	Row[2][1] *= scale;
	Row[0][2] *= scale;
	Row[1][2] *= scale;
	Row[2][2] *= scale;
}

// FUNCTION: BFME2 0x000746C8 size=140 name=?Scale@Matrix3D@@QAEXMMM@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
void Matrix3D::Scale(float x, float y, float z)
{
	Row[0][0] *= x;
	Row[1][0] *= x;
	Row[2][0] *= x;
	Row[0][1] *= y;
	Row[1][1] *= y;
	Row[2][1] *= y;
	Row[0][2] *= z;
	Row[1][2] *= z;
	Row[2][2] *= z;
}

// FUNCTION: BFME2 0x00074754 size=145 name=?Scale@Matrix3D@@QAEXAAVVector3@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
void Matrix3D::Scale(const Vector3 &scale)
{
	Row[0][0] *= scale[0];
	Row[1][0] *= scale[0];
	Row[2][0] *= scale[0];
	Row[0][1] *= scale[1];
	Row[1][1] *= scale[1];
	Row[2][1] *= scale[1];
	Row[0][2] *= scale[2];
	Row[1][2] *= scale[2];
	Row[2][2] *= scale[2];
}

// FUNCTION: BFME2 0x000747E5 size=23 name=?rva000747E5@Rva000747E5@@QAEXXZ evidence=?:placeholder tier=B hypothesis:mm1-gap-unit
// FUNCTION: BFME2 0x000747FC size=227 name=?rva000747FC@Rva000747FC@@QAEXPAVVector3@@HPBV2@M_N@Z evidence=?:placeholder tier=B hypothesis:mm1-gap-unit
// FUNCTION: BFME2 0x000748DF size=23 name=?rva000748DF@Rva000747E5@@QAEXH@Z evidence=?:placeholder tier=B hypothesis:mm1-gap-unit
