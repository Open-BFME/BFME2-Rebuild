// ParabolicEase.h -- GameClient easing curve.
//
// Two floats after the implicit this: the setter stores the normalised ease-in
// at +0 and 1 - ease-out at +4, and operator() reads them in that order.
#pragma once

#include "Common/GameType.h"

class ParabolicEase
{
public:
	ParabolicEase(Real easeInTime = 0.0f, Real easeOutTime = 0.0f, Real duration = 1.0f);
	void setEaseTimes(Real easeInTime, Real easeOutTime, Real duration);
	Real operator()(Real param) const;

private:
	Real m_in;	// +0x00
	Real m_out;	// +0x04
};
// SIZE 0x08
