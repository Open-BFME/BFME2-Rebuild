// INIFileRecordTable.h: the 0x18-byte record table at INIFileTable +0x04 (built by 0x00524415).
// Target facts: INIFileTable::getFileId 0x006018B6, getLine 0x006018DC and getName 0x00601903
// range-check an index against (m_end - m_begin) / 12 and read the 12-byte records
// {file id +0, line +4, name index +8}; getRecordName 0x0060188C copies m_names[nameIndex]
// (AsciiString copy 0x000365F0). The record vector sits at +0, the name vector at +0x0C.
// Decl_queue f09 14:53 (CORRECTION of 14:44), ACCEPTED k10.
#pragma once
#ifndef BFME2_INIFILERECORDTABLE_H
#define BFME2_INIFILERECORDTABLE_H

#include "Common/AsciiString.h"

struct INIFileRecord
{
	unsigned int m_fileId;		// +0x00
	int m_line;			// +0x04
	int m_nameIndex;		// +0x08
};

class INIFileRecordTable
{
public:
	AsciiString getRecordName(int fileIndex) const;	// 0x0060188C

	INIFileRecord *m_begin;		// +0x00 the record vector
	INIFileRecord *m_end;		// +0x04
	INIFileRecord *m_capacity;	// +0x08
	AsciiString *m_names;		// +0x0C the name vector; its begin is the only name field the bodies read
	AsciiString *m_namesEnd;	// +0x10
	AsciiString *m_namesCapacity;	// +0x14
};
typedef char INIFileRecordTable_size[(sizeof(INIFileRecordTable) == 0x18) ? 1 : -1];

#endif
