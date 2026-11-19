#pragma once
// SIZE 0x8: two ints, lo +0 and hi +4 (retail IntRange::combine 0x000062D8 folds another
// range into this one: lo = min, hi = max).
struct IntRange
{
    int lo; // 0x0
    int hi; // 0x4
    void combine(IntRange &other); // 0x000062D8
};
typedef char IntRangeSize[(sizeof(IntRange) == 0x8) ? 1 : -1];
