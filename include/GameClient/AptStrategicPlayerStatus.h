// AptStrategicPlayerStatus.h: the in-game strategic player-status window ("StrategicPlayerStatus.apt").
//
// Target facts (retail game.dat, RVAs; table addresses are VAs):
//  - Constructor 0x00523825 (one argument for _bfme_AptGameWindow's constructor 0x0051268C), destructor
//    0x00523775, deleting destructor 0x00523809. Tables VA 0x00867C34 (primary) and 0x00867C30 (the
//    AptCallbackHost secondary).
//  - The registration strings the constructor passes ("StrategicPlayerStatus::OnCloseWindow", "...::EnemyColor_%d",
//    "...::%s" over the table _s_statusNames) name the class StrategicPlayerStatus, and the map names the
//    callbacks StrategicPlayerStatus::rvaNNNNNNNN, while the constructor and tables are the map's
//    AptStrategicPlayerStatus. The class below carries the callbacks and members; AptStrategicPlayerStatus
//    derives from it and owns only the constructor/destructor identity (no data of its own), so both
//    mangled spellings exist. (A map rename would let the two collapse.)
//  - Members: +0x27C, +0x280 and +0x284 are ints (the constructor zeroes them; the extern handler
//    0x00523438 prints one of them with "%d" by index 0..2); +0x288 is a vector<unsigned int> (constructed
//    with 0x00211E58, freed by the destructor through 0x00030830): the enemy colour values the handler
//    0x0052373A prints as "0x%x" masked with 0x00BFFFFF. SIZE 0x294.
#pragma once
#ifndef BFME2_APTSTRATEGICPLAYERSTATUS_H
#define BFME2_APTSTRATEGICPLAYERSTATUS_H

#include "GameClient/BfmeAptGameWindow.h"
#include "Common/Vector.h"

class StrategicPlayerStatus : public _bfme_AptGameWindow
{
public:
	void OnCloseWindow(const char *);				// 0x00523481
	void rva00523438(int, char *, bool);			// 0x00523438: extern handler, writes "%d" of m_stat[index]
	void rva0052373A(int, char *, bool);			// 0x0052373A: extern handler, writes "0x%x" of an enemy colour
	static void rva005234AD();						// 0x005234AD: opens the window when the game state allows
	static void rva00523592();						// 0x00523592: closes the open window or opens it

	int m_stat0;									// +0x27C
	int m_stat1;									// +0x280
	int m_stat2;									// +0x284
	_STL::vector<unsigned int> m_enemyColors;		// +0x288
};
typedef char StrategicPlayerStatusSizeCheck[sizeof(StrategicPlayerStatus)==0x294 ? 1 : -1];

class AptStrategicPlayerStatus : public StrategicPlayerStatus
{
public:
	AptStrategicPlayerStatus(void *);				// 0x00523825
	virtual ~AptStrategicPlayerStatus();			// 0x00523775
};

#endif
