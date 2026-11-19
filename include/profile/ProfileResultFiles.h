// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// ProfileResultFiles.h: the profile library's file result functions (Zero Hour's internal_result.h). Target facts (retail
// game.dat): vftables 0x008E85E4 (CSV), 0x008E84D0 (DOT), 0x008E85D8 (GTT), each WriteResults, the shared Delete thunk
// 0x006C74C0 and GetName; the CSV constructor 0x006C75A0 copies its file name to +4, the DOT constructor 0x006C6ED0 its file
// name to +4 (default "profile.dot"), frame name +8 and fold threshold +0xC; the GTT constructor 0x006C74D0 clears +0x10..+0x18
// and copies the same three values; the GTT variant ("file_gtt_dot") is BFME 2's and has no Zero Hour counterpart.
#pragma once
#ifndef BFME2_PROFILERESULTFILES_H
#define BFME2_PROFILERESULTFILES_H

#include "profile/ProfileResultInterface.h"

void *ProfileAllocMemory(unsigned int numBytes);	// 0x006C5460
void *ProfileReAllocMemory(void *oldPtr, unsigned int newSize);	// 0x006C54B0

class ProfileResultFileCSV : public ProfileResultInterface
{
public:
	ProfileResultFileCSV(const char *fileName = 0);					// 0x006C75A0
	static ProfileResultInterface *Create(int argn, const char *const *argv);	// 0x006C80B0
	virtual void WriteResults();									// slot 0: 0x006C7A60
	virtual void Delete();											// slot 1: 0x006C74C0
	virtual const char *GetName() const;							// slot 2: 0x006C75F0

private:
	char *m_fileName;												// +0x04
};

class ProfileResultFileDOT : public ProfileResultInterface
{
public:
	ProfileResultFileDOT(const char *fileName = 0, const char *frameName = 0, int foldThreshold = 999);	// 0x006C6ED0
	static ProfileResultInterface *Create(int argn, const char *const *argv);	// 0x006C7C50
	virtual void WriteResults();									// slot 0: 0x006C6F70
	virtual void Delete();											// slot 1: 0x006C74C0
	virtual const char *GetName() const;							// slot 2: 0x006C7580

private:
	char *m_fileName;												// +0x04
	char *m_frameName;												// +0x08
	int m_foldThreshold;											// +0x0C
};

class ProfileResultFileGTT : public ProfileResultInterface
{
public:
	ProfileResultFileGTT(const char *fileName = 0, const char *frameName = 0, int foldThreshold = 100);	// 0x006C74D0
	static ProfileResultInterface *Create(int argn, const char *const *argv);	// 0x006C7D10
	bool MarkVisited(unsigned int first, unsigned int second);		// 0x006C7440: true when the pair was already marked
	virtual void WriteResults();									// slot 0: 0x006C8120
	virtual void Delete();											// slot 1: 0x006C74C0
	virtual const char *GetName() const;							// slot 2: 0x006C7590

private:
	char *m_fileName;												// +0x04
	char *m_frameName;												// +0x08
	int m_foldThreshold;											// +0x0C
	struct VisitedPair
	{
		unsigned int first;
		unsigned int second;
	};
	VisitedPair *m_visited;											// +0x10 the pairs MarkVisited 0x006C7440 has seen (grown by 1000 entries)
	unsigned int m_visitedCount;									// +0x14
	unsigned int m_visitedAlloc;									// +0x18 capacity in pairs
};

#endif
