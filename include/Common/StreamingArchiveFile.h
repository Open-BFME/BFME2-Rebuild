// StreamingArchiveFile.h: a File view over a byte range of another File.
// SIZE 0x2C.
// Target facts (retail game.dat): constructor 0x00605A28 runs RAMFile's
// constructor 0x006054E7, zeroes m_file/m_startingPos/m_curPos at +0x20/+0x24/
// +0x28 and stores table 0x00C7AA50 at +0; destructor 0x00605A45 restores that
// table, calls File::close 0x0060259A and then RAMFile::~RAMFile 0x00605504.
// Its table is RAMFile's 20 slots with 0 (deleting destructor 0x00605B1A),
// 1 (open 0x00605B36), 2 (close 0x00605A7D, a 5-byte tail jump to File::close,
// folded with LocalFile::close), 3 (read 0x00605A82), 5 (seek 0x00605ADD) and
// 18 (openFromArchive 0x00605B68) overridden; open(File*) stays RAMFile's.
// read locks the inner file, seeks it to m_startingPos + m_curPos, clamps the
// byte count to m_size - m_curPos, reads, unlocks and advances m_curPos.
// open(const char*,int) opens through TheFileSystem and hands the result to
// open(File*); openFromArchive validates by seeking and re-reading.
#pragma once
#ifndef BFME2_STREAMINGARCHIVEFILE_H
#define BFME2_STREAMINGARCHIVEFILE_H

#include "Common/RAMFile.h"

class StreamingArchiveFile : public RAMFile
{
public:
	StreamingArchiveFile();												// 0x00605A28
	virtual ~StreamingArchiveFile();									// 0x00605A45

	virtual bool open(const char *filename, int access);				// slot 1, 0x00605B36
	virtual void close();												// slot 2, 0x00605A7D
	virtual int read(void *buffer, int bytes);							// slot 3, 0x00605A82
	virtual int seek(int pos, seekMode mode);							// slot 5, 0x00605ADD
	virtual void nextLine(char *buffer = 0, int bytes = 0) {}			// slot 6, stub folded at 0x0050B238
	virtual bool scanInt(int &value) { return false; }					// slot 7, stub folded at 0x005CB9FF
	virtual bool scanReal(float &value) { return false; }				// slot 8, stub folded at 0x005CB9FF
	virtual bool scanString(AsciiString &value) { return false; }		// slot 9, stub folded at 0x005CB9FF
	virtual char *readEntireAndClose() { return 0; }					// slot 13, stub folded at 0x000D43D0
	virtual bool open(File *file) { return true; }						// slot 17, stub folded at 0x005CB9FA
	virtual bool openFromArchive(File *archiveFile, const AsciiString &filename, int offset, int size);	// slot 18, 0x00605B68

	virtual bool copyDataToFile(File *file) { return false; }			// slot 19, stub folded at 0x005CB9FF

protected:
	File *m_file;														// 0x20
	int m_startingPos;													// 0x24
	int m_curPos;														// 0x28
};
typedef char StreamingArchiveFileSize[sizeof(StreamingArchiveFile) == 0x2C ? 1 : -1];

#endif
