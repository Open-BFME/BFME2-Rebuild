// RectClass.h: WWMath's axis-aligned float rectangle (WWVegas/WWMath/rect.h): left, top, right,
// bottom in that order. W3DDisplayString::setClipRegion 0x00105E9C builds one from the integer clip
// region (stores the four converted floats at +0/+4/+8/+C of a stack object and passes its address
// to Render2DSentenceClass::Set_Clipping_Rect 0x00105D96). Only the four-float constructor is
// declared; the other members have no retail row yet.
#pragma once
#ifndef BFME2_RECTCLASS_H
#define BFME2_RECTCLASS_H

class RectClass
{
public:
	float Left;		// +0x0
	float Top;		// +0x4
	float Right;	// +0x8
	float Bottom;	// +0xC

	RectClass() {}
	RectClass(float left, float top, float right, float bottom) { Left = left; Top = top; Right = right; Bottom = bottom; }
	RectClass &operator=(const RectClass &rhs);	// 0x0004254E, out of line in retail (copies the four floats and returns this)
};
// SIZE 0x10

#endif
