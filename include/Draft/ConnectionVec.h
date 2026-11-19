// ConnectionVec.h: the region's list of connections, a plain three-pointer vector of records.
//
// Target facts (retail RVAs):
//  - Three words, SIZE 0x0C: begin at +0, end at +4, end of storage at +8.
//  - clear 0x003F3739 hands the whole range to erase 0x003F35B0 (begin and
//    end, both pushed from the object).
//  - erase 0x003F35B0 copies the tail down, destroys the vacated range through
//    0x003F0CA1 and stores the new end at +4.
//  - ScrapStorage 0x00565A24 destroys the range and frees the storage.
//  - The element is LivingWorldRegionConnection (0x18 bytes).
#pragma once
#ifndef BFME2_CONNECTIONVEC_H
#define BFME2_CONNECTIONVEC_H

#include "Draft/LivingWorldRegionConnection.h"

class ConnectionVec
{
public:
	void clear();								// 0x003F3739
	LivingWorldRegionConnection *rva003F35B0(LivingWorldRegionConnection *first, LivingWorldRegionConnection *last);	// 0x003F35B0

private:
	LivingWorldRegionConnection *m_start;				// +0x00
	LivingWorldRegionConnection *m_finish;				// +0x04
	LivingWorldRegionConnection *m_endOfStorage;			// +0x08
};

#endif
