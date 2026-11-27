// A byte field of an owner not yet recovered; the pad stands in for the owner.
#pragma once
#ifndef BFME2_RVA001E33F0BYTEFIELD_H
#define BFME2_RVA001E33F0BYTEFIELD_H

class Rva001E33F0ByteField
{
public:
	unsigned char get() const;

private:
	unsigned char m_unk0x000[0x249];
	unsigned char m_value;
};

#endif
