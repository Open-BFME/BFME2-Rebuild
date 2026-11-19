// wwfile.h: WWLib's abstract file interface (WWVegas/WWLib/wwfile.h).
// Target facts (retail game.dat): FileClass has one vftable of 18 slots, at
// 0x007C6730, ending where FileFactoryClass's (0x007C6778) begins; slots 0..17
// are the deleting destructor, File_Name, Set_Name, Create, Delete, Is_Available,
// Is_Open, Open(int), Open(const char *, int), Read, Seek, Tell, Size, Write,
// Close, Get_Date_Time, Set_Date_Time and Get_File_Handle, so Error and Bias are
// RawFileClass's own virtuals (its table 0x0087C388 has slots 18..21 Error, Bias,
// Attach and Detach). The two Open overloads sit in reverse declaration order,
// as MSVC 7.1 lays out same-name overloads. Tell is the one inline body (slot
// 11 is 0x0007824D, `return Seek(0)`). The class holds no data: RawFileClass's
// first member is at +4.
// Donor facts: names and default arguments follow Zero Hour's WWLib/WWFILE.H
// (GPL), used for naming only.
#pragma once
#ifndef BFME2_WWFILE_H
#define BFME2_WWFILE_H

#ifndef SEEK_SET
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2
#endif

class FileClass
{
public:
	enum
	{
		READ = 1,
		WRITE = 2,
		PRINTF_BUFFER_SIZE = 1024
	};

	virtual ~FileClass() {}								// slot 0
	virtual const char *File_Name() const = 0;					// slot 1
	virtual const char *Set_Name(const char *filename) = 0;			// slot 2
	virtual int Create() = 0;							// slot 3
	virtual int Delete() = 0;							// slot 4
	virtual bool Is_Available(int forced = false) = 0;				// slot 5
	virtual bool Is_Open() const = 0;						// slot 6
	virtual int Open(const char *filename, int rights = READ) = 0;		// slot 8
	virtual int Open(int rights = READ) = 0;					// slot 7
	virtual int Read(void *buffer, int size) = 0;				// slot 9
	virtual int Seek(int pos, int dir = SEEK_CUR) = 0;				// slot 10
	virtual int Tell() { return Seek(0); }						// slot 11
	virtual int Size() = 0;							// slot 12
	virtual int Write(const void *buffer, int size) = 0;			// slot 13
	virtual void Close() = 0;							// slot 14
	virtual unsigned long Get_Date_Time() { return 0; }				// slot 15
	virtual bool Set_Date_Time(unsigned long) { return false; }			// slot 16
	virtual void *Get_File_Handle() { return (void *)-1; }			// slot 17
};
// SIZE 0x04

#endif
