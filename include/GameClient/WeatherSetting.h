// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// WeatherSetting.h: overridable weather, snow, lightning, and hardware fog settings.
#pragma once
#ifndef BFME2_WEATHERSETTING_H
#define BFME2_WEATHERSETTING_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/AsciiString.h"
#include "Common/Coord2D.h"
#include "Common/RGBColor.h"
#include "Common/RandomValue.h"
#include "Common/Overridable.h"
#include "Common/INI/INIFieldEditRegistration.h"

struct FieldParse;

class WeatherSetting : public Overridable, public INIFieldEditRegistration
{
public:
	WeatherSetting();
	virtual ~WeatherSetting();

	virtual void *getEditableInstance();
	virtual void onFieldsEdited();
	virtual const FieldParse *getEditableFieldParse();

	const FieldParse *getFieldParse() const { return m_weatherSettingFieldParseTable; }
	static const FieldParse m_weatherSettingFieldParseTable[];

	AsciiString m_snowTexture;
	Real m_snowFrequencyScaleX;
	Real m_snowFrequencyScaleY;
	Real m_snowAmplitude;
	Real m_snowPointSize;
	Real m_snowMaxPointSize;
	Real m_snowMinPointSize;
	Real m_snowQuadSize;
	Real m_snowBoxHeight;
	Real m_snowSpacing;
	Real m_snowSpeed;
	Bool m_snowPointSprites;
	Bool m_snowEnabled;
	Bool m_isSnowing;
	Int m_numberTiles;
	Coord2D m_snowXYSpeed;
	Bool m_lightningEnabled;
	GameClientRandomVariable m_lightningFactor;
	Int m_lightningDuration;
	Real m_lightningChance;
	Bool m_spellEnabled;
	Int m_spellDuration;
	Coord2D m_rampControl;
	Coord2D m_rampSpeed;
	Coord2D m_rampSpacing;
	RGBColor m_hardwareFogColor;
	Bool m_hardwareFogEnable;
	Real m_hardwareFogStart;
	Real m_hardwareFogEnd;
	Coord2D m_cloudTextureSize;
	Coord2D m_cloudOffsetPerSecond;
};

extern OVERRIDE<WeatherSetting> TheWeatherSetting;

typedef char WeatherSettingSize[(sizeof(WeatherSetting) == 0xB4) ? 1 : -1];
typedef char WeatherSettingSnowTextureOffset[(offsetof(WeatherSetting, m_snowTexture) == 0x18) ? 1 : -1];
typedef char WeatherSettingFogColorOffset[(offsetof(WeatherSetting, m_hardwareFogColor) == 0x8C) ? 1 : -1];
typedef char WeatherSettingFogEnableOffset[(offsetof(WeatherSetting, m_hardwareFogEnable) == 0x98) ? 1 : -1];
typedef char WeatherSettingFogStartOffset[(offsetof(WeatherSetting, m_hardwareFogStart) == 0x9C) ? 1 : -1];
typedef char WeatherSettingFogEndOffset[(offsetof(WeatherSetting, m_hardwareFogEnd) == 0xA0) ? 1 : -1];
typedef char WeatherSettingCloudOffset[(offsetof(WeatherSetting, m_cloudOffsetPerSecond) == 0xAC) ? 1 : -1];

#endif
