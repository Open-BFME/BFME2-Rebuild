// BFMEDesyncCheck.h: the client/logic CRC desync check (CRCDebug.cpp, retail 0x002CEA22).
#pragma once
#ifndef BFME2_BFMEDESYNCCHECK_H
#define BFME2_BFMEDESYNCCHECK_H

#include "Common/GameType.h"

// Records the game logic CRC when a client CRC check is enabled and a mode-gated game is running.
class BFMEDesyncCheck
{
public:
	BFMEDesyncCheck();
	void writeReportIfMismatched();

private:
	UnsignedInt m_crc;
};

#endif
