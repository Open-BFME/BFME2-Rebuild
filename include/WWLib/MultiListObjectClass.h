// MultiListObjectClass.h: WWLib's multi-list membership base (multilist.h).
// Target facts: one vptr and the list-node head; RenderObjClass's copy
// constructor 0x0013B430 stores this class's vftable (VA 0x00BC5C6C) at its +8
// and zeroes +0xC before installing its own. The scalar deleting destructor is
// the COMDAT 0x000657E6; the destructor body (unlinking from every list) is out
// of line in multilist.cpp.
#pragma once
#ifndef BFME2_MULTILISTOBJECTCLASS_H
#define BFME2_MULTILISTOBJECTCLASS_H

class MultiListNodeClass;

class MultiListObjectClass
{
public:
	MultiListObjectClass() : ListNode(0) {}
	virtual ~MultiListObjectClass();

	MultiListNodeClass *Get_List_Node() const { return ListNode; }
	void Set_List_Node(MultiListNodeClass *node) { ListNode = node; }

private:
	MultiListNodeClass *ListNode;		// +0x4
};
// SIZE 0x8

#endif
