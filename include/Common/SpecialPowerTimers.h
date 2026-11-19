// SpecialPowerTimers.h: the shared special-power timer surface a Player exposes.
// Target facts (old ledger Rva002AC6B1PlayerTimers, re-read from the image):
// SpecialPowerModule::isReady 0x004934DE and getPercentReady 0x0049355E reach a
// player timer through a one-argument thiscall that returns the ready frame
// (`mov ecx,<player>; push <template>; call rel32`), and the reset counterpart
// takes the same argument. The receiver-prefix view keeps the address-derived
// name until Player's own method names are recovered; Player.h is h04's.
#pragma once
#ifndef BFME2_SPECIALPOWERTIMERS_H
#define BFME2_SPECIALPOWERTIMERS_H

class SpecialPowerTemplate;

class Rva002AC6B1PlayerTimers
{
public:
	// 0x002AC7A0: the ready frame, starting the countdown if it has not begun.
	unsigned getOrStart(const SpecialPowerTemplate *specialPowerTemplate);
	// 0x002AC6E1
	void resetOrStart(const SpecialPowerTemplate *specialPowerTemplate);
	// 0x002AC75C: store the frame the countdown starts from.
	void rva002AC75C(const SpecialPowerTemplate *specialPowerTemplate, unsigned int frame);
};

// 0x002AA0B1: a Player float read by startPowerRecharge when the template's flag bit 5 is set
// (a one-instruction getter; the map spells it Rva002AA0B1FloatField::get).
class Rva002AA0B1FloatField
{
public:
	float get() const;
};

#endif /* BFME2_SPECIALPOWERTIMERS_H */
