// RadiusDecalUpdate.h: an update module that keeps one radius decal alive.
//
// Layout evidence (RadiusDecalUpdate object O003913E4): UpdateModule is 0x20, the
// RadiusDecal sits at +0x20 (its clear/update/xfer are called through that
// offset from 0x00391515, 0x0039155F, 0x0039157B and 0x003915AE) and the
// kill-when-no-longer-attacking flag is the byte at +0x30. `update` is compiled
// on the UpdateModuleInterface subobject at +0x10, so it reads the object at
// -8, the decal at +0x10 and the flag at +0x20.
#pragma once
#ifndef BFME2_RADIUSDECALUPDATE_H
#define BFME2_RADIUSDECALUPDATE_H

#include "Common/GameType.h"
#include "Common/RadiusDecal.h"
#include "GameLogic/Module/UpdateModule.h"

class Coord3D;
class RadiusDecalTemplate;

class RadiusDecalUpdate : public UpdateModule
{
public:
	RadiusDecalUpdate(Thing *thing, const ModuleData *moduleData);
	virtual ~RadiusDecalUpdate();

	const char *name(void) const;

	virtual NameKeyType getModuleNameKey(void) const;
	virtual UpdateSleepTime update(void);

	void createRadiusDecal(const RadiusDecalTemplate &radiusDecalTemplate, Real radius, const Coord3D &pos);
	void killRadiusDecal(void);

protected:
	virtual void xfer(Xfer *xfer);

private:
	RadiusDecal m_deliveryDecal;				// +0x20
	Bool m_killWhenNoLongerAttacking;			// +0x30
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 0x00497805: a jmp to UpdateModule's, ICF-folded across classes (h05)
protected:
	virtual const char *GetSnapshotName() const;	// 0x0039143F (retail slot 2 of the class table)
};
// SIZE 0x34

#endif
