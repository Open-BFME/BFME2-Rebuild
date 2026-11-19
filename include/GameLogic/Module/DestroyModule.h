// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// DestroyModule.h: the interface a behavior module returns from BehaviorModuleInterface slot 6
// when it wants to hear that its object is being destroyed (Zero Hour's DestroyModuleInterface).
// Retail evidence: GameLogic::destroyObject 0x00242C09 calls slot 6 (+0x18) of every behavior module's
// interface and, for a non-null result, calls slot 0 of the returned interface.
#pragma once
#ifndef BFME2_DESTROYMODULE_H
#define BFME2_DESTROYMODULE_H

class DestroyModuleInterface
{
public:
	virtual void onDestroy() = 0;
};

#endif
