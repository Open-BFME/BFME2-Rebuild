// StlMap.h: STLport 4.6's map<K,T> as retail lays it out (a red-black tree).
// Target facts: StateMachine's state map at +0x08 is 0xC bytes (ctor 0x004D79E1
// constructs it with the out-of-line map constructor 0x0033C432 and the next
// member is at +0x14); its lookups call the out-of-line _Rb_tree::_M_find
// 0x00357180, insert_unique 0x004D795B, clear 0x004D751D and the tree
// destructor 0x004D755F. STLport 4.6 _Rb_tree: _M_header (node pointer),
// _M_node_count, _M_key_compare (empty functor), 0xC bytes.
// The members are out of line in retail (one body per key type, folded across
// pointer value types), so they are declared here and never defined.
#pragma once
#ifndef BFME2_STLMAP_H
#define BFME2_STLMAP_H

#include "Common/StlAllocator.h"

// A unit that opts into retail's STLport (`// stlport46`) gets the real header;
// see Common/Vector.h for the include-order rule.
#if defined(_STLPORT_VERSION)
#include <map>
#else

namespace _STL
{

template <class _Tp>
struct less
{
};

struct _Rb_tree_node_base
{
	bool _M_color;
	_Rb_tree_node_base *_M_parent;
	_Rb_tree_node_base *_M_left;
	_Rb_tree_node_base *_M_right;
};

template <class _Key, class _Tp, class _Compare = less<_Key> >
class map
{
public:
	map();
	~map();
	void clear();

	_Rb_tree_node_base *_M_header;	// +0x00
	unsigned int _M_node_count;		// +0x04
	_Compare _M_key_compare;		// +0x08
};

}

#endif

#endif
