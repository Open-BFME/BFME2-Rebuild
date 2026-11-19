#pragma once
#include "Apt/AptDisplayList.h"
// The runtime instance of a character that an AptCIH points at (AptCIH::mpInstance, +0x4C). Only the members
// retail bodies read are recovered; nothing may embed these structs. `struct` in retail (runStream's mangled
// name spells PAUAptCharacterInst).
// +0x0C the character (AptCharacter) the instance was made from: 0x0070F370 and 0x006F8EC0 reach the movie's
//       characters through pCharacter->parent->animation; 0x006CD7A0 reads its frame interval.
// +0x18 the current frame (AptSpriteInstBase names the same word).
// The animation instance (the root movie's: AptCIH::rva006CD650 asserts isAnimationInst() before returning it)
// adds the loaded file at +0x34 (0x006E3E00).
#include "Apt/AptCharacter.h"
#include "Apt/EAStringC.h"

struct AptNativeHash;
class AptValue;

// The loaded movie file. +0x08 EAStringC file name ("Couldn't find export named '%s' in '%s'", 0x006E32E0),
// +0x10 the raw loaded data (the version tag is read from its bytes 8 and 9), +0x14 the movie's main character
// (0x006E32E0: "mCharacter->animation.aExports[i].nID"). The member names mAptData/mCharacter follow the old
// decomp's release-PDB reading; the offsets are retail's.
struct AptFileKey;
struct AptFile
{
    char reserved00[4];
    unsigned int mSize;       // 0x04: the file's size in bytes (the zombie report 0x006CDA40 prints it as "size = %d bytes")
    EAStringC mFilename;      // 0x08
    char reserved0C[4];
    const char *mAptData;     // 0x10
    AptCharacter *mCharacter; // 0x14
    void *field18;            // 0x18 (stored with the data and the character by setData; the pooled record is 0x1C bytes)

    // 0x006CFD90 (the map files it under the placeholder class Rva006D0280, the class whose 0x1C-byte record is this one): stores the loaded data, its
    // character and a third word at +0x10, +0x14 and +0x18.
    void setData(const char *data, AptCharacter *character, void *extra);
    // 0x006D0660 (the map files it as contains on the placeholder class Rva006D0660): whether one of this file's imports comes from the movie the key names.
    int contains(AptFileKey key);
    // 0x006E32E0 (the map files it as findNamedSlot on the placeholder class Rva006E32E0Owner): the character an export of the file's movie names (strcmp of the name against
    // animation.aExports), asserting the id is in range and the character exists; logs `Couldn't find export named '%s' in '%s'` and asserts when there is none.
    AptCharacter *findNamedSlot(const char *name);
};

struct AptCharacterInst
{
    // Retail instances are polymorphic: AptCIH::PreDestroy (0x006E0440) tail-jumps through slot 0 of the instance's table (0x0069E440 in the
    // sprite and text tables, a bare `ret`), AptCIH::DestroyGCPointers (0x006E0460) calls slot 2 (0x006E5FD0: clears the hash at +0x10 and
    // sets the byte at +0x14) and slot 1 is the deleting destructor (0x006EFB70, 0x006EFB90, ... tables 0x008ECB90, 0x008ECB9C). Only the
    // three slots are declared; the names of slots 0 and 2 are the AptCIH virtuals that call them.
    virtual void PreDestroy();
    virtual ~AptCharacterInst();
    virtual void DestroyGCPointers();
    int field04;              // 0x04: a signed word AptDisplayList 0x006F79B0 tests for negative before it draws a child (name not evidenced)
    char reserved08[4];
    AptCharacter *pCharacter; // 0x0C
    AptNativeHash *pHash;     // 0x10: the instance's own member hash (AptValue::rva006E04A0 0x006E04A0 returns it; 0x006F99D0 walks it for focus candidates)
    unsigned char mbGCPointersDestroyed; // 0x14: set by DestroyGCPointers (0x006E5FD0)
    char reserved15[3];
    int nFrame;               // 0x18
};

struct AptCharacterAnimationInst : AptCharacterInst
{
    unsigned int flags; // 0x1C: aptIsPlaying tests bit 25 for this animation instance.
    char reserved20[4];
    AptDisplayList displayList; // 0x24: AptCIH::Release clears it through the checked animation view.
    char reserved28[0x0C];
    AptFile *pFile; // 0x34

    // 0x006E3E00: the Flash version the file was made for: byte 9 of the data minus '0' when byte 8 is ':', else 6.
    int getSwfVersion();
};

// The instance of a text field (an AptCIH of type 0xF points at one; AptValue::rva006E0F40 returns it). Only what AptValue::clearInstanceContents
// (0x006E1540) touches is recovered: the text buffer at +0x20 (given back through the host free callback with size 2; the shared empty buffer
// at VA 0x00DDC2E0 is never freed) and a state word at +0x6C. Nothing may embed it.
// The text field character definition: the initial text of a bound field sits at +0x34 (0x006EBE60 reads it).
struct AptTextCharacterDefinition
{
    char reserved00[0x34];
    const char *pDefaultText; // 0x34
};

struct AptTextInst
{
    // 0x006EBE60: binds the field to its variable when it is created (a name starting with $ is taken as the text itself; otherwise the variable is read from the
    // nearest sprite or animation ancestor, created with the default text when undefined). 0x006EBFF0: re-reads the variable and, when the text changed, stores it and
    // marks the field changed (flag word bit 1 set, bit 0 clear). The names are this header's (the map files them under the placeholder class Rva006EBFF0).
    void rva006EBE60(AptValue *parent);
    void rva006EBFF0(AptValue *parent);

    char reserved00[0xC];
    AptTextCharacterDefinition *pDefinition; // 0x0C
    char reserved10[8];
    EAStringC mText;     // 0x18: the displayed text
    EAStringC mVariable; // 0x1C: the bound variable name
    void *pText;    // 0x20
    char reserved24[0x48];
    int field6C;    // 0x6C
};
