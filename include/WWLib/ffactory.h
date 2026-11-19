// ffactory.h: WWLib's file factories (WWVegas/WWLib/ffactory.h).
// Target facts (retail game.dat): FileFactoryClass has two virtuals after the
// destructor, Get_File and Return_File (table slots 1 and 2). The retail
// SimpleFileFactoryClass table 0x0087C1D8 is the deleting destructor 0x006131A0,
// Get_File 0x00613750 and Return_File 0x006132A0 (the shared `delete file` body).
// SimpleFileFactoryClass is SIZE 0x14: vptr +0, the sub-directory StringClass +4,
// the strip-path flag +8, the critical section +0xC. RawFileFactoryClass is a plain
// object with non-virtual Get_File 0x00613240 (builds a RawFileClass, 0x1C bytes) and
// Return_File. file_auto_ptr (File +0, Factory +4) asks the factory for a file and
// falls back to a BufferedFileClass; its destructor returns the file to the factory.
// Donor facts: names follow Zero Hour's WWLib/ffactory.h (GPL), naming only.
#pragma once
#ifndef BFME2_FFACTORY_H
#define BFME2_FFACTORY_H

#include "WWLib/wwfile.h"
#include "WWLib/rawfile.h"
#include "WWLib/mutex.h"
#include "WWLib/wwstring.h"

class FileFactoryClass
{
public:
	virtual ~FileFactoryClass() {}
	virtual FileClass *Get_File(const char *filename) = 0;
	virtual void Return_File(FileClass *file) = 0;
};

class RawFileFactoryClass
{
public:
	RawFileClass *Get_File(const char *filename);			// 0x00613240
	void Return_File(FileClass *file);				// 0x006132A0
};

class SimpleFileFactoryClass : public FileFactoryClass
{
public:
	SimpleFileFactoryClass();					// 0x00613300
	virtual ~SimpleFileFactoryClass() {}
	virtual FileClass *Get_File(const char *filename);		// 0x00613750
	virtual void Return_File(FileClass *file) { ::delete file; }	// 0x006132A0

	void Set_Sub_Directory(const char *sub_directory);		// 0x00613370
	void Get_Sub_Directory(StringClass &new_dir) const;		// 0x006134F0
	void Prepend_Sub_Directory(const char *sub_directory);		// 0x00613550
	void Append_Sub_Directory(const char *sub_directory);		// 0x00613650

private:
	StringClass SubDirectory;					// +0x04
	bool IsStripPath;						// +0x08
	mutable CriticalSectionClass Mutex;				// +0x0C
};
// SIZE 0x14

class file_auto_ptr
{
public:
	file_auto_ptr(FileFactoryClass *fac, const char *filename);	// 0x006131C0
	~file_auto_ptr();						// 0x00613230

	FileClass *File;						// +0x00
	FileFactoryClass *Factory;					// +0x04
};
// SIZE 0x08

extern FileFactoryClass *_TheFileFactory;
extern RawFileFactoryClass *_TheWritingFileFactory;


#endif
