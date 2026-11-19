#pragma once
// SIZE 0x34: AptScriptFunctionBase (0x30) plus the pointer to its DefineFunction
// record at +0x30. Vtable 0x008EE938 (25 slots) fills the base's pure slots:
// Duplicate 0x0070A390 (allocates 0x34 bytes and builds a clone from the
// original), GetName 0x00709BC0 ([rec+0]), GetNumArguments 0x00709D00
// ([rec+4]), GetByteCodeBase 0x00709BD0 (rec+0x18: the byte code follows the
// record), GetByteCodeSize 0x00709BE0 ([rec+0xC]), GetConstantPool 0x00709BF0
// (the pair at rec+0x10), SetArgument 0x00709C10. Type id 0x2B.
// The two constructors: 0x00709B90 builds from a record, 0x00709CA0 clones
// (base constructor with the original as parent, then copies +0x30).
#include "Apt/AptScriptFunctionBase.h"
#include "Apt/AptValueGC_PoolManager.h"

// 0x18 bytes, followed in memory by the byte code. Offsets from the accessors above.
struct AptAction_DefineFunction
{
    const char *szName;           // 0x0
    int nParams;                  // 0x4
    const char **aszParams;       // 0x8
    int nCodeSize;                // 0xC
    mutable AptConstantPool constantPool; // 0x10
};

class AptScriptFunction1 : public AptScriptFunctionBase
{
public:
    AptScriptFunction1(AptScriptFunctionBase *parent, const AptAction_DefineFunction *record, AptCIH *cih);
    AptScriptFunction1(AptScriptFunction1 *original, AptCIH *cih);
    static void *operator new(unsigned int size) { return g_pAptValueGCPoolManager->allocBlock(size); }
    static void operator delete(void *block, unsigned int size) { g_pAptValueGCPoolManager->freeBlock(block, size); }
    virtual AptScriptFunctionBase *Duplicate(AptCIH *cih);
    virtual const char *GetName() const;
    virtual unsigned int GetNumArguments();
    virtual const unsigned char *GetByteCodeBase();
    virtual unsigned int GetByteCodeSize();
    virtual AptConstantPool GetConstantPool();
    virtual void SetArgument(AptValue *value, int index);
protected:
    virtual ~AptScriptFunction1();
    const AptAction_DefineFunction *mpFunction; // 0x30
};

typedef char AptScriptFunction1Size[(sizeof(AptScriptFunction1) == 0x34) ? 1 : -1];
