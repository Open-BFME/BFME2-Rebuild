// simplevec.h: WWLib's simple vector templates (SimpleVecClass,
// SimpleDynVecClass), written from the retail instantiations for Vector3
// (and the Vector2, Vector4, unsigned char and pointer copies, which share
// these bodies).
//
// Target facts:
// - SimpleVecClass<T>: vptr +0 (vftable 0x00BC6F44 for Vector3: slot 0 scalar
//   deleting destructor, 1 Resize, 2 Uninitialised_Grow), Vector +4,
//   VectorMax +8. SimpleDynVecClass<T> adds ActiveCount at +0xC (16 bytes);
//   its vftable (0x00BC6F50) has the same three slots.
// - Neither class declares a copy constructor or assignment: retail's
//   SegmentedLineClass copies (0x0015E640, 0x0015E670) are memberwise stores
//   of Vector, VectorMax and ActiveCount, so a copy shares the buffer. They
//   are implicit here too.
// - Destructors (0x0007E1AE, 0x0007E2D7): free Vector with the scalar operator
//   delete and zero it (and VectorMax, inside the same test);
//   SimpleDynVecClass does the free itself and then runs the base destructor.
// - Resize (0x0007E1CF): a new size equal to VectorMax does nothing; a
//   positive size allocates a new array, copies min(newsize, VectorMax)
//   elements with memcpy, frees the old array; a size of zero or less frees
//   the array and zeroes VectorMax. Always returns true. Uninitialised_Grow
//   (0x0007E26F) only reallocates (no copy) when the requested size is larger
//   than VectorMax.
// - SimpleDynVecClass::Resize (0x0007E9BA): the base Resize, then ActiveCount
//   is clamped to the new VectorMax.
// - Grow (0x001001D5): the larger of VectorMax + VectorMax/4 and VectorMax + 4,
//   raised to the caller's hint, handed to the virtual Resize. Shrink
//   (0x00100210): Resize(ActiveCount) when ActiveCount < VectorMax/4.
// - Add (0x001002C5), Delete (0x00100306), Delete_All and Delete(const T&)
//   (0x00100354) are ordinary members; callers in /O2 units inline them.
#pragma once
#ifndef BFME2_SIMPLEVEC_H
#define BFME2_SIMPLEVEC_H

#include <string.h>

template <class T> class SimpleVecClass
{
public:
	SimpleVecClass(int size = 0) : Vector(0), VectorMax(0)
	{
		if (size > 0)
			Resize(size);
	}

	virtual ~SimpleVecClass()
	{
		if (Vector) {
			delete[] Vector;
			Vector = 0;
			VectorMax = 0;
		}
	}

	T &operator[](int index) { return Vector[index]; }
	const T &operator[](int index) const { return Vector[index]; }
	int Length() const { return VectorMax; }

	virtual bool Resize(int newsize)
	{
		if (VectorMax != newsize) {
			if (newsize > 0) {
				T *fresh = new T[newsize];
				if (Vector) {
					int keep = (newsize < VectorMax) ? newsize : VectorMax;
					memcpy(fresh, Vector, keep * sizeof(T));
					delete[] Vector;
					Vector = 0;
				}
				Vector = fresh;
				VectorMax = newsize;
			} else {
				VectorMax = 0;
				if (Vector) {
					delete[] Vector;
					Vector = 0;
				}
			}
		}
		return true;
	}

	virtual bool Uninitialised_Grow(int newsize)
	{
		if (newsize > VectorMax && newsize > 0) {
			delete[] Vector;
			Vector = new T[newsize];
			VectorMax = newsize;
		}
		return true;
	}

protected:
	T *Vector;			// +0x4
	int VectorMax;		// +0x8
};

template <class T> class SimpleDynVecClass : public SimpleVecClass<T>
{
public:
	SimpleDynVecClass(int size = 0) : SimpleVecClass<T>(size), ActiveCount(0) {}

	virtual ~SimpleDynVecClass()
	{
		if (this->Vector) {
			delete[] this->Vector;
			this->Vector = 0;
		}
	}

	int Count() const { return ActiveCount; }
	T &operator[](int index) { return this->Vector[index]; }
	const T &operator[](int index) const { return this->Vector[index]; }

	virtual bool Resize(int newsize)
	{
		if (!SimpleVecClass<T>::Resize(newsize))
			return false;
		if (this->VectorMax < ActiveCount)
			ActiveCount = this->VectorMax;
		return true;
	}

	bool Add(const T &object, int new_size_hint = 0)
	{
		if (ActiveCount >= this->VectorMax) {
			if (!Grow(new_size_hint))
				return false;
		}
		(*this)[ActiveCount++] = object;
		return true;
	}

	bool Delete(int index, bool allow_shrink = true)
	{
		if (index < ActiveCount - 1)
			memmove(&this->Vector[index], &this->Vector[index + 1], (ActiveCount - index - 1) * sizeof(T));
		ActiveCount--;
		if (allow_shrink)
			Shrink();
		return true;
	}

	void Delete_All(bool allow_shrink = true)
	{
		ActiveCount = 0;
		if (allow_shrink)
			Shrink();
	}

	// W3DFontLibrary::~W3DFontLibrary 0x0009023C: `mov [ActiveCount], 0` then the out-of-line Shrink (0x00100210).
	void Clear(void)
	{
		ActiveCount = 0;
		Shrink();
	}

protected:
	bool Grow(int new_size_hint)
	{
		// Zero Hour's MAX(a, b) macro: both operands are written out in each arm.
		int newsize = (this->VectorMax + this->VectorMax / 4 > this->VectorMax + 4) ? (this->VectorMax + this->VectorMax / 4) : (this->VectorMax + 4);
		newsize = (newsize > new_size_hint) ? newsize : new_size_hint;
		return Resize(newsize);
	}

	bool Shrink()
	{
		if (ActiveCount < this->VectorMax / 4)
			return Resize(ActiveCount);
		return true;
	}

	int ActiveCount;	// +0xC
};

#endif
