// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// Profile.h: the profile library's range recorder (Zero Hour's profile.h; the functions common to both profilers). Target
// facts (retail game.dat): the recorded frame names are the 0x1C-byte FrameName records of m_frameNames (0x00A0C1E8, count
// m_names 0x00A0C1EC: name +0, frames +4, isRecording +8, doAppend +9, funcIndex +0xC, highIndex +0x10, lastGlobalIndex
// +0x18, as StartRange 0x006C5940 initializes them), the recorded range names are m_recNames 0x00A0C1F0 / m_rec 0x00A0C1F4,
// the pattern list is firstPatternEntry 0x00A0C1E4 (entries: next +0, isActive +4, pattern +8; matched with
// Debug::SimpleMatch 0x00039480) and the clock rate is the 64-bit m_clockCycles 0x00A0C200.
#pragma once
#ifndef BFME2_PROFILE_H
#define BFME2_PROFILE_H

class Profile
{
	friend class ProfileCmdInterface;
	Profile();

public:
	static void StartRange(const char *range = 0);			// 0x006C5940
	static void AppendRange(const char *range = 0);			// 0x006C5B50
	static void StopRange(const char *range = 0);			// 0x006C57B0
	static bool IsEnabled();								// 0x006C5380
	static unsigned int GetFrameCount();					// 0x006C53B0
	static const char *GetFrameName(unsigned int frame);	// 0x006C53C0
	static void ClearTotals();								// 0x006C5410
	static __int64 GetClockCyclesPerSecond();				// 0x006C53E0

private:
	struct FrameName
	{
		char *name;
		unsigned int frames;
		bool isRecording;
		bool doAppend;
		int index0C;				// +0xC set to -1 with the two below when no pattern is active (a third frame index; role not recovered)
		int highIndex;				// +0x10 ProfileId::FrameStart
		int funcIndex;				// +0x14 ProfileFuncLevelTracer::FrameStart
		int lastGlobalIndex;		// +0x18
	};
	struct PatternListEntry
	{
		PatternListEntry *next;
		bool isActive;
		char *pattern;
		int count;					// +0xC remaining uses; the entry is removed when it reaches 0 (StartRange 0x006C5940)
	};

	static void RemovePatternEntry(PatternListEntry *entry);	// 0x006C5310 (held at 98 percent)

	static PatternListEntry *firstPatternEntry;				// 0x00A0C1E4
	static PatternListEntry *lastPatternEntry;
	static unsigned int m_rec;								// 0x00A0C1F4
	static char **m_recNames;								// 0x00A0C1F0
	static unsigned int m_names;							// 0x00A0C1EC
	static FrameName *m_frameNames;							// 0x00A0C1E8
	static __int64 m_clockCycles;							// 0x00A0C200
};

#endif
