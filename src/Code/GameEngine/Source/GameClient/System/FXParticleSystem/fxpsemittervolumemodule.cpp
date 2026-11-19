// cl: /Oy-
#include "Common/Coord3D.h"
#include "Common/RandomValue.h"
// Tier A; object=O003AFA64 evidence=W flags=align=A
// Extent is a hypothesis; only Object VERIFIED establishes recovery.
// Edges: left=unknown-rows right=unknown-rows
// A random unit vector for the emitter volumes: three client random values in
// [-1,1), retried while all three are zero, then normalised into the caller's
// coordinate.
// FUNCTION: BFME2 0x003AFA64 size=198 name=?Rva003AFA64FillUnitVector@@YAPAUCoord3D@@PAU1@@Z evidence=?:placeholder

Coord3D *Rva003AFA64FillUnitVector(Coord3D *out)
{
	Coord3D tmp;
	do
	{
		tmp.x = GetGameClientRandomValueReal(-1.0f, 1.0f, (char *)"C:\\projects\\bfme2patch103\\bfme2\\Code\\GameEngine\\Source\\GameClient\\System\\FXParticleSystem\\fxpsemittervolumemodule.cpp", 31);
		tmp.y = GetGameClientRandomValueReal(-1.0f, 1.0f, (char *)"C:\\projects\\bfme2patch103\\bfme2\\Code\\GameEngine\\Source\\GameClient\\System\\FXParticleSystem\\fxpsemittervolumemodule.cpp", 32);
		tmp.z = GetGameClientRandomValueReal(-1.0f, 1.0f, (char *)"C:\\projects\\bfme2patch103\\bfme2\\Code\\GameEngine\\Source\\GameClient\\System\\FXParticleSystem\\fxpsemittervolumemodule.cpp", 33);
	}
	while (tmp.x == 0.0f && tmp.y == 0.0f && tmp.z == 0.0f);

	tmp.normalize();
	out->x = tmp.x;
	out->y = tmp.y;
	out->z = tmp.z;
	return out;
}
