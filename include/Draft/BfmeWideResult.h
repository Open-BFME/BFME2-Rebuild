// BfmeWideResult.h: the map's placeholder name for a partition range-query result (an iterator over the
// objects a range query found). Only next() is declared: SpecialPowerModule::rva0049402B (0x0049402B)
// walks it with 0x00045623 until it returns null.
#pragma once
#ifndef BFME2_BFMEWIDERESULT_H
#define BFME2_BFMEWIDERESULT_H

class Object;

struct BfmeWideResult
{
	Object *next();		// 0x00045623 (W3DDisplay.cpp holds the row)
};

#endif
