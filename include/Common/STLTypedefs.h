// STLTypedefs.h: the rts:: hash functor SAGE passes to its STLport hash_maps.
// Target facts: retail's object table is
//   _STL::hashtable<pair<const ObjectID, Object *>, ObjectID, rts::hash<ObjectID>,
//                   _Select1st<...>, _STL::equal_to<ObjectID>, allocator<...> >
// (destructor/insert/ctor rows of the ObjectID instantiations in the map, and
// _M_find 0x002888D4 shared by the NameKeyType instantiation). _M_find divides
// the key itself by the bucket count, so the functor is the identity on the key.
// Only the hash lives in rts:: in retail; equality is STLport's own equal_to.
#pragma once
#ifndef BFME2_STLTYPEDEFS_H
#define BFME2_STLTYPEDEFS_H

#include <stddef.h>

namespace rts
{

template <class _Key>
struct hash
{
	size_t operator()(const _Key &key) const { return (size_t)key; }
};

} // namespace rts

// AsciiString keys hash case-insensitively. Target facts: retail's out-of-line
// rts::hash<AsciiString>::operator() 0x0002BF8C copies the key (StringBase copy
// constructor 0x000365F0), lowercases the copy (toLower 0x00036A70), hashes its
// characters with STLport's __stl_hash_string 0x0002BA61 and destroys the copy;
// _M_bkt_num_key 0x0002C669 calls it. It needs STLport's string hash, so it is
// declared only in units built against STLport (<cstddef> settles which).
#include <cstddef>
#if defined(_STLPORT_VERSION)
#include <hash_map>
#include "Common/AsciiString.h"

namespace rts
{

template <>
struct hash<AsciiString>
{
	size_t operator()(const AsciiString &key) const
	{
		AsciiString lower = key;
		lower.toLower();
		return _STL::__stl_hash_string(lower.str());
	}
};

} // namespace rts
#endif

#endif
