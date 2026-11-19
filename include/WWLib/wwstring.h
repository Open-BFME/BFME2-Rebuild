// wwstring.h: WWLib's StringClass, a pointer to a reference-less heap buffer.
// SIZE 0x04, no bases, no virtuals, no vptr.
// Target facts (retail game.dat): every constructor stores the empty-string
// pointer (global at VA 0x00DD528C, i.e. m_EmptyString) at +0 and no vptr;
// the destructor 0x00065F5B is a 5-byte tail jump to Free_String 0x00610A40. A
// buffer carries an 8-byte header before the characters: Allocate_Buffer
// 0x006109F0 allocates length+8, writes the allocated length at +0 and the
// cached length at +4 and returns allocation+8; Get_Length 0x0011FB10 reads the
// cached length at buffer-4; Store_Allocated_Length 0x00610A20 and
// Get_Allocated_Length 0x00610AB0 use buffer-8. Strings of up to 248 characters
// may live in one of eight 256-byte temporary slots guarded by a lock word
// (Get_String 0x00610B00). The array destructor 0x0011F6B0 and
// VectorClass<StringClass> 0x0011F980 use a 4-byte element stride.
// Out of line in retail (rowed): constructor (const StringClass &) 0x00129BF0,
// operator= 0x000F0E8D (const char *) and 0x00121450, operator+= 0x001214C0
// (const char *), 0x00129C80, 0x006133D0 (char), Get_Length 0x0011FB10, Is_Empty
// 0x00130FEA, Get_Buffer 0x0013B190, Compare_No_Case 0x00154190, Format_Args
// 0x00610D70, Format 0x00610DD0, Copy_Wide 0x00610E30 and the private helpers
// at 0x006109F0..0x00610CF0. The constructors (int,bool) 0x00065F34 and (bool)
// 0x00187CB0 are comdat bodies, and (const char *,bool) 0x000F0ED1 is both
// emitted and expanded in place, so those three are defined inline here.
// Donor facts: names follow Zero Hour's WWLib/wwstring.h (GPL), naming only.
#pragma once
#ifndef BFME2_WWSTRING_H
#define BFME2_WWSTRING_H

#include <stdarg.h>
#include <string.h>
#include "WWLib/mutex.h"

class StringClass
{
public:
	StringClass(bool hint_temporary);					// 0x00187CB0
	StringClass(int initial_len = 0, bool hint_temporary = false);		// 0x00065F34
	StringClass(const StringClass &string, bool hint_temporary = false);	// 0x00129BF0
	StringClass(const char *string, bool hint_temporary = false);		// 0x000F0ED1
	~StringClass() { Free_String(); }					// 0x00065F5B

	const StringClass &operator=(const StringClass &string);		// 0x00121450
	const StringClass &operator=(const char *string);			// 0x000F0E8D
	const StringClass &operator+=(const StringClass &string);		// 0x00129C80
	const StringClass &operator+=(const char *string);			// 0x001214C0
	const StringClass &operator+=(char ch);					// 0x006133D0

	const char &operator[](int index) const { return m_Buffer[index]; }
	char &operator[](int index) { return m_Buffer[index]; }
	operator const char *() const { return m_Buffer; }

	friend StringClass operator+(const char *string1, const StringClass &string2);	// 0x00613440

	int Get_Length() const;							// 0x0011FB10 (inline)
	bool Is_Empty() const { return m_Buffer[0] == m_NullChar; }		// 0x00130FEA
	int __cdecl Format(const char *format, ...);				// 0x00610DD0
	int __cdecl Format_Args(const char *format, const va_list &arg_list);	// 0x00610D70
	int Compare(const char *string) const { return strcmp(m_Buffer, string); }
	int Compare_No_Case(const char *string) const { return _strcmpi(m_Buffer, string); }	// 0x00154190
	bool Copy_Wide(const unsigned short *source);				// 0x00610E30
	char *Get_Buffer(int new_length) { Uninitialised_Grow(new_length); return m_Buffer; }	// 0x0013B190
	char *Peek_Buffer() { return m_Buffer; }
	const char *Peek_Buffer() const { return m_Buffer; }

private:
	struct HEADER
	{
		int allocated_length;		// -8
		int length;			// -4
	};
	enum
	{
		MAX_TEMP_STRING = 8,
		MAX_TEMP_LEN = 256 - sizeof(HEADER),
		MAX_TEMP_BYTES = MAX_TEMP_LEN + sizeof(HEADER),
		ALL_TEMP_STRINGS_USED_MASK = 0xff
	};

	void Get_String(int length, bool is_temp);				// 0x00610B00
	char *Allocate_Buffer(int length);					// 0x006109F0 (inline)
	void Resize(int size);							// 0x00610C70
	void Uninitialised_Grow(int length);					// 0x00610CF0
	void Free_String();							// 0x00610A40 (inline)
	void Store_Length(int length) { if (m_Buffer != m_EmptyString) ((HEADER *)m_Buffer)[-1].length = length; }
	void Store_Allocated_Length(int allocated_length);			// 0x00610A20 (inline)
	int Get_Allocated_Length() const;					// 0x00610AB0 (inline)
	void Set_Buffer_And_Allocated_Length(char *buffer, int length);	// 0x00610AC0 (inline)

	char *m_Buffer;				// +0x00
	static char m_NullChar;
	static char *m_EmptyString;
	static unsigned ReservedMask;
	static char m_TempStrings[];
	static FastCriticalSectionClass m_Mutex;
};
// SIZE 0x04

// BFME tags its array allocations: Allocate_Buffer and Resize call
// ??_U@YAPAXII@Z (0x006C4C30) with the size and the tag 'rts' (0x737472).
void *__cdecl operator new[](unsigned int size, unsigned int tag);

inline char *StringClass::Allocate_Buffer(int length)
{
	char *buffer = new('str') char[length + sizeof(HEADER)];
	((HEADER *)buffer)->length = 0;
	((HEADER *)buffer)->allocated_length = length;
	return buffer + sizeof(HEADER);
}

inline void StringClass::Store_Allocated_Length(int allocated_length)
{
	if (m_Buffer != m_EmptyString) ((HEADER *)m_Buffer)[-1].allocated_length = allocated_length;
}

inline int StringClass::Get_Allocated_Length() const
{
	int length = 0;
	if (m_Buffer != m_EmptyString) length = ((HEADER *)m_Buffer)[-1].allocated_length;
	return length;
}

inline void StringClass::Set_Buffer_And_Allocated_Length(char *buffer, int length)
{
	Free_String();
	m_Buffer = buffer;
	if (m_Buffer != m_EmptyString) {
		Store_Allocated_Length(length);
		Store_Length(0);
	}
}

inline int StringClass::Get_Length() const
{
	int length = 0;
	if (m_Buffer != m_EmptyString) {
		length = ((HEADER *)m_Buffer)[-1].length;
		if (length == 0) {
			length = strlen(m_Buffer);
			((StringClass *)this)->Store_Length(length);
		}
	}
	return length;
}

inline StringClass operator+(const char *string1, const StringClass &string2)
{
	StringClass new_string(string1, true);
	new_string += string2;
	return new_string;
}

inline StringClass::StringClass(bool hint_temporary) : m_Buffer(m_EmptyString)
{
	Get_String(MAX_TEMP_LEN, hint_temporary);
	m_Buffer[0] = m_NullChar;
}

inline StringClass::StringClass(int initial_len, bool hint_temporary) : m_Buffer(m_EmptyString)
{
	Get_String(initial_len, hint_temporary);
	m_Buffer[0] = m_NullChar;
}

inline StringClass::StringClass(const char *string, bool hint_temporary) : m_Buffer(m_EmptyString)
{
	int len = string ? strlen(string) : 0;
	if (hint_temporary || len > 0) Get_String(len + 1, hint_temporary);
	(*this) = string;
}

inline StringClass::StringClass(const StringClass &string, bool hint_temporary) : m_Buffer(m_EmptyString)
{
	int len = string.Get_Length();
	if (hint_temporary || len > 0) Get_String(len + 1, hint_temporary);
	(*this) = string;
}

#endif
