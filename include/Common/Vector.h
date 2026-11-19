// Vector.h: STLport's vector<T> and vector<bool> as retail lays them out.
// Target facts (old repo Code/Libraries vendored STLport, stlport_vector_bool.cpp):
// vector<T> is three pointers (_M_start, _M_finish, _M_end_of_storage), 0xC bytes.
// vector<bool> is _Bvector_base: _Bit_iterator _M_start and _M_finish (a word
// pointer plus a bit offset each) and a word pointer _M_end_of_storage, 0x14
// bytes; DockUpdate's ctor places its bit vector at +0x6C over +0x80.
// resize/insert/_M_fill_insert are out of line in retail, so they are declared
// here and never defined: an inline definition would be inlined into callers
// and change their bytes.
#pragma once
#ifndef BFME2_VECTOR_H
#define BFME2_VECTOR_H

#include <cstddef>
#include "Common/GameType.h"
#include "Common/StlAllocator.h"

// A unit that opts into retail's STLport (`// stlport46`) gets the real headers:
// it includes <vector> before anything else, which defines _STLPORT_VERSION, and
// then the containers below are the vendored ones rather than these stand-ins.
// The <cstddef> above makes the test independent of include order: in an STLport
// unit it is STLport's wrapper and defines _STLPORT_VERSION.
#if defined(_STLPORT_VERSION)
#include <vector>

namespace _STL
{
typedef vector<bool> bit_vector;
}

#else

namespace _STL
{

// STLport's range destructor, out of line per element type (the CameraMarker
// copy is 0x0048CE25 and its vector's cleanup 0x0048D003 calls it).
template <class _Tp> void _Destroy(_Tp *__first, _Tp *__last);

// STLport's _STLP_alloc_proxy: vector's _M_end_of_storage is one (the end
// pointer plus the allocator). This view lets a body call the allocator through
// it the way STLport does (vector<Coord3D>::reserve 0x00390729).
template <class T> class _Vector_alloc_proxy
{
public:
	T *allocate(unsigned int n, const void *hint = 0);
	T *_M_data;
};

template <class T> class vector
{
public:
	typedef T *iterator;
	typedef const T *const_iterator;
	typedef unsigned int size_type;

	vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
	~vector() { if (_M_start) _M_deallocate(); }

	size_type size() const { return (size_type)(_M_finish - _M_start); }
	T &operator[](size_type n) { return _M_start[n]; }
	const T &operator[](size_type n) const { return _M_start[n]; }
	iterator begin() { return _M_start; }
	iterator end() { return _M_finish; }
	const_iterator begin() const { return _M_start; }
	const_iterator end() const { return _M_finish; }
	bool empty() const { return _M_start == _M_finish; }
	// erase is out of line in retail (e.g. vector<ParticleSystemID>'s 0x00532803).
	iterator erase(iterator first, iterator last);
	void clear() { erase(begin(), end()); }

	void insert(iterator position, size_type count, const T &value) { _M_fill_insert(position, count, value); }

	void resize(size_type newSize, T value);
	void resize(size_type newSize);
	void push_back(const T &value);
	void reserve(size_type n);
	size_type capacity() const { return (size_type)(_M_end_of_storage - _M_start); }
	// STLport's out-of-line growth helpers (e.g. 0x000E016A for 12-byte elements).
	T *_M_allocate_and_copy(size_type n, const T *first, const T *last);
	T *_M_allocate(size_type n);
	T &back() { return *(_M_finish - 1); }
	// Inline in retail: friend_awakenUpdateModule 0x0024297F drops the last sleepy entry with `add [vec+4], -4`.
	void pop_back() { --_M_finish; }
	void _M_fill_insert(iterator position, size_type count, const T &value);
	void _M_deallocate() { ::operator delete(_M_start); }

	T *_M_start;				// 0x0
	T *_M_finish;				// 0x4
	T *_M_end_of_storage;		// 0x8
};
// SIZE 0xC

// STLport's _Bit_reference: a word pointer and the bit's mask inside it.
class _Bit_reference
{
public:
	_Bit_reference(unsigned int *word, unsigned int mask) : _M_p(word), _M_mask(mask) {}
	_Bit_reference(const _Bit_reference &that) : _M_p(that._M_p), _M_mask(that._M_mask) {}

	_Bit_reference &operator=(bool value)
	{
		if (value)
			*_M_p |= _M_mask;
		else
			*_M_p &= ~_M_mask;
		return *this;
	}
	_Bit_reference &operator=(const _Bit_reference &that) { return operator=((bool)that); }

	operator bool() const { return (*_M_p & _M_mask) != 0; }

	unsigned int *_M_p;			// 0x0
	unsigned int _M_mask;		// 0x4
};
// SIZE 0x8

// STLport's _Bit_iterator.
class _Bit_iterator
{
public:
	_Bit_iterator() : _M_p(0), _M_offset(0) {}
	_Bit_iterator(unsigned int *word, unsigned int offset) : _M_p(word), _M_offset(offset) {}

	_Bit_reference operator*() const { return _Bit_reference(_M_p, 1U << _M_offset); }

	_Bit_iterator operator+(int n) const
	{
		unsigned int offset = _M_offset + (unsigned int)n;
		return _Bit_iterator(_M_p + (offset >> 5), offset & 31);
	}

	int operator-(const _Bit_iterator &that) const
	{
		return (int)(_M_p - that._M_p) * 32 + (int)_M_offset - (int)that._M_offset;
	}

	unsigned int *_M_p;			// 0x0
	unsigned int _M_offset;		// 0x4
};
// SIZE 0x8

// vector<bool>: STLport's _Bvector_base plus the element access DockUpdate uses.
class bit_vector
{
public:
	typedef _Bit_iterator iterator;
	typedef unsigned int size_type;

	bit_vector() : _M_start(), _M_finish(), _M_end_of_storage(0) {}
	~bit_vector() { if (_M_start._M_p) _M_deallocate(); }

	size_type size() const { return (size_type)(_M_finish - _M_start); }
	_Bit_reference operator[](size_type n);
	_Bit_reference operator[](size_type n) const { return *(_M_start + (int)n); }
	iterator begin() const { return _M_start; }
	iterator end() const { return _M_finish; }

	void insert(iterator position, size_type count, bool value);
	void resize(size_type newSize, bool value);
	void resize(size_type newSize);
	void push_back(bool value);
	void _M_deallocate() { ::operator delete(_M_start._M_p); }

	_Bit_iterator _M_start;			// 0x0
	_Bit_iterator _M_finish;		// 0x8
	unsigned int *_M_end_of_storage;	// 0x10
};
// SIZE 0x14

}

#endif

#endif
