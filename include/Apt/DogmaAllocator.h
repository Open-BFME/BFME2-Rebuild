#pragma once
// The Apt block pool ("DOGMA"): every Apt object, hash table and array element
// block comes from it. One instance, reached through the pointer at VA 0x00E176E8.
// allocBlock (0x006DB160, 75 call sites) and freeBlock (0x006DB270, 155 call
// sites) take sizes in bytes and round them up to a multiple of 4 (and to a minimum
// held in the byte at +0x13 & 0xF); sizes over the field at +0xC fall through to the
// system allocator callback (VA 0x00E17728 for allocation, 0x00E17730 for free).
// Partial layout read from the bodies: +0 an array of free-list heads indexed by
// size/4, +4 the chunk list (each chunk: next at +0, capacity at +8), +8 the chunk
// size, +0xC the largest pooled size, +0x13 a small alignment field, +0x14 an
// allocation counter; the complete size is NOT recovered, so the class is only
// usable through a pointer and declares no data members.
// The method names allocBlock/freeBlock are the old ledger's; the retail assert text
// says "Deallocate" (`pNowFree != NULL && "Attempting to Deallocate NULL pointer!"`),
// and the file is ...\Apt\DogmaAllocator.cpp.
// One pool chunk as GetTotalBytesUsed 0x006DB140 reads it: the next chunk at +0 and two counters whose
// difference it sums (+4 minus +8). Their meaning is not evidenced, so they carry no names.
struct DogmaChunk
{
    DogmaChunk *mpNext;   // 0x0
    unsigned int field04; // 0x4
    unsigned int field08; // 0x8
    // The payload starts after the 12-byte header; field04 - field08 is the used byte count (GetTotalBytesUsed 0x006DB140
    // sums it). AptValueGC_PoolManager's walks (0x006D2B70, 0x006D2CB0) test every block address against that range.
    inline bool contains(const void *p) const
    {
        return (unsigned int)p >= (unsigned int)(this + 1) && (unsigned int)p < (unsigned int)(this + 1) + field04 - field08;
    }
};

class DOGMA_PoolManager
{
public:
    void *allocBlock(int size);
    void freeBlock(void *block, int size);
    unsigned int GetTotalBytesUsed();
    // 0x006DAFA0 (the map files it as rva006DAFA0): gives the free-list array and then every chunk back to the system allocator.
    void rva006DAFA0();
    // The size a request is rounded to: a multiple of 4, at least the pool's minimum block size (0x006DB120).
    unsigned int roundSize(unsigned int size) const;
    // 0x006DAFE0 (the map files it as rva006DAFE0): takes the first block off the free list of this size.
    void *rva006DAFE0(unsigned int nSize);
    // 0x006DB090 (the map files it as rva006DB090): puts a block back on the free list of this size.
    void rva006DB090(void *block, unsigned int nSize);

    unsigned int **mpFreeLists;   // 0x0: one free-list head per size / 4
    DogmaChunk *mpChunks;         // 0x4: the chunk list
    unsigned int mnChunkSize;     // 0x8
    unsigned int mnMaxPoolSize;   // 0xC: larger requests go to the system allocator
    // 0x10: the layout of a free block and the debug features. The retail asserts name the offsets
    // (`mnOffsetToStorePrev`, `mnOffsetToStoreSize`); the other member names are this header's.
    unsigned int mnOffsetToStoreNext : 8;  // 0x10: dword index of the link to the next free block
    unsigned int mnOffsetToStoreSize : 8;  // 0x11
    unsigned int mnOffsetToStorePrev : 8;  // 0x12
    unsigned int mnMinBlockSize : 4;       // 0x13 low nibble
    unsigned int mbStoreSize : 1;          // bit 28
    unsigned int mbStorePrev : 1;          // bit 29
    unsigned int mSpare : 2;               // bits 30, 31
    unsigned int mnAllocations;            // 0x14
    int mnFreeBlocks;                      // 0x18
};

extern DOGMA_PoolManager *g_pDogmaPoolManager; // VA 0x00E176E8
