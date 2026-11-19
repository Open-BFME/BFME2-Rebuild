#pragma once
#include "Common/Coord3D.h"
// BoneFX's array coordinates have out-of-line lifetime hooks in retail
// (construction 0x0087A6A9, destruction 0x004B3FD0).
struct BoneFXBonePosition : Coord3D
{
    BoneFXBonePosition();
    ~BoneFXBonePosition();
};
// SIZE 0xC
