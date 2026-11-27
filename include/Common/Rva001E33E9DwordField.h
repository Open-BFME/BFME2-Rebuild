// A dword field of an owner not yet recovered; the pad stands in for the owner.
#pragma once
#ifndef BFME2_RVA001E33E9DWORDFIELD_H
#define BFME2_RVA001E33E9DWORDFIELD_H

class Rva001E33E9DwordField
{
public:
	int get() const;

private:
	unsigned char m_unk0x000[0x25C];
	int m_value;
};

#endif
