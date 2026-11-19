#pragma once
// The entry type of the gperf-generated word tables behind the Apt natives' member lookups (the
// `in_word_set` functions at 0x006E7650, 0x0070FE80 and the `Rva008...` ones such as 0x006E8360): a
// pointer to the member's name and one 32-bit value (the member id the native object switches on).
// Retail's tables are {name pointer, value} records of 8 bytes. The map spells the three entry types
// BfmeW1229 (the Key object's table), BfmeW1228 (the Stage object's) and R4Word (every other table,
// which the lookups share one type for); the declarations keep those names because the lookups'
// mangled names carry them.
struct BfmeW1229
{
    const char *name;
    int id;
};

struct BfmeW1228
{
    const char *name;
    int id;
};

struct R4Word
{
    const char *name;
    int value;
};
