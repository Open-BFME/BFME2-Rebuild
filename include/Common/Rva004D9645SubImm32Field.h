// A field read back less the size of one 0x154-byte record.
#pragma once
#ifndef BFME2_RVA004D9645SUBIMM32FIELD_H
#define BFME2_RVA004D9645SUBIMM32FIELD_H

class Rva004D9645SubImm32Field
{
public:
	int get() const;

private:
	unsigned char m_unk0x00[4];
	int m_value;
};

#endif
