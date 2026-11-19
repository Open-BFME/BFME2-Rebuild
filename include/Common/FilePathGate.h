// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// FilePathGate.h: the restricted-file-list gate FileSystem consults before it opens, tests or describes a file (BFME 2
// addition; FileSystem::initialiseRestrictedFilelist 0x00601572 builds it). Target facts: TheFilePathGate is the global at
// 0x00A06A4C; allow 0x00600AE5 answers whether a path may be used.
#pragma once
#ifndef BFME2_FILEPATHGATE_H
#define BFME2_FILEPATHGATE_H

class FilePathGate
{
public:
	bool allow(const char *filename);	// 0x00600AE5
};

extern FilePathGate *TheFilePathGate;

#endif
