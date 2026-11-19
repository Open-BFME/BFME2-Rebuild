#pragma once
class Object;
class PartitionFilter
{
public:
 PartitionFilter() : m_next(0) {}
 virtual ~PartitionFilter() {}
 virtual bool allow(Object *) = 0;
 virtual int getPlayerMask() { return -1; }
 PartitionFilter *link(PartitionFilter *);
 PartitionFilter &And(PartitionFilter &rhs); // 0x00625790
 PartitionFilter *m_next; // 0x04
};
// SIZE 0x08
