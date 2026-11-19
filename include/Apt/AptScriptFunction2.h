#pragma once
// SIZE 0x34: AptScriptFunctionBase plus the DefineFunction2 record pointer at
// +0x30. Vtable 0x008EE9A0 (25 slots): Duplicate 0x0070A4C0, GetName 0x00709BC0,
// GetNumArguments 0x00709D00, GetByteCodeBase 0x00709D10 (rec+0x1C),
// GetByteCodeSize 0x00709D20 ([rec+0x10]), GetConstantPool 0x00709D30 (the pair
// at rec+0x14), SetupBeforeExecution 0x0070A040, SetArgument 0x0070A400,
// CleanupAfterExecution 0x0070A230 (restores the register block frame).
// Type id 0x2C. Constructors: 0x00709CD0 builds from a record (base constructor
// with the extra flag), 0x00709D50 clones.
#include "Apt/AptScriptFunctionBase.h"
#include "Apt/AptValueGC_PoolManager.h"

// One parameter record of DefineFunction2: the register it binds (0: a named scope variable) and its name.
struct AptAction_DefineFunction2Param
{
    int nRegister;     // 0x0
    const char *szName; // 0x4
};

// 0x1C bytes, followed by the byte code. Offsets from the accessors above.
struct AptAction_DefineFunction2
{
    const char *szName;     // 0x0
    int nParams;            // 0x4
    short nRegisterCount;   // 0x8
    short nFlags;           // 0xA
    const AptAction_DefineFunction2Param *aParams;  // 0xC
    int nCodeSize;          // 0x10
    mutable AptConstantPool constantPool; // 0x14
};

class AptScriptFunction2 : public AptScriptFunctionBase
{
public:
    AptScriptFunction2(AptScriptFunctionBase *parent, const AptAction_DefineFunction2 *record, AptCIH *cih);
    AptScriptFunction2(AptScriptFunction2 *original, AptCIH *cih);
    static void *operator new(unsigned int size) { return g_pAptValueGCPoolManager->allocBlock(size); }
    static void operator delete(void *block, unsigned int size) { g_pAptValueGCPoolManager->freeBlock(block, size); }
    virtual AptScriptFunctionBase *Duplicate(AptCIH *cih);
    virtual const char *GetName() const;
    virtual unsigned int GetNumArguments();
    virtual const unsigned char *GetByteCodeBase();
    virtual unsigned int GetByteCodeSize();
    virtual AptConstantPool GetConstantPool();
    virtual void SetupBeforeExecution(_AptScriptFunctionState *state, AptValue *self);
    virtual void SetArgument(AptValue *value, int index);
    virtual void CleanupAfterExecution(_AptScriptFunctionState *state);
protected:
    virtual ~AptScriptFunction2();
    const AptAction_DefineFunction2 *mpFunction; // 0x30
};

typedef char AptScriptFunction2Size[(sizeof(AptScriptFunction2) == 0x34) ? 1 : -1];
