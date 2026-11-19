// Snapshot.h: SAGE save/load base. SIZE 0x4.
// Target facts: retail vtable 0x00BBB554 is the deleting destructor and three
// __purecall slots, in retail order loadPostProcess, GetSnapshotName, xfer
// (old repo reference/shims/moduledata/Common/Snapshot.h). Retail has no crc slot.
// Constructor 0x000053A6, destructor 0x0049B47C (7 bytes: restore 0x00BBB554).
// The destructor is inline and empty. A module-data chain collapses it into a
// bare vptr store (??1StrafeAreaUpdateModuleData 0x003A4ECF ends in
// `mov [this],0x00BBB554` with no call); the module chains' jump to 0x0049B47C
// (~LaserUpdate 0x00362F64, ~UpdateModule 0x0024A797) is the out-of-line
// ~Module (Module.h), whose body is the same 7 bytes.
#pragma once
#ifndef BFME2_SNAPSHOT_H
#define BFME2_SNAPSHOT_H

class Xfer;
class GameState;

class Snapshot
{
	friend class Xfer;
	friend class GameState;		// GameState::addPostProcessSnapshot 0x002DCF6C calls the protected GetSnapshotName (slot 2)
	friend class XferSave;		// XferSave::operator==(Snapshot &) 0x0060C8FA opens a block named GetSnapshotName()
	friend class XferLoad;		// XferLoad::operator==(Snapshot &) 0x0060C549, the same
public:
	Snapshot() {}
	virtual ~Snapshot() {}					// slot 0; COMDAT 0x0049B47C

protected:
	virtual void loadPostProcess() = 0;			// slot 1
	virtual const char *GetSnapshotName() const = 0;	// slot 2
	virtual void xfer(Xfer *xfer) = 0;			// slot 3
};
// SIZE 0x4

#endif
