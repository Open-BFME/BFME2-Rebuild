// Coord3D.h: SAGE's 3-float world coordinate.
// Target facts: 12 bytes, x +0, y +4, z +8; lengthSqr (RVA 0x00065A1A) is out
// of line in retail, set/add are inline. Only the data-only surface lives here.
// Base: retail spells Coord3D's operands as Coord3DBase (exports
// ?sub@Coord3D@@QAEXPBUCoord3DBase@@@Z, ??4Coord3D@@QAEAAV0@ABUCoord3DBase@@@Z,
// ??0Coord3D@@QAE@ABUCoord3DBase@@@Z), so the three floats live in that base.
// Class key: class. Retail's exports spell it V (?expandBy@Region3D@@QAEXABVCoord3D@@@Z,
// ??0Matrix4D@@QAE@ABVCoord3D@@@Z) and so do 1,118 of the recorded names (13 spell U).
// A forward declaration does not move the key in this compiler; the definition decides
// the key. Other headers also forward-declare 'class Coord3D;'.
#pragma once
#ifndef BFME2_COORD3D_H
#define BFME2_COORD3D_H

#include "Common/Coord3DBase.h"

class Coord3D : public Coord3DBase
{
public:
	// The special members are user-declared and inline.
	// Empty default constructor (FXListAtBonePosFXNugget::doFxAtBones 0x001E2DD6 builds
	// Coord3D[40] through the EH vector-constructor iterator with the fold 0x0047A6A9),
	// memberwise copy (retail copies with scalar moves. TeamPrototype::moveTeamTo
	// 0x0039F03A passes one by value float by float; the movsd copies are assignments, Zero Hour's
	// `Coord3D origin; origin = *p;`: Weapon 0x002C961F), three floats
	// (Object::Get2DBorderVectorTo 0x002C986B), and from the base (export
	// ??0Coord3D@@QAE@ABUCoord3DBase@@@Z).
	Coord3D() {}
	Coord3D(Real ax, Real ay, Real az) { x = ax; y = ay; z = az; }
	Coord3D(const Coord3D &that) { x = that.x; y = that.y; z = that.z; }
	Coord3D(const Coord3DBase &that) { x = that.x; y = that.y; z = that.z; }
	// Empty destructor: doFxAtBones 0x001E2DD6 hands 0x000B3FD0 (bare ret) to the EH
	// vector-constructor iterator 0x00629512 and destroys the array with 0x00629110;
	// MSVC uses those iterators only for an element type with a destructor (h06).
	~Coord3D() {}

	float lengthSqr() const;
	float length() const;				// out of line (PhysicsBehavior::rva00390E61 0x00390E61)
	void normalize();				// out of line (0x000035B6)
	float Normalize();				// 0x00005A70 (returns the length; Thing::getUnitDirectionVector3D 0x0030A8EE)
	float Normalize2D();			// 0x00005AB2 (x and y only, returns the 2D length)
	float GetLength() const;		// 0x00005A26 (square root of GetLengthSqrd)
	float GetLengthSqrd() const;		// 0x00005A51 (retail keeps this x87 copy beside lengthSqr 0x00065A1A)
	float GetLength2D() const;		// 0x0000599F
	float GetLengthSqrd2D() const;		// 0x000059BF
	float GetLengthEstimate() const;	// 0x00003ACE (map ?GetLengthEstimate@Coord3D@@QBEMXZ, N:ledger; AIGuardReturnState::onEnter 0x00543CC4; decl_queue f04 18:00, ACCEPTED k11)
	void sub(const Coord3DBase *a) { x -= a->x; y -= a->y; z -= a->z; }

	void zero() { x = 0.0f; y = 0.0f; z = 0.0f; }
	void scale(float s) { x *= s; y *= s; z *= s; }
	bool operator==(const Coord3D &that) const
	{
		return x == that.x && y == that.y && z == that.z;
	}
	bool operator!=(const Coord3D &that) const
	{
		return x != that.x || y != that.y || z != that.z;
	}
	void set(const Coord3DBase *a) { x = a->x; y = a->y; z = a->z; }
	void set(float ax, float ay, float az) { x = ax; y = ay; z = az; }
	void add(const Coord3DBase *a) { x += a->x; y += a->y; z += a->z; }
	// Zero Hour's (BaseType.h); Thing::setOrientation 0x0030AB9D expands it twice.
	static void crossProduct( const Coord3D *a, const Coord3D *b, Coord3D *r )
	{
		r->x = (a->y * b->z - a->z * b->y);
		r->y = (a->z * b->x - a->x * b->z);
		r->z = (a->x * b->y - a->y * b->x);
	}
};

#endif
