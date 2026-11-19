#pragma once
// SIZE 0x44: AptScriptFunctionBase (0x30) plus the byte code description at +0x30.
// A script function over a raw byte-code block (the movie's action code, not an
// ActionScript function definition record). Type id 0x2D. Constructor 0x0070A2C0
// takes (code, size, constant pool {count, items}, name, CIH, parent): it passes
// type 0x2D to AptScriptFunctionBase's constructor 0x00709870, then stores
// code at +0x30, size at +0x34, the name at +0x38 and the pool at +0x3C/+0x40.
// Vtable 0x008EEAE8 (25 slots): GetName 0x0066EA50 ([+0x38]), GetNumArguments
// 0x0065CE90 (zero), GetByteCodeBase 0x0066D5B0 ([+0x30]), GetByteCodeSize
// 0x00149580 ([+0x34]), GetConstantPool 0x0070A310 (the pair at +0x3C),
// SetArgument 0x0070A330 and Duplicate 0x0070A360 (both assert false: a
// block is never cloned and takes no arguments).
#include "Apt/AptScriptFunctionBase.h"

class AptScriptFunctionByteCodeBlock : public AptScriptFunctionBase
{
public:
    static void *operator new(unsigned int size) { return g_pAptValueGCPoolManager->allocBlock(size); }
    AptScriptFunctionByteCodeBlock(const unsigned char *code, int size, AptConstantPool constantPool,
        const char *name, AptCIH *cih, AptScriptFunctionBase *parent);
    virtual AptScriptFunctionBase *Duplicate(AptCIH *cih);
    virtual const char *GetName() const;
    virtual unsigned int GetNumArguments();
    virtual const unsigned char *GetByteCodeBase();
    virtual unsigned int GetByteCodeSize();
    virtual AptConstantPool GetConstantPool();
    virtual void SetArgument(AptValue *value, int index);
protected:
    virtual ~AptScriptFunctionByteCodeBlock();
    const unsigned char *mpByteCodeBase; // 0x30
    int mnByteCodeSize;                  // 0x34
    const char *mpName;                  // 0x38
    AptConstantPool mConstantPool;       // 0x3C
};

typedef char AptScriptFunctionByteCodeBlockSize[(sizeof(AptScriptFunctionByteCodeBlock) == 0x44) ? 1 : -1];
