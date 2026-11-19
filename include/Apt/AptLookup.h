#pragma once
#include "Apt/AptValue.h"
// The two constant-operand values the action stream pushes before the interpreter resolves them
// (types AptVFT_Lookup and AptVFT_Register in AptVirtualFunctionTable.h). Both are an AptValue
// followed by one 32-bit payload at +0x8: stackPushIndirect 0x006FE610 reads it right after the checked
// casts 0x006DCD50 (lookup) and 0x006DCDD0 (register) and uses it as an index into the constant pool
// (lookup) or the register block (register). Names of the payloads are the old views' (nLookup, nVal).
class AptLookup : public AptValue
{
public:
    int nLookup; // 0x8: index into the interpreter's constant pool
};

class AptRegister : public AptValue
{
public:
    int nVal; // 0x8: register number
};
