// LaserUpdate.h: the client-side laser update module. SIZE 0x54.
// Target facts: ctor 0x00362EEE (base Rva00362EC7 at 0x00362EC7) installs
// vtable 0x00C170E0, clears +0x24 and the floats +0x0C..+0x20, +0x28..+0x50 and
// sets +0x3C to 1.0f. Destructor 0x00362F64 destroys the two particle systems at
// +0x28/+0x2C through TheParticleSystemManager. xfer 0x00362FF8 (slot 3)
// transfers +0x0C..+0x50 with version 1.2. clientUpdate 0x00363233 widens or
// decays +0x3C between the frame pairs +0x34/+0x38 and +0x40/+0x44 until the
// expiry frame +0x48, then repositions the two systems and the drawable from the
// drawables at +0x4C/+0x50. getCurrentLaserRadius 0x00363193 walks the
// drawable's draw modules for the laser interface. initLaser 0x0036355C starts
// from a bone (or the passed position) and creates the two systems.
#pragma once
#ifndef BFME2_LASERUPDATE_H
#define BFME2_LASERUPDATE_H

#include "Common/GameType.h"
#include "Common/Coord3D.h"
#include "Common/Rva00362EC7.h"
#include "GameClient/ParticleSys.h"

class Drawable;
class LaserUpdateModuleData;
class ModuleData;
class Object;
class Thing;
class Xfer;

class LaserUpdate : public Rva00362EC7
{
public:
	LaserUpdate(Thing *thing, const ModuleData *moduleData);
	virtual ~LaserUpdate();

	virtual void clientUpdate();

	Drawable *getDrawable() const { return m_drawable; }
	const LaserUpdateModuleData *getLaserUpdateModuleData() const { return (const LaserUpdateModuleData *)m_moduleData; }

	Real getCurrentLaserRadius() const;
	void initLaser(const Object *parent, const Coord3D *startPos, const Coord3D *endPos, Int sizeDeltaFrames);
	void initFromDrawables(void *primary, Drawable *parent, Drawable *target, void *d);
	void decayLaser(UnsignedInt frames);
	virtual NameKeyType getModuleNameKey() const;			// slot 4, 0x00363102

	Coord3D m_startPos;									// 0x0C
	Coord3D m_endPos;									// 0x18
	Bool m_dirty;										// 0x24
	UnsignedByte m_unk0x25[3];
	ParticleSystemID m_particleSystemID;				// 0x28
	ParticleSystemID m_targetParticleSystemID;			// 0x2C
	Bool m_widening;									// 0x30
	Bool m_decaying;									// 0x31
	UnsignedByte m_unk0x32[2];
	UnsignedInt m_widenStartFrame;						// 0x34
	UnsignedInt m_widenFinishFrame;						// 0x38
	Real m_currentWidthScalar;							// 0x3C
	UnsignedInt m_decayStartFrame;						// 0x40
	UnsignedInt m_decayFinishFrame;						// 0x44
	UnsignedInt m_expiryFrame;							// 0x48
	DrawableID m_parentID;								// 0x4C
	DrawableID m_targetID;								// 0x50

protected:
	virtual void xfer(Xfer *xfer);						// slot 3
	virtual void loadPostProcess() {}
	virtual const char *GetSnapshotName() const;		// slot 2, 0x00362F5E
};
// SIZE 0x54

#endif
