// StlAlgobase.h: STLport 4.6's min and max (stl/_algobase.h), by reference.
#pragma once
#ifndef BFME2_STLALGOBASE_H
#define BFME2_STLALGOBASE_H
#if defined(_STLPORT_VERSION)
#include <stl/_algobase.h>
#else

namespace _STL {

template <class _Tp>
inline const _Tp &min(const _Tp &__a, const _Tp &__b) { return __b < __a ? __b : __a; }

template <class _Tp>
inline const _Tp &max(const _Tp &__a, const _Tp &__b) { return __a < __b ? __b : __a; }

// Out of line in retail, one body per iterator type (AI::destroyGroup 0x002FE712
// calls the list-iterator copy 0x0029B694).
template <class _InputIter, class _Tp>
_InputIter find(_InputIter __first, _InputIter __last, const _Tp &__val);

}

#endif // real STLport or declaration surface
#endif
