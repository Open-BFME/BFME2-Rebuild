// CreateAHeroCamera.h: the camera record the create-a-hero screen keeps per hero class (the manager's lookup 0x00219F36
// returns it). Target facts (retail game.dat, RVAs): five start values at
// +0x00, +0x04, +0x08, +0x0C, +0x10 and five end values at +0x18, +0x1C, +0x20, +0x24, +0x28; a fraction below zero stands for
// the default fraction at +0x60 (0x005B01D3 reads it). The five accessors 0x005B0569, 0x005B0588, 0x005B05A8, 0x005B05C8 and
// 0x005B05E8 interpolate one parameter pair each through 0x005B01D3; AptMyHero::rva005B04B6's camera refresh uses the first two
// as pitch and zoom (AptMyHeroRefreshCamera in the earlier decomp).
#pragma once
#ifndef BFME2_CREATEAHEROCAMERA_H
#define BFME2_CREATEAHEROCAMERA_H

class CreateAHeroCamera
{
public:
	float lerp(float startValue, float endValue, float fraction);	// 0x005B01D3 (no source row of its own): fraction below zero becomes m_defaultFraction
	float lerpParameter0(float fraction);	// 0x005B0569
	float lerpParameter1(float fraction);	// 0x005B0588 (the zoom)
	float lerpParameter2(float fraction);	// 0x005B05A8
	float lerpParameter3(float fraction);	// 0x005B05C8
	float lerpParameter4(float fraction);	// 0x005B05E8

	float m_start[5];						// +0x00
	unsigned char m_gap0x14[0x4];			// +0x14
	float m_end[5];							// +0x18
	unsigned char m_gap0x2C[0x34];			// +0x2C
	float m_defaultFraction;				// +0x60
};

#endif
