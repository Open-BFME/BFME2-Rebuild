// v3_rnd.h: WWMath's random-point generators over a volume (WWVegas/WWMath/v3_rnd.h).
// Target facts (retail game.dat): Vector3Randomizer is an abstract base with a virtual
// destructor (the shared deleting body 0x007235D0) and five pure slots after it: Class_ID,
// Get_Vector, Get_Maximum_Extent, Scale, Clone. Class_ID returns 0 (solid box, folded with
// a bare `xor eax,eax`), 1 (solid sphere), 2 (hollow sphere 0x0013C5F0) and 3 (solid
// cylinder 0x0013C650). The generators draw from one shared Random3Class
// (Vector3Randomizer::Randomizer, VA 0x00E1D070) and scale each signed draw by 2^-31.
// Layouts from the constructors and Clone bodies: SolidBox is vptr +0 and the extents
// +4/+8/+0xC (SIZE 0x10, extra slot 6 Get_Extents 0x00723140); SolidSphere is vptr and the
// radius +4 (SIZE 8, slot 6 Get_Radius, folded with Get_Maximum_Extent 0x007234A0);
// HollowSphere has the same layout and its own Scale and Get_Maximum_Extent (the linker
// folded the identical bodies, which is why its table names SolidSphere's);
// SolidCylinder is vptr, the extent +4 and the radius +8 (SIZE 0xC, slots 6 Get_Radius
// 0x00723490 and 7 Get_Height 0x007234A0).
#pragma once
#ifndef BFME2_V3_RND_H
#define BFME2_V3_RND_H

#include "WWMath/Vector3.h"
#include "WWLib/random.h"

class Vector3Randomizer
{
public:
	enum
	{
		CLASSID_UNKNOWN = 0xFFFFFFFF,
		CLASSID_SOLIDBOX = 0,
		CLASSID_SOLIDSPHERE = 1,
		CLASSID_HOLLOWSPHERE = 2,
		CLASSID_SOLIDCYLINDER = 3
	};

	virtual ~Vector3Randomizer() {}							// slot 0: 0x007235D0
	virtual unsigned int Class_ID() const = 0;					// slot 1
	virtual void Get_Vector(Vector3 &vector) = 0;					// slot 2
	virtual float Get_Maximum_Extent() = 0;						// slot 3
	virtual void Scale(float scale) = 0;						// slot 4
	virtual Vector3Randomizer *Clone() const = 0;					// slot 5

protected:
	static Random3Class Randomizer;							// VA 0x00E1D070
};
// SIZE 0x04

class Vector3SolidBoxRandomizer : public Vector3Randomizer
{
public:
	Vector3SolidBoxRandomizer(const Vector3 &extents);				// 0x007230F0
	virtual unsigned int Class_ID() const { return CLASSID_SOLIDBOX; }
	virtual void Get_Vector(Vector3 &vector);					// 0x00723150
	virtual float Get_Maximum_Extent();						// 0x007231C0
	virtual void Scale(float scale);						// 0x007231F0
	virtual Vector3Randomizer *Clone() const;					// 0x00723690
	virtual const Vector3 &Get_Extents() const { return Extents; }			// slot 6: 0x00723140

private:
	Vector3 Extents;								// +0x04
};
// SIZE 0x10

class Vector3SolidSphereRandomizer : public Vector3Randomizer
{
public:
	Vector3SolidSphereRandomizer(float radius);					// 0x00723230
	Vector3SolidSphereRandomizer(const Vector3SolidSphereRandomizer &other) : Radius(other.Radius) {}
	virtual unsigned int Class_ID() const { return CLASSID_SOLIDSPHERE; }
	virtual void Get_Vector(Vector3 &vector);					// 0x00723260
	virtual float Get_Maximum_Extent() { return Radius; }				// 0x007234A0
	virtual void Scale(float scale) { scale = (scale > 0.0f) ? scale : 0.0f; Radius *= scale; }	// 0x00723320
	virtual Vector3Randomizer *Clone() const;					// 0x007235F0
	virtual float Get_Radius() const { return Radius; }				// slot 6: 0x007234A0

protected:
	float Radius;									// +0x04
};
// SIZE 0x08

class Vector3HollowSphereRandomizer : public Vector3Randomizer
{
public:
	Vector3HollowSphereRandomizer(float radius);					// 0x00723350
	Vector3HollowSphereRandomizer(const Vector3HollowSphereRandomizer &other) : Radius(other.Radius) {}
	virtual unsigned int Class_ID() const { return CLASSID_HOLLOWSPHERE; }	// 0x0013C5F0
	virtual void Get_Vector(Vector3 &vector);					// 0x00723380
	virtual float Get_Maximum_Extent() { return Radius; }
	virtual void Scale(float scale) { scale = (scale > 0.0f) ? scale : 0.0f; Radius *= scale; }
	virtual Vector3Randomizer *Clone() const;					// 0x00723630
	virtual float Get_Radius() const { return Radius; }

private:
	float Radius;									// +0x04
};
// SIZE 0x08

class Vector3SolidCylinderRandomizer : public Vector3Randomizer
{
public:
	Vector3SolidCylinderRandomizer(float extent, float radius);			// 0x00723450
	Vector3SolidCylinderRandomizer(const Vector3SolidCylinderRandomizer &other) : Extent(other.Extent), Radius(other.Radius) {}
	virtual unsigned int Class_ID() const { return CLASSID_SOLIDCYLINDER; }	// 0x0013C650
	virtual void Get_Vector(Vector3 &vector);					// 0x007234B0
	virtual float Get_Maximum_Extent();						// 0x00723570
	virtual void Scale(float scale);						// 0x007235A0
	virtual Vector3Randomizer *Clone() const;					// 0x00723660
	virtual float Get_Radius() const { return Radius; }				// slot 6: 0x00723490
	virtual float Get_Height() const { return Extent; }				// slot 7: 0x007234A0

private:
	float Extent;									// +0x04
	float Radius;									// +0x08
};
// SIZE 0x0C

#endif
