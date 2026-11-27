// The element count of a vector of 0x154-byte records, from its first and one-past-last pointers.
#pragma once
#ifndef BFME2_RVA004D94A5DIVMOVFIELD_H
#define BFME2_RVA004D94A5DIVMOVFIELD_H

class Rva004D94A5DivMovField
{
public:
	int get() const;

private:
	struct Element
	{
		unsigned char m_unk0x000[0x154];
	};

	Element *m_start;
	Element *m_finish;
};

#endif
