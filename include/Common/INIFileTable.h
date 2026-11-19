// INIFileTable.h: the per-file record table behind INI +0x838 (the helper INI's constructor builds).
// Target facts: INI::getFilename 0x0002C005 and INI::getLineNum 0x0002BBED add 0x838 to their
// this and call 0x00601903 (map ?getName@INIFileTable@@QBE?AVAsciiString@@H@Z) and 0x006018DC
// (?getLine@INIFileTable@@QBEHH@Z) with the INI's file index (+0x10). The constructor 0x00601BBC
// stores the vftable 0x00C7A6B0 (slot 0 the deleting destructor 0x00602056), builds the 0x18-byte
// record table at +4 (its constructor 0x00524415; getRecordName 0x0060188C reads 0x0C-byte records
// from its first vector), two int vectors at +0x1C and +0x28, and runs the once-only character-class
// table initialiser 0x006016C9; the destructor 0x00601BF3 clears everything through 0x00601A8D.
// The map still files the constructor, destructor and clear under the placeholder class
// Rva00601BBCHelper (size 0x34).
#pragma once
#ifndef BFME2_INIFILETABLE_H
#define BFME2_INIFILETABLE_H

#include "Common/AsciiString.h"
#include "Common/INIFileRecordTable.h"

class INIFileTable
{
public:
	INIFileTable();						// 0x00601BBC
	virtual ~INIFileTable();			// slot 0: 0x00602056 (deleting), destructor 0x00601BF3

	int getFileId(int index) const;		// 0x006018B6
	int getLine(int index) const;		// 0x006018DC
	AsciiString getName(int index) const;	// 0x00601903

private:
	INIFileRecordTable m_records;		// +0x04 the record table (two vectors; 0x0C-byte records), built by 0x00524415
	unsigned char m_vectors[0x18];		// +0x1C two int vectors (+0x1C and +0x28; their constructor is 0x00211E58)
};
// SIZE 0x34

#endif
