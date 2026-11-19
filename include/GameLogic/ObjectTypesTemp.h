// ObjectTypesTemp.h: the script conditions' scratch list (Zero Hour defines it in
// ScriptConditions.cpp). Target facts: ctor 0x003BA7FF (out-of-line COMDAT) stores
// 0 at +0, news a 0x14-byte ObjectTypes (ctor 0x003769F9) and stores the pointer;
// callers keep m_types at +0 and destroy it with the virtual destructor
// (flag 0, so `::delete`) followed by the global operator delete (the folded destructor 0x001F4206).
#pragma once
#ifndef BFME2_OBJECTTYPESTEMP_H
#define BFME2_OBJECTTYPESTEMP_H

#include "GameLogic/ObjectTypes.h"

class ObjectTypesTemp
{
public:
	ObjectTypesTemp();				// 0x003BA7FF
	~ObjectTypesTemp() { ::delete m_types; }		// 0x001F4206

	ObjectTypes *m_types;				// +0x00
};
// SIZE 0x04

#endif
