// SmoothCameraHeightField.h: smoothed terrain height grid embedded in W3DView.
#pragma once
#ifndef BFME2_SMOOTHCAMERAHEIGHTFIELD_H
#define BFME2_SMOOTHCAMERAHEIGHTFIELD_H

#include <stddef.h>
#include <vector>
#include "Common/GameType.h"

class SmoothCameraHeightField
{
public:
	Real getHeight(Real x, Real y);
	void clear();
	void Initialize(const UnsignedShort *heightData, Int width, Int height, Int cellSize, Int borderSize);

	std::vector<Real> m_heights;
	Int m_width;
	Int m_height;
	Real m_cellSize;
	Int m_borderSize;
	Bool m_initialized;
};

typedef char SmoothCameraHeightFieldSize[(sizeof(SmoothCameraHeightField) == 0x20) ? 1 : -1];
typedef char SmoothCameraHeightFieldWidthOffset[(offsetof(SmoothCameraHeightField, m_width) == 0x0C) ? 1 : -1];
typedef char SmoothCameraHeightFieldInitOffset[(offsetof(SmoothCameraHeightField, m_initialized) == 0x1C) ? 1 : -1];

#endif
