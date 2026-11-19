// Dict.h: SAGE's property dictionary, the map/INI bag mapLoadPostProcess reads.
// Target facts (old repo Code/GameEngine/Include/Common/Dict.h): the payload is
// a reference count at data+0, an allocated count at data+2, a used count at +4
// and eight-byte key/value pairs from +6; getReal (0x003131FC) looks a pair up by
// key, reads its value at +4 and tests tag 2, reporting presence through the
// optional bool. Only the surface module units reach is declared here.
// The pair layout and the payload's pair offset are proven by the getters and
// the pair teardown (0x0031334A, 0x0031339C): the pairs are packed at two-byte
// alignment, so the first pair starts at +6, not +8.
#pragma once
#ifndef BFME2_DICT_H
#define BFME2_DICT_H

#include "Common/GameType.h"

class AsciiString;
class UnicodeString;

class Dict
{
public:
	enum DataType
	{
		DICT_NONE = -1,
		DICT_BOOL = 0,
		DICT_INT,
		DICT_REAL,
		DICT_ASCIISTRING,
		DICT_UNICODESTRING
	};

	// One pair: +0 holds the key shifted left by eight with the DataType in the
	// low byte; +4 holds the value (a bool byte, an int, a float, or a string
	// object's pointer for the two string types).
#pragma pack(push, 2)
	struct DictPair
	{
		void clear();										// 0x0031334A
		void setNameAndType(Int key, DataType type);		// 0x00313376
		void copyFrom(DictPair *src);						// 0x00313404

		union
		{
			UnsignedInt m_nameAndType;
			UnsignedByte m_type;
		};
		union
		{
			Bool m_bool;
			Int m_int;
			Real m_real;
			void *m_string;
		};
	};
#pragma pack(pop)

	Dict(Int size);						// 0x00313581
	Dict(const Dict &that) : m_data(that.m_data) { if (m_data) ++m_data->m_refCount; }	// retail's inline copy (DataChunkInput::readDict 0x00307833 returns its local by value)
	// Inline: SidesInfo::~SidesInfo 0x0032D4C0 releases its Dict by calling releaseData 0x0031339C directly.
	~Dict() { releaseData(); }
	Dict &operator=(const Dict &that);	// 0x003133E1
	void clear();						// 0x00313574
	Bool getBool(Int key, Bool *exists = 0) const;		// 0x00313198
	AsciiString getAsciiString(Int key, Bool *exists = 0) const;	// 0x0031359F
	Int getInt(Int key, Bool *exists = 0) const;		// 0x003131CA
	float getReal(Int key, Bool *exists = 0) const;		// 0x003131FC
	DataType getType(Int key) const;					// 0x0031317C
	// 0x00306BA9, 0x00306BCD: out-of-line copies in DataChunk's unit; the pair count is read inline (m_used).
	UnsignedShort getPairCount() const { return m_data ? m_data->m_used : 0; }
	NameKeyType getNthKey(Int index) const;
	DataType getNthType(Int index) const;
	void setBool(Int key, Bool value);				// 0x003136F6
	void setInt(Int key, Int value);				// 0x00313716
	void setReal(Int key, Real value);				// 0x00313736
	Bool remove(Int key);								// 0x003137A8
	void copyPairFrom(const Dict &src, Int key);			// 0x003137EE
	void setAsciiString(Int key, const AsciiString &value);	// 0x0031375A
	void setUnicodeString(Int key, const UnicodeString &value);	// 0x00313781
	AsciiString getNthAsciiString(Int index) const;			// 0x0031362D
	UnicodeString getNthUnicodeString(Int index) const;		// 0x00313665
	UnicodeString getUnicodeString(Int key, Bool *exists = 0) const;	// 0x003135E6
	Bool getNthBool(Int index) const;					// 0x00313232
	Int getNthInt(Int index) const;						// 0x00313253
	Real getNthReal(Int index) const;					// 0x00313274

private:
	// The payload: reference count, allocated count and used count, then the
	// pairs at +6 (two-byte packing keeps the first pair there).
#pragma pack(push, 2)
	struct DictPayload
	{
		UnsignedShort m_refCount;						// +0
		UnsignedShort m_allocated;						// +2
		UnsignedShort m_used;							// +4
		DictPair m_pairs[1];							// +6
	};
#pragma pack(pop)

	DictPair *findPairByKey(Int key) const;				// 0x0031313B
	DictPair *setPrep(Int key, DataType type);			// 0x0031369D
	void sortPairs();									// 0x00313299
	void releaseData();									// 0x0031339C
	DictPair *ensureUnique(Int size, Bool, DictPair *);	// 0x0031346B
	friend class SidesInfo;	// SidesInfo::swap 0x0032B070 exchanges the payload pointers directly
	DictPayload *m_data;								// 0x00
};
// SIZE 0x04


inline NameKeyType Dict::getNthKey(Int index) const
{
	if (m_data && index >= 0 && index < m_data->m_used)
		return (NameKeyType)(m_data->m_pairs[index].m_nameAndType >> 8);
	return (NameKeyType)0;
}

inline Dict::DataType Dict::getNthType(Int index) const
{
	if (m_data && index >= 0 && index < m_data->m_used)
		return (DataType)(m_data->m_pairs[index].m_nameAndType & 0xff);
	return DICT_NONE;
}

#endif
