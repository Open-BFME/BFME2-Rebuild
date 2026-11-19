#pragma once
class ObjectStatusMask
{
public:
 ObjectStatusMask() {}
 ObjectStatusMask(unsigned,unsigned,unsigned,unsigned,unsigned);
 ObjectStatusMask *init(int,int) throw();
 unsigned m_bits[4];
};
// SIZE 0x10
