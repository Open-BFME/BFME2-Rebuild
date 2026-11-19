#pragma once
#include "Common/AsciiString.h"
// Native allocation21861C is20; ctor2175BA/dtor217537 use sole table7E5AAC.
// ZH's next/name/pointSize/height/fontData/bold names are corroborated by
// styled lookup21857E and W3DFontLibrary::loadFontData902A6. BFME2 pointSize
// is float; +1C controls raster-size scaling. The donor MemoryPoolObject
// has no independently evidenced retail class/table (h03); no base is invented.
class GameFont {
public:
    GameFont() {}                  // 0x002175BA (vptr; the name string is cleared), inline at its one user
    virtual ~GameFont() {}         // 0x00217537, deleting dtor 0x00217545
    // Native library deletion uses virtual destruction with flag0 then global
    // deletion. This is the same depooling boundary as TerrainType.
    void operator delete(void *p) { ::operator delete(p); }
    GameFont *next; //4
    AsciiString nameString; //8
    float pointSize; //C
    int height; //10
    void *fontData; //14, device-specific font object
    bool bold; //18
    int rasterScale; //1C, inferred:21857E rounds size on this grid
};
typedef char GameFontSizeCheck[sizeof(GameFont)==0x20 ? 1 : -1];