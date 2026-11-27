// Portions derived from Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
#include "WWMath/PlaneClass.h"
// Generated from the ledger; map snapshot c591f7146f88c8a8248ebb8ae22f0c978c22aed463db64e05ef57ff0dfae99cc
// object=G0010022F name-evidence=invented
// Extents and edges are hypotheses; byte verification alone does not establish original TU boundaries.
// FUNCTION: BFME2 0x0010022F size=98 name=?Equal_Within_Epsilon@@YA_NABVVector3@@0M@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
bool Equal_Within_Epsilon(const Vector3 &a, const Vector3 &b, float epsilon)
{
	return (WWMath::Fabs(a.X - b.X) < epsilon) && (WWMath::Fabs(a.Y - b.Y) < epsilon) && (WWMath::Fabs(a.Z - b.Z) < epsilon);
}
// FUNCTION: BFME2 0x00100291 size=52 name=?In_Front@PlaneClass@@QBE_NABVVector3@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
bool PlaneClass::In_Front(const Vector3 &point) const
{
	float dist = Vector3::Dot_Product(point, N);
	return dist > D;
}
// FUNCTION: BFME2 0x0010034F size=5 name=?clear@Rva0010034FDwordClearer@@QAEXXZ evidence=?:placeholder tier=B hypothesis:mm1-gap-unit
