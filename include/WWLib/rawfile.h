// rawfile.h: WWLib's file class over a Win32 handle (WWVegas/WWLib/rawfile.h).
// SIZE 0x1C, one base (FileClass), vftable 0x0087C388 with 22 slots.
// Target facts: the constructor 0x0061A3B0 stores the table at +0, Rights (1,
// READ) at +4, BiasStart (0) at +8, BiasLength (-1) at +0xC, Handle (-1) at +0x10,
// builds the StringClass at +0x14 and zeroes the two date words at +0x18 and +0x1A.
// RawFileFactoryClass::Get_File 0x00613240 allocates 0x1C bytes for it and
// BufferedFileClass's first member is at +0x1C. File_Name 0x0066D450 (slot 1) is
// the 4-byte body `return Filename`. The vftable's slot order after FileClass's
// 18 slots is Error 0x00619FE0, Bias 0x0061A750, Attach 0x0061A880, Detach
// 0x0061A390. Open, Is_Available and Delete convert the narrow file name to
// UTF-16 once per call and use the W entry points.
// Donor facts: names follow Zero Hour's WWLib/rawfile.h (GPL), naming only.
#pragma once
#ifndef BFME2_RAWFILE_H
#define BFME2_RAWFILE_H

#include "WWLib/wwfile.h"
#include "WWLib/wwstring.h"

class RawFileClass : public FileClass
{
public:
	RawFileClass(const char *filename);					// 0x0061A420
	RawFileClass();							// 0x0061A3B0
	virtual ~RawFileClass();						// 0x0061A8C0

	virtual const char *File_Name() const { return Filename; }		// slot 1, 0x0066D450
	virtual const char *Set_Name(const char *filename);			// 0x0061A4E0
	virtual int Create();							// 0x0061A1B0
	virtual int Delete();							// 0x0061AC30
	virtual bool Is_Available(int forced = false);				// 0x0061AAD0
	virtual bool Is_Open() const;						// 0x00619FD0
	virtual int Open(const char *filename, int rights = READ);		// 0x0061A000
	virtual int Open(int rights = READ);					// 0x0061A950
	virtual int Read(void *buffer, int size);				// 0x0061A060
	virtual int Seek(int pos, int dir = SEEK_CUR);				// 0x0061A640
	virtual int Size();							// 0x0061A130
	virtual int Write(const void *buffer, int size);			// 0x0061A510
	virtual void Close();							// 0x0061A020
	virtual unsigned long Get_Date_Time();					// 0x0061A1F0
	virtual bool Set_Date_Time(unsigned long datetime);			// 0x0061A2A0
	virtual void *Get_File_Handle() { return Handle; }			// slot 17, 0x0013E9F0
	virtual void Error(int error, int canretry = false, const char *filename = 0);	// slot 18, 0x00619FE0
	virtual void Bias(int start, int length = -1);				// 0x0061A750
	virtual void Attach(void *handle, int rights = READ);			// 0x0061A880
	virtual void Detach();							// 0x0061A390

protected:
	enum { NULL_HANDLE = -1 };

	void Reset();								// 0x0061A4C0
	int Raw_Seek(int pos, int dir = SEEK_CUR);				// 0x0061A310

	int Rights;				// +0x04
	int BiasStart;				// +0x08
	int BiasLength;			// +0x0C
	void *Handle;				// +0x10
	StringClass Filename;			// +0x14
	unsigned short Date;			// +0x18
	unsigned short Time;			// +0x1A
};
// SIZE 0x1C

#endif
