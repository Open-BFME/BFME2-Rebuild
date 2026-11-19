#pragma once
// ContainmentList: the contain list a ContainmentListView points at and that
// 0x0036AE51 (toList) copies out. Retail's is the STLport list<Object *> every
// contain module keeps (OpenContain::m_containList, ContainedItemsList): its
// destructor 0x004EC395 is the list's own (h05; f06's heldfold rows onDelete
// 0x00464120, onDie 0x004641B1, redeployOccupants 0x00464286 copy it as one).
// Outside STLport units the name stays an opaque class.
class Object;
#if defined(_STLPORT_VERSION)
#include <list>
typedef _STL::list<Object *> ContainmentList;
#else
class ContainmentList;
#endif
