// AptImageMode: one row of the Apt image-mode name table (value and its name).
// Retail's table is six (mode, name) pairs; the image draw parser reads them by index.
#pragma once

struct AptImageMode
{
	int mode;
	const char *name;
};
