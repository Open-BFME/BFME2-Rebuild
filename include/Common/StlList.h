// StlList.h: the STLport node list the game was built with, as a declaration
// surface. Retail's TUs are compiled against STLport 4.6 (namespace _STL), so
// the container member names, method symbols and inline iterator shapes here
// are STLport's. Only the call surface is declared: the bodies live in
// STLport's own unit, exactly as retail has them.
#pragma once
#ifndef BFME2_STLLIST_H
#define BFME2_STLLIST_H

#include "Common/StlAllocator.h"
#if defined(_STLPORT_VERSION)
#include <list>
#else

namespace _STL {

template <class _Tp>
struct _List_node
{
	_List_node *_M_next;
	_List_node *_M_prev;
	_Tp _M_data;
};

template <class _Tp>
class _List_iterator
{
public:
	_List_node<_Tp> *_M_node;

	_List_iterator() {}
	_List_iterator(_List_node<_Tp> *__x) : _M_node(__x) {}
	// STLport 4.6 spells the copy constructor out; it makes a by-value iterator
	// argument be constructed in place (list<Drawable *>::insert in
	// BloodthirstyUpdate::sacrificeHorde 0x0044E374).
	_List_iterator(const _List_iterator &__x) : _M_node(__x._M_node) {}

	_Tp &operator*() const { return _M_node->_M_data; }
	_Tp *operator->() const { return &_M_node->_M_data; }
	// Inline, as STLport has them (FireWeaponUpdate 0x0048BA3E walks its list in place).
	_List_iterator &operator++() { _M_node = _M_node->_M_next; return *this; }
	_List_iterator &operator--();
	bool operator==(const _List_iterator &__x) const { return _M_node == __x._M_node; }
	bool operator!=(const _List_iterator &__x) const { return _M_node != __x._M_node; }
};

template <class _Tp, class _Alloc>
class _List_base
{
public:
	_List_base(const _Alloc &__a);
	~_List_base();

	_List_node<_Tp> *m_node; // +0x00
};

template <class _Tp>
class list : public _List_base<_Tp, allocator<_Tp> >
{
public:
	typedef _List_iterator<_Tp> iterator;
	typedef _List_iterator<_Tp> const_iterator;

	list(const allocator<_Tp> &__a = allocator<_Tp>()) : _List_base<_Tp, allocator<_Tp> >(__a) {}

	void push_back(const _Tp &__x);
	iterator insert(iterator __position, const _Tp &__x);
	void push_front(const _Tp &__x);
	void pop_back();
	void pop_front();
	void clear();
	void remove(const _Tp &__x);
	iterator erase(iterator __position);
	iterator begin() { return iterator(this->m_node->_M_next); }
	iterator end() { return iterator(this->m_node); }
	const_iterator begin() const { return const_iterator(this->m_node->_M_next); }
	const_iterator end() const { return const_iterator(this->m_node); }
	_Tp &front();
	const _Tp &front() const;
	unsigned int size() const;
	bool empty() const;
};

} // namespace _STL

#endif // real STLport or declaration surface

#endif
