// DRAFT (h05 fpgen): CloudEffect from retail's field-parse table (VA 0x00BE5390+0x0).
// Offsets are compile-checked below; no function matches against it yet, so it stays
// in Draft until one does. Types are what each entry's parse function stores; names are
// the INI tokens.
// SIZE not established: members reach 0x98; bases and vptr, if any, are the leading gap.
// Unresolved members: m_darkeningFactor (+0x20, INI::parseRGBColor), m_darkeningFactorRain (+0x2C, INI::parseRGBColor), m_lightningShadowColor (+0x50, INI::parseRGBColor), m_lightningDuration (+0x60, INI::parseGameClientRandomVariable), m_lightningIntensity (+0x70, INI::parseGameClientRandomVariable).
#pragma once
#ifndef BFME2_CLOUD_EFFECT_H
#define BFME2_CLOUD_EFFECT_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/AsciiString.h"
#include "Common/Coord2D.h"


class CloudEffect
{
public:

	AsciiString m_cloudTexture;                            // +0x0 CloudTexture
	AsciiString m_darkCloudTexture;                        // +0x4 DarkCloudTexture
	AsciiString m_alphaTexture;                            // +0x8 AlphaTexture
	Real m_propagateSpeed;                                 // +0xC PropagateSpeed
	Int m_angle;                                           // +0x10 Angle
	AsciiString m_dissipateTexture;                        // +0x14 DissipateTexture
	Real m_dissipateStartLevel;                            // +0x18 DissipateStartLevel
	Real m_dissipateSpeed;                                 // +0x1C DissipateSpeed
	UnsignedByte m_darkeningFactor[0xC];                   // +0x20 DarkeningFactor; unresolved: INI::parseRGBColor
	UnsignedByte m_darkeningFactorRain[0xC];               // +0x2C DarkeningFactorRain; unresolved: INI::parseRGBColor
	Int m_darkeningRate;                                   // +0x38 DarkeningRate
	Int m_lighteningRate;                                  // +0x3C LighteningRate
	Real m_cloudScrollSpeed;                               // +0x40 CloudScrollSpeed
	Real m_dissipateRateScale;                             // +0x44 DissipateRateScale
	Bool m_lightningShadows;                               // +0x48 LightningShadows
	Bool m_jitterLightningLightPosition;                   // +0x49 JitterLightningLightPosition
	Bool m_jitterLightningLightIntensity;                  // +0x4A JitterLightningLightIntensity
	Real m_lightningChance;                                // +0x4C LightningChance
	UnsignedByte m_lightningShadowColor[0xC];              // +0x50 LightningShadowColor; unresolved: INI::parseRGBColor
	Real m_lightningShadowIntensity;                       // +0x5C LightningShadowIntensity
	UnsignedByte m_lightningDuration[0xC];                 // +0x60 LightningDuration; unresolved: INI::parseGameClientRandomVariable
	Real m_lightningFrequency;                             // +0x6C LightningFrequency
	UnsignedByte m_lightningIntensity[0xC];                // +0x70 LightningIntensity; unresolved: INI::parseGameClientRandomVariable
	Coord2D m_lightningLightPosition1;                     // +0x7C LightningLightPosition1
	Coord2D m_lightningLightPosition2;                     // +0x84 LightningLightPosition2
	Coord2D m_lightningLightPosition3;                     // +0x8C LightningLightPosition3
	AsciiString m_lightningFX;                             // +0x94 LightningFX
};
typedef char CloudEffect_off_m_cloudTexture[(offsetof(CloudEffect, m_cloudTexture) == 0x0) ? 1 : -1];
typedef char CloudEffect_off_m_darkCloudTexture[(offsetof(CloudEffect, m_darkCloudTexture) == 0x4) ? 1 : -1];
typedef char CloudEffect_off_m_alphaTexture[(offsetof(CloudEffect, m_alphaTexture) == 0x8) ? 1 : -1];
typedef char CloudEffect_off_m_propagateSpeed[(offsetof(CloudEffect, m_propagateSpeed) == 0xC) ? 1 : -1];
typedef char CloudEffect_off_m_angle[(offsetof(CloudEffect, m_angle) == 0x10) ? 1 : -1];
typedef char CloudEffect_off_m_dissipateTexture[(offsetof(CloudEffect, m_dissipateTexture) == 0x14) ? 1 : -1];
typedef char CloudEffect_off_m_dissipateStartLevel[(offsetof(CloudEffect, m_dissipateStartLevel) == 0x18) ? 1 : -1];
typedef char CloudEffect_off_m_dissipateSpeed[(offsetof(CloudEffect, m_dissipateSpeed) == 0x1C) ? 1 : -1];
typedef char CloudEffect_off_m_darkeningFactor[(offsetof(CloudEffect, m_darkeningFactor) == 0x20) ? 1 : -1];
typedef char CloudEffect_off_m_darkeningFactorRain[(offsetof(CloudEffect, m_darkeningFactorRain) == 0x2C) ? 1 : -1];
typedef char CloudEffect_off_m_darkeningRate[(offsetof(CloudEffect, m_darkeningRate) == 0x38) ? 1 : -1];
typedef char CloudEffect_off_m_lighteningRate[(offsetof(CloudEffect, m_lighteningRate) == 0x3C) ? 1 : -1];
typedef char CloudEffect_off_m_cloudScrollSpeed[(offsetof(CloudEffect, m_cloudScrollSpeed) == 0x40) ? 1 : -1];
typedef char CloudEffect_off_m_dissipateRateScale[(offsetof(CloudEffect, m_dissipateRateScale) == 0x44) ? 1 : -1];
typedef char CloudEffect_off_m_lightningShadows[(offsetof(CloudEffect, m_lightningShadows) == 0x48) ? 1 : -1];
typedef char CloudEffect_off_m_jitterLightningLightPosition[(offsetof(CloudEffect, m_jitterLightningLightPosition) == 0x49) ? 1 : -1];
typedef char CloudEffect_off_m_jitterLightningLightIntensity[(offsetof(CloudEffect, m_jitterLightningLightIntensity) == 0x4A) ? 1 : -1];
typedef char CloudEffect_off_m_lightningChance[(offsetof(CloudEffect, m_lightningChance) == 0x4C) ? 1 : -1];
typedef char CloudEffect_off_m_lightningShadowColor[(offsetof(CloudEffect, m_lightningShadowColor) == 0x50) ? 1 : -1];
typedef char CloudEffect_off_m_lightningShadowIntensity[(offsetof(CloudEffect, m_lightningShadowIntensity) == 0x5C) ? 1 : -1];
typedef char CloudEffect_off_m_lightningDuration[(offsetof(CloudEffect, m_lightningDuration) == 0x60) ? 1 : -1];
typedef char CloudEffect_off_m_lightningFrequency[(offsetof(CloudEffect, m_lightningFrequency) == 0x6C) ? 1 : -1];
typedef char CloudEffect_off_m_lightningIntensity[(offsetof(CloudEffect, m_lightningIntensity) == 0x70) ? 1 : -1];
typedef char CloudEffect_off_m_lightningLightPosition1[(offsetof(CloudEffect, m_lightningLightPosition1) == 0x7C) ? 1 : -1];
typedef char CloudEffect_off_m_lightningLightPosition2[(offsetof(CloudEffect, m_lightningLightPosition2) == 0x84) ? 1 : -1];
typedef char CloudEffect_off_m_lightningLightPosition3[(offsetof(CloudEffect, m_lightningLightPosition3) == 0x8C) ? 1 : -1];
typedef char CloudEffect_off_m_lightningFX[(offsetof(CloudEffect, m_lightningFX) == 0x94) ? 1 : -1];

#endif
