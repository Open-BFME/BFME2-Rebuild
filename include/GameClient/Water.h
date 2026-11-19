// Water.h -- the water settings the client's water renderers read.
//
// Layout evidence (Water.cpp object O00309CF8, read off the constructor's
// stores): the vptr at +0 is 0x00C082E8, two AsciiString members at +0x04 and
// +0x08 (both constructed by the same AsciiString constructor), an Int at
// +0x0C, a Real at +0x10, six RGBAColorInt blocks from +0x14 to +0x73, and two
// Reals at +0x74 and +0x78 -- 0x7C bytes, which is the element size the
// static initializer at 0x007AE6C2 uses for the six-element array at
// 0x00DFF1A0 that INI::parseWaterSettingDefinition indexes as WaterSettings.
// Member spellings are the Zero Hour Water.cpp donor's; the offsets are
// retail's.
#pragma once
#ifndef BFME2_WATER_H
#define BFME2_WATER_H

#include "Common/GameType.h"
#include "Common/AsciiString.h"
#include "Common/RGBAColorInt.h"

struct FieldParse;

class WaterSetting
{
public:
	WaterSetting(void);
	virtual ~WaterSetting(void);

	const FieldParse *getFieldParse() const { return m_waterSettingFieldParseTable; }
	static const FieldParse m_waterSettingFieldParseTable[];	///< the parse table for INI definition

	AsciiString m_skyTextureFile;			// +0x04
	AsciiString m_waterTextureFile;			// +0x08
	Int m_waterRepeatCount;					// +0x0C
	Real m_skyTexelsPerUnit;				// +0x10
	RGBAColorInt m_vertex00Diffuse;			// +0x14
	RGBAColorInt m_vertex10Diffuse;			// +0x24
	RGBAColorInt m_vertex11Diffuse;			// +0x34
	RGBAColorInt m_vertex01Diffuse;			// +0x44
	RGBAColorInt m_waterDiffuseColor;		// +0x54
	RGBAColorInt m_transparentWaterDiffuse;	// +0x64
	Real m_uScrollPerMs;					// +0x74
	Real m_vScrollPerMs;					// +0x78
};
// SIZE 0x7C

#endif

// Target ctor200D80/allocation200F0E: size5C. Overridable primary table7E2BA4
// contains only its deleting destructor200E79. Registration at+10 has separate
// table7E2B98: 200BD9, bare return, 200BBA. Parser table8080B0 types the members.
#pragma once
#include "Common/Overridable.h"
#include "Common/INI/INIFieldEditRegistration.h"
#include "Common/RGBColor.h"
#include "Common/Coord2D.h"
#include <stddef.h>
class WaterTransparencySetting : public Overridable, public INIFieldEditRegistration
{
public:
    WaterTransparencySetting();
    // Implicit virtual destructor: retail200E95 does not rewrite vptrs.
    virtual void *getEditableInstance();
    virtual void onFieldsEdited() {}
    virtual const FieldParse *getEditableFieldParse();
    WaterTransparencySetting &operator=(const WaterTransparencySetting &);
    const FieldParse *getFieldParse() const { return m_waterTransparencySettingFieldParseTable; }
    static const FieldParse m_waterTransparencySettingFieldParseTable[];

    float m_transparentWaterDepth; // +18
    float m_minWaterOpacity; // +1C
    RGBColor m_standingWaterColor; // +20
    RGBColor m_radarColor; // +2C
    bool m_additiveBlend; // +38
    AsciiString m_standingWaterTexture; // +3C
    float m_riverTransparencyMultiplier; // +40
    int m_unk44;
    int m_unk48;
    float m_reflectionPlaneZ; // +4C
    bool m_reflectionOn; // +50
    Coord2D m_reflectionGuard; // +54
};
// The handle at 0x009FF488 (map name ?TheWaterTransparency@@3V?$OVERRIDE@VWaterTransparencySetting@@@@A).
class WaterTransparencySetting;
extern OVERRIDE<WaterTransparencySetting> TheWaterTransparency;
typedef char WaterTransparencySize[sizeof(WaterTransparencySetting)==0x5C?1:-1];
typedef char WaterTransparencyTextureOffset[offsetof(WaterTransparencySetting,m_standingWaterTexture)==0x3C?1:-1];