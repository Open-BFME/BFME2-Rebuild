// StlHashMap.h: STLport 4.6's hash_map as retail lays it out.
// Target facts (retail _M_find 0x002888D4, GameLogic::findObjectByID 0x00049DC5,
// GameLogic ctor 0x002470AE):
//   hashtable +0x00 _M_hash, +0x01 _M_equals, +0x02 _M_get_key (empty functors),
//             +0x04 _M_buckets (vector<void *>: start +0x04, finish +0x08),
//             +0x10 _M_num_elements; SIZE 0x14.
//   _Hashtable_node: +0x00 _M_next, +0x04 key, +0x08 mapped value.
// _M_find is out of line in retail (one body per key type, folded across the
// enum keys), so it is declared here and never defined: an inline definition
// would be inlined into callers and change their bytes.
#pragma once
#ifndef BFME2_STLHASHMAP_H
#define BFME2_STLHASHMAP_H

#include "Common/Vector.h"

// A unit that opts into retail's STLport (`// stlport46`) gets the real header;
// see Common/Vector.h for the include-order rule.
#if defined(_STLPORT_VERSION)
#include <hash_map>
#else

namespace _STL
{

template <class _T1, class _T2>
struct pair
{
	_T1 first;
	_T2 second;
};

template <class _Pair>
struct _Select1st
{
};

template <class _Tp>
struct equal_to
{
};

template <class _Val>
struct _Hashtable_node
{
	_Hashtable_node *_M_next;
	_Val _M_val;
};

template <class _Val, class _Key, class _HF, class _ExK, class _EqK, class _All>
class hashtable;

template <class _Val, class _Key, class _HF, class _ExK, class _EqK, class _All>
struct _Ht_iterator
{
	typedef _Hashtable_node<_Val> _Node;
	_Ht_iterator(_Node *__n, const hashtable<_Val, _Key, _HF, _ExK, _EqK, _All> *__tab) : _M_cur(__n), _M_ht(__tab) {}
	_Val &operator*() const { return _M_cur->_M_val; }
	_Val *operator->() const { return &_M_cur->_M_val; }
	bool operator==(const _Ht_iterator &__it) const { return _M_cur == __it._M_cur; }
	bool operator!=(const _Ht_iterator &__it) const { return _M_cur != __it._M_cur; }

	_Node *_M_cur;
	const hashtable<_Val, _Key, _HF, _ExK, _EqK, _All> *_M_ht;
};

template <class _Val, class _Key, class _HF, class _ExK, class _EqK, class _All>
class hashtable
{
public:
	typedef _Hashtable_node<_Val> _Node;
	typedef _Ht_iterator<_Val, _Key, _HF, _ExK, _EqK, _All> iterator;
	typedef unsigned int size_type;

	iterator find(const _Key &__key) { return iterator(_M_find(__key), this); }
	iterator end() { return iterator(0, this); }
	size_type size() const { return _M_num_elements; }

	template <class _KT> _Node *_M_find(const _KT &__key) const;

	_HF _M_hash;				// +0x00
	_EqK _M_equals;				// +0x01
	_ExK _M_get_key;			// +0x02
	vector<void *> _M_buckets;		// +0x04
	size_type _M_num_elements;		// +0x10
};

template <class _Key, class _Tp, class _HashFcn, class _EqualKey>
class hash_map
{
public:
	typedef pair<const _Key, _Tp> value_type;
	typedef hashtable<value_type, _Key, _HashFcn, _Select1st<value_type>, _EqualKey, allocator<value_type> > _Ht;
	typedef typename _Ht::iterator iterator;
	typedef typename _Ht::size_type size_type;

	iterator find(const _Key &__key) { return _M_ht.find(__key); }
	iterator end() { return _M_ht.end(); }
	size_type size() const { return _M_ht.size(); }

	_Ht _M_ht;
};

} // namespace _STL

#endif // real STLport or declaration surface

#endif
