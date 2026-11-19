// CharSource.h: the character sources StringBase's buffer code copies from.
// Names: the map's ledger spellings of StringBase<char>::ensureUniqueBufferOfSize
// 0x000364A0 (?ensureUniqueBufferOfSize@?$StringBase@D@@AAEXH_NPBV?$CharSource@D@@1@Z;
// WorldBuilder's assert text names "StringBase<char>::ensureUniqueBufferOfSize")
// and of the two sources' rows (NarrowStrLenSource 0x00035CF0.., WideStrLenSource
// 0x00035E50..).
// Target facts (retail game.dat):
// - NarrowStrLenSource table 0x007BE78C {getLength 0x00144010 (ICF-folded
//   `mov eax,[ecx+8]; ret`), getCharRange 0x00035D10, getChars 0x00035D40};
//   WideStrLenSource table 0x007BE798 {0x00144010, 0x00035E70, 0x00035EA0}.
// - ctor 0x00035CF0 (out of line): vptr, text +4, length +8. SIZE 0xC.
// - getCharRange(dest, start, count) copies count characters from text + start;
//   getChars(dest) copies the whole text and returns its length (rep movs).
// - The sources' destructors restore the abstract base's table (??1WideStrLenSource
//   0x00035780, 7 bytes), so CharSource declares an empty destructor.
#pragma once
#ifndef BFME2_CHARSOURCE_H
#define BFME2_CHARSOURCE_H

template <class CharT> class CharSource
{
public:
	~CharSource() {}
	virtual int getLength() const = 0;									// slot 0
	virtual void getCharRange(CharT *dest, int start, int count) const = 0;	// slot 1
	virtual int getChars(CharT *dest) const = 0;							// slot 2
};

class NarrowStrLenSource : public CharSource<char>
{
public:
	NarrowStrLenSource(const char *text, int length);					// 0x00035CF0
	virtual int getLength() const { return m_length; }
	virtual void getCharRange(char *dest, int start, int count) const;	// 0x00035D10
	virtual int getChars(char *dest) const;								// 0x00035D40

private:
	const char *m_text;													// +0x04
	int m_length;														// +0x08
};
// SIZE 0xC

class WideStrLenSource : public CharSource<unsigned short>
{
public:
	WideStrLenSource(const unsigned short *text, int length);			// 0x00035E50
	virtual int getLength() const { return m_length; }
	virtual void getCharRange(unsigned short *dest, int start, int count) const;	// 0x00035E70
	virtual int getChars(unsigned short *dest) const;					// 0x00035EA0

private:
	const unsigned short *m_text;										// +0x04
	int m_length;														// +0x08
};
// SIZE 0xC

#endif
