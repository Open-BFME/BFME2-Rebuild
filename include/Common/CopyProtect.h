// CopyProtect.h: the launcher/copy-protection statics (Common/System/CopyProtection.cpp).
// Target facts: every member is static (the map's SA/SAX spellings): isLauncherRunning
// 0x00232BED, notifyLauncher 0x00232C1F, checkForMessage 0x00232CE7 (unsigned int, long),
// validate 0x00232D0C, shutdown 0x00232D63, and the verified flag accessors in
// src/unk/O0002FB00.cpp: setVerified 0x0002FB00, isVerified 0x0002FB20.
// SubsystemInterface::loadIniFilesFromLegend 0x001B5384 calls setVerified with the
// result of testing for the file "mithriledition.txt".
#pragma once
#ifndef BFME2_COPYPROTECT_H
#define BFME2_COPYPROTECT_H

class CopyProtect
{
public:
	static bool isLauncherRunning();	// 0x00232BED
	static bool notifyLauncher();		// 0x00232C1F
	static void checkForMessage(unsigned int message, long param);	// 0x00232CE7
	static bool validate();				// 0x00232D0C
	static bool isProtectedDataValid();	// 0x00232D38: the launcher's whole message equals the registry string, or validate() accepts it
	static void shutdown();				// 0x00232D63
	static void setVerified(bool verified);	// 0x0002FB00
	static bool isVerified();			// 0x0002FB20
private:
	static void *s_protectedData;		// VA 0x00DFE724 (?s_protectedData@CopyProtect@@0PAXA): the launcher's mapped message
};

#endif
