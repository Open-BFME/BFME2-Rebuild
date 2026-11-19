#pragma once
#include <stddef.h>
// SIZE 0x4, no bases or virtual functions. Retail constructors 0x006D2F90,
// 0x006D2FC0 and 0x006D4C80 store only the representation pointer at +0;
// array consumers 0x0070B603/0x0070DB95 advance by four bytes.
// The representation's four 16-bit words and text at +8 are witnessed by
// 0x006D2E10, 0x006D3750, 0x006D2DF0, 0x006D3BA0 and 0x00620090.
// The old shared header and private views agree on this representation.
class EAStringC
{
public:
    class StringDataC
    {
    public:
        unsigned short m_uRefCount; // 0x0
        unsigned short m_uSize; // 0x2
        unsigned short m_uMaxSize; // 0x4
        unsigned short m_uHash; // 0x6
    };
    enum CBPushZero { CB_NO_PUSH_ZERO, CB_PUSH_ZERO };

    EAStringC();
    EAStringC(const EAStringC &other);
    EAStringC(const char *text);
    EAStringC(unsigned int size);
    EAStringC(unsigned int fillChar, unsigned int size);
    ~EAStringC();
    EAStringC &operator=(const EAStringC &other);
    EAStringC operator+(const EAStringC &other) const;
    EAStringC operator+(const char *other) const;

    static void FreeData(StringDataC *data);
    // 0x006CD4A0 (folded with AsciiString::hasData: `[this] != 0`) and 0x006D3C20 (asserts data != NULL,
    // line 0x517, frees the representation and nulls it). Names are this header's.
    bool hasData() const;
    void FreeAndNull();
    // 0x006D36F0 (the map files it as rva006D36F0): whether two strings hold the same text (the hash table's key comparison).
    bool rva006D36F0(const EAStringC *other) const;
    unsigned short GetHashValue() const;  // 0x006D2F40: the 16-bit hash (the map files it as rva006D2F40)
    unsigned short UpdateHashValue() const; // 0x006D3D10: computes and caches the 16-bit hash at +6 of the representation
    unsigned int GetLength() const;   // 0x006D3750: the size word at +2 of the representation
    const char *c_str() const;        // 0x00620090: the text at +8 of the representation (the map files it as rva00620090)
    bool HasCapacityAbove(unsigned int limit) const; // 0x006D3860: limit < the representation's max size (word at +4)
    unsigned int GetInternalMaxSize() const;
    void setCapacityWord_Rva006D2E00(unsigned int capacity);
    unsigned int GetInternalRefCount() const;
    void SetInternalRefCount(unsigned int value);
    void rva006D2E70();
    bool IsEmpty() const;
    int GetAt(int index) const;
    bool IsEqualTo(const EAStringC *other) const;
    bool rva006D30D0(const EAStringC *other) const; // 0x006D30D0 (the map name): the inequality of two strings (the binding refresh of a text field tests it)
    // 0x006D3090 (the map files it under a placeholder name): content equality with another string.
    bool Equals(const EAStringC *other) const;
    // 0x006D3490 / 0x006D3510 (placeholder-class members in the map): equality with a C string, exact and ignoring case.
    bool EqualsText(const char *text) const;
    bool EqualsTextNoCase(const char *text) const;
    bool operator==(const char *text) const; // 0x006D3230: byte-wise equality with a C string
    int Compare(const char *text) const;
    int Compare(const EAStringC &other) const;
    int rva006D3670(const char *text) const;
    bool operator<(const EAStringC &other) const;
    bool operator<=(const EAStringC &other) const;
    bool operator>(const EAStringC &other) const;
    bool operator>=(const EAStringC &other) const;
    bool operator<(const char *other) const;
    bool operator<=(const char *other) const;
    bool operator>(const char *other) const;
    bool operator>=(const char *other) const;
    void Reserve(int size);
    int Find(const char *text, int start = 0);
    int Find(char character, int start = 0);
    int Replace(char oldCharacter, char newCharacter);
    bool startsWith(const char *text) const;
    bool endsWith(const char *text) const;
    void SetSize(int size);
    // Rows the map files under placeholder method names of EAStringC (their bodies are the asserts the source kept):
    // 0x006D3920 / 0x006D3980 / 0x006D39B0 / 0x006D3A10 assert `false` and return 0 (unimplemented overloads);
    // 0x006D3BA0 stores the lower-case hash of the text; 0x006D3C20 frees the representation and nulls it;
    // 0x006D3C60 points an invalid string at the shared empty representation.
    int rva006D3920(const char *pStrText);
    int rva006D3980(int);
    int rva006D39B0(int, const char *pStrText);
    int rva006D3A10(int, int);
    void rva006D3BA0() const;
    void rva006D3C20();
    int rva006D36C0(const EAStringC &other) const; // 0x006D36C0: the case-insensitive ordering StringPool::Check uses (the map files it under the placeholder StringRef class)
    void rva006D53F0(const char *format, char *args); // 0x006D53F0: formats into this string from a printf-style format and va_list
    void rva006D62C0(const char *pStrFormat, ...); // 0x006D62C0: printf-style assignment (cdecl, this on the stack)
    EAStringC &rva006D6100(); // 0x006D6100: lower-cases every UTF-8 character in place
    EAStringC &rva006D6170(); // 0x006D6170: upper-cases every UTF-8 character in place
    void rva006D5300(const char *pStrFormat, ...); // 0x006D5300: appends printf-style text
    bool rva006D5CD0(const char *pStrText); // 0x006D5CD0: removes a leading prefix when the string starts with it
    bool rva006D5DA0(const char *pStrText); // 0x006D5DA0: removes a trailing suffix when the string ends with it
    void rva006D4190(int iCharacter); // 0x006D4190: stores one code point as the string's UTF-8 text (SetSize, hash cleared)
    EAStringC &Rva006D4F00Append(const EAStringC &other); // 0x006D4F00 (the map name; Append(const EAStringC &) above is its caller-side spelling)
    EAStringC &Rva006D50A0Append(const char *pStrText);   // 0x006D50A0
    int rva006D54B0(int start, int count); // 0x006D54B0: erases `count` bytes at `start` and returns the new length
    EAStringC rva006D5690(int count) const;      // 0x006D5690: the last `count` bytes
    EAStringC rva006D5ED0(int charIndex) const;  // 0x006D5ED0: the text from a UTF-8 character index on
    EAStringC rva006d5f30(int start, int count) const; // 0x006D5F30: the UTF-8 character range [start, start + count) (a negative start widens the count)
    int rva006d6070(const char *pStrText, int iStartChar); // 0x006D6070: the UTF-8 character index of the first match at or after a character index, or -1
    EAStringC &rva006D61E0(int iCharacter); // 0x006D61E0: assigns a single code point (UTF-8) to the string
    void rva006D3470(); // 0x006D3470: StringPool::Teardown runs it on every constant (the map name is the placeholder)
    void rva006D3C60();
    // 0x006D6230 (the map names it bfmeTruncVKK): ChangeBuffer to min(size, current length) characters.
    void Truncate(unsigned int size);
    // 0x006D5E70 (map: rva006d5e70 on the placeholder class Rva006D5E70String): the address of the character at
    // the (UTF-8 aware) index, or null when the index is past the end. 0x006D5EC0: the UTF-8 character count.
    const char *rva006D5E70(int charIndex) const;
    int RemoveAll(char character);           // 0x006D5580: removes every occurrence of the character; returns how many
    int GetCodePointAt(int charIndex) const; // 0x006D5E90: the code point at a UTF-8 character index, or -1 past the end
    // 0x006D5FE0 (map: a placeholder-class member): assigns `count` UTF-8 characters starting at `start`.
    void AssignUtf8(const char *start, int count);
    // 0x006D5EC0: the number of UTF-8 characters.
    int GetUtf8Length() const;
    // 0x006D5F30: the substring of `count` UTF-8 characters from character index `start`.
    EAStringC Utf8Substr(int start, int count) const;
    // 0x006D4F00 (the map files it under a placeholder name): appends another string.
    EAStringC &operator+=(const EAStringC &other);
    // 0x006D61E0: builds the UTF-8 encoding of one code point into this (empty) string.
    void AssignCodePoint(int codePoint);
    int rva006D5EC0();
    void rva006D3CA0(const EAStringC *other);   // 0x006D3CA0: adopts another string's representation (asserts this is invalid first)
    unsigned short rva006D3D10() const;         // 0x006D3D10: the cached lower-case hash, computed on first use (the retail assert text calls the member m_pData)
    void Assign(const char *text);
    EAStringC &Assign(const EAStringC &other);
    EAStringC &Append(const char *text, unsigned int limit);
    // 0x006D50A0 (the map files it under a placeholder name): appends a C string.
    EAStringC &operator+=(const char *text);
    // 0x006D4F00 and 0x006D50A0 (the map names them Rva006D4F00Append / Rva006D50A0Append): append a string / a C string.
    EAStringC &Append(const EAStringC &other);
    EAStringC &Append(const char *text);
    // 0x00620090: the NUL-terminated text of the representation (name is this header's).
    const char *GetText() const;
    EAStringC Left(int count) const;
    EAStringC Mid(int start) const;
    EAStringC Mid(int start, int count) const;
    EAStringC &MakeLower();
    // 0x006D59A0 (the map names it bfmeReverseVKK on a placeholder class): reverses the characters in place (a private copy first), clears the cached hash.
    EAStringC &Reverse();
    EAStringC &MakeUpper();
    // 0x006D6100 / 0x006D6170: the UTF-8 aware case conversions the ActionScript string methods use (the map files
    // them as rva006D6100 and rva006D6170; the names are this header's).
    void MakeLowerUtf8();
    void MakeUpperUtf8();
    EAStringC &TrimRight(const char *text);
    void Trim(const char *text); // 0x006D5CB0: TrimLeft (0x006D59F0, filed in the map as rva006D59F0) then TrimRight
    EAStringC &rva006D5FE0(void *source, int nChars); // 0x006D5FE0 (map name kept; the map types the source as void *): appends the first nChars UTF-8 characters of the text
    EAStringC &rva006D59F0(const char *text);        // 0x006D59F0 (map name kept)
    int rva006D47F0(const char *first, const char *second); // 0x006D47F0 (map name kept)

    StringDataC *data; // 0x0; all accesses go through the representation
private:
    void ChangeBuffer(unsigned int reserve, unsigned int offset,
        unsigned int count, CBPushZero pushZero, unsigned int size);
};
// The lifetime members are defined (inline) only in EAString.cpp: the empty-string
// construction in Mid/Left/Assign (0x006D55B0, 0x006D5770, 0x006D5850) is in place there,
// while every other unit calls the emitted copies, the COMDAT rows at 0x006D2F90 (folded
// with clear) and 0x006D3010 -- AptActionInterpreter's PushString 0x007053B0 calls both
// out of line, as do AptNativeHash::ClearData and the interpreter's GetVariable handler.
extern EAStringC::StringDataC g_eaEmptyStringData; // 0x00DDC020


typedef char EAStringCSize[(sizeof(EAStringC) == 4) ? 1 : -1];
typedef char EAStringCDataSize[(sizeof(EAStringC::StringDataC) == 8) ? 1 : -1];
typedef char EAStringCDataSizeOffset[(offsetof(EAStringC::StringDataC,m_uSize) == 2) ? 1 : -1];
