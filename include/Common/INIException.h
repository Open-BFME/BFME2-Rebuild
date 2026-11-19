// Target: ctor 0x0002F681 stores message +0 and int +4; destructor
// 0x0002BD30 frees message. CatchableType 0x008FE2D8 proves size 8,
// no base adjustment, copy ctor 0x000588C5. Second member name unknown.
// The destructor is a header inline: retail has no out-of-line copy, each TU emits it
// as a COMDAT and the linker folds them with XferException's (0x0002BD30: delete[] the
// message). The copy constructor 0x000588C5 and assignment 0x0002F6D6 are folds of the
// same kind; they stay declarations until the map identifies strlen/strcpy, which they call.
#pragma once
#include <new>
class INIException
{
public:
 INIException(int,const char *,...);
 INIException(const INIException &);
 ~INIException() { delete[] m_failureMessage; }
 char *m_failureMessage; // 0x00
 int m_unk0x04; // 0x04
};
// SIZE 0x08
