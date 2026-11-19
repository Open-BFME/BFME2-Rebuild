#pragma once
// The loaded definition of a button character (AptCharacter type 4), as the button code in AptInput.cpp reads it
// through a button instance's character pointer (AptCharacterInst::pCharacter, +0x0C of the instance).
// Evidence (all retail reads, no writes are recovered):
//  * the part from +0x08 on is what the hit test hands out as one descriptor (0x006FA420 passes character + 8
//    to the mesh hit test 0x006F9DE0 and reads the counts through it);
//  * +0x2C the number of hit records and +0x30 their array (stride 0x44: the mask at +0 with bit 3 (8) set for a
//    record that takes part in hit tests, a shape pointer at +4, a 6-float matrix at +0x0C): AptCIH::rva006E1F00
//    0x006E1F00 and the hit test 0x006FA420. A shape is a type word (1 mesh, 2 rectangle) and for the rectangle
//    four floats from +8 (x0, y0, x1, y1);
//  * +0x34 the number of action records and +0x38 their array (stride 8: the state mask the record answers to
//    and the action stream): AptAnimationPoolData::rva006FA100 0x006FA100 queues each record whose mask has the
//    state bit;
//  * +0x3C a table of four sound pointers (nothing when null), indexed 0 over, 1 down, 2 up-inside, 3 up-outside
//    in the state switch of 0x006FA100; a sound object holds its handle at +8.
// Everything between +0x08 and +0x2B is not recovered (the button's own bounds and flags). Nothing may embed or
// allocate these structs; they are only read through pointers.
#include "Apt/AptRenderingContext.h"

struct AptButtonHitShape
{
    int eType;         // 0x00: 1 mesh, 2 rectangle
    int reserved04;    // 0x04
    float x0;          // 0x08
    float y0;          // 0x0C
    float x1;          // 0x10
    float y1;          // 0x14
};

struct AptButtonHitRecord
{
    unsigned int flags;       // 0x00: bit 3 (8) = takes part in hit tests
    AptButtonHitShape *pShape; // 0x04: the shape character (the type word first; AptCIH::rva006E1F00 hands it to the display list as the character)
    int reserved08;           // 0x08
    AptMatrix matrix;         // 0x0C
    unsigned int cxform[8];   // 0x24: the colour transform (32 bytes)
};

typedef char AptButtonHitRecordSize[(sizeof(AptButtonHitRecord) == 0x44) ? 1 : -1];

struct AptButtonActionRecord
{
    unsigned int mask; // 0x0
    void *actions;     // 0x4: the action stream the record runs
};

struct AptButtonSound
{
    int reserved0;     // 0x0
    int reserved4;     // 0x4
    void *sound;       // 0x8: the host's sound, the cell AptCharacter::sound holds; both go to g_pfnAptSound
};

struct AptButtonDescriptor
{
    char reserved00[0x24];                   // 0x00..0x23 (character +0x08..+0x2B)
    int nHitRecords;                         // 0x24 (character +0x2C)
    AptButtonHitRecord *aHitRecords;         // 0x28
    int nActionRecords;                      // 0x2C
    AptButtonActionRecord *aActionRecords;   // 0x30
    AptButtonSound **aSounds;                // 0x34
};

struct AptCharacterButton
{
    int type;                  // 0x00
    void *pParentAnim;         // 0x04
    AptButtonDescriptor desc;  // 0x08
};
