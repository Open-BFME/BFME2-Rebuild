// Errors.h: the exception codes game code throws (Zero Hour's Common/Errors.h; retail throws 0xDEAD0005 from DataChunkInput::readDict 0x00307833).
// Portions derived from Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
#pragma once
#ifndef BFME2_ERRORS_H
#define BFME2_ERRORS_H

enum ErrorCode
{
	ERROR_BASE = 0xdead0001,
	ERROR_BUG = (ERROR_BASE + 0x0000),
	ERROR_OUT_OF_MEMORY = (ERROR_BASE + 0x0001),
	ERROR_BAD_ARG = (ERROR_BASE + 0x0002),
	ERROR_INVALID_FILE_VERSION = (ERROR_BASE + 0x0003),
	ERROR_CORRUPT_FILE_FORMAT = (ERROR_BASE + 0x0004),
	ERROR_BAD_INI = (ERROR_BASE + 0x0005),
	ERROR_INVALID_D3D = (ERROR_BASE + 0x0006),
	ERROR_LAST
};

#endif
