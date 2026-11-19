#pragma once
// The parameter block Apt's init entry point hands to the interpreter's init (0x006FEAF0) and to
// AptScriptFunctionBase::InitializeStaticData (0x00709610). A leaf struct: the map's mangled names
// (`ABUAptInitParmsT@@`) spell it `struct`. Only the members those bodies and the init caller
// (the function around 0x006CF8F8 that calls 0x006FEAF0 with ecx = the native interpreter) read are
// named; the first 0x20 bytes (the host's callback table) are not accessed by the interpreter.
// Offsets, from the retail bodies:
//   0x006FEAF0 reads +0x20 (the value stack's capacity) and +0x24 (the capacity of the with, set-target
//   and this stacks and of the debug call stack),
//   0x00709610 reads +0x30 (the register block's size; the donor's name is iRegArraySize),
//   the init caller reads +0x28, +0x2C and +0x34 for other pools (not used by the interpreter).
struct AptInitParmsT
{
    unsigned char hostCallbacks[0x20]; // 0x00: not accessed by the interpreter
    int nValueStackSize;               // 0x20
    int nScopeStackSize;               // 0x24
    int unknown28;                     // 0x28
    int unknown2C;                     // 0x2C
    int iRegArraySize;                 // 0x30
    int unknown34;                     // 0x34
};
