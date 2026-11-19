// AptRef.h: the intrusive reference-counted handle that retail's Apt code and the
// font library share, and the base class its referents derive from. SIZE 0x4
// (handle) and 0x8 (target).
//
// Target facts (retail RVAs; table addresses are VAs). The referent has one
// virtual function, its deleting destructor, in the single-slot table VA
// 0x007C6F20 (slot 0x0007E5EE); 76 constructors store that table as their base
// vptr and 0x0007DF07 is the stand-alone constructor (vptr, count 0). The count
// is the dword at +4. The release body 0x0007DEEF decrements it, and at <= 0 calls
// slot 0 with flag 0 (an explicit destructor call) and frees the storage through
// the global operator delete (0x0002FD60); it is out of line and called from every
// handle destructor that is not inlined. The handle is four bytes (the pointer);
// copying it increments +4 inline (pair copy 0x00358B43), the destructor releases
// a non-null pointee (inline in 0x002175CE, out of line 0x005F8F96) and the
// assignment 0x002174A4 retains the source, releases the destination and stores
// (self-assignment is a no-op); it is out of line.
//
// Names: the handle is `AptRef<T>` because the map's folded rows for 0x002174A4 and
// 0x005F8F96 are spelled that way for the Apt classes (AptCommandMap,
// AptOverButtonHandler, AptTimer, AptExternHandler, AptCustomRender); the base class
// name is role-inferred (the old repo called the same table Rva0007DF07).
#pragma once
#ifndef BFME2_APTREF_H
#define BFME2_APTREF_H

class AptRefTarget
{
public:
	AptRefTarget() : m_refCount(0) {}					// 0x0007DF07
	virtual ~AptRefTarget() {}							// slot 0: deleting dtor 0x0007E5EE
	void Release();										// 0x0007DEEF, out of line

	int m_refCount;										// +0x04
};
// SIZE 0x8

template <class T>
class AptRef
{
public:
	AptRef() : m_ptr(0) {}
	AptRef(T *ptr) : m_ptr(ptr) { if (m_ptr) ++m_ptr->m_refCount; }
	AptRef(const AptRef &other) : m_ptr(other.m_ptr) { if (m_ptr) ++m_ptr->m_refCount; }	// inline: 0x00358B43
	~AptRef() { if (m_ptr) m_ptr->Release(); }
	AptRef &operator=(const AptRef &other);				// 0x002174A4, out of line

	T *get() const { return m_ptr; }

	T *m_ptr;											// +0x00
};
// SIZE 0x4

#endif
