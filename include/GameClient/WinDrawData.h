#pragma once
class Image;
// SIZE 0x0C, no vptr. init0x00322174 and GameWindow draw setters prove
// three dwords and 12-byte stride. Names: donor (ZH) WinInstanceData.h.
struct WinDrawData { const Image *image; int color; int borderColor; };
typedef char WinDrawDataSizeCheck[sizeof(WinDrawData)==0xC ? 1 : -1];
