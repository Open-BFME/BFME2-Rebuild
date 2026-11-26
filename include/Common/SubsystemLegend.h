// The subsystem legend supplies the INI input lists. The constructor
// 0x0022E0E5 builds SubsystemInterface and the entry list at +0xC.
// The allocation at 0x0022E45E is 0x10 bytes.
#pragma once

#include "Common/SubsystemInterface.h"
#include <list>
#include <vector>

class INI;

// The LoadSubsystem parser at 0x001B4D7F uses the table at 0x007D7710.
// SubsystemInterface::loadIniFilesFromLegend reads four of its vectors.
struct IniLoadFileList
{
	AsciiString m_name;								// +0x00, key compared in 0x001B49C4.
	_STL::vector<AsciiString> m_files;				// +0x04, InitFile.
	_STL::vector<AsciiString> m_directories;			// +0x10, InitPath.
	_STL::vector<AsciiString> m_extensions;			// +0x1C, Extension.
	_STL::vector<AsciiString> m_cinematicExclusions;	// +0x28, IncludePathCinematics.
	_STL::vector<AsciiString> m_exclusions;			// +0x34, ExcludePath.
	Int m_loader;									// +0x40, Loader lookup word.
	AsciiString m_debugInitFile;						// +0x44, InitFileDebug.
};

class SubsystemLegend : public SubsystemInterface
{
public:
	SubsystemLegend();								// 0x0022E0E5
	virtual ~SubsystemLegend();
	virtual void init();								// 0x001B4CD0 calls the entry-list clear.
	virtual void reset() {}							// Slot 9: bare ret.
	virtual void update() {}							// Slot 10: bare ret.

	IniLoadFileList *rva001B49C4(AsciiString name);		// Lookup by subsystem name.
	void parseLoadSubsystem(INI *ini);

private:
	_STL::list<IniLoadFileList> m_entries;				// +0x0C, traversed by 0x001B49C4.
};

typedef char IniLoadFileListSize[(sizeof(IniLoadFileList) == 0x48) ? 1 : -1];
typedef char SubsystemLegendSize[(sizeof(SubsystemLegend) == 0x10) ? 1 : -1];

extern SubsystemLegend *TheSubsystemLegend;			// 0x009FD93C
extern Bool TheSubsystemLegendSkipCinematicExclusions;	// 0x009FD944, read by loadIniFilesFromLegend.

void parseSubsystemLegendDefinition(INI *ini);

