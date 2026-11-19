// Portions derived from Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// widestring.h: WWLib's wide-character string, the 16-bit twin of StringClass
// (WWVegas/WWLib/widestring.h). SIZE 0x04, no bases, no virtuals.
// Target facts (retail game.dat): the buffer pointer lives at +0 and equals the
// empty-string pointer (VA 0x00DD6338, pointing at a null word at 0x00E0958C) when
// the string is empty; a real buffer has the 8-byte header StringClass uses
// (allocated length at -8, cached length at -4) with 16-bit characters. Four
// 0x208-byte temporary buffers (256 characters plus header) are handed out from
// the pointer table at VA 0x00DD633C (free) and recorded in the table at VA
// 0x00E09590 (reserved); the count of reserved ones is at VA 0x00E09588 and the
// lock word at VA 0x00E095A0. Allocation is the plain operator new (0x0002FDA0),
// unlike StringClass's tagged array allocation.
// Rowed members: Allocate_Buffer 0x00619B30, Store_Allocated_Length 0x00619B60,
// Free_String 0x00619B80, Is_ANSI 0x00619C00, Get_Allocated_Length 0x00619C30,
// Set_Buffer_And_Allocated_Length 0x00619C40, Get_String 0x00619C80, Resize
// 0x00619DC0, Uninitialised_Grow 0x00619E30, Convert_From 0x00619EB0, Format_Args
// 0x00619F10, Format 0x00619F70; also Get_Buffer 0x0060F100, Store_Length 0x0060F130,
// operator=(const WCHAR *) 0x0060FC30, Get_Length 0x00615EF0, the (const WCHAR *,bool)
// constructor 0x006164B0.
// Donor facts: names follow Zero Hour's WWLib/widestring.h (GPL), naming only.
#pragma once
#ifndef BFME2_WIDESTRING_H
#define BFME2_WIDESTRING_H

#include <stdarg.h>
#include <string.h>
#include <wchar.h>
#include "WWLib/mutex.h"

class WideStringClass
{
public:
	WideStringClass(int initial_len = 0, bool hint_temporary = false);
	WideStringClass(const unsigned short *string, bool hint_temporary = false);	// 0x006164B0
	~WideStringClass() { Free_String(); }						// 0x00615B70

	const WideStringClass &operator=(const unsigned short *string);			// 0x0060FC30

	operator const unsigned short *() const { return m_Buffer; }

	int Get_Length() const;								// 0x00615EF0
	bool Is_Empty() const { return m_Buffer[0] == m_NullChar; }
	bool Is_ANSI();									// 0x00619C00
	bool Convert_From(const char *text);						// 0x00619EB0
	int __cdecl Format(const unsigned short *format, ...);				// 0x00619F70
	int __cdecl Format_Args(const unsigned short *format, const va_list &arg_list);	// 0x00619F10
	unsigned short *Get_Buffer(int new_length) { Uninitialised_Grow(new_length); return m_Buffer; }	// 0x0060F100

private:
	struct HEADER
	{
		int allocated_length;		// -8
		int length;			// -4
	};
	enum
	{
		MAX_TEMP_STRING = 4,
		MAX_TEMP_LEN = 256,
		MAX_TEMP_BYTES = (MAX_TEMP_LEN * sizeof(unsigned short)) + sizeof(HEADER)
	};

	void Get_String(int length, bool is_temp);					// 0x00619C80
	unsigned short *Allocate_Buffer(int length);					// 0x00619B30
	void Resize(int size);								// 0x00619DC0
	void Uninitialised_Grow(int length);						// 0x00619E30
	void Free_String();								// 0x00619B80
	void Store_Length(int length) { if (m_Buffer != m_EmptyString) ((HEADER *)m_Buffer)[-1].length = length; }	// 0x0060F130
	void Store_Allocated_Length(int allocated_length);				// 0x00619B60
	int Get_Allocated_Length() const;						// 0x00619C30
	void Set_Buffer_And_Allocated_Length(unsigned short *buffer, int length);	// 0x00619C40

	unsigned short *m_Buffer;		// +0x00
	static unsigned short m_NullChar;
	static unsigned short *m_EmptyString;
	static char m_TempString1[MAX_TEMP_BYTES];
	static char m_TempString2[MAX_TEMP_BYTES];
	static char m_TempString3[MAX_TEMP_BYTES];
	static char m_TempString4[MAX_TEMP_BYTES];
	static unsigned short *m_FreeTempStrings[MAX_TEMP_STRING];
	static unsigned short *m_ResTempStrings[MAX_TEMP_STRING];
	static int m_UsedTempStringCount;
	static FastCriticalSectionClass m_TempMutex;
};
// SIZE 0x04

// (const WCHAR *, bool) constructor 0x006164B0: starts empty, takes a temporary or sized buffer when asked
// or when the text is not empty, then copies through the operator= body.
inline WideStringClass::WideStringClass(const unsigned short *string, bool hint_temporary)
	: m_Buffer(m_EmptyString)
{
	int length = string != 0 ? (int)wcslen((const wchar_t *)string) : 0;
	if (hint_temporary || length > 0) Get_String(length + 1, hint_temporary);
	*this = string;
}

inline int WideStringClass::Get_Length() const
{
	int length = 0;
	if (m_Buffer != m_EmptyString) {
		length = ((HEADER *)m_Buffer)[-1].length;
		if (length == 0) {
			length = wcslen(m_Buffer);
			((WideStringClass *)this)->Store_Length(length);
		}
	}
	return length;
}

inline void WideStringClass::Store_Allocated_Length(int allocated_length)
{
	if (m_Buffer != m_EmptyString) ((HEADER *)m_Buffer)[-1].allocated_length = allocated_length;
}

inline int WideStringClass::Get_Allocated_Length() const
{
	int length = 0;
	if (m_Buffer != m_EmptyString) length = ((HEADER *)m_Buffer)[-1].allocated_length;
	return length;
}

inline void WideStringClass::Set_Buffer_And_Allocated_Length(unsigned short *buffer, int length)
{
	Free_String();
	m_Buffer = buffer;
	if (m_Buffer != m_EmptyString) {
		Store_Allocated_Length(length);
		Store_Length(0);
	}
}

inline unsigned short *WideStringClass::Allocate_Buffer(int length)
{
	//
	//	Allocate a buffer that is 'length' characters long, plus the
	// bytes required to hold the header.
	//
	char *buffer = (char *)::operator new((sizeof(unsigned short) * length) + sizeof(WideStringClass::HEADER));

	//
	//	Fill in the fields of the header
	//
	HEADER *header = reinterpret_cast<HEADER *>(buffer);
	header->length = 0;
	header->allocated_length = length;

	//
	//	Return the buffer as if it was a WCHAR pointer
	//
	return reinterpret_cast<unsigned short *>(buffer + sizeof(WideStringClass::HEADER));
}

// operator=(const WCHAR *) 0x0060FC30 (an inline copy: a null source leaves the string alone; otherwise the
// buffer grows to length + 1, the length is stored and the characters, terminator included, are copied).
inline const WideStringClass &WideStringClass::operator=(const unsigned short *string)
{
	if (string != 0) {
		int length = (int)wcslen((const wchar_t *)string);
		Uninitialised_Grow(length + 1);
		Store_Length(length);
		memcpy(m_Buffer, string, (length + 1) * sizeof(unsigned short));
	}
	return *this;
}

#endif
