// HashTableClass.h: WWLib's chained hash table over HashableClass keys.
//
// Target facts:
//  - HashTableSize is at +0 and the bucket array at +4: the constructor
//    0x00613C20 stores the size then the allocation, Reset 0x00613BA0 walks
//    [this+0] entries of [this+4], and the destructor 0x00613B90 releases
//    [this+4] through scalar operator delete (0x0002FD60).
//  - Hash 0x00613BD0 is CRC_Stringi(key, 0) & (HashTableSize-1); the
//    two-argument CRC_Stringi is 0x00615840.
//  - The iterator holds the table by reference at +0, its bucket index at +4,
//    the lookahead entry at +8 and the current entry at +0xC (First 0x00613DB0
//    and Next 0x00613D70 write them in that order).
#pragma once
#ifndef BFME2_HASHTABLECLASS_H
#define BFME2_HASHTABLECLASS_H

class HashableClass;

class HashTableClass
{
public:
	HashTableClass(int size);
	~HashTableClass(void);

	void Reset(void);
	void Add(HashableClass *entry);
	bool Remove(HashableClass *entry);
	HashableClass *Find(const char *key);

	int HashTableSize;		// +0x00
	HashableClass **HashTable;	// +0x04

private:
	int Hash(const char *key);
};

class HashTableIteratorClass
{
public:
	HashTableIteratorClass(HashTableClass *table);

	void First(void);
	void Next(void);

private:
	void Advance_Next(void);

	// Target facts: Next 0x00613D70 stores the current entry at +0xC and the
	// lookahead at +0x10, Advance_Next 0x00613BF0 counts buckets at +8 and
	// reads the table pointer at +4; the leading word is not read by any of
	// the three and is not typed yet.
	void *m_unk0x00;		// +0x00
	HashTableClass *Table;		// +0x04
	int Index;			// +0x08
	HashableClass *CurrentEntry;	// +0x0C
	HashableClass *NextEntry;	// +0x10
};

#endif
