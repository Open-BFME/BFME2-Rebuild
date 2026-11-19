#pragma once
// SIZE 0xC, no base, no virtual functions. Same three fields as
// AptBasePtrStack (the interpreter's with/setTarget/this stacks are
// AptValuePtrStack<AptValue>); the retail assert text names the file
// ...\apt\_AptValuePtrStack.h and `at` is the row 0x006DCC10
// (?at@?$AptValuePtrStack@VAptValue@@@@QAEPAVAptValue@@H@Z).
// AptIntervalTimer's parameter stack (0x006E3CF0) is the same {count, capacity, array} triple but is
// built as {0, 6, a 24-byte block from the Apt pool}, so a second construction form exists in
// retail (AptIntervalTimerParams in AptIntervalTimer.h models it as a plain struct); the form here is
// the zero one the interpreter's constructor inlines.
template <class T>
class AptValuePtrStack
{
public:
    AptValuePtrStack()
        : m_nElements(0), m_nCapacity(0), m_aElements(0)
    {
    }
    ~AptValuePtrStack(); // the interpreter's destructor 0x006FE9C0 expands it three times (with, set-target, this stacks)
    T *at(int nPos);
    // Inline in retail (runStream 0x007002C0 pushes and pops the `this` stack with them): Push asserts room (line 0x76), stores the pointer, counts it
    // and takes a reference; Pop asserts the stack is not empty (line 0x7D), releases the top and drops the count.
    void Push(T *pItem);
    void Pop();

    int m_nElements; // 0x0
    int m_nCapacity; // 0x4
    T **m_aElements; // 0x8
};

#include "Apt/Assert.h"
#include "Apt/DogmaAllocator.h"
extern void (__cdecl *g_pfnMemFreeSize)(void *block, unsigned int size); // VA 0x00E17730 (gAptFuncs.pfnMemFreeSize)

template <class T>
inline AptValuePtrStack<T>::~AptValuePtrStack()
{
    if (m_aElements) {
        if (!g_pfnMemFreeSize) {
            g_bfmeAptAssertAtE17734("gAptFuncs.pfnMemFreeSize", "c:\\projects\\bfme2patch103\\bfme2\\code\\libraries\\source\\apt\\_AptValuePtrStack.h", 0x52);
            if (g_bfmeAptBreakOnAssertAtDDC01C) APT_ASSERT_BREAK();
        }
        g_pDogmaPoolManager->freeBlock(m_aElements, m_nCapacity * 4);
    }
}

template <class T>
inline void AptValuePtrStack<T>::Push(T *pItem)
{
    if (!(m_nElements < m_nCapacity)) {
        g_bfmeAptAssertAtE17734("m_nElements < m_nSize", "c:\\projects\\bfme2patch103\\bfme2\\code\\libraries\\source\\apt\\_AptValuePtrStack.h", 0x76);
        if (g_bfmeAptBreakOnAssertAtDDC01C) APT_ASSERT_BREAK();
    }
    m_aElements[m_nElements] = pItem;
    m_nElements++;
    pItem->AddRef();
}

template <class T>
inline void AptValuePtrStack<T>::Pop()
{
    if (!(m_nElements > 0)) {
        g_bfmeAptAssertAtE17734("size() > 0", "c:\\projects\\bfme2patch103\\bfme2\\code\\libraries\\source\\apt\\_AptValuePtrStack.h", 0x7d);
        if (g_bfmeAptBreakOnAssertAtDDC01C) APT_ASSERT_BREAK();
    }
    m_aElements[m_nElements - 1]->Release();
    m_nElements--;
}
