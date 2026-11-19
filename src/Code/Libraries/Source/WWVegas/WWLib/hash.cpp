// cl: /O2
#include <string.h>
#include "WWLib/HashTableClass.h"
#include "WWLib/HashableClass.h"
#include "WWLib/crc.h"
// Tier A; object=O00613B90 evidence=W flags=align=A
// Extent is a hypothesis; only Object VERIFIED establishes recovery.
// Edges: left=unknown-rows right=unknown-rows
// WWLib's chained hash table. Buckets are keyed by the case-insensitive string
// CRC masked to the table size, and the table array is released with scalar
// operator delete (0x0002FD60) after scalar operator new (0x0002FDA0).
// FUNCTION: BFME2 0x00613B90 size=11 name=??1HashTableClass@@QAE@XZ evidence=N:ledger

HashTableClass::~HashTableClass(void)
{
	::operator delete(HashTable);
}

// FUNCTION: BFME2 0x00613BA0 size=34 name=?Reset@HashTableClass@@QAEXXZ evidence=N:ledger

void HashTableClass::Reset(void)
{
	for (int i = 0; i < HashTableSize; i++)
		HashTable[i] = NULL;
}

// FUNCTION: BFME2 0x00613BD0 size=29 name=?Hash@HashTableClass@@AAEHPBD@Z evidence=N:ledger

int HashTableClass::Hash(const char *key)
{
	return CRC_Stringi(key, 0) & (HashTableSize - 1);
}

// FUNCTION: BFME2 0x00613BF0 size=42 name=?Advance_Next@HashTableIteratorClass@@AAEXXZ evidence=N:ledger

void HashTableIteratorClass::Advance_Next(void)
{
	// Skip empty buckets until one has a chain or the table ends.
	while (NextEntry == NULL)
	{
		Index++;
		if (Index >= Table->HashTableSize)
			return;
		NextEntry = Table->HashTable[Index];
	}
}

// FUNCTION: BFME2 0x00613C20 size=55 name=??0HashTableClass@@QAE@H@Z evidence=N:ledger

HashTableClass::HashTableClass(int size)
{
	HashTableSize = size;
	HashTable = (HashableClass **)::operator new(HashTableSize * sizeof(HashableClass *));
	Reset();
}

// FUNCTION: BFME2 0x00613C60 size=53 name=?Add@HashTableClass@@QAEXPAVHashableClass@@@Z evidence=N:ledger

void HashTableClass::Add(HashableClass *entry)
{
	int index = Hash(entry->Get_Key());
	entry->NextHash = HashTable[index];
	HashTable[index] = entry;
}

// FUNCTION: BFME2 0x00613CA0 size=102 name=?Remove@HashTableClass@@QAE_NPAVHashableClass@@@Z evidence=N:ledger

bool HashTableClass::Remove(HashableClass *entry)
{
	const char *name = entry->Get_Key();
	int slot = Hash(name);

	if (HashTable[slot] != NULL)
	{
		// The chain head is unlinked by repointing the bucket.
		if (HashTable[slot] == entry)
		{
			HashTable[slot] = entry->NextHash;
			return true;
		}

		// Otherwise walk the chain looking for the predecessor.
		HashableClass *link = HashTable[slot];
		while (link->NextHash != NULL)
		{
			if (link->NextHash == entry)
			{
				link->NextHash = entry->NextHash;
				return true;
			}
			link = link->NextHash;
		}
	}

	return false;
}

// FUNCTION: BFME2 0x00613D10 size=89 name=?Find@HashTableClass@@QAEPAVHashableClass@@PBD@Z evidence=N:ledger

HashableClass *HashTableClass::Find(const char *key)
{
	int slot = Hash(key);
	for (HashableClass *link = HashTable[slot]; link != NULL; link = link->NextHash)
	{
		if (::_strcmpi(link->Get_Key(), key) == 0)
			return link;
	}
	return NULL;
}

// FUNCTION: BFME2 0x00613D70 size=58 name=?Next@HashTableIteratorClass@@QAEXXZ evidence=N:ledger

void HashTableIteratorClass::Next(void)
{
	CurrentEntry = NextEntry;
	if (NextEntry != NULL)
	{
		NextEntry = NextEntry->NextHash;
		Advance_Next();
	}
}

// FUNCTION: BFME2 0x00613DB0 size=53 name=?First@HashTableIteratorClass@@QAEXXZ evidence=N:ledger

void HashTableIteratorClass::First(void)
{
	Index = 0;
	NextEntry = Table->HashTable[Index];
	Advance_Next();
	Next();		// promote the first chain head to the current entry
}
