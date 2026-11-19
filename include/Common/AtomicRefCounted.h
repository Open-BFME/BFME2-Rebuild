#pragma once
// Role-inferred identity of old OpaqueRefCounted/Rva00051E4D. Target ctor50F19
// stores table7C5128 and count0; Release_Ref50ED3 calls the sole deleting-dtor
// slot, then ::operator delete when InterlockedDecrement is <=0. This is a
// different hierarchy from WWLib RefCountClass (two slots, non-atomic count).
// No donor spelling is asserted. AudioEventInfo and pooled audio holders use it.
extern "C" __declspec(dllimport) long __stdcall InterlockedIncrement(volatile long *);
class AtomicRefCounted
{
public:
    AtomicRefCounted() : m_referenceCount(0) {}
    virtual ~AtomicRefCounted() {}
    void Add_Ref() { InterlockedIncrement(&m_referenceCount); }
    void Release_Ref(); //50ED3
    bool tryAddRef(); //50EFA: increment, retain only if resulting count >1
private:
    long m_referenceCount; //+4
};
typedef char AtomicRefCountedSize[sizeof(AtomicRefCounted)==8?1:-1];