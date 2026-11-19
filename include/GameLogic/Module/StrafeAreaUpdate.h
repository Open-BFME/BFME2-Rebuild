// StrafeAreaUpdate.h: the strafe-run update module. SIZE 0x48.
// Target facts: the ctor 0x003A4D23 (factory 0x0024D6FF news 0x48) installs
// three vftables and zeroes +0x20..+0x44; the destructor 0x003A4CAE restores
// them and tail-calls 0x0024A797. xfer 0x003A4DC0 transfers the two coordinate
// triples, the sweep offset, the mode, the flag and the counter at version 1.3.
// rva003A4F21 0x003A4F21 starts a strafe run: it stores the target at +0x20 and
// the start at +0x2C, takes the module data's +0x1C phase into +0x38, clears
// +0x3C/+0x40, aims the object along the run and moves the object's AI there.
#pragma once
#ifndef BFME2_STRAFEAREAUPDATE_H
#define BFME2_STRAFEAREAUPDATE_H

#include "Common/GameType.h"
#include "Common/Coord3D.h"
#include "GameLogic/Module/UpdateModule.h"

class ModuleData;
class Thing;
class Xfer;

// The run direction rva003A4F21 builds from the target and start positions; only
// its x and y are used, for the atan2 heading.
struct StrafeDirection
{
	StrafeDirection(const Coord3D &v) : x(v.x), y(v.y), z(v.z) {}
	void sub(const Coord3D &v) { x -= v.x; y -= v.y; z -= v.z; }

	Real x, y, z;
};

class StrafeAreaUpdate : public UpdateModule
{
public:
	StrafeAreaUpdate(Thing *thing, const ModuleData *moduleData);
	virtual ~StrafeAreaUpdate();

	virtual NameKeyType getModuleNameKey() const;		// 0x003A4CCD
	virtual UpdateSleepTime update();					// 0x003A4FDA
	Bool rva003A4F21(const Coord3D *from, const Coord3D *to);	// 0x003A4F21
	const char *name() const;							// 0x003A4CC7

	Coord3D m_target;									// 0x20
	Coord3D m_start;									// 0x2C
	Real m_sweepOffset;									// 0x38
	Int m_strafeMode;									// 0x3C
	Bool m_strafing;									// 0x40
	UnsignedByte m_unk0x41[3];
	UnsignedInt m_strafeCounter;						// 0x44

protected:
	virtual void xfer(Xfer *xfer);						// 0x003A4DC0
protected:
	virtual const char *GetSnapshotName() const;	// slot 2 of the class table, 0x003A4CC7 (h05 r9)
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 1 0x00497805: a jmp to UpdateModule's (h05)
};
// SIZE 0x48

#endif
