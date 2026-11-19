#pragma once
// SIZE 0x8, plain struct. GetConstantPool 0x00709D30 returns it through a
// hidden pointer ({[rec+0x14], [rec+0x18]}), DefineDictionary 0x006FDC30
// stores both words into the interpreter at +0x40/+0x44.
class AptValue;
struct AptConstantPool
{
    int nItems;         // 0x0
    AptValue **apItems; // 0x4
};
