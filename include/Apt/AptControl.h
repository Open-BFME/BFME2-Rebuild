#pragma once
// The per-frame control records of a loaded movie (see AptCharacter.h). A frame is {count, controls}: an array of
// pointers to controls, each a tagged record whose first word is the type. Retail's assert text names one
// type, AptControlType_PlaceObject2 (0x0070F040 "pItem->pControl->eType == AptControlType_PlaceObject2"); the
// other values are named here for what AptMovie::doFrameControls (0x0070F370) does with them, not from a string.
// The placement record's member name nCharacterID is retail's (0x006F8EC0 "pPlaceObject2->nCharacterID").

enum AptControlType
{
    AptControlType_DoAction = 1,        // runs an action stream (the movie loader relocates it: AptMovie::relocate 0x0070E900)
    AptControlType_FrameLabel = 2,      // names the frame (the loader enters it in the movie's label hash)
    AptControlType_PlaceObject2 = 3,    // places or updates the character at a depth
    AptControlType_RemoveObject = 4,    // removes the object at a depth (value = the depth)
    AptControlType_Background = 5,      // hands value to the host's background callback once
    AptControlType_StartSound = 6,      // value = character id of the sound; given to the host sound callback
    AptControlType_StartSoundStream = 7, // logged as "soundstream level is always 0" and otherwise ignored
    AptControlType_InitAction = 8       // a script run once the movie's characters are bound
};

struct AptControlPlaceObject2
{
    int flags;           // 0x00: bit 0 modify, 1 new character, 2 matrix, 3 colour transform, 4 ratio, 5 name, 7 actions
    int depth;           // 0x04
    int nCharacterID;    // 0x08
    float matrix[6];     // 0x0C
    unsigned int cxform[2]; // 0x24 (the pair of colour words the display list takes by pointer)
    float ratio;         // 0x2C
    const char *name;    // 0x30
    int clip;            // 0x34
    void *actions;       // 0x38
};

// A DoAction control: the action byte stream at +0x4 (retail's assert text: `action.actions.aActionStream`).
struct AptControlDoAction
{
    const unsigned char *pStream; // 0x4
};

// A FrameLabel control: the label text at +0x4 (retail's assert text: `frameLabel.szLabel`).
struct AptControlFrameLabel
{
    const char *szLabel; // 0x4
};

// One entry of a placement's event action list (12 bytes; only the stream at +0x8 is read: 0x0070E900 `aEventActions[k].actions.aActionStream`).
struct AptEventAction
{
    int reserved0;
    int reserved4;
    const unsigned char *pStream; // 0x8
};

// A placement's event actions (place.actions points at one): the count at +0x0 and the entries at +0x4.
struct AptEventActions
{
    int nEventActions;            // 0x0
    AptEventAction *aEventActions; // 0x4
};

struct AptControlInitAction
{
    int nSprite;         // 0x0: the character id the script belongs to; negated once it has run
    const unsigned char *pStream; // 0x4: the action byte stream
};

// SIZE depends on the type; every kind starts with the type word.
struct AptControl
{
    int eType; // 0x00
    union {
        AptControlInitAction init;      // 0x04
        AptControlDoAction doAction;    // 0x04
        AptControlFrameLabel frameLabel; // 0x04
        AptControlPlaceObject2 place;   // 0x04
        int value;                      // 0x04
    };
};

struct AptFrame
{
    int count;             // 0x0
    AptControl **controls; // 0x4
};
