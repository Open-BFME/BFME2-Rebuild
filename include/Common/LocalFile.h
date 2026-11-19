// LocalFile.h: a File backed by an OS descriptor (Common/System/LocalFile.cpp).
// Layout: the File base (0x14 bytes; m_open at +0xC, which read and write test)
// and the CRT descriptor at +0x14, proven by every body passing [this+0x14] to a
// low-level io import (read 0x00605DF7, write 0x00605E34, seek 0x00605E5D) and
// by the destructor (0x00605CA7) closing it when it is not -1.
// Virtual table: File's slots, with open(char const *) overridden at slot 1
// (0x00605CFB, which forwards to the wide open) and open(unsigned short const *)
// as the extra virtual at slot 0x44 (0x00605D36) that the narrow open calls.
#pragma once
#ifndef BFME2_LOCALFILE_H
#define BFME2_LOCALFILE_H

#include "Common/File.h"

class LocalFile : public File
{
public:
	virtual ~LocalFile();						// slot 0, 0x00605CA7
	virtual bool open(const char *, int = 0);	// slot 1, 0x00605CFB
	virtual int read(void *, int);				// slot 3, 0x00605DF7
	virtual int write(const void *, int);		// slot 4, 0x00605E34
	virtual int seek(int, seekMode = CURRENT);	// slot 5, 0x00605E5D
	virtual void nextLine(char * = 0, int = 0);	// slot 6, 0x00605E91
	virtual bool scanInt(int &);				// slot 7, 0x0060606F
	virtual bool scanReal(float &);				// slot 8, 0x00606133
	virtual bool scanString(AsciiString &);		// slot 9, 0x00605F54
	virtual char *readEntireAndClose();			// slot 13, 0x00605F0D
	virtual File *convertToRAMFile();			// slot 14, 0x00605FDF
	virtual bool open(const unsigned short *, int);	// slot 0x44, 0x00605D36

private:
	int m_fd;									// +0x14
};
// SIZE 0x18
typedef char LocalFileSize[(sizeof(LocalFile) == 0x18) ? 1 : -1];

#endif
