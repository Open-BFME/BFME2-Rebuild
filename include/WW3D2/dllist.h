// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// Intrusive doubly-linked lists used by the sorting renderer and the smudge pools.
// Lists leave node lifetime to their owners. A node unlinks itself when destroyed.
#pragma once

template <class T> class DLNodeClass;

template <class T>
class DLListClass
{
	friend class DLNodeClass<T>;

	DLNodeClass<T>* first;	// +0x04
	DLNodeClass<T>* last;	// +0x08

public:
	DLListClass() : first(0), last(0) {}
	virtual ~DLListClass() { }

	void Add_Head(DLNodeClass<T>* node);
	void Remove_Head();
	void Add_Tail(DLNodeClass<T>* node);
	void Remove_Tail();

	T* Head() { return static_cast<T*>(first); }
	T* Tail() { return static_cast<T*>(last); }
	const T* Const_Head() const { return static_cast<const T*>(first); }
	const T* Const_Tail() const { return static_cast<const T*>(last); }
};

template <class T>
class DLNodeClass
{
	friend class DLListClass<T>;

	DLNodeClass<T>* following;	// +0x00
	DLNodeClass<T>* preceding;	// +0x04
	DLListClass<T>* owner;	// +0x08

public:
	DLNodeClass() : following(0), preceding(0), owner(0) {}
	~DLNodeClass() { Remove(); }

	// Link this node in front of `anchor`, which must already be on a list.
	void Insert_Before(DLNodeClass<T>* anchor)
	{
		owner = anchor->owner;
		following = anchor;
		preceding = anchor->preceding;
		if (anchor->preceding != 0)
			anchor->preceding->following = this;
		anchor->preceding = this;

		if (owner->first == anchor)
			owner->first = this;
	}

	// Link this node behind `anchor`, which must already be on a list.
	void Insert_After(DLNodeClass<T>* anchor)
	{
		owner = anchor->owner;
		preceding = anchor;
		following = anchor->following;
		if (anchor->following != 0)
			anchor->following->preceding = this;
		anchor->following = this;

		if (owner->last == anchor)
			owner->last = this;
	}

	// Take this node off its list. A node at either end is handed to the list's own pop.
	void Remove()
	{
		DLListClass<T>* list = owner;
		if (list == 0)
			return;
		if (list->Head() == this) {
			owner = 0;
			list->Remove_Head();
		} else if (list->Tail() == this) {
			owner = 0;
			list->Remove_Tail();
		} else {
			if (following != 0)
				following->preceding = preceding;
			if (preceding != 0)
				preceding->following = following;
			owner = 0;
		}
	}

	T* Succ() { return static_cast<T*>(following); }
	T* Pred() { return static_cast<T*>(preceding); }
	const T* Const_Succ() const { return static_cast<const T*>(following); }
	const T* Const_Pred() const { return static_cast<const T*>(preceding); }

	DLListClass<T>* List() { return owner; }
};

template <class T>
inline void DLListClass<T>::Add_Head(DLNodeClass<T>* node)
{
	node->owner = this;
	if (first != 0) {
		node->Insert_Before(first);
		first = node;
	} else {
		last = node;
		first = node;
		node->following = 0;
		node->preceding = 0;
	}
}

template <class T>
inline void DLListClass<T>::Add_Tail(DLNodeClass<T>* node)
{
	node->owner = this;
	if (last != 0) {
		node->Insert_After(last);
		last = node;
	} else {
		last = node;
		first = node;
		node->following = 0;
		node->preceding = 0;
	}
}

template <class T>
inline void DLListClass<T>::Remove_Head()
{
	DLNodeClass<T>* node = first;
	if (node == 0)
		return;
	first = node->Succ();
	if (first == 0)
		last = 0;
	else
		first->preceding = 0;
	node->Remove();
}

template <class T>
inline void DLListClass<T>::Remove_Tail()
{
	DLNodeClass<T>* node = last;
	if (node == 0)
		return;
	last = node->Pred();
	if (last == 0)
		first = 0;
	else
		last->following = 0;
	node->Remove();
}
