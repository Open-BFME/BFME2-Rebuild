#pragma once
#include "Common/OpaqueRefElement4.h"
// Two-word value returned by Drawable's keyed sound lookup 0x00274CD8.
class DrawableAudioEvent
{
public:
    DrawableAudioEvent();
    DrawableAudioEvent(const DrawableAudioEvent &);
    int m_unk0x00;
    OpaqueRefElement4 m_ref; // 0x04
};
// SIZE 0x8
