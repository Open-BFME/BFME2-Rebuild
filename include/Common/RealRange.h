#pragma once
// SIZE 0x8. No base or virtual functions.
// Retail XferSaveAsText::operator== 0x0060DC12 reads these typed fields in order;
// old private views and library headers corroborate the layout. Names follow
// those views; the transfer format strings independently label the components.

#include <stddef.h>
struct RealRange
{
    float lo; // 0x0
    float hi; // 0x4
    void combine(RealRange &other); // 0x000062AD: lo = min, hi = max of the two ranges
};
typedef char RealRangeSize[(sizeof(RealRange) == 0x8) ? 1 : -1];
