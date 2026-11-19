// PooledString.h: a string interned in a shared pool. SIZE 0x4.
// Target facts (retail game.dat; names from the map's ledger rows): one pointer at +0 to a pool entry, so
// equal texts share one entry. The entry is {chain +0, case-folded twin +4, text +8}. Out of line: the
// (pointer, length) comparisons 0x0060BCAB / 0x0060BCE2, which hand this string's data and length, the
// argument's data and length and a one-byte trait object to compareRange / compareRangeNoCase
// (0x00005816 / 0x00005841); the pointer, PooledString and AsciiString forms call them with a strlen
// or the AsciiString's length; the constructor and assignment from an AsciiString go through the private
// setString (its pool lock is the game's string lock). Xfer's operator==(PooledString &) 0x0060C0A0 writes
// the text like an AsciiString.
#pragma once
#ifndef BFME2_POOLEDSTRING_H
#define BFME2_POOLEDSTRING_H

class AsciiString;

// The one-byte trait object compareRange takes (the narrow workers never read it).
struct CharCompare
{
	char m_unused;
};

int compareRange(const char *a, int alen, const char *b, int blen, CharCompare tag);			// 0x00005816
int compareRangeNoCase(const char *a, int alen, const char *b, int blen, CharCompare tag);	// 0x00005841

// The wide trait object: also one byte, but its two workers are out of line
// (retail 0x000052F8 compare, a plain loop; 0x00005344 compareNoCase, through
// msvcr71's towlower); the wide range workers and StringBase<unsigned short>'s
// prefix tests call them on a stack copy (lea ecx,[esp+...]).
struct WideCharCompare
{
	char m_unused;
	int compare(const unsigned short *a, const unsigned short *b, int count) const;			// 0x000052F8
	int compareNoCase(const unsigned short *a, const unsigned short *b, int count) const;	// 0x00005344
};

int compareRange(const unsigned short *a, int alen, const unsigned short *b, int blen, WideCharCompare tag);			// 0x0000586D
int compareRangeNoCase(const unsigned short *a, int alen, const unsigned short *b, int blen, WideCharCompare tag);	// 0x00005898

class PooledString
{
public:
	struct StringEntry
	{
		StringEntry *m_next;		// +0
		StringEntry *m_noCase;		// +4
		char m_text[1];				// +8
	};

	PooledString(const AsciiString &that);						// 0x0060BE03

	PooledString &operator=(const AsciiString &that);			// 0x0060BE7F

	int compare(const char *str, int len) const;				// 0x0060BCAB
	int compare(const char *str) const;							// 0x0060BD47
	int compare(const PooledString &that) const;				// 0x0060BD6C
	int compare(const AsciiString &that) const;					// 0x0060BE2B
	int compareNoCase(const char *str, int len) const;			// 0x0060BCE2
	int compareNoCase(const char *str) const;					// 0x0060BD8E
	int compareNoCase(const PooledString &that) const;			// 0x0060BDB3
	int compareNoCase(const AsciiString &that) const;			// 0x0060BE55

	StringEntry *m_entry;										// +0
private:
	void setString(const char *text);
};
// SIZE 0x4

#endif
