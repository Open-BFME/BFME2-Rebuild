#pragma once
#include <stddef.h>
// SIZE 0x10, no base or virtual functions. The four float fields at
// +0/+4/+8/+C agree across thirteen old views and are read as promoted
// float arguments by retail XferSaveAsText::operator== at 0x0060DC95.
struct RGBAColorReal
{
    float red; // 0x0
    float green; // 0x4
    float blue; // 0x8
    float alpha; // 0xC
};
typedef char RGBAColorRealSize[(sizeof(RGBAColorReal) == 0x10) ? 1 : -1];
typedef char RGBAColorRealAlphaOffset[(offsetof(RGBAColorReal,alpha) == 0xC) ? 1 : -1];
