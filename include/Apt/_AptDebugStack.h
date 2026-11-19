#pragma once
// SIZE 0xC, no base, no virtual functions. Retail asserts in the destructor
// 0x006FE160 (the only emitted copy, COMDAT) name the file
// ...\apt\_AptDebugStack.h and test m_nElements == 0, then free m_aElements
// through the allocator callback table. The interpreter's debug call stack
// (+0x34) is the one user: element type AptActionInterpreter::DebugCallStackInfo_t.
#include "Apt/Assert.h"
extern void *(__cdecl *g_pfnMemAlloc)(unsigned int size);        // VA 0x00E17728 (assert text gAptFuncs.pfnMemAlloc)
extern void (__cdecl *g_pfnMemFreeSize)(void *block, unsigned int size); // VA 0x00E17730 (gAptFuncs.pfnMemFreeSize)
template <class T>
class AptDebugStack
{
public:
    AptDebugStack()
        : m_nElements(0), m_nCapacity(0), m_aElements(0)
    {
    }
    ~AptDebugStack();
    // The out-of-line copies retail emits for the interpreter's init and shutdown (COMDAT rows 0x006FE1D0 and
    // 0x006FE270; the map files the first under the placeholder name Rva006FE1D0). The asserts name this header.
    void Rva006FE1D0(int nCapacity);
    void Shutdown();

    // Inline in retail (expanded in PrepareForExecution 0x00700090 and CleanupAfterExecution 0x00706950): Push asserts room (line 0x6A) and
    // stores the pointer; Pop drops the count first, then destroys and frees the top element and clears its slot.
    void Push(T *pItem);
    void Pop();

    int m_nElements; // 0x0
    int m_nCapacity; // 0x4
    T **m_aElements; // 0x8
};

template <class T>
inline AptDebugStack<T>::~AptDebugStack()
{
    if (!(m_nElements == 0)) {
        g_bfmeAptAssertAtE17734("m_nElements == 0", "c:\\projects\\bfme2patch103\\bfme2\\code\\libraries\\source\\apt\\_AptDebugStack.h", 0x3c);
        if (g_bfmeAptBreakOnAssertAtDDC01C) APT_ASSERT_BREAK();
    }
    if (m_aElements) {
        if (!g_pfnMemFreeSize) {
            g_bfmeAptAssertAtE17734("gAptFuncs.pfnMemFreeSize", "c:\\projects\\bfme2patch103\\bfme2\\code\\libraries\\source\\apt\\_AptDebugStack.h", 0x3f);
            if (g_bfmeAptBreakOnAssertAtDDC01C) APT_ASSERT_BREAK();
        }
        g_pfnMemFreeSize(m_aElements, m_nCapacity * 4);
    }
}

template <class T>
inline void AptDebugStack<T>::Rva006FE1D0(int nCapacity)
{
    if (!(m_nCapacity == 0)) {
        g_bfmeAptAssertAtE17734("m_nCapacity == 0", "c:\\projects\\bfme2patch103\\bfme2\\code\\libraries\\source\\apt\\_AptDebugStack.h", 0x46);
        if (g_bfmeAptBreakOnAssertAtDDC01C) APT_ASSERT_BREAK();
    }
    m_nCapacity = nCapacity;
    if (!g_pfnMemAlloc) {
        g_bfmeAptAssertAtE17734("gAptFuncs.pfnMemAlloc", "c:\\projects\\bfme2patch103\\bfme2\\code\\libraries\\source\\apt\\_AptDebugStack.h", 0x48);
        if (g_bfmeAptBreakOnAssertAtDDC01C) APT_ASSERT_BREAK();
    }
    m_aElements = (T **)g_pfnMemAlloc(m_nCapacity * 4);
    if (!(m_aElements != 0)) {
        g_bfmeAptAssertAtE17734("m_aElements != NULL", "c:\\projects\\bfme2patch103\\bfme2\\code\\libraries\\source\\apt\\_AptDebugStack.h", 0x4e);
        if (g_bfmeAptBreakOnAssertAtDDC01C) APT_ASSERT_BREAK();
    }
}

template <class T>
inline void AptDebugStack<T>::Shutdown()
{
    if (!(m_nElements == 0)) {
        g_bfmeAptAssertAtE17734("m_nElements == 0", "c:\\projects\\bfme2patch103\\bfme2\\code\\libraries\\source\\apt\\_AptDebugStack.h", 0x53);
        if (g_bfmeAptBreakOnAssertAtDDC01C) APT_ASSERT_BREAK();
    }
    if (m_aElements) {
        if (!g_pfnMemFreeSize) {
            g_bfmeAptAssertAtE17734("gAptFuncs.pfnMemFreeSize", "c:\\projects\\bfme2patch103\\bfme2\\code\\libraries\\source\\apt\\_AptDebugStack.h", 0x56);
            if (g_bfmeAptBreakOnAssertAtDDC01C) APT_ASSERT_BREAK();
        }
        g_pfnMemFreeSize(m_aElements, m_nCapacity * 4);
    }
    m_nCapacity = 0;
    m_nElements = 0;
    m_aElements = 0;
}

template <class T>
inline void AptDebugStack<T>::Push(T *pItem)
{
    if (!(m_nElements < m_nCapacity)) {
        g_bfmeAptAssertAtE17734("m_nElements < m_nCapacity", "c:\\projects\\bfme2patch103\\bfme2\\code\\libraries\\source\\apt\\_AptDebugStack.h", 0x6a);
        if (g_bfmeAptBreakOnAssertAtDDC01C) APT_ASSERT_BREAK();
    }
    m_aElements[m_nElements] = pItem;
    m_nElements++;
}

template <class T>
inline void AptDebugStack<T>::Pop()
{
    m_nElements--;
    T *item = m_aElements[m_nElements];
    if (item)
        delete item;
    m_aElements[m_nElements] = 0;
}
