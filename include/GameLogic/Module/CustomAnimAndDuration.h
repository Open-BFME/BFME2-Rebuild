// CustomAnimAndDuration.h: a custom animation type and its duration, as
// INI::iniParseAnimAndDuration (RVA 0x0044E6CD) stores it. SIZE 0xC.
// Moved out of FlammableUpdateModuleData.h unchanged so that UpgradeMuxData
// (+0x100) can use it without that header's STLport include order.
#pragma once
#ifndef BFME2_CUSTOM_ANIM_AND_DURATION_H
#define BFME2_CUSTOM_ANIM_AND_DURATION_H

#include "Common/GameType.h"

// An animation type (-1 for none) and two words; FlammableUpdateModuleData's
// constructor (at +0x4C) and UpgradeMuxData's (at +0x100) store -1, 0, 0.
struct CustomAnimAndDuration
{
	CustomAnimAndDuration() {}
	// For a module-data constructor that stores the three words in member order (SpecialAbilityUpdateModuleData 0x0044EB54 stores -1, 0, 0 at +0x18 and +0x24 between its neighbours).
	CustomAnimAndDuration(Int animType, UnsignedInt frames, Int extra) : m_animType(animType), m_frames(frames), m_extra(extra) {}
	void reset()
	{
		m_animType = -1;
		m_frames = 0;
		m_extra = 0;
	}

	Int m_animType;								// 0x0
	UnsignedInt m_frames;						// 0x4
	Int m_extra;								// 0x8
};
// SIZE 0xC

#endif
