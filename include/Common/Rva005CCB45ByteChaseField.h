// A byte field reached through a pointer of an owner not yet recovered; the pads stand in for both.
#pragma once
#ifndef BFME2_RVA005CCB45BYTECHASEFIELD_H
#define BFME2_RVA005CCB45BYTECHASEFIELD_H

class Rva005CCB45ByteChaseField
{
public:
	unsigned char get() const;

private:
	struct Target
	{
		unsigned char m_unk0x00[0x22];
		unsigned char m_value;
	};

	unsigned char m_unk0x00[8];
	Target *m_target;
};

#endif
