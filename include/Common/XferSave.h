// XferSave.h: the save-game writer (an Xfer over a File). Vtable VA 0x00C7B010 (RVA 0x0087B010), constructor
// 0x0060D1F7, destructor 0x0060D0B3. Target facts (retail game.dat, tables 0x87B010 and 0x87AF18):
//  - +0x04 File *m_file, called through its vtable slot 4 (+0x10, write) and slot 5 (+0x14, seek);
//    +0x08 Bool (Open's third argument: the block names go through a string table);
//    +0x0C vector<Int> (the file position pushed by BeginBlock and popped by EndBlock);
//    +0x18 a STLport hash_map<string, UnsignedInt> (the block-name string table: node 0x14 bytes, next/key at +4/id at
//    +0x10; 0x0060D2E9 finds the key through the node finder 0x0060CAEA, writes 0xFF and the id, or the length byte,
//    the name and a new id, constructor 0x0060D06A, destructor 0x0060D031 destroys each key string, bucket count 100);
//    +0x2C a hash_map<const char *, Int> (pointer-keyed, node 0xC bytes, hash of the characters, equality on the
//    pointers; 0x0060CE41 and operator[] 0x0060CDFD, destructor 0x0060CC99 frees nodes directly);
//    the new id is the sum of both tables' element counts (+0x28 and +0x3C). SIZE 0x40.
//  - slots 5/6/7 are BeginBlock 0x0060D435, EndBlock 0x0060CA26 and a `ret 4` no-op SkipBlock 0x0047A69C; slot 38
//    XferData 0x0060C935 writes the first argument's 4 bytes as a type tag when it is nonzero, then `size` bytes of
//    the second argument. Every write failure throws XferException(1, 0).
#pragma once
#ifndef BFME2_XFERSAVE_H
#define BFME2_XFERSAVE_H

#include "Common/GameType.h"
#include "Common/Xfer.h"
#include "Common/Vector.h"
#include <hash_map>
#include <string>

class File;

class XferSave : public Xfer
{
public:
	XferSave();																	// 0x0060D1F7
	virtual ~XferSave();

	UnsignedByte Open(File *file, Int version, Bool stringTable);
	void Close();																// 0x0060C8CD: writes the END tag and detaches the file

	virtual Bool IsStoring() const { return true; }								// slot 2 (retail folds the body into 0x0050B5C6)
	virtual Int BeginBlock(const char *name);									// slot 5, 0x0060D435 (map: rva0060D435@XferSave)
	virtual void EndBlock();													// slot 6, 0x0060CA26
	virtual void SkipBlock(const char *name);									// slot 7, 0x0047A69C (a no-op)
	virtual Xfer &operator==(const char *const &value);							// slot 11
	virtual Xfer &operator==(Snapshot &snapshot);								// slot 12, 0x0060C8FA (table 0x0087B010; XferMD5CRC's table shares it)

protected:
	virtual void XferData(UnsignedInt type, void *data, UnsignedInt size);		// slot 38, 0x0060C935

private:
	void rva0060D2E9(const char *name);											// Writes a block name through the string table

	File *m_file;																// +0x04
	Bool m_stringTableMode;														// +0x08
	unsigned char m_unk0x09[3];													// +0x09
	_STL::vector<Int> m_blockStack;												// +0x0C
	_STL::hash_map<_STL::string, UnsignedInt> m_nameIds;						// +0x18 block name -> id (0x0060D2E9)
	_STL::hash_map<const char *, Int> m_literalIds;								// +0x2C name pointer -> id (0x0060CE41)
};
// SIZE 0x40

#endif
