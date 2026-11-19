#pragma once
#include <string.h>
class ModelConditionMask
{
public:
 ModelConditionMask(int,int);
 static int getSingleBitFromName(const char *);
 ModelConditionMask() { memset(m_bits,0,sizeof(m_bits)); }
 __declspec(nothrow) ModelConditionMask(const ModelConditionMask &);
 unsigned test(int bit) const { return m_bits[bit>>5] & (1u<<(bit&31)); }
 void set(int bit) { m_bits[bit>>5] |= 1u<<(bit&31); }
 void clear(int bit) { m_bits[bit>>5] &= ~(1u<<(bit&31)); }
 // 0x00263546: true when this mask shares any bit with `other`.
 bool rva00263546(const ModelConditionMask *other) const;
 unsigned int m_bits[19]; // 0x00
};
// SIZE 0x4C
