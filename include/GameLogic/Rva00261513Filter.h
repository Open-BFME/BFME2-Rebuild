#pragma once
#include "GameLogic/PartitionFilter.h"
class Player;
class Rva00261513Filter : public PartitionFilter
{
public:
 Rva00261513Filter(const Object *obj,bool flag,float value) : m_obj(obj),m_flag(flag),m_value(value) {}
 virtual bool allow(Object *);

 const Object *m_obj;
 bool m_flag;
 float m_value;
};
