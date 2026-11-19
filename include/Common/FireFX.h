// FireFX.h: one entry of a fire-effect list — the effect and the bone it plays
// at. Retail copies it through the folded {dword, AsciiString} copy constructor
// at 0x000CF475, so the copy constructor is declared only.
#pragma once
#ifndef BFME2_FIREFX_H
#define BFME2_FIREFX_H

#include "Common/AsciiString.h"

class FXList;

struct TreeKey00242F5E
{
	const FXList *fx;							// 0x0
	AsciiString boneName;						// 0x4
	TreeKey00242F5E(const TreeKey00242F5E &that);
};
// SIZE 0x8

#endif
