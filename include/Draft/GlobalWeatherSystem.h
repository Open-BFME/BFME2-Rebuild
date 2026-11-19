// GlobalWeatherSystem: the five-weather table and its INI block (SAGE).
// Donor: Zero Hour GameLogic/System/GlobalWeatherSystem.cpp (WeatherData layout and
// the parseWeatherData body).
// Target facts: parseWeatherData 0x0031842A (142B) is the only row this unit owns. The
// FieldParse table at 0x00C0C694 has two 16-byte records plus the zero sentinel: token
// "WeatherSound" -> the rowed audio token proc (RVA 0x339900), token "HasLightning" ->
// INI::parseBool (RVA 0x2E850), offsets 0 and 4. The five 8-byte weather entries start at
// +0x30 of the system (BFME1 put them at +0x28), and TheGlobalWeatherSystem lives at
// VA 0x00E01CE4.
#pragma once
#ifndef BFME2_GLOBALWEATHERSYSTEM_H
#define BFME2_GLOBALWEATHERSYSTEM_H

#include "Common/INI/FieldParse.h"

struct WeatherData
{
	int m_weatherSound;								// 0x00
	int m_hasLightning;								// 0x04

	static const FieldParse m_fieldParseTable[];
};

class GlobalWeatherSystem
{
public:
	static void parseWeatherData(INI *ini);

private:
	char m_unknown00[0x30];
	WeatherData m_weather[5];						// 0x30
};

extern GlobalWeatherSystem *TheGlobalWeatherSystem;

#endif /* BFME2_GLOBALWEATHERSYSTEM_H */
