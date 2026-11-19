#pragma once
#include "GameLogic/ObjectIteratorData.h"
class Object;
class ObjectIterator
{
public:
 ObjectIterator();
 ObjectIterator(const ObjectIterator &);
 ~ObjectIterator();
 unsigned count() const { const ObjectIteratorData *data=(const ObjectIteratorData *)m_value; return data->m_end-data->m_begin; }
 Object *next() throw();
 void *m_value; // 0x00
};
// SIZE 0x04
