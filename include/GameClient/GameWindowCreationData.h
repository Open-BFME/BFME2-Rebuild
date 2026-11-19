#pragma once
#include "GameClient/GameWindow.h"
// Target GameWindow ctor313847 copies this record's bounds and callbacks;
// winCreate2C2533 and W3D factories8FCE3..8FFFD use allocator+18/instance+30.
// Synthesized GadgetCreateView and Rva0047EDE0CreateInfo are the same34-byte
// record. Class/member names describe these roles; original record spelling
// is unknown. Callback ABI is independently used by the GameWindow methods.
struct GameWindowCreationData
{
    GameWindow *parent; // 0
    unsigned int status; // 4
    int x, y, width, height; // 8..14
    GameWindowFactory allocator; // 18
    GameWindowInputFunc system; // 1C
    GameWindowInputFunc input; // 20
    GameWindowDrawFunc draw; // 24
    GameWindowTooltipFunc tooltip; // 28
    GameWindowCallback callback; // 2C
    WinInstanceData *instance; // 30
};
typedef char GameWindowCreationDataSize[sizeof(GameWindowCreationData)==0x34?1:-1];