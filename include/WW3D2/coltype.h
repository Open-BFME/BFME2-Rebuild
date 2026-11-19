// coltype.h: the collision type bit field of WW3D2's render object collision tests.
// Values from Zero Hour's coltype.h (a test's collision type is ANDed with the render object's; COLL_TYPE_ALL is
// the always-set low bit). Retail's RenderObjClass::Intersect 0x0013C3B0 stores the word 1 (COLL_TYPE_ALL) into the
// ray test's CollisionType at +0x04.
// Portions derived from Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
#pragma once
#ifndef BFME2_COLTYPE_H
#define BFME2_COLTYPE_H

enum
{
	COLL_TYPE_ALL = 0x01,		// perform this test against everything
	COLL_TYPE_0 = 0x02,
	COLL_TYPE_1 = 0x04,
	COLL_TYPE_2 = 0x08,
	COLL_TYPE_3 = 0x10,
	COLL_TYPE_4 = 0x20,
	COLL_TYPE_5 = 0x40,
	COLL_TYPE_6 = 0x80,

	COLL_TYPE_PHYSICAL = COLL_TYPE_0,
	COLL_TYPE_PROJECTILE = COLL_TYPE_1,
	COLL_TYPE_VIS = COLL_TYPE_2,
	COLL_TYPE_CAMERA = COLL_TYPE_3,
	COLL_TYPE_VEHICLE = COLL_TYPE_4
};

#endif
