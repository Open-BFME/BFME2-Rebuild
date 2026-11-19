// DRAFT: the buff logic singleton (TheBuffLogic, VA 0x00DFF190). Only the call BuffInstance reaches is
// declared: addBuff 0x0030682A takes the buff's ThingTemplate and the Drawable and returns the effect object
// that BuffInstance keeps at +0x18 (BuffInstance::MakeNormalBuff 0x00362518 and DoXfer 0x003628D3 call it
// through [0xDFF190]). Size, layout and the rest of the class are not recovered yet (BuffLogic.cpp).
#pragma once
#ifndef BFME2_BUFF_LOGIC_H
#define BFME2_BUFF_LOGIC_H

class BuffLogic
{
public:
	void *addBuff(void *thingTemplate, void *drawable);		// 0x0030682A
};

extern BuffLogic *TheBuffLogic;

#endif
