// GlowEffect.h: global glow post-processing configuration parsed from INI.
#pragma once
#ifndef BFME2_GLOWEFFECT_H
#define BFME2_GLOWEFFECT_H

#include <stddef.h>
#include "Common/GameType.h"

struct FieldParse;

struct GlowEffect
{
	static const FieldParse m_fieldParseTable[];

	Bool m_glowEnabled;
	Int m_glowDiameter;
	Real m_glowIntensity;
	Int m_glowTextureWidth;
	Real m_radiusScale1;
	Real m_amplitude1;
	Real m_radiusScale2;
	Real m_amplitude2;
	Bool m_terrainGlow;
	Bool m_multipassGlowEnabled;
};

GlowEffect *GetGlowEffect();

typedef char GlowEffectSize[(sizeof(GlowEffect) == 0x24) ? 1 : -1];
typedef char GlowEffectTerrainGlowOffset[(offsetof(GlowEffect, m_terrainGlow) == 0x20) ? 1 : -1];
typedef char GlowEffectMultipassOffset[(offsetof(GlowEffect, m_multipassGlowEnabled) == 0x21) ? 1 : -1];

#endif
