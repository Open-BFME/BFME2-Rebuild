// DynamicShroudClearingRangeUpdate.h: the module that animates an object's
// shroud clearing range. SIZE 0x230.
// Target facts: the module data holds the shrink/grow timing and the decal
// template; this module keeps the state machine from +0x20, the decals-created
// flag at +0x40, the native and current clearing ranges at +0x48/+0x4C and the
// 30-entry decal array at +0x50 (ctor 0x0048B067, killGridDecals 0x0048B03E,
// createGridDecals 0x0048B20E, animate 0x0048B256, update 0x0048B33F,
// xfer 0x0048B505).
#pragma once
#ifndef BFME2_DYNAMICSHROUDCLEARINGRANGEUPDATE_H
#define BFME2_DYNAMICSHROUDCLEARINGRANGEUPDATE_H

#include "Common/GameType.h"
#include "Common/RadiusDecal.h"
#include "GameLogic/Module/DynamicShroudClearingRangeUpdateModuleData.h"
#include "GameLogic/Module/UpdateModule.h"

class Coord3D;
class Thing;

class DynamicShroudClearingRangeUpdate : public UpdateModule
{
public:
	DynamicShroudClearingRangeUpdate(Thing *thing, const ModuleData *moduleData);
	virtual ~DynamicShroudClearingRangeUpdate();

	virtual NameKeyType getModuleNameKey() const;
	virtual UpdateSleepTime update();
	// Own override: the table slot is the fold 0x00497805 of the per-module
	// `{ UpdateModule::loadPostProcess(); }` bodies, not UpdateModule's 0x0058B03E.
	virtual void loadPostProcess();
	virtual void xfer(Xfer *xfer);

	void killGridDecals();
	void createGridDecals(const RadiusDecalTemplate &radiusDecalTemplate, Real radius, const Coord3D &pos);
	void rva0048B256();

	enum DSCRU_STATE
	{
		DSCRU_NOT_STARTED_YET,
		DSCRU_GROWING,
		DSCRU_SUSTAINING,
		DSCRU_SHRINKING,
		DSCRU_DONE_FOREVER,
		DSCRU_SLEEPING
	};

protected:
	const DynamicShroudClearingRangeUpdateModuleData *getDynamicShroudClearingRangeUpdateModuleData() const
	{
		return (const DynamicShroudClearingRangeUpdateModuleData *)m_moduleData;
	}

	DSCRU_STATE m_state;				// 0x20
	int m_stateCountDown;				// 0x24
	int m_totalFrames;					// 0x28
	UnsignedInt m_growStartDeadline;	// 0x2C
	UnsignedInt m_sustainDeadline;		// 0x30
	UnsignedInt m_shrinkStartDeadline;	// 0x34
	UnsignedInt m_doneForeverFrame;		// 0x38
	UnsignedInt m_changeIntervalCountdown;	// 0x3C
	Bool m_decalsCreated;				// 0x40
	unsigned char m_pad0x41[3];			// 0x41
	Real m_visionChangePerInterval;		// 0x44
	Real m_nativeClearingRange;			// 0x48
	Real m_currentClearingRange;		// 0x4C
	RadiusDecal m_gridDecal[30];		// 0x50
protected:
	virtual const char *GetSnapshotName() const;	// 0x0048B164 (retail slot 2 of the class table)
};
// SIZE 0x230

#endif
