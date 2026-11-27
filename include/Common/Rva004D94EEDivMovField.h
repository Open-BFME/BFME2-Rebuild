// The element capacity of a vector of 0x154-byte records, from its first and end-of-storage pointers.
#pragma once
#ifndef BFME2_RVA004D94EEDIVMOVFIELD_H
#define BFME2_RVA004D94EEDIVMOVFIELD_H

class Rva004D94EEDivMovField
{
public:
	int get() const;

private:
	struct Element
	{
		unsigned char m_unk0x000[0x154];
	};

	Element *m_start;
	Element *m_unk0x04;
	Element *m_end;
};

#endif
