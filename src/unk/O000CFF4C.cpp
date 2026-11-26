// cl: /O2
#include <math.h>
#include "WWMath/Vector3.h"
#include "GameClient/W3DStreakDraw.h"
#include "Common/NameKeyGenerator.h"
// Tier B; object=O000CFF4C evidence=invented flags=align=U;ehform=EH_prolog
// Extent is a hypothesis; only Object VERIFIED establishes recovery.
// Edges: left=unproven right=unknown-rows
// W3DStreakDraw's name is the literal class name (retail's six-byte
// `mov eax,<literal>; ret`, the same shape as the other W3D draw names), and
// its unit carries the Vector3::Distance copy its streak maths uses.
// FUNCTION: BFME2 0x000CFF4C size=51 name=?Rva000CFF4C@W3DStreakDraw@@UAEX_N@Z evidence=?:placeholder

// FUNCTION: BFME2 0x000CFF7F size=84 name=?Distance@Vector3@@SAMABV1@0@Z evidence=N:ledger

// FUNCTION: BFME2 0x000CFFD3 size=115 name=??1W3DStreakDraw@@UAE@XZ evidence=N:ledger

// FUNCTION: BFME2 0x000D0046 size=6 name=?name@W3DStreakDraw@@QBEPBDXZ evidence=N:ledger

const char *W3DStreakDraw::GetSnapshotName() const
{
	return "W3DStreakDraw";
}

// FUNCTION: BFME2 0x000D004C size=69 name=?getModuleNameKey@W3DStreakDraw@@UBE?AW4NameKeyType@@XZ evidence=N:ledger
NameKeyType W3DStreakDraw::getModuleNameKey() const
{
	static NameKeyType TheW3DStreakDrawNameKey = TheNameKeyGenerator->nameToKey("W3DStreakDraw");
	return TheW3DStreakDrawNameKey;
}

// FUNCTION: BFME2 0x000D0091 size=534 name=?rva000D0091@W3DStreakDraw@@QAEXXZ evidence=?:placeholder

// FUNCTION: BFME2 0x000D02A7 size=406 name=??0W3DStreakDraw@@QAE@PAVThing@@PBVModuleData@@@Z evidence=N:ledger

// FUNCTION: BFME2 0x000D0459 size=8 name=?rva000D0459@W3DStreakDraw@@QAEXPAX00@Z evidence=?:placeholder
void W3DStreakDraw::reactToTransformChange(const Matrix3D *oldMtx, const Coord3D *oldPos, Real oldAngle)
{
	rva000D0091();
}

// FUNCTION: BFME2 0x000D0461 size=8 name=?rva000D0461@W3DStreakDraw@@QAEXPAX@Z evidence=?:placeholder
void W3DStreakDraw::doDrawModule(const Matrix3D *transformMtx)
{
	rva000D0091();
}
