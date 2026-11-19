// DrawableList.h: a list of drawables (Zero Hour GameClient: typedef std::list<Drawable *>).
#pragma once
#ifndef BFME2_DRAWABLELIST_H
#define BFME2_DRAWABLELIST_H

#include "Common/StlList.h"

class Drawable;
typedef _STL::list<Drawable *> DrawableList;

// Retail expands this instantiation's push_back in place as STLport's
// insert(end(), x), passing the end iterator by value
// (BloodthirstyUpdate::sacrificeHorde 0x0044E374).
namespace _STL {
// Retail stores no exception state before this list's destructor call (the one out-of-line body, 0x00239AF4, the map's DrawableList destructor):
// SpecialAbilityUpdate::startPacking 0x00450635 calls it with the state still at the list's own.
template <> _List_base<Drawable *, allocator<Drawable *> >::~_List_base() throw();
template <> inline void list<Drawable *>::push_back(Drawable *const &__x)
{
	insert(end(), __x);
}
}

#endif
