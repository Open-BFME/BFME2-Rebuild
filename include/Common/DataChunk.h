// DataChunk.h: the map-file chunk reader and writer, and the chunk-name table they share.
// Target facts (retail RVAs; layouts from the constructors and the chunk-stack accessors):
// - DataChunkTableOfContents (SIZE 0x10): constructor 0x00306D48 stores a null
//   list head at +0, a zero count at +4, next ID 1 at +8 and false at +0xC.
//   The destructor 0x00306D5C frees the mapping list at +0.
// - DataChunkInput (SIZE 0x28, no virtual functions): stream at +0, the table at +4,
//   the stream's tell result at +0x14, the parser list at +0x18, the chunk stack at
//   +0x1C, and two user pointers at +0x20 and +0x24.
// - InputChunk: vtable at +0 (virtual destructor, deleted with ::delete), next chunk
//   at +4, ID at +8, version word at +0xC, data size at +0x14, bytes left at +0x18.
// - DataChunkOutput (SIZE 0x1C, no virtual functions): stream at +0, FILE at +4, the
//   table at +8, the output chunk stack at +0x18.
#pragma once
#ifndef BFME2_COMMON_DATACHUNK_H
#define BFME2_COMMON_DATACHUNK_H

#include <stdio.h>
#include "Common/GameType.h"
#include "Common/AsciiString.h"
#include "Common/ChunkInputStream.h"
#include "Common/Dict.h"

class UnicodeString;
class Dict;
class OutputStream;
class UserParser;
class OutputChunk;

class DataChunkInput;

typedef unsigned short DataChunkVersionType;
typedef bool (*DataChunkParserPtr)(DataChunkInput &file, struct DataChunkInfo *info, void *userData);

// What a registered parser is handed for the chunk it is called on (the locals DataChunkInput::parse 0x00307AC0 fills; Zero Hour's DataChunkInfo).
struct DataChunkInfo
{
	AsciiString m_label;						// +0x00
	AsciiString m_parentLabel;					// +0x04
	DataChunkVersionType m_version;				// +0x08
	UnsignedInt m_dataSize;						// +0x0C
};
// SIZE 0x10

class InputChunk
{
public:
	virtual ~InputChunk() {}					// retail's deleting destructor is the one ::delete calls

	InputChunk *m_next;							// +0x04
	UnsignedInt m_id;							// +0x08
	UnsignedShort m_version;					// +0x0C
	UnsignedByte m_unknown0E[2];				// +0x0E, not yet recovered
	UnsignedInt m_filePos;						// +0x10, stream position of the chunk's data
	UnsignedInt m_dataSize;						// +0x14
	Int m_dataLeft;								// +0x18, signed: the end test compares it with zero
};

// One entry of the chunk-name table. The concrete object is allocated elsewhere, so the
// destructor is virtual and the table deletes entries with ::delete.
class Mapping
{
public:
	virtual ~Mapping() {}

	Mapping *m_next;							// +0x04
	AsciiString m_name;							// +0x08
	UnsignedInt m_id;							// +0x0C
};
// SIZE 0x10

class DataChunkTableOfContents
{
public:
	DataChunkTableOfContents();					// 0x00306D48
	~DataChunkTableOfContents();				// 0x00306D5C

	UnsignedInt getID(const AsciiString &name);				// 0x00307A53
	AsciiString getName(UnsignedInt id);					// 0x003070F1
	UnsignedInt allocateID(const AsciiString &name);		// 0x00307A6A
	Bool isOpenedForRead() const { return m_headerOpened; }
	void write(OutputStream &stream);						// 0x0030715D
	void read(ChunkInputStream &stream);					// 0x003071E9

	Mapping *m_list;							// +0x00
	Int m_listLength;							// +0x04
	UnsignedInt m_nextID;						// +0x08
	Bool m_headerOpened;						// +0x0C

private:
	Mapping *findMapping(const AsciiString &name);			// 0x00307A2C
};
// SIZE 0x10

// A registered chunk parser. The parsers form a list headed in the input; each entry
// also records the field that points at it, so it can be unlinked.
class UserParser
{
public:
	UserParser();								// 0x00307371
	virtual ~UserParser();						// 0x0030739E

	UserParser *m_next;							// +0x04
	UserParser **m_link;						// +0x08, the pointer field that refers to this entry
	DataChunkParserPtr m_parser;				// +0x0C
	AsciiString m_label;						// +0x10
	AsciiString m_parentLabel;					// +0x14
	void *m_userData;							// +0x18
};
// SIZE 0x1C

class DataChunkInput
{
public:
	DataChunkInput(ChunkInputStream *stream);	// 0x00307316
	~DataChunkInput();							// 0x00306F01

	Bool atEndOfFile();							// 0x00306C08
	Bool atEndOfChunk();						// 0x00306E2E
	void reset();								// 0x00306DCC
	void clearChunkStack();						// 0x00306DA4
	AsciiString getChunkLabel();				// 0x003074D7
	DataChunkVersionType getChunkVersion();		// 0x00306E06
	UnsignedInt getChunkDataSize();				// 0x00306E16
	UnsignedInt getChunkDataSizeLeft();			// 0x00306E22
	Real readReal();							// 0x00306E56
	Int readInt();								// 0x00306E78
	UnsignedByte readByte();					// 0x00306E9A
	void readArrayOfBytes(char *buffer, Int length);	// 0x00306EBC
	NameKeyType readNameKey();					// 0x003077E0
	AsciiString readAsciiString();				// 0x0030750A
	Bool parse(void *userData);					// 0x00307AC0
	UnicodeString readUnicodeString();			// 0x003075A3
	Dict readDict();							// 0x00307833
	AsciiString openDataChunk(DataChunkVersionType *version);	// 0x0030740F
	UserParser *registerParser(const AsciiString &label, const AsciiString &parentLabel,
		DataChunkParserPtr parser, void *userData);	// 0x00307779

protected:
	void decrementDataLeft(Int size);			// 0x00306E40
	void closeDataChunk();						// 0x00306F53

	ChunkInputStream *m_file;					// +0x00
	DataChunkTableOfContents m_contents;		// +0x04
	Int m_fileposOfFirstChunk;					// +0x14
	UserParser *m_parserList;					// +0x18
	InputChunk *m_chunkStack;					// +0x1C
	void *m_currentObject;						// +0x20
	void *m_userData;							// +0x24
};
// SIZE 0x28

class OutputChunk
{
public:
	virtual ~OutputChunk() {}

	OutputChunk *m_next;						// +0x04
	UnsignedInt m_id;							// +0x08, the chunk's name ID
	Int m_fileStart;							// +0x0C, file position of the chunk's size field
};

class DataChunkOutput
{
public:
	DataChunkOutput(OutputStream *stream);		// 0x00306FA5
	~DataChunkOutput();							// 0x00307680

	void openDataChunk(char *name, DataChunkVersionType version);	// 0x00307C76
	void closeDataChunk();						// 0x00306C88
	void writeByte(UnsignedByte value);			// 0x00306D17
	void writeArrayOfBytes(char *buffer, Int length);	// 0x00306D2F
	void writeAsciiString(const AsciiString &text) throw();	// 0x00307033
	void writeUnicodeString(UnicodeString text);	// 0x0030708E
	void writeInt(Int value);					// 0x00306CFF (folded with writeReal)
	void writeReal(Real value);					// 0x00306CFF (folded with writeInt)
	void writeDict(const Dict &d);				// 0x00307D85
	void writeNameKey(NameKeyType key);			// 0x00307D29

protected:
	OutputStream *m_pOut;						// +0x00
	FILE *m_tmp_file;							// +0x04
	DataChunkTableOfContents m_contents;		// +0x08
	OutputChunk *m_chunkStack;					// +0x18
};
// SIZE 0x1C

#endif
