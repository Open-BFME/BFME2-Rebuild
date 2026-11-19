// W3DSnowManager: the Direct3D snow (SnowManager's only concrete class; TheGameClient's createSnowManager 0x0004C5F8 allocates 0xB0 bytes for it). Target facts:
// the constructor 0x00093162 runs SnowManager's constructor 0x0020109B, stores the table VA 0x00BC8144 and zeroes +0x74..+0x8C, +0xA0, +0xA8, +0xAC and the
// floats +0x90, +0x94, +0x98, +0x9C, +0xA4. Slots: 0 the deleting destructor 0x00094626, 1 init 0x00094642, 9 reset 0x00093B3F, 10 update 0x000949D9,
// 14 updateIniSettings 0x00093ACB. SIZE 0xB0. The members are not recovered beyond that.
#pragma once
#ifndef BFME2_W3DSNOWMANAGER_H
#define BFME2_W3DSNOWMANAGER_H

#include "GameClient/SnowManager.h"

class W3DSnowManager : public SnowManager
{
public:
	W3DSnowManager();														// 0x00093162
	virtual ~W3DSnowManager();												// slot 0, 0x00094626
	virtual void init();													// slot 1, 0x00094642
	virtual void reset();													// slot 9, 0x00093B3F
	virtual void update();													// slot 10, 0x000949D9
	virtual void updateIniSettings();										// slot 14, 0x00093ACB

	unsigned char m_unk0x74[0x3C];											// +0x74..0xB0
};
typedef char W3DSnowManagerSize[(sizeof(W3DSnowManager) == 0xB0) ? 1 : -1];

#endif
