// ObjectIDList.h: SAGE's list of object IDs, the STLport 4.6 list.
// Target facts: ArrowStormUpdate keeps one at +0x88 (four bytes: STLport folds
// the allocator into the node proxy), and SpecialAbilityUpdate another.
// Retail instantiates the list members out of line in ArrowStormUpdate's unit
// (front 0x004907A2, pop_front 0x004907AA; ICF folds them with other pointer-
// sized lists) and reaches clear through _List_base 0x0026549E. Its nodes come
// from a FreelistPool (FreelistPool.h; erase 0x002ABB20 frees to 0x00DA60F0). A unit that
// includes this header builds with `// stlport46`.
#pragma once
#ifndef BFME2_OBJECTIDLIST_H
#define BFME2_OBJECTIDLIST_H

#include <list>
#include "Common/GameType.h"
#include "Common/FreelistPool.h"

typedef _STL::list<ObjectID, FreelistAllocator<ObjectID> > ObjectIDList;
typedef ObjectIDList::iterator ObjectIDListIterator;
typedef ObjectIDList::const_iterator ObjectIDListConstIterator;

// The pooled lists' erase. PROVISIONAL form: an explicit specialization per
// pooled element type. Target facts: all four retail copies, Object *
// 0x0029DFC5, Drawable * 0x0029DFE9, TeamPrototype * 0x002ABAC0 and ObjectID
// 0x002ABB20, load the node's prev link before its next link; every other
// list::erase copy (e.g. CameraMarker's 0x002A12B7) loads next first, which is
// STLport 4.5.2's and 4.6's source order. MSVC 7.1 /O2 follows source order for
// those two loads (a compile-only probe of both orders gives exactly the two
// retail shapes), so the pooled lists' erase was separate source. _List_base::
// clear for this list compiles to retail 0x0026549E unchanged, so the rest of
// the class is STLport's. Whether retail wrote a specialization, a partial
// specialization of list for FreelistAllocator, or its own copy of the list
// is not recorded in the binary.
_STLP_BEGIN_NAMESPACE
template <>
inline ObjectIDList::iterator ObjectIDList::erase(iterator __position)
{
	_List_node_base *__prev_node = __position._M_node->_M_prev;
	_List_node_base *__next_node = __position._M_node->_M_next;
	_Node *__n = (_Node *)__position._M_node;
	__prev_node->_M_next = __next_node;
	__next_node->_M_prev = __prev_node;
	_STLP_STD::_Destroy(&__n->_M_data);
	this->_M_node.deallocate(__n, 1);
	return iterator((_Node *)__next_node);
}
_STLP_END_NAMESPACE

#endif
