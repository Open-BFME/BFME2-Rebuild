#pragma once
// SIZE 0x20 (the ctor 0x006E3CF0 initialises +0..+0x1C; the timer array in
// AptAnimationPoolData steps by 0x20). A ActionScript setInterval/setTimeout entry.
// Evidence: the constructor zeroes +0..+0x0C (+0x10 is left for the caller), then builds the
// parameter stack at +0x14 (count 0, capacity 6, an array of six pointers from the Apt
// block pool); cleanParams 0x006E3D50 releases and pops every parameter, with the
// assertions `size() > 0` and `m_aElements != NULL` from ...\apt\_AptValuePtrStack.h
// (which fix the stack's member names m_nElements/m_nCapacity/m_aElements and the
// layout {count, capacity, array}). The five leading members' names are the donor's
// spelling; nothing recovered here exercises them beyond their offsets.
class AptValue;
#include "Apt/Assert.h"

struct AptIntervalTimerParams
{
    // The inline accessor of the retail header _AptValuePtrStack.h (line 0x89, expression `m_nElements - nPos > 0`) that the timer tick
    // (AptAnimationPoolData::tickIntervalTimers 0x006E4150) expands for every parameter it pushes: counted from the top.
    AptValue *peek(int nPos) const
    {
        if (!(m_nElements - nPos > 0)) {
            g_bfmeAptAssertAtE17734("m_nElements - nPos > 0", "c:\\projects\\bfme2patch103\\bfme2\\code\\libraries\\source\\apt\\_AptValuePtrStack.h", 0x89);
            if (g_bfmeAptBreakOnAssertAtDDC01C)
                APT_ASSERT_BREAK();
        }
        return m_aElements[m_nElements - nPos - 1];
    }

    // The inline Push of the same header (line 0x76, expression `m_nElements < m_nSize`) that setInterval 0x006FEFF0 expands for every extra argument:
    // asserts room, stores the pointer, counts it and takes a reference. The name is this header's.
    void push(AptValue *pValue);

    int m_nElements;     // 0x14
    int m_nCapacity;     // 0x18
    AptValue **m_aElements; // 0x1C
};

struct AptIntervalTimer
{
    AptIntervalTimer();
    ~AptIntervalTimer();
    void cleanParams();

    int active;          // 0x00
    AptValue *func;      // 0x04
    float interval;      // 0x08
    float remaining;     // 0x0C
    AptValue *target;    // 0x10
    AptIntervalTimerParams params; // 0x14
};

typedef char AptIntervalTimerSize[(sizeof(AptIntervalTimer) == 0x20) ? 1 : -1];

#include "Apt/AptValue.h"

inline void AptIntervalTimerParams::push(AptValue *pValue)
{
    if (!(m_nElements < m_nCapacity)) {
        g_bfmeAptAssertAtE17734("m_nElements < m_nSize","c:\\projects\\bfme2patch103\\bfme2\\code\\libraries\\source\\apt\\_AptValuePtrStack.h", 0x76);
        if (g_bfmeAptBreakOnAssertAtDDC01C)
            APT_ASSERT_BREAK();
    }
    m_aElements[m_nElements] = pValue;
    m_nElements++;
    pValue->AddRef();
}
