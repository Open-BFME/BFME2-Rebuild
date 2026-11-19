// StlAllocator.h: STLport 4.6's _STL::allocator, shared by the container headers
// (StlList.h, Vector.h) so a unit can include more than one of them.
// It is an empty class; retail's containers reach the node or malloc allocator
// through out-of-line members.
#pragma once
#ifndef BFME2_STLALLOCATOR_H
#define BFME2_STLALLOCATOR_H

// A unit that opts into retail's STLport (`// stlport46`) includes <vector>
// first, which defines _STLPORT_VERSION; the vendored headers then bring their
// own _STL::allocator, so the stand-in below stays out of the way.
// <cstddef> first: in an STLport unit it is STLport's wrapper, whose prolog
// defines _STLPORT_VERSION even when no container header came before this one;
// elsewhere it is the compiler's own header and defines nothing extra.
#include <cstddef>
#if !defined(_STLPORT_VERSION)

namespace _STL {

template <class _Tp>
class allocator
{
public:
	allocator() {}
};

}

#endif

#endif
