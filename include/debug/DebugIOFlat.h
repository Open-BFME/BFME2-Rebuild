// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// DebugIOFlat.h: the debug I/O class that writes the log to flat (optionally split) files (Zero Hour's debug_io_flat.cpp,
// internal_io.h). Target facts (retail game.dat): vftable 0x007C0ACC; Create 0x0003FD90 allocates 0x214 bytes through
// DebugAllocMemory. Layout from the constructor 0x0003FD00 and Write 0x000407B0: +4 m_firstSplit, +8 m_lastSplitPtr (points
// at +4), +0xC m_firstStream, +0x10 m_lastStreamPtr (points at +0xC), +0x14 m_baseFilename[256], +0x114 m_copyDir[256].
// OutputStream is a 0x1C byte object (Create 0x0003F8E0): +0 m_fileName, +4 m_fileHandle, +8 m_limitedFileSize, +0xC
// m_buffer, +0x10 m_bufferUsed, +0x14 m_bufferSize, +0x18 m_nextChar. A split entry is 0x20C bytes: +0 next, +4 stringTypes,
// +8 items[256], +0x108 name[256], +0x208 stream. Slot 1 (Read) is the shared stub 0x0003FD80 (`xor eax,eax; ret 8`) and
// slot 6 (Delete) the shared thunk 0x000386C0.
#pragma once
#ifndef BFME2_DEBUGIOFLAT_H
#define BFME2_DEBUGIOFLAT_H

#include "debug/DebugIOInterface.h"

void DebugFreeMemory(void *ptr);

class DebugIOFlat : public DebugIOInterface
{
	// single output stream
	class OutputStream
	{
		OutputStream(const OutputStream &);
		OutputStream &operator=(const OutputStream &);

		char *m_fileName;				// +0 (dynamically allocated)
		void *m_fileHandle;				// +4 (only if the output buffer is not size limited)
		bool m_limitedFileSize;			// +8
		char *m_buffer;					// +0xC
		unsigned int m_bufferUsed;		// +0x10
		unsigned int m_bufferSize;		// +0x14
		unsigned int m_nextChar;		// +0x18 (next free char of the ring buffer)

		OutputStream(const char *filename, unsigned int maxSize);			// 0x0003F840
		~OutputStream() {}

		void InternalWrite(const char *src, unsigned int len);				// 0x00040000

	public:
		static OutputStream *Create(const char *filename, unsigned int maxSize);	// 0x0003F8E0
		void Delete(const char *path = 0);									// 0x0003FDF0
		const char *GetFilename() { return m_fileName; }
		void Write(const char *src);										// 0x00040720
		void Flush();														// 0x0003F960
	};

	struct SplitListEntry
	{
		SplitListEntry *next;
		unsigned int stringTypes;
		char items[256];
		char name[256];
		OutputStream *stream;
	};

	struct StreamListEntry
	{
		StreamListEntry *next;
		OutputStream *stream;
	};

	SplitListEntry *m_firstSplit;			// +4
	SplitListEntry **m_lastSplitPtr;		// +8
	StreamListEntry *m_firstStream;			// +0xC
	StreamListEntry **m_lastStreamPtr;		// +0x10
	char m_baseFilename[256];				// +0x14
	char m_copyDir[256];					// +0x114

	static void ExpandMagic(const char *src, const char *splitName, char *buf);	// 0x0003FA20

public:
	DebugIOFlat();																// 0x0003FD00
	virtual ~DebugIOFlat();														// slot 0: 0x00040780 (deleting), body 0x000400D0

	virtual int Read(char *buf, int maxchar) { return 0; }						// slot 1: 0x0003FD80
	virtual void Write(StringType type, const char *src, const char *str);		// slot 3: 0x000407B0
	virtual void EmergencyFlush();												// slot 4: 0x0003FD60
	virtual void Execute(Debug &dbg, const char *cmd, bool structuredCmd, unsigned int argn, const char *const *argv);	// slot 5: 0x00040160
	virtual void Delete()														// slot 6: 0x000386C0
	{
		this->~DebugIOFlat();
		DebugFreeMemory(this);
	}

	static DebugIOInterface *Create();											// 0x0003FD90
};

#endif
