// multilist.h: WWLib's intrusive lists where one object can sit in many lists
// (each membership is a node chained from the object's MultiListObjectClass).
//
// Target facts:
// - MultiListNodeClass() 0x00065802 zeroes +8, +4, +0, +0xC, +0x10 in that
//   order: Prev +0, Next +4, NextList +8 (the object's next membership),
//   Object +0xC, List +0x10. SIZE 0x14. Nodes come from an
//   AutoPoolClass<MultiListNodeClass, 256> (0x00610770/0x00610780), which only
//   multilist.cpp's own bodies use.
// - GenericMultiListClass() 0x00065815: vftable 0x007C5C70 (one slot, the
//   deleting destructor 0x0006583C), sentinel node Head at +4 linked to itself
//   (Prev then Next), its Object and NextList cleared. SIZE 0x18. The
//   destructor 0x00610640, Contains 0x00610650, Count 0x00610750 and the
//   Internal_* bodies (0x006107A0..0x006109A0) are out of line in multilist.cpp.
// - Is_Empty (0x00065858, `Head.Next == &Head`) and Internal_Get_List_Head
//   (0x00143DD0) have out-of-line copies but are expanded in place by their
//   callers (MultiListClass<CameraShakerClass>'s destructor 0x00065CA4,
//   RefMultiListClass<RenderObjClass>::Peek_Head 0x0006EFAD), so they are
//   inline here.
// - The typed wrappers convert T * to and from MultiListObjectClass * with a
//   null check (MultiListClass<RenderObjClass>::Add 0x001410E0 adds 8, the
//   offset of RenderObjClass's MultiListObjectClass base). RefMultiListClass
//   takes a reference on a successful add (0x0006EF29) and drops one on a
//   successful remove (0x0006EF4E).
// - GenericMultiListIterator is the list then the current node:
//   First(list) 0x00065866, First 0x00065875, Next 0x0006587E, Is_Done
//   0x00065888 (`CurNode == &List->Head`).
#pragma once
#ifndef BFME2_MULTILIST_H
#define BFME2_MULTILIST_H

#include <stddef.h>
#include "WWLib/MultiListObjectClass.h"
#include "WWLib/mempool.h"

class GenericMultiListClass;

class MultiListNodeClass : public AutoPoolClass<MultiListNodeClass, 256>
{
public:
	MultiListNodeClass(void)
	{
		Prev = Next = NextList = 0;
		Object = 0;
		List = 0;
	}

	MultiListNodeClass *Prev;		// +0x00
	MultiListNodeClass *Next;		// +0x04
	MultiListNodeClass *NextList;		// +0x08
	MultiListObjectClass *Object;		// +0x0C
	GenericMultiListClass *List;		// +0x10
};

typedef char MultiListNodeClassSize[(sizeof(MultiListNodeClass) == 0x14) ? 1 : -1];

class GenericMultiListClass
{
public:
	GenericMultiListClass(void)
	{
		Head.Next = Head.Prev = &Head;
		Head.Object = 0;
		Head.NextList = 0;
	}
	virtual ~GenericMultiListClass(void);					// 0x00610640

	bool Is_Empty(void) { return Head.Next == &Head; }
	bool Is_In_List(MultiListObjectClass *obj) { return Contains(obj); }
	bool Contains(MultiListObjectClass *obj);				// 0x00610650
	int Count(void);							// 0x00610750

protected:
	bool Internal_Add(MultiListObjectClass *obj, bool onlyonce = true);			// 0x006107A0
	bool Internal_Add_Tail(MultiListObjectClass *obj, bool onlyonce = true);		// 0x00610820
	bool Internal_Add_After(MultiListObjectClass *obj, const MultiListObjectClass *existing, bool onlyonce = true);	// 0x006108A0
	bool Internal_Remove(MultiListObjectClass *obj);					// 0x00610930
	MultiListObjectClass *Internal_Remove_List_Head(void);				// 0x006109A0
	MultiListObjectClass *Internal_Get_List_Head(void)
	{
		if (Is_Empty())
			return 0;
		return Head.Next->Object;
	}

	MultiListNodeClass Head;		// +0x04

	friend class GenericMultiListIterator;
};

typedef char GenericMultiListClassSize[(sizeof(GenericMultiListClass) == 0x18) ? 1 : -1];

template <class ObjectType>
class MultiListClass : public GenericMultiListClass
{
public:
	MultiListClass(void) {}
	virtual ~MultiListClass(void)
	{
		while (!Is_Empty())
			Remove_Head();
	}

	bool Add(ObjectType *obj, bool onlyonce = true) { return Internal_Add(obj, onlyonce); }
	bool Add_Tail(ObjectType *obj, bool onlyonce = true) { return Internal_Add_Tail(obj, onlyonce); }
	bool Add_After(ObjectType *obj, const ObjectType *existing, bool onlyonce = true) { return Internal_Add_After(obj, existing, onlyonce); }
	bool Remove(ObjectType *obj) { return Internal_Remove(obj); }
	ObjectType *Remove_Head(void) { return static_cast<ObjectType *>(Internal_Remove_List_Head()); }
	ObjectType *Peek_Head(void) { return static_cast<ObjectType *>(Internal_Get_List_Head()); }
};

// The list holds a reference on each member while it is linked.
template <class ObjectType>
class RefMultiListClass : public GenericMultiListClass
{
public:
	// 0x0006F29B (RenderObjClass): drop every member, then the base destructor.
	virtual ~RefMultiListClass(void)
	{
		while (!Is_Empty())
			Release_Head();
	}

	bool Add(ObjectType *obj, bool onlyonce = true)
	{
		bool added = Internal_Add(obj, onlyonce);
		if (added == true)
			obj->Add_Ref();
		return added;
	}

	bool Add_Tail(ObjectType *obj, bool onlyonce = true)
	{
		bool added = Internal_Add_Tail(obj, onlyonce);
		if (added == true)
			obj->Add_Ref();
		return added;
	}

	// The emitted copy 0x0006EF4E tests the result for non-zero.
	bool Remove(ObjectType *obj)
	{
		bool removed = Internal_Remove(obj);
		if (removed)
			obj->Release_Ref();
		return removed;
	}

	ObjectType *Remove_Head(void) { return static_cast<ObjectType *>(Internal_Remove_List_Head()); }
	ObjectType *Peek_Head(void) { return static_cast<ObjectType *>(Internal_Get_List_Head()); }

	bool Release_Head(void)
	{
		ObjectType *obj = Remove_Head();
		if (obj) {
			obj->Release_Ref();
			return true;
		}
		return false;
	}

	void Reset_List(void)
	{
		while (Peek_Head() != NULL)
			Release_Head();
	}
};

class GenericMultiListIterator
{
public:
	GenericMultiListIterator(GenericMultiListClass *list) { First(list); }

	void First(GenericMultiListClass *list)
	{
		List = list;
		CurNode = List->Head.Next;
	}
	void First(void) { CurNode = List->Head.Next; }
	void Next(void) { CurNode = CurNode->Next; }
	bool Is_Done(void) { return CurNode == &List->Head; }

protected:
	MultiListObjectClass *Current_Object(void) { return CurNode->Object; }

	GenericMultiListClass *List;		// +0x0
	MultiListNodeClass *CurNode;		// +0x4
};

typedef char GenericMultiListIteratorSize[(sizeof(GenericMultiListIterator) == 0x8) ? 1 : -1];

template <class ObjectType>
class MultiListIterator : public GenericMultiListIterator
{
public:
	MultiListIterator(MultiListClass<ObjectType> *list) : GenericMultiListIterator(list) {}

	ObjectType *Peek_Obj(void) { return static_cast<ObjectType *>(Current_Object()); }
};

template <class ObjectType>
class RefMultiListIterator : public GenericMultiListIterator
{
public:
	RefMultiListIterator(RefMultiListClass<ObjectType> *list) : GenericMultiListIterator(list) {}

	ObjectType *Peek_Obj(void) { return static_cast<ObjectType *>(Current_Object()); }
};

#endif
