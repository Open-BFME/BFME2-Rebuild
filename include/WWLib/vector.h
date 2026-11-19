// vector.h: WWLib's VectorClass and DynamicVectorClass templates, written from
// retail's instantiations (pointer, float, Vector3 and TextureStatisticsStruct
// copies compared) and the demangled member names the map records.
//
// Target facts:
// - VectorClass<T> is 0xE bytes plus padding to 0x10: vptr +0, Vector +4,
//   VectorMax +8, IsValid +0xC, IsAllocated +0xD. Its vftable has six slots:
//   0 scalar deleting destructor, 1 operator==, 2 Resize, 3 Clear,
//   4 ID(const T &), 5 ID(const T *) (the later declaration comes first).
//   ID(const T *) divides an unsigned byte distance by sizeof(T) (retail's
//   0x001298B0 multiplies by the reciprocal of 12).
//   There is no virtual operator=; copies are memberwise.
// - DynamicVectorClass<T> adds ActiveCount +0x10 and GrowthStep +0x14 (size
//   0x18) and overrides slots 2, 3 and 4 (Resize, Clear, ID); slot 5 stays the
//   base ID(const T *), whose result is an index by pointer difference.
// - The VectorClass constructor takes (int size, const T *array): Vector 0,
//   VectorMax = size, IsValid true, IsAllocated false; a non-zero size either
//   constructs its elements in the caller's array (placement new[], so the
//   array needs room for the element-count cookie when T has a destructor) or
//   allocates one (and sets IsAllocated).
// - The DynamicVectorClass constructor takes (unsigned size, const T *array),
//   runs the base constructor, then stores the derived vftable, GrowthStep = 10
//   and ActiveCount = 0 (in that order).
// - Clear and the destructor are the same sequence: free the array when it is
//   owned, zero Vector, then IsAllocated = false and VectorMax = 0.
// - Resize (0x00129990, 0x001A3720): a size of 0 calls the virtual Clear;
//   otherwise IsValid is cleared while the new array is obtained (the caller's,
//   or a fresh allocation), set again afterwards, a null array fails with
//   false, the first min(newsize, VectorMax) elements are copied with
//   element assignment, an owned old array is freed, then Vector/VectorMax are
//   stored and IsAllocated = (array == 0).
// - DynamicVectorClass::Resize runs the base Resize and then clamps ActiveCount
//   to the new VectorMax. Add (0x00129B70, 0x001A3A60) appends one element,
//   growing by GrowthStep through the virtual Resize when the array is full,
//   but only when it is owned (or empty) and GrowthStep is positive.
#pragma once
#ifndef BFME2_WWLIB_VECTOR_H
#define BFME2_WWLIB_VECTOR_H

#include <new>

template <class T> class VectorClass
{
public:
	VectorClass(int size = 0, const T *array = 0) :
		Vector(0),
		VectorMax(size),
		IsValid(true),
		IsAllocated(false)
	{
		if (size != 0) {
			if (array != 0) {
				Vector = new((void *)array) T[size];
			} else {
				Vector = new T[size];
				IsAllocated = true;
			}
		}
	}

	virtual ~VectorClass()
	{
		Clear();
	}

	virtual bool operator==(const VectorClass<T> &that) const
	{
		if (VectorMax != that.VectorMax)
			return false;
		for (int slot = 0; slot < VectorMax; slot++) {
			if (!(Vector[slot] == that.Vector[slot]))
				return false;
		}
		return true;
	}

	virtual bool Resize(int newsize, const T *array = 0)
	{
		if (newsize != 0) {
			IsValid = false;
			T *fresh;
			if (array == 0)
				fresh = new T[newsize];
			else
				fresh = new((void *)array) T[newsize];
			IsValid = true;
			if (fresh == 0)
				return false;

			if (Vector != 0) {
				int keep = (newsize < VectorMax) ? newsize : VectorMax;
				for (int slot = 0; slot < keep; slot++)
					fresh[slot] = Vector[slot];
				if (IsAllocated) {
					delete[] Vector;
					Vector = 0;
				}
			}

			Vector = fresh;
			VectorMax = newsize;
			IsAllocated = (array == 0);
		} else {
			Clear();
		}
		return true;
	}

	virtual void Clear()
	{
		if (Vector != 0 && IsAllocated) {
			delete[] Vector;
			Vector = 0;
		}
		IsAllocated = false;
		VectorMax = 0;
	}

	virtual int ID(const T *ptr)
	{
		if (!IsValid)
			return 0;
		return (int)(((unsigned long)ptr - (unsigned long)Vector) / sizeof(T));
	}

	virtual int ID(const T &object)
	{
		if (!IsValid)
			return 0;
		for (int slot = 0; slot < VectorMax; slot++) {
			if (Vector[slot] == object)
				return slot;
		}
		return -1;
	}

	T &operator[](int index) { return Vector[index]; }
	const T &operator[](int index) const { return Vector[index]; }
	int Length() const { return VectorMax; }

protected:
	T *Vector;			// +0x4
	int VectorMax;		// +0x8
	bool IsValid;		// +0xC
	bool IsAllocated;	// +0xD
};

template <class T> class DynamicVectorClass : public VectorClass<T>
{
public:
	DynamicVectorClass(unsigned size = 0, const T *array = 0) : VectorClass<T>(size, array)
	{
		GrowthStep = 10;
		ActiveCount = 0;
	}

	virtual bool Resize(int newsize, const T *array = 0)
	{
		if (VectorClass<T>::Resize(newsize, array)) {
			if (this->VectorMax < ActiveCount)
				ActiveCount = this->VectorMax;
			return true;
		}
		return false;
	}

	virtual void Clear()
	{
		ActiveCount = 0;
		VectorClass<T>::Clear();
	}

	virtual int ID(const T &object)
	{
		for (int slot = 0; slot < ActiveCount; slot++) {
			if ((*this)[slot] == object)
				return slot;
		}
		return -1;
	}

	bool Add(const T &object)
	{
		if (ActiveCount >= this->VectorMax) {
			if ((this->IsAllocated || this->VectorMax == 0) && GrowthStep > 0) {
				if (!Resize(this->VectorMax + GrowthStep))
					return false;
			} else {
				return false;
			}
		}
		(*this)[ActiveCount++] = object;
		return true;
	}

	// Removes one element by shifting the tail down (retail's 0x00196520 for pointer elements).
	bool Delete(int index)
	{
		if (index < ActiveCount) {
			ActiveCount--;
			if (index < ActiveCount) {
				for (int i = index; i < ActiveCount; i++) {
					(*this)[i] = (*this)[i + 1];
				}
			}
			return true;
		}
		return false;
	}

	bool Delete(const T &object)
	{
		int id = ID(object);
		if (id != -1) {
			return Delete(id);
		}
		return false;
	}

	int Count() const { return ActiveCount; }
	void Reset_Active(void) { ActiveCount = 0; }

	// Empties the vector and reallocates it at its old capacity
	// (MaterialInfoClass::Free 0x0016EE70: VectorMax saved, slot 3, then slot 2
	// with that size and no array).
	void Delete_All()
	{
		int len = this->VectorMax;
		Clear();
		Resize(len);
	}

protected:
	int ActiveCount;	// +0x10
	int GrowthStep;		// +0x14
};

#endif
