// ProxyClass.h: what HLodClass::Get_Proxy hands back for one proxy of a model: the name of the object to create
// and the bone's transform as a quaternion and a position (BFME 2 stores no Matrix3D here).
// Target facts: HLodClass::Get_Proxy 0x0019C750 writes the name at +0 and the 28-byte transform at +4;
// ProxyClass::Set_Transform 0x0019BFF0 (48 bytes, ret 4) copies those 28 bytes. The map spells the argument
// type BfmeProxyTransform; it has PivotTransform's layout (a rotation, then a translation).
// Portions derived from Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
#pragma once
#ifndef BFME2_PROXYCLASS_H
#define BFME2_PROXYCLASS_H

#include "WWLib/wwstring.h"
#include "WW3D2/pivot.h"

struct BfmeProxyTransform : public PivotTransform
{
	BfmeProxyTransform(void) {}
	BfmeProxyTransform(const PivotTransform &that) : PivotTransform(that) {}
};
typedef char BfmeProxyTransformSize[(sizeof(BfmeProxyTransform) == 0x1C) ? 1 : -1];

class ProxyClass
{
public:
	void Set_Name(const char *name) { Name = name; }
	void Set_Transform(const BfmeProxyTransform &transform);	// 0x0019BFF0

private:
	StringClass Name;			// +0x00
	BfmeProxyTransform Transform;		// +0x04
};

#endif
