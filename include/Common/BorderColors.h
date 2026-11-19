// BorderColors.h: the colours of the map boundaries a script names by number (Parameter::getUiText prints the name).
// Target facts: Parameter::getUiText 0x003B4B1D reads the name at [index * 8 + table]; the eight names and colours are the
// bytes of retail's table, in order. The shape is Zero Hour's Common/BorderColors.h.
// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
#pragma once
#ifndef BFME2_BORDERCOLORS_H
#define BFME2_BORDERCOLORS_H

struct BorderColor
{
	char *m_colorName;
	long m_borderColor;
};

const BorderColor BORDER_COLORS[] =
{
	{ "Orange",	0xFFFF8700, },
	{ "Green",	0xFF00FF00, },
	{ "Blue",	0xFF0000FF, },
	{ "Cyan",	0xFF00FFFF, },
	{ "Magenta",	0xFFFF00FF, },
	{ "Yellow",	0xFFFFFF00, },
	{ "Purple",	0xFF9E00FF, },
	{ "Pink",	0xFFFF8670, },
};

const long BORDER_COLORS_SIZE = sizeof(BORDER_COLORS) / sizeof (BORDER_COLORS[0]);

#endif
