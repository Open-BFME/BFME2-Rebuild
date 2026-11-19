// Version.h: the game's version record (Zero Hour's Version by role; names are ours where the retail bodies
// and string literals agree, offsets and the size are retail's).
// Target facts:
//  - SIZE 0x30. GameInfo embeds one at +0x90 (GameInfo::GameInfo 0x00400A07 runs the constructor, ~GameInfo 0x00400A7F
//    the destructor, GameInfo::reset 0x003FFF8F rebuilds it through initializeBuildMetadata).
//  - Constructor 0x00238852: seven AsciiStrings at +0x10..+0x28 start empty, then initializeBuildMetadata 0x0023870E
//    runs. The destructor is the compiler-generated one (the seven strings in reverse order), emitted out of line at
//    0x00002152 (the map's ??1Version).
//  - initializeBuildMetadata fills +0..+0xC from four globals (0x00DFE760..0x00DFE76C; the first call also runs
//    initVersionGlobals 0x00238646 once), then asks a VersionBlockParser over the build block at 0x00E25000
//    ("||VER1|512|BUILD=...|CONFIG=Release|DATE=...") for ID (default "sometitle"), MACHINE ("somewhere"), USER
//    ("somebody"), GUID (an all-X pattern), TIME ("15:50:55"), DATE ("Sep 25 2006") and CONFIG ("Release"), storing
//    them at +0x10, +0x14, +0x18, +0x1C, +0x20, +0x24 and +0x28 in that order of the lookups (+0x10, +0x18, +0x14, +0x1C,
//    +0x20, +0x24, +0x28), and clears the byte +0x2C.
//  - Formats: getAsciiVersion 0x00237EC1 prints the four numbers as "%d.%.2d.%d.%d"; getAsciiBuildTime 0x0023819B
//    prints "%s %s" of +0x20 and +0x24 (as their text); getAsciiBuildLocation 0x002382FD copies +0x14;
//    getBuildGuid 0x0009AA0B returns the address of +0x1C.
#pragma once
#ifndef BFME2_VERSION_H
#define BFME2_VERSION_H

#include "Common/GameType.h"
#include "Common/AsciiString.h"

class UnicodeString;

class Version
{
public:
	Version();													// 0x00238852
	void initializeBuildMetadata();								// 0x0023870E
	UnsignedInt getVersionNumber();								// 0x00237E1F (major << 16 | minor; Zero Hour's name)
	void getVersionNumbers(Version *dest);						// 0x00237E28 copies the four numbers into dest
	void setVersion(Int major, Int minor, Int buildNum, Int localBuildNum);	// 0x00237E45 (Zero Hour's setVersion, the four numbers only)
	Int getVersionColor(const Version *other, Bool ignoreSame);	// 0x00237E63 the display colour of other's major version; green or red by whether the numbers match
	AsciiString getAsciiMajorMinor();							// 0x00237F25 ("%d.%d")
	AsciiString getAsciiConfiguredVersion();					// 0x00237F83 ("%d.%d.%d.%d_%s_%s": numbers, build config, build location)
	AsciiString getAsciiVersionNumbers();						// 0x0023851C ("%d.%d.%d.%d")
	AsciiString getAsciiVersion();								// 0x00237EC1 ("%d.%.2d.%d.%d")
	AsciiString getAsciiBuildTime();							// 0x0023819B (build time and date)
	AsciiString getAsciiBuildLocation();						// 0x002382FD
	UnicodeString getUnicodeVersion();							// 0x0023800A (Version:Format2 with major and minor)
	UnicodeString getFullUnicodeVersion();						// 0x0023809E
	UnicodeString getUnicodeBuildTime();							// 0x00238215
	UnicodeString getUnicodeBuildLocation();						// 0x00238318 (Version:BuildMachine)
	UnicodeString getUnicodeBuildUser();							// 0x002383F0 (Version:BuildUser)						// 0x00238215
	AsciiString *getBuildGuid() { return &m_buildGuid; }		// 0x0009AA0B (folded lea getter)

	UnsignedInt m_major;										// +0x00
	UnsignedInt m_minor;										// +0x04
	UnsignedInt m_buildNum;										// +0x08
	UnsignedInt m_localBuildNum;								// +0x0C
	AsciiString m_buildID;										// +0x10 (lookup key "ID")
	AsciiString m_buildLocation;								// +0x14 ("MACHINE")
	AsciiString m_buildUser;									// +0x18 ("USER")
	AsciiString m_buildGuid;									// +0x1C ("GUID")
	AsciiString m_buildTime;									// +0x20 ("TIME")
	AsciiString m_buildDate;									// +0x24 ("DATE")
	AsciiString m_buildConfig;									// +0x28 ("CONFIG")
	Bool m_showFullVersion;										// +0x2C
	unsigned char m_pad2D[3];									// +0x2D
};
// SIZE 0x30
typedef char VersionSize[(sizeof(Version) == 0x30) ? 1 : -1];

extern Version *TheVersion;	// 0x00DFE770 (the pointer the Recorder reads)

#endif
