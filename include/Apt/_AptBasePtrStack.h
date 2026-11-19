#pragma once
// The inline bodies of AptBasePtrStack, included only by the units that retail
// expands them in (AptActionInterpreter.cpp): the assertion text in those expansions
// names this file (...\apt\_AptBasePtrStack.h, with the line numbers 0x98, 0xBF,
// 0xC9, 0xCD and 0x10A used below) while AptDate.cpp's natives call the same members
// out of line (0x006FE580 At is a call there), so the class declaration
// (AptBasePtrStack.h) keeps them out of line and a unit that wants them inline
// includes this header.
// Evidence for each body: the retail assert expressions and line numbers, and
// the standalone copies 0x006FE580 (At), 0x006FDF70 (PopNoDec), 0x006FDFA0 (PopAndPush).
#include "Apt/AptBasePtrStack.h"
#include "Apt/AptValue.h"
#include "Apt/Assert.h"

inline AptValue *AptBasePtrStack::At(int nPos)
{
    if (!(m_nElements - nPos > 0)) {
        g_bfmeAptAssertAtE17734("m_nElements - nPos > 0", "c:\\projects\\bfme2patch103\\bfme2\\code\\libraries\\source\\apt\\_AptBasePtrStack.h", 0x10a);
        if (g_bfmeAptBreakOnAssertAtDDC01C) APT_ASSERT_BREAK();
    }
    return m_aElements[m_nElements - nPos - 1];
}

inline void AptBasePtrStack::PopNoDec()
{
    if (!(m_nElements > 0)) {
        g_bfmeAptAssertAtE17734("false && \"[APT] Error, Popping from Stack with 0 elements. Please contact the Apt Team for Support.\"", "c:\\projects\\bfme2patch103\\bfme2\\code\\libraries\\source\\apt\\_AptBasePtrStack.h", 0xbf);
        if (g_bfmeAptBreakOnAssertAtDDC01C) APT_ASSERT_BREAK();
    } else {
        m_nElements--;
    }
}


inline void AptBasePtrStack::Pop()
{
    if (!(m_nElements > 0)) {
        g_bfmeAptAssertAtE17734("false && \"[APT] Error, Popping from Stack with 0 elements. Please contact the Apt Team for Support.\"", "c:\\projects\\bfme2patch103\\bfme2\\code\\libraries\\source\\apt\\_AptBasePtrStack.h", 0x98);
        if (g_bfmeAptBreakOnAssertAtDDC01C) APT_ASSERT_BREAK();
    } else {
        m_aElements[m_nElements - 1]->Release();
        m_nElements--;
    }
}

// evidence: retail expands Pop(2) in place at every constant-count site (Add2 0x00702B60: five copies, each the full body: the two asserts at lines 0xA7/0xAA, the Release loop, the `add [esi], -2`; CastOp 0x00705C70 the same) and calls the one standalone copy 0x006E3AA0 (the identical body: same asserts, same loop) only with a variable count (0x00706038, 0x007061F5, AptAnimation.cpp).
// Plain forms tried: (a) a pointer-adjusting Pop(int) with the per-element work out of line cannot give retail's expansion, which contains the loop and both asserts in place; (b) retail has a single Pop(int), not a constant-count twin (0x006E3AA0 is the same body as each expansion); (c) plain `inline` with /Ob2 on the cl line: Add2 646 B (retail 956), Add2 58%, CastOp 74% (Pop(2) outlined at every site), and with the default level one site expanded and the rest outlined. Only the attribute reproduces retail's per-site decision.
__forceinline void AptBasePtrStack::Pop(int nItems)
{
    if (!(nItems >= 0)) {
        g_bfmeAptAssertAtE17734("nItems >= 0", "c:\\projects\\bfme2patch103\\bfme2\\code\\libraries\\source\\apt\\_AptBasePtrStack.h", 0xa7);
        if (g_bfmeAptBreakOnAssertAtDDC01C) APT_ASSERT_BREAK();
    }
    if (!(m_nElements >= nItems)) {
        g_bfmeAptAssertAtE17734("false && \"[APT] Error, Popping more elements than the stack contains. Please contact the Apt Team for Support.\"", "c:\\projects\\bfme2patch103\\bfme2\\code\\libraries\\source\\apt\\_AptBasePtrStack.h", 0xaa);
        if (g_bfmeAptBreakOnAssertAtDDC01C) APT_ASSERT_BREAK();
        return;
    }
    for (int i = 1; i <= nItems; i++)
        m_aElements[m_nElements - i]->Release();
    m_nElements -= nItems;
}
