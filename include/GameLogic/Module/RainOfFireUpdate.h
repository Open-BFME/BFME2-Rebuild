// RainOfFireUpdate.h: rains weapon fire over the tactical view and darkens the screen.
// Target facts (old repo Code/GameEngine/Source/GameLogic/Object/Update/RainOfFireUpdateView.h):
// the module is 0x38 bytes: UpdateModule (0x20), a two-slot interface at +0x20,
// then the start frame +0x24, darkness +0x28, scale +0x2C, pending shots +0x30
// and the ramp +0x34. Module data (field table VA 0x00C54B00): StartRainTime
// +0x08, DarknessFadeTime +0x0C, RainEmitterHeight +0x10, DarknessLevel +0x14,
// JitterRadius +0x18, DPSMin +0x1C, DPSMax +0x20, DPSRampupTime +0x24,
// RainOffset +0x28.
#pragma once
#ifndef BFME2_RAINOFFIREUPDATE_H
#define BFME2_RAINOFFIREUPDATE_H

#include "GameLogic/Module/UpdateModule.h"
#include "GameLogic/Module/ModuleData.h"
#include "Common/Coord3D.h"
#include "Common/Coord2D.h"

class RainOfFireUpdateModuleData : public ModuleData
{
public:
	RainOfFireUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &p);

	UnsignedInt m_startRainTime;		// 0x08
	UnsignedInt m_darknessFadeTime;		// 0x0C
	Real m_height;						// 0x10
	Real m_darkness;					// 0x14
	Real m_jitter;						// 0x18
	Real m_dpsMin;						// 0x1C
	Real m_dpsMax;						// 0x20
	Real m_rampup;						// 0x24
	Coord2D m_offset;					// 0x28
};
// SIZE 0x30

class RainOfFireUpdateInterface
{
public:
	virtual void setScale(Real scale) = 0;			// 0 (a shared float-field setter in retail)
	virtual void rva004AC135(Real delta) = 0;		// 1 adds to the scale
};

class RainOfFireUpdate : public UpdateModule, public RainOfFireUpdateInterface
{
public:
	RainOfFireUpdate(Thing *thing, const ModuleData *moduleData);
	virtual ~RainOfFireUpdate();
	virtual NameKeyType getModuleNameKey() const;
	virtual UpdateSleepTime update();

	virtual void rva004AC135(Real delta);
	virtual void setScale(Real scale) { m_scale = scale; }
	virtual RainOfFireUpdateInterface *getRainOfFireUpdateInterface() { return this; }
	void rva004AC18D(Real fadeFraction);		// applies the darkness to TheDisplay

	const RainOfFireUpdateModuleData *getRainOfFireUpdateModuleData() const
	{
		return (const RainOfFireUpdateModuleData *)m_moduleData;
	}

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);
	virtual void loadPostProcess();

	UnsignedInt m_startFrame;	// 0x24
	Real m_darkness;			// 0x28
	Real m_scale;				// 0x2C
	Real m_pending;				// 0x30
	Real m_ramp;				// 0x34
};
// SIZE 0x38

#endif
