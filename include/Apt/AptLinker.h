#pragma once
// The Apt linker: the object the global at VA 0x00E176F8 points to. It keeps the movies that were loaded into
// a movie clip as a singly linked list of "load links" (the clip's CIH and the file it was filled from) and
// AptLinker::Load (0x006D1F80, AptLoad.cpp) is the one place that fills it.
//
// What the bodies prove (all of it is the list; nothing else of the object is read by a recovered row):
//  * +0 is the head of the list. Nothing else is stored: the whole object is one pointer, and the callers
//    that hand `this` to the list members below pass the linker itself.
//  * A node is 8 bytes from the Apt block pool (the pool at VA 0x00E176E8, alloc 0x006DB160, free
//    0x006DB270 with size 8): the link it owns at +0 and the next node at +4. 0x006D15B0 pushes a new node on the
//    head and takes a count on the link it was given (`inc [link]`: the link is a counted object whose first
//    dword is its use count); 0x006D1C90 erases the node an iterator names (the head directly, any other node
//    after finding its predecessor), runs the node's teardown 0x006CD530 (drops the count on the link) and
//    returns the node to the pool.
//  * The map files these three under three different placeholder class names (Rva006D15B0Stack, BfmeListVMU,
//    Rva006D1D10) and AptLinker::Load / the two 0x006D1450 / 0x006D17F0 rows under AptLinker: they are one
//    class, the linker. The two list names below are the map's spellings of the node and of the list part;
//    they stay until the map's owner renames them.
// Not recovered: how the linker is created and destroyed (no recovered row constructs it), the link record
// the node points to (a counted pair of the file key and the target CIH) and the file key type.
#include "Apt/DogmaAllocator.h"
#include "Apt/AptListFamily.h"

class AptCIH;

// The counted link record a node points to (the first dword is its use count; bfmeDrop 0x006D2580 reads the target CIH at +8).
// Only those two words are recovered; the record is not constructed by any recovered row.
struct AptLoadLink
{
    unsigned int mnRefs;  // 0x0
    int reserved04;       // 0x4
    AptCIH *mpTarget;     // 0x8: the clip the loaded movie replaced the character of
};

// One list node (map name BfmeNodeVMU).
class BfmeNodeVMU
{
public:
    // 0x006CD530 (the map files it as detach on the placeholder class Rva006CD530): drops the count the node holds on its link.
    void detach();
    // The node pushLink (0x006D15B0) builds: takes a count on the link it points to.
    BfmeNodeVMU(AptLoadLink *link) : mpLink(link)
    {
        if (mpLink)
            ++mpLink->mnRefs;
        mpNext = 0;
    }
    static void *operator new(unsigned int size, void *place) { return place; }

    AptLoadLink *mpLink;   // 0x0: the counted link record
    BfmeNodeVMU *mpNext;   // 0x4
};

typedef char AptLinkNodeSize[(sizeof(BfmeNodeVMU) == 8) ? 1 : -1];

// The list part of the linker (map name BfmeListVMU; the linker's members start with it).
class BfmeListVMU
{
public:
    // 0x006D1C90: removes the node an iterator names (the iterator is a pointer to the node pointer).
    void bfmeEraseVMU(BfmeNodeVMU **it);
    // 0x006D15B0 (the map files it as rva006D15B0 on the placeholder class Rva006D15B0Stack): pushes a node for a link on the head and takes a count on the link.
    void pushLink(AptLoadLink *const *link);

    BfmeNodeVMU *mpHead; // 0x0
};

typedef char AptLinkListSize[(sizeof(BfmeListVMU) == 4) ? 1 : -1];

// The linker itself (map name AptLinker; its list part is BfmeListVMU above). Load (0x006D1F80, 1525 B, AptLoad.cpp) is the one
// entry the movie clip script methods use: it resolves the file name through the string key list, finds the target clip by name
// and replaces the clip's character with the loaded movie (an empty file name unloads).
#include "Apt/EAStringC.h"

class AptLinker : public BfmeListVMU
{
public:
    void Load(const EAStringC &file, EAStringC target); // 0x006D1F80
    // 0x006D2580 (the map files it as bfmeDrop on the placeholder class BfmeTracker4310): erases the node whose link targets the given clip.
    void bfmeDrop(AptCIH *target);
    // 0x006D1450 (the map files it as rva006D1450 on AptLinker): registers a counted file handle on the linker's pending vector (at +4) unless it is there already.
    void registerFile(AptFileKey key);
    AptBlockVector mEntries; // 0x04: the files registered by 0x006D1450
};

extern AptLinker *g_pAptLinker; // VA 0x00E176F8: created by Apt's first tick
