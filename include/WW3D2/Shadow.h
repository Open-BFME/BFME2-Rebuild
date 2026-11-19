// Native Shadow base: ctor 0x000F0F2B calls initialize 0x000EFA4E,
// installs table RVA 0x007CEFA0 (three slots), then writes +0x58..+0x64.
// W3DVolumetricShadow ctor 0x000F1528 and projected ctor 0x0010BA2C call
// that constructor and begin their own members at +0x68. SIZE 0x68.
// Older 0x58 private views describe only initialize's prefix.
#pragma once
#ifndef BFME2_WW3D2_SHADOW_H
#define BFME2_WW3D2_SHADOW_H
#include "Common/GameType.h"
#include "Common/Coord3D.h"
#include "Common/AsciiString.h"

class Shadow
{
public:
    // Native ctor 0x00079514 / dtor 0x000793FA; 0x28-byte stack object
    // in RadiusDecalTemplate::createRadiusDecal 0x00331459.
    struct ShadowTypeInfo
    {
        ShadowTypeInfo();
        ~ShadowTypeInfo();
        AsciiString m_texture;       // 00
        AsciiString m_texture2;      // 04
        Int m_type;                  // 08
        Real m_sizeX, m_sizeY;       // 0C,10
        Real m_offsetX, m_offsetY;   // 14,18
        Real m_unknown1C;
        Real m_scale;                // 20
        Bool m_unknown24;
        Bool m_allowUpdates;         // 25
        Bool m_allowWorldAlign;      // 26
    };
    virtual ~Shadow();              // 0
    virtual void slot01();          // 1, folded empty body; role unresolved
    virtual void release();         // 2, pure in base; RadiusDecal::clear

    void setPosition(const Coord3D &pos) { m_position = pos; }
    void setAngle(Real angle) { m_angle = angle; }
    void setColor(Int color);       // 0x00330995
    void setOpacity(Int opacity);   // 0x003308F6
    void setSize(Real width, Real height); // 0x000B22CB

    Bool m_enabled;                 // 04, initialized true by 0x000EFA4E
    Bool m_invisible;               // 05
    Coord3D m_position;             // 08
    Coord3D m_direction;            // 14, initialized (0,0,1)
    Real m_angle;                   // 20
    Int m_color;                    // 24
    Int m_diffuse;                  // 28
    Int m_opacity;                  // 2C
    Bool m_unknown30;               // 30
    Int m_type;                     // 34
    UnsignedInt m_fadeWords[8];     // 38..54, used by manager 0x0010BE72
    Real m_width, m_height;         // 58,5C
    Real m_scale;                   // 60
    Bool m_largeRadius;             // 64, set when radius >110 by 0x00331459
};
typedef char ShadowSize[(sizeof(Shadow) == 0x68) ? 1 : -1];
typedef char ShadowTypeInfoSize[(sizeof(Shadow::ShadowTypeInfo) == 0x28) ? 1 : -1];
class Xfer;
void XferShadowType(Xfer *xfer, Int *value); // 0x00305F7A
#endif
