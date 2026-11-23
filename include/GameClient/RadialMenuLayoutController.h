// RadialMenuLayoutController.h: base radial command menu layout controller.
#pragma once
#ifndef BFME2_RADIALMENULAYOUTCONTROLLER_H
#define BFME2_RADIALMENULAYOUTCONTROLLER_H

#include <stddef.h>
#include <map>
#include "Common/GameType.h"
#include "Common/Coord2D.h"
#include "Common/ICoord2D.h"

class RadialMenuLayoutController
{
public:
	RadialMenuLayoutController();
	virtual ~RadialMenuLayoutController();

	virtual Bool ShouldButtonFlash(Int index) = 0;
	virtual void PositionButton(Int index, const Coord2D &pos, const Coord2D &size) = 0;
	virtual void EnableButtonInput(Int index, Bool enable) {}
	virtual ICoord2D getCenterScreenPos() = 0;
	virtual void onLayoutComplete();

	void reset();
	void updateLayout();
	void secondarySetCount(Int count);

	Bool m_unk0x04;
	Int m_buttonCount;
	UnsignedByte m_unk0x0C[0x10];
	Int m_unk0x1C;
	Int m_unk0x20;
	Int m_unk0x24;
	Real m_unk0x28;
	Real m_unk0x2C;
	std::map<UnsignedByte, Int> m_slotMap;
};

typedef char RadialMenuLayoutControllerSize[(sizeof(RadialMenuLayoutController) == 0x3C) ? 1 : -1];
typedef char RadialMenuLayoutControllerMapOffset[(offsetof(RadialMenuLayoutController, m_slotMap) == 0x30) ? 1 : -1];

#endif
