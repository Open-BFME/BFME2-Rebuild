#pragma once
// The loaded movie data the Apt runtime walks: a movie is an AptCharacter whose animation member holds the frames,
// the table of characters it defines and its import and export lists. Everything here is plain data written by the
// loader (AptLinker::Load, 0x006D1F80); nothing in it is virtual. The member names that retail's assertion
// strings spell are used as they are: nFrames (0x0070F370 "nFrame >= 0 && nFrame < (int)nFrames"),
// nCharacters and apCharacters (0x006E3E20 "apCharacters[aImports[i].nID] == NULL"), aImports[i].nID,
// aExports[i].nID ("mCharacter->animation.aExports[i].nID >= 0 && ... < mCharacter->animation.nCharacters",
// 0x006E32E0), pPlaceObject2->nCharacterID (0x006F8EC0), pControl->eType ("AptControlType_PlaceObject2",
// 0x0070F040). Offsets are the ones the retail bodies use; names the strings do not give say so.
// AptCharacter::type 0x00: the character kind (its values are the loader's; this header names none).
// AptCharacter::pParentAnim 0x04 (retail's name, 0x006F8EC0: "pParent->pData->pCharacter->pParentAnim->animation.nCharacters"): the movie character that owns this one (the movie's own parent is itself or null).
// AptCharacter::animation 0x08: the movie data (kind movie / sprite); a sound character keeps its sample
// pointer at the same offset (0x0070F370 case 6 passes characters[value]->sound to the host callback).
#include "Apt/AptControl.h"

struct AptNativeHash;
class AptRenderingContext;
class EAStringC;
class AptCIH;
class AptDisplayList;
class AptPseudoDisplayList;
struct AptCharacter;
struct AptFile;

// One entry of animation.aExports. 0x006E46A0 reads the name (strstr for "__Packages.") and the id.
struct AptExport
{
    const char *pName; // 0x0
    int nID;           // 0x4: negated once the export's class definition has run (0x006E46A0)
};

// One entry of animation.aImports (16 bytes). The id (+8) is the character slot the imported character fills
// (0x006E3E20: apCharacters[aImports[i].nID] must be NULL before the fill); the file (+0xC) is the loaded movie
// the character comes from; the name (+4) is what the file is asked for (AptFile::findNamedSlot).
struct AptImport
{
    const char *pMovie;  // 0x0: the name of the movie the import comes from (0x006D0660 compares it with a loaded file's name)
    const char *pName;   // 0x4
    int nID;             // 0x8
    AptFile *pFile;      // 0xC
};

struct AptMovie
{
    int nFrames;       // 0x0
    AptFrame *pFrames; // 0x4 (0x0070F370 steps frames in 8-byte AptFrame records)

    // 0x0070F370: runs the frame's controls against a live instance (two passes: the init actions, then the rest).
    void doFrameControls(AptDisplayList *displayList, AptCIH *cih, int nFrame);
    // 0x0070F040: the same walk against a pseudo display list (no instances): only placements and removals matter.
    void DoTemporaryFrameControls(AptPseudoDisplayList *displayList, int nFrame);
    // 0x0070F010 (the map files it as ?bfmeGo1034F@BfmeF1034@@QAEHH@Z): the frame a label names, or -1. GotoLabel 0x00704A10 and CallFrame
    // 0x00704E10 pass an EAStringC (the label), and test the result for < 0.
    int labelToFrame(const class EAStringC *label);
    // 0x0070F5C0: plays one frame's actions against the instance (CallFrame 0x00704E10 passes the instance's CIH and the frame).
    void runFrameControls(AptCIH *cih, int frame);
    // 0x0070F680 (the map files it as rva0070F680 on the placeholder class Rva0070F680): queues the frame's action controls for the instance
    // (AptCIH::jumpToFrame calls it last, with the frame it settled on).
    void queueFrameActions(AptCIH *cih, int nFrame);
    // 0x0070E900 (the map files it as rva0070E900 on the placeholder class Rva0070E900; `this` is the animation, whose label hash is at +8): the loader's relocation pass over the
    // movie: turns the file-relative offsets of the frame table, controls, action streams, names and event actions into pointers (adds pBase),
    // runs 0x006FD060 on every action stream and enters each frame label in a new label hash.
    void relocate(void *pBase, void *pConstantFile, void *pCount);
    // 0x0070EDC0 (the map files it as rva0070EDC0 on the placeholder class Rva0070EDC0): the inverse of relocate before the file's data is released: runs 0x006FD040 on every action
    // stream, subtracts delta from every pointer, restores the init-action sprite ids to positive and destroys the label hash.
    void unrelocate(int delta, void *context);
};

// SIZE 0x30. The AptCharacter member `animation`.
struct AptCharacterAnimation
{
    AptMovie movie;               // 0x00
    AptNativeHash *pLabels;       // 0x08: the frame labels (name -> frame number; 0x0070F010 looks one up)
    int nCharacters;              // 0x0C
    AptCharacter **apCharacters;  // 0x10
    int reserved14;               // 0x14
    int reserved18;               // 0x18
    unsigned int frameInterval;   // 0x1C: 0x006CD7A0 steps a movie in whole intervals of it
    int nImports;                 // 0x20
    AptImport *aImports;          // 0x24
    int nExports;                 // 0x28
    AptExport *aExports;          // 0x2C

    // 0x006E4610 (the map files it under the placeholder class Rva008A1CF0): the character id the given import
    // resolves to in its own file (strcmp of the import's name against that file's export table; -1 if absent).
    int GetIDFromImportFile(int importIndex);
    // The index of the import that fills the given character slot, or -1 (an inline loop in retail).
    int IsImport(int id)
    {
        for (int i = 0; i < nImports; ++i) {
            if (aImports[i].nID == id)
                return i;
        }
        return -1;
    }
    // 0x006E3410 (the map files it under the placeholder class Rva006E3410): the slot a character pointer occupies in apCharacters.
    int GetIDFromCharacter(AptCharacter *pCharacter);
    void ExecuteInitActions(AptCIH *cih, int characterId);
    void ExecuteInitAction(AptCIH *cih, int characterId);
    void ExportClassDefinitionAssets(AptCIH *cih);
};

typedef char AptCharacterAnimationSize[(sizeof(AptCharacterAnimation) == 0x30) ? 1 : -1];

struct AptCharacter
{
    int type;                  // 0x00
    AptCharacter *pParentAnim; // 0x04
    union {
        AptCharacterAnimation animation; // 0x08
        void *sound;                     // 0x08
        struct {
            char reserved08[0x10];
            unsigned int nShapeId; // 0x18: the id the host's draw callback is given (0x006EBD30 asserts it is not zero)
        } shape;                         // 0x08 (type 1)
    };
    // 0x006EBD30 (the map files it as render on the placeholder class Rva006EBD30): draws the character: a shape goes to the host's draw-rendering-unit callback under the pushed matrix.
    void render(AptRenderingContext *context, void *argument, void *matrix);
    // 0x006EBDD0 (the map name): grows the rect by the character's bounds. 0x006EBD30 is render (same parameters).
    void bounds(AptRenderingContext *context, void *rect, void *matrix);
};
