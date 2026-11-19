// RAMFile.h: a File whose whole contents live in memory. SIZE 0x20.
// Target facts (retail game.dat): constructor 0x006054E7 calls File::File
// 0x006024FD, zeroes the three 4-byte fields at +0x14/+0x18/+0x1C and stores
// table 0x00C7AA00 at +0; destructor 0x00605504 restores that table,
// array-deletes +0x14, calls File::close 0x0060259A and then File::~File
// 0x006025CE, which restores File's own table. LocalFile::convertToRAMFile
// pushes 0x20 before allocating and dispatches open(File*) through +0x44, so
// sizeof(RAMFile) is 0x20 with File at +0..+0x13.
// Table 0x00C7AA00 has 20 slots: 0 deleting destructor 0x006056A1 (calls
// 0x00605504), 1 open(const char*,int) 0x006056BD, 2 close 0x00605547,
// 3 read 0x00605564, 4 write 0x00574473, 5 seek 0x006055AA, 6 nextLine
// 0x006055E7, 7 scanInt 0x0060580C, 8 scanReal 0x006058CD, 9 scanString
// 0x006056FA, 10 File::print 0x00602451, 11 File::size 0x00602412,
// 12 File::position 0x00602447, 13 readEntireAndClose 0x00605682,
// 14 convertToRAMFile 0x0047A6A9 (`mov eax,ecx; ret`), 15 File::lock
// 0x006024CA, 16 File::unlock 0x006024EE, 17 open(File*) 0x006059B5,
// 18 openFromArchive 0x0060577A, 19 copyDataToFile 0x0060565E.
// read 0x00605564 loads +0x14 as the buffer, +0x18 as its cursor and +0x1C as
// the extent; seek 0x006055AA selects +0x1C for END and +0x18 for CURRENT;
// copyDataToFile 0x0060565E passes +0x1C/+0x14 to File::write. Field names are
// Zero Hour's (Common/RAMFile.h: m_data, m_pos, m_size).
#pragma once
#ifndef BFME2_RAMFILE_H
#define BFME2_RAMFILE_H

#include <stddef.h>
#include "Common/File.h"

class RAMFile : public File
{
public:
	RAMFile();															// 0x006054E7
	virtual ~RAMFile();													// 0x00605504

	virtual bool open(const char *filename, int access = 0);			// slot 1, 0x006056BD
	virtual void close();												// slot 2, 0x00605547
	virtual int read(void *buffer, int bytes);							// slot 3, 0x00605564
	virtual int write(const void *buffer, int bytes);					// slot 4, 0x00574473
	virtual int seek(int pos, seekMode mode = CURRENT);					// slot 5, 0x006055AA
	virtual void nextLine(char *buffer = 0, int bytes = 0);				// slot 6, 0x006055E7
	virtual bool scanInt(int &value);									// slot 7, 0x0060580C
	virtual bool scanReal(float &value);								// slot 8, 0x006058CD
	virtual bool scanString(AsciiString &value);						// slot 9, 0x006056FA
	virtual char *readEntireAndClose();									// slot 13, 0x00605682
	virtual File *convertToRAMFile() { return this; }					// slot 14, fold 0x0047A6A9
	virtual bool open(File *file);										// slot 17, 0x006059B5
	virtual bool openFromArchive(File *archiveFile, const AsciiString &filename, int offset, int size);	// slot 18, 0x0060577A
	virtual bool copyDataToFile(File *file);							// slot 19, 0x0060565E

protected:
	char *m_data;														// 0x14
	int m_pos;															// 0x18
	int m_size;															// 0x1C
};
typedef char RAMFileSize[sizeof(RAMFile) == 0x20 ? 1 : -1];

#endif
