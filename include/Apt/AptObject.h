#pragma once
// SIZE 0x20: AptValueWithHash plus one flags word at +0x1C. Constructor
// 0x006D6410 (type, hash size) calls AptValueWithHash's, clears the low byte
// of +0x1C and bits 8 and 9, then stores vtable 0x008EA264. getHasClass
// 0x006D6460 returns bit 8, setHasClass 0x006D6440 writes it; objectMemberLookup
// 0x0070DFB0 is AptObject's own, RegisterReferences/DestroyGCPointers
// (0x0070DFD0, 0x0070DFE0) are thunks to the hash's.
// The field names (mnImplementedObjects:8, mbHasClass:1, mbIsInMainInst:1) come
// from the donor's PDB; the widths are what the constructor and accessors prove
// (bit 9's role is not exercised by any recovered body).
#include "Apt/AptValueWithHash.h"
#include "Apt/EAStringC.h"
#include "Apt/AptValueGC_PoolManager.h"

class AptObject : public AptValueWithHash
{
public:
    AptObject(AptVirtualFunctionTable_Indices type, int hashSize = 8)
        : AptValueWithHash(type, hashSize)
    {
        mnImplementedObjects = 0;
        mbHasClass = 0;
        mbIsInMainInst = 0;
    }
    // Instances come from the AptValue pool (0x006CC58A style: `mov ecx,[0x00E176F4]; push 0x20; call 0x006D29E0`); the sized delete is the out-of-line pool free.
    static void *operator new(unsigned int size) { return g_pAptValueGCPoolManager->allocBlock(size); }
    static void operator delete(void *block, unsigned int size) { g_pAptValueGCPoolManager->freeBlock(block, size); }
    bool DoesImplementObject(AptValue *object) const;
    void SetInterfaces(AptValue *interfaces, unsigned char count); // 0x0070DFF0 (the map files it under a placeholder class)
    AptValue *GetInterfaces(int *pCount);                         // 0x0070E060
    virtual void setHasClass(int hasClass);
    virtual int getHasClass() const;
    virtual AptValue *objectMemberLookup(AptValue *const self, const EAStringC *const name) const;
    virtual void RegisterReferences() const;
    virtual void DestroyGCPointers();
protected:
    virtual ~AptObject();
    unsigned int mnImplementedObjects : 8; // 0x1C
    unsigned int mbHasClass : 1;
    unsigned int mbIsInMainInst : 1;
};

typedef char AptObjectSize[(sizeof(AptObject) == 0x20) ? 1 : -1];
