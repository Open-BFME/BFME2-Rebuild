// SubsystemInterfaceList.h: the engine's ordered list of subsystems (TheSubsystemList).
// Target facts: the constructor 0x001B4FB8 builds three vectors at +0x00, +0x0C and
// +0x18 and clears the dword at +0x24; the destructor 0x001B5207 frees their buffers
// in reverse; initSubsystem 0x001B5266 appends a {subsystem, second argument} pair to
// the vector at +0 and loads the subsystem's INI files with the Xfer at +0x24
// (SubsystemInterface::loadIniFilesFromLegend 0x001B5384 reads the same dword).
// Size 0x28. Role names follow Zero Hour's SubsystemInterfaceList where the bodies
// agree (postProcessLoadAll, resetAll); the reload and refresh lists have no Zero Hour twin.
#pragma once
#ifndef BFME2_SUBSYSTEMINTERFACELIST_H
#define BFME2_SUBSYSTEMINTERFACELIST_H

#include <utility>
#include <vector>
#include "Common/GameType.h"
#include "Common/AsciiString.h"

class Xfer;
class SubsystemInterface;

// One registered subsystem: initSubsystem 0x001B5266 stores the subsystem and its
// second argument; 0x001B51C0 destroys the second member through its slot 0.
typedef _STL::pair<SubsystemInterface *, SubsystemInterface *> SubsystemEntry;	// first: the subsystem, second: its owned object (destroyed through its slot 0 by 0x001B51C0)

class SubsystemInterfaceList
{
public:
	SubsystemInterfaceList();	// 0x001B4FB8
	~SubsystemInterfaceList();	// 0x001B5207

	Xfer *getXfer() const { return m_xfer; }

	Bool attemptReload(SubsystemInterface *subsystem, Bool *needsRestart);	// 0x001B4E21 (map ?rva001B4E21@SubsystemInterfaceList@@QAEEPAVSubsystemInterface@@PAE@Z, placeholder): marks the subsystem as reloading around its attemptReload slot
	void postProcessLoadAll();	// 0x001B4E49: postProcessLoad on every subsystem, in registration order
	void removeSubsystem(SubsystemInterface *subsystem);	// 0x001B4E82: takes the subsystem out of the pending-reload list
	void resetAll();			// 0x001B4F6D: reset on every subsystem, last registered first
	void clearReloadStates();	// 0x001B4FE9 (map ?rva001B4FE9@SubsystemInterfaceList@@QAEXXZ, placeholder): clearReloadState on every refreshed subsystem, then empties that list
	void rva001B51C0();			// 0x001B51C0 (map ?rva001B51C0@SubsystemInterfaceList@@QAEXXZ, placeholder; Zero Hour's shutdownAll): deletes the owned object of every entry, last registered first, then empties the list
	void initSubsystem(SubsystemInterface *subsystem, void *owned, const char *path1, const char *path2, const char *directory, Xfer *xfer, AsciiString name);	// 0x001B5266
	unsigned char notifySubsystems();	// 0x001B505F: attempts a reload of every registered and pending subsystem and tells the reloaded ones
	Bool attemptRefreshAll();	// 0x001B5140 (map ?rva001B5140@Rva001B5140@@QAE_NXZ, placeholder): refills the refreshed list with the subsystems whose attemptRefresh slot says so
	void dumpAllToCSV();		// 0x001B4EAB (map ?rva001B4EAB@Rva001B4EAB@@QAEXXZ, placeholder): dumpToCSV on every subsystem with a "%s_%02d.csv" file name
	void queueForReload(SubsystemInterface *subsystem);	// 0x001B5018 (map ?rva001B5018@SubsystemInterfaceList@@QAEXPAVSubsystemInterface@@@Z, placeholder): adds a subsystem that is neither registered nor pending

private:
	_STL::vector<SubsystemEntry> m_subsystems;				// +0x00
	_STL::vector<SubsystemInterface *> m_pendingReload;		// +0x0C
	_STL::vector<SubsystemInterface *> m_refreshed;			// +0x18
	Xfer *m_xfer;											// +0x24 cleared by the constructor, read by loadIniFilesFromLegend 0x001B5384
};

extern SubsystemInterfaceList *TheSubsystemList;	// 0x009FD940

#endif
