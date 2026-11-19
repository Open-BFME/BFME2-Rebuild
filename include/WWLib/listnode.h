// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// listnode.h: WWLib's intrusive doubly linked list with sentinel nodes.
// Target facts (retail game.dat), all from the rows ini.cpp's unit emits:
//  - GenericNode is vptr +0 (table 0x00C7C284: the deleting destructor only), next link +4 and
//    previous link +8, SIZE 0x0C. The constructor 0x00615B00 stores the table and clears both
//    links; the destructor 0x006162B0 unlinks the node inline. A node is "valid" (a real
//    element, not a sentinel) when both links are set (Is_Valid 0x00116DF0).
//  - Unlink 0x00615DF0 joins a valid node's neighbours and clears its links. Link 0x00615E20
//    first unlinks the node it is given, then inserts it after this node.
//  - GenericList is vptr +0 (table 0x00C7C2D4), a head sentinel at +4 and a tail sentinel at
//    +0x10, SIZE 0x1C. Its constructor 0x00616330 links the tail sentinel after the head;
//    Is_Empty 0x00615B20 asks whether the head's next node is valid; Add_Tail 0x00615E70
//    links the node after the tail sentinel's previous node; the destructor 0x006163A0 unlinks
//    every valid node, then lets the two sentinels unlink themselves.
//  - Node<T> adds no data: INIEntry's own members start at +0x0C. List<T>::Delete 0x00615CB0
//    deletes valid first nodes, through the virtual destructor, until the list is empty.
#pragma once
#ifndef BFME2_LISTNODE_H
#define BFME2_LISTNODE_H

class GenericNode
{
public:
	GenericNode() : NextNode(0), PrevNode(0) {}			// 0x00615B00
	virtual ~GenericNode() { Unlink(); }				// 0x006162B0

	GenericNode *Next() const { return NextNode; }
	GenericNode *Prev() const { return PrevNode; }
	bool Is_Valid() const { return this != 0 && NextNode != 0 && PrevNode != 0; }	// 0x00116DF0
	GenericNode *Next_Valid() const							// 0x0060FC20
	{
		return ((NextNode && NextNode->NextNode) ? NextNode : (GenericNode *)0);
	}

	void Unlink()								// 0x00615DF0
	{
		if (Is_Valid()) {
			PrevNode->NextNode = NextNode;
			NextNode->PrevNode = PrevNode;
			PrevNode = 0;
			NextNode = 0;
		}
	}
	void Link(GenericNode *node)						// 0x00615E20
	{
		node->Unlink();
		node->NextNode = NextNode;
		node->PrevNode = this;
		if (NextNode != 0) NextNode->PrevNode = node;
		NextNode = node;
	}

private:
	GenericNode *NextNode;		// +0x04
	GenericNode *PrevNode;		// +0x08
};
// SIZE 0x0C

class GenericList
{
public:
	GenericList()							// 0x00616330
	{
		FirstNode.Link(&LastNode);
	}
	virtual ~GenericList()						// 0x006163A0
	{
		while (FirstNode.Next()->Is_Valid()) FirstNode.Next()->Unlink();
	}

	GenericNode *First() const { return FirstNode.Next(); }
	GenericNode *Last() const { return LastNode.Prev(); }
	bool Is_Empty() const { return !FirstNode.Next()->Is_Valid(); }	// 0x00615B20
	void Add_Tail(GenericNode *node) { LastNode.Prev()->Link(node); }	// 0x00615E70

private:
	GenericNode FirstNode;		// +0x04
	GenericNode LastNode;		// +0x10
};
// SIZE 0x1C

template<class T>
class Node : public GenericNode
{
public:
	Node() {}
	T Next() const { return (T)(Node<T> *)GenericNode::Next(); }
	T Prev() const { return (T)(Node<T> *)GenericNode::Prev(); }
	T Next_Valid() const { return (T)(Node<T> *)GenericNode::Next_Valid(); }
};

template<class T>
class List : public GenericList
{
public:
	T First() const { return (T)(Node<T> *)GenericList::First(); }
	T Last() const { return (T)(Node<T> *)GenericList::Last(); }
	void Add_Tail(Node<T> *node) { GenericList::Add_Tail(node); }
	void Delete()							// 0x00615CB0
	{
		while (GenericList::First()->Is_Valid()) delete GenericList::First();
	}
};

#endif
