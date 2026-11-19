// trim.h: in-place trimming of blanks around a text buffer.
// Target facts: strtrim 0x0061B600 (105 bytes, narrow) and wcstrim 0x0061B670 (102 bytes, wide);
// both return the buffer they were given. INIClass::Strip_Comments and the INI loader trim
// entry and value text with the narrow form.
#pragma once
#ifndef BFME2_TRIM_H
#define BFME2_TRIM_H

#include <wchar.h>

char *strtrim(char *buffer);			// 0x0061B600
wchar_t *wcstrim(wchar_t *buffer);		// 0x0061B670

#endif
