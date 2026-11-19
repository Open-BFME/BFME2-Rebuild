// RandomValue.h: SAGE logic/client random number entry points.
#pragma once
#ifndef BFME2_RANDOMVALUE_H
#define BFME2_RANDOMVALUE_H

#include "Common/GameType.h"

Int GetGameLogicRandomValue(Int lo, Int hi, char *file, Int line);
Real GetGameLogicRandomValueReal(Real lo, Real hi, char *file, Int line);
Int GetGameClientRandomValue(Int lo, Int hi, char *file, Int line);
Real GetGameClientRandomValueReal(Real lo, Real hi, char *file, Int line);

// GameClientRandomVariable: native ctorB4979 zeros type/low/high at0/4/8;
// accessors421795/452D82/4F5F92 read those exact words. No base or virtuals.
// Copy assignment353A copies12 bytes. Distribution names are the native table
// DBA3E8 read by INI parser338A9D; enum/member spellings are ZH name leads.
// SIZE 0xC.
class GameClientRandomVariable
{
public:
	enum DistributionType
	{
		CONSTANT, UNIFORM, GAUSSIAN, TRIANGULAR, LOW_BIAS, HIGH_BIAS
	};

	GameClientRandomVariable() : m_type(CONSTANT), m_low(0.0f), m_high(0.0f) {}
	void setRange(Real low, Real high, DistributionType type = UNIFORM);
	Real getValue() const;
	Real getMinimumValue() const;
	Real getMaximumValue() const;
	DistributionType getDistributionType() const;
	static const char *DistributionTypeNames[];

	DistributionType m_type;	// 0x0
	Real m_low;					// 0x4
	Real m_high;				// 0x8
};
// SIZE 0xC
typedef char GameClientRandomVariableSize[sizeof(GameClientRandomVariable)==0xC?1:-1];

// Zero Hour RandomValue.h: the logic-side twin. SIZE 0xC.
class GameLogicRandomVariable
{
public:
	enum DistributionType
	{
		CONSTANT, UNIFORM, GAUSSIAN, TRIANGULAR, LOW_BIAS, HIGH_BIAS
	};

	void setRange(Real low, Real high, DistributionType type = UNIFORM);
	Real getValue() const;

	DistributionType m_type;	// 0x0
	Real m_low;					// 0x4
	Real m_high;				// 0x8
};
// SIZE 0xC

#endif
