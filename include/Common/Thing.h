// Thing.h: SAGE common base of Object and Drawable. SIZE 0x60.
// Target facts:
// ??0Thing 0x0030A188 stores vftable 0x00C086E0 (8 slots), ~Thing 0x0030A120.
//  0 calculateHeightAboveTerrain 0x0030A4AC; 1-4 return null in Thing, Object's
//  table returns this in 1-2, Drawable's in 3-4 (MSVC places each overload pair
//  in reverse declaration order); 5 reactToTransformChange (pure); 6 unnamed `ret`;
//  7 the scalar deleting destructor.
#pragma once
#ifndef BFME2_THING_H
#define BFME2_THING_H

#include "Common/GameType.h"
#include "Common/Coord3D.h"
#include "WWMath/Matrix3D.h"

class Drawable;
class Object;
class ThingTemplate;
template <int> class BitFlags;
enum KindOfType;

class Thing
{
public:
	Thing(const ThingTemplate *thingTemplate);

	friend inline Object *AsObject(Thing *thing) { return thing ? thing->asObjectMeth() : 0; }
	friend inline Drawable *AsDrawable(Thing *thing) { return thing ? thing->asDrawableMeth() : 0; }
	friend inline const Object *AsObject(const Thing *thing) { return thing ? thing->asObjectMeth() : 0; }
	friend inline const Drawable *AsDrawable(const Thing *thing) { return thing ? thing->asDrawableMeth() : 0; }

	const ThingTemplate *getTemplate() const { return m_template; }
	// (SquishCollide.cpp rows 0x004BB976, 0x004BB980: the template's +0x608/+0x60C forwarded; out of line):
	Real getRamPower() const;	// 0x004BB976
	Real getRamZMult() const;	// 0x004BB980
	void setPosition(const Coord3D *pos);
	void setPositionZ(Real z);
	void setOrientation(Real angle);
	const Coord3D *getPosition() const { return &m_cachedPos; }
	Real getOrientation() const { return m_cachedAngle; }
	const Coord3D *getUnitDirectionVector2D() const;
	void getUnitDirectionVector2D(Coord3D &dir) const;
	void getUnitDirectionVector3D(Coord3D &dir) const;
	const Matrix3D *getTransformMatrix() const { return &m_transform; }
	void setTransformMatrix(const Matrix3D *mx);
	Real getHeightAboveTerrain() const;
	Real getHeightAboveTerrainOrWater() const;
	// 0x0030AD8B: Zero Hour's isSignificantlyAboveTerrain with BFME 2's extra height and a
	// wall-climber/ship exclusion (KindOf byte +0x11F & 0x81); -9 * gravity is "three frames to land".
	Bool isSignificantlyAboveTerrain(Real extraHeight) const;
	Bool isAboveTerrain() const;
    Bool isKindOfMulti(const BitFlags<218> &,const BitFlags<218> &) const; // 30AD7D; retail kind-of mask
    Bool isAnyKindOf(const BitFlags<218> &anyKindOf) const; // 0x0030ADC7 (COMDAT)
	// Zero Hour's convenience test through the template, inline in BFME 2: a constant kind compiles to one byte
	// test on the template (AIPlayer getPlayerStructureBounds 0x004F1B8C `mov ecx,[obj+4]; test byte [ecx+0x108],0x80`,
	// PropagandaTowerBehavior::update 0x00481C0C `test cl,0xA0; test ch,3` for four kinds). Its out-of-line copy
	// 0x000456C1 (`mov ecx,[ecx+4]; jmp ThingTemplate::isKindOf 0x000456AC`) has no caller in retail. Defined after
	// ThingTemplate in Common/ThingTemplate.h.
	inline Bool isKindOf(KindOfType t) const;
	void transformPoint(const Coord3D *in, Coord3D *out) const;
	void convertBonePosToWorldPos(const Coord3D *bonePos, const Matrix3D *boneTransform,
		Coord3D *worldPos, Matrix3D *worldTransform) const;
	Drawable *getDrawable() const;

protected:
	virtual Real calculateHeightAboveTerrain() const;									// slot 0
	virtual Object *asObjectMeth() { return 0; }										// slot 2
	virtual Drawable *asDrawableMeth() { return 0; }									// slot 4
	virtual const Object *asObjectMeth() const { return 0; }							// slot 1
	virtual const Drawable *asDrawableMeth() const { return 0; }						// slot 3
	virtual void reactToTransformChange(const Matrix3D *oldMtx, const Coord3D *oldPos, Real oldAngle) = 0;	// slot 5
	virtual void vslot6() {}															// slot 6
	virtual ~Thing();																	// slot 7

public:
	const ThingTemplate *m_template;				// 0x04
	Matrix3D m_transform;							// 0x08
	Coord3D m_cachedPos;							// 0x38
	Real m_cachedAngle;								// 0x44
	// Lazily filled by the const getters (getUnitDirectionVector2D 0x0030A25F,
	// getHeightAboveTerrain 0x0030A12D, getHeightAboveTerrainOrWater 0x0030A4D0),
	// which set bits 1, 2 and 4 of m_cacheValidBits.
	mutable Coord3D m_headingVectorCache;				// 0x48
	mutable Real m_groundClearanceCache;		// 0x54
	mutable Real m_surfaceClearanceCache;	// 0x58
	mutable Int m_cacheValidBits;						// 0x5C

	// VA 0x00DFF4A8: the ctor 0x0030A188 increments it, ~Thing 0x0030A120
	// decrements it (map: placeholder g_Rva0030A120Count).
	static Int s_liveCount;
};
// SIZE 0x60

#endif
