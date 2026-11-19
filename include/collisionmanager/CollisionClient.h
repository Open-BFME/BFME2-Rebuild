// CollisionClient.h: an object as the collision manager sees it. SIZE 0x4.
// Class and method names are PROVISIONAL, from what Object's bodies do.
// Target facts (retail game.dat; functions by RVA, tables by VA):
//  - Its own table 0x00BFBCBC is ten __purecall (ICF-folded); its constructor
//    0x0028AB0D only stores it. Object's ctor stores it at +0x70 inline.
//  - Object's table at +0x70, 0x00BFC290:
//    0 0x0028C037 / 1 0x0028C08D the bounding box's low and high corners by
//      value (position -/+ the geometry's radius and height);
//    2 and 3 0x00299C78 the Object itself; 4 0x005801F2 the ObjectID (+0x74);
//    5 0x0028E84A 1 for a template with kind-of bit 2, otherwise the container
//      (+0x274) or the Object itself, as a handle;
//    6 0x00299C7C stores its argument at +0x4CC, 7 0x00042272 returns it;
//    8 0x0028C0E3 forwards the other object to Object 0x0028B35B (ret 0xC);
//    9 0x0028E868 runs the collide modules for a contact (ret 0xC).
//  - Object 0x0028BAC0: if (+0x4CC) TheCollisionManager->...(this + 0x70).
//    WorldBuilder's library has CollisionManager::Data (UpdateCoord, ~Data);
//    the data pointer stays void * until that class has a header.
#pragma once
#ifndef BFME2_COLLISIONCLIENT_H
#define BFME2_COLLISIONCLIENT_H

#include "Common/GameType.h"

class Object;
class Coord3D;

class CollisionClient
{
public:
	virtual Coord3D CollisionGetBoundsMin() = 0;							// slot 0
	virtual Coord3D CollisionGetBoundsMax() = 0;							// slot 1
	virtual Object *CollisionGetObject() = 0;								// slot 3
	virtual const Object *CollisionGetObject() const = 0;					// slot 2
	virtual ObjectID CollisionGetID() const = 0;							// slot 4
	virtual UnsignedInt CollisionGetGroup() const = 0;						// slot 5
	virtual void CollisionSetData(void *data) = 0;							// slot 6
	virtual void *CollisionGetData() const = 0;								// slot 7
	virtual Bool CollisionShouldCollide(const Object *other, UnsignedInt a,
		UnsignedInt b) const = 0;											// slot 8
	virtual void CollisionOnCollide(Object *other, const Coord3D *loc,
		const Coord3D *normal) = 0;											// slot 9
};
// SIZE 0x4

#endif
