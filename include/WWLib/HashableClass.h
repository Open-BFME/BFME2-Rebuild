// Hash table entry base. The retail table 0x007CEF94 has the deleting destructor
// in slot 0 and the key accessor in slot 1. HashTableClass::Add at 0x00613C60
// stores the prior bucket head in NextHash at +0x4. HAnimClass uses this
// eight-byte base at +0x8.
#pragma once
#ifndef BFME2_HASHABLECLASS_H
#define BFME2_HASHABLECLASS_H

#pragma pack(push, 1)
class HashableClass
{
public:
	HashableClass() : NextHash(0) {}
	virtual ~HashableClass() {}
	virtual const char *Get_Key(void) = 0;

	HashableClass *NextHash;
};
#pragma pack(pop)

typedef char HashableClassSizeCheck[(sizeof(HashableClass) == 0x8) ? 1 : -1];

#endif
