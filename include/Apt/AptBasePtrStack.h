#pragma once
// SIZE 0xC, no base, no virtual functions. Retail asserts (file
// ...\apt\_AptBasePtrStack.h) name the members m_nElements, m_nCapacity and
// m_aElements; bodies 0x006FE580 (At), 0x006FE6E0 (Push) and 0x006FDD40
// (destructor) read them at +0, +4 and +8, and the interpreter constructor
// 0x006FE980 zeroes them in that order.
// The map still spells the element type BfmeAptValue006DCD20, the placeholder
// for AptValue, in every one of these mangled names; the declarations here use
// the real type.
class AptValue;

class AptBasePtrStack
{
public:
    AptBasePtrStack()
        : m_nElements(0), m_nCapacity(0), m_aElements(0)
    {
    }
    ~AptBasePtrStack();
    AptValue *At(int nPos);
    AptValue *rva007062F0(); // 0x007062F0: the top element (At(0) as its own function)
    // Pool-backed storage (retail's _AptBasePtrStack.h expansions, one out-of-line copy each; the map files them under
    // these placeholder names): 0x006FDDB0 sets the capacity and allocates the array, 0x006FDE50 frees it (asserting
    // the stack is empty) and zeroes the triple, 0x006FE0B0 is the same allocation with _AptValuePtrStack.h's asserts,
    // 0x006FE120 the free-and-zero without asserts.
    void rva006FDDB0(int nCapacity);
    void rva006FDE50();
    void rva006FE0B0(int nCapacity);
    void rva006FE120();
    void Push(AptValue *pValue);
    // 0x006FE7B0: Push without the AddRef; both typed pushes assert `pValue`, `isLookup() == false`
    // and `isRegister() == false` (AptActionInterpreter.inl lines 0x85-0x87 and 0x9C-0x9E).
    // The name is this header's.
    void PushNoAddRef(AptValue *pValue);
    void PopNoDec();
    // Out-of-line base-class copies (the map files them under placeholder names): 0x006FDEE0 pushes and AddRefs,
    // 0x006FDF30 pushes without AddRef (assert lines 0x80/0x90), 0x006FE050 pops n (assert line 0xF0), 0x006FE5D0 pops
    // and returns the top (assert line 0x110). The names are this header's.
    void PushRaw(AptValue *pValue);
    void PushRawNoAddRef(AptValue *pValue);
    void PopN(int nItems);
    AptValue *PopReturn();
    void Pop();
    // Pop(nItems): asserts nItems >= 0 (line 0xA7) and enough elements (0xAA, 0x006E3AA0 is the standalone copy).
    void Pop(int nItems);
    void rva006FE920(); // 0x006FE920 (the map name): the out-of-line copy of Pop()
    void rva006E3AA0(int nItems); // 0x006E3AA0: the out-of-line copy of Pop(nItems) AptAnimation.cpp calls (the map names it rva006E3AA0)
    int GetSize() const { return m_nElements; }
    void PopAndPush(int nItems, AptValue *pValue);
    // 0x006FE880 (the map files it as rva006FE880): the interpreter's own out-of-line version, with the value-type asserts.
    void PopAndPushValue(int nItems, AptValue *pValue);

    int m_nElements; // 0x0
    int m_nCapacity; // 0x4
    AptValue **m_aElements; // 0x8
};

typedef char AptBasePtrStackSize[(sizeof(AptBasePtrStack) == 0xC) ? 1 : -1];
