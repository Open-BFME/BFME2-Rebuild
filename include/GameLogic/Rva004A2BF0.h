// Rva004A2BF0.h: the stealth detector's player-facing filter. SIZE 0xC.
// Target facts: allow 0x004A2BF0 asks the object's 0x002943B2 about the player
// at +0x08 and then the object's +0x250 interface for slots 4 (+0x10) and 73
// (+0x124). The interface's other slots are not exercised here.
#pragma once
#ifndef BFME2_RVA004A2BF0_H
#define BFME2_RVA004A2BF0_H

#include "Common/GameType.h"

class Object;
class Player;

class Rva004A2BF0Interface
{
public:
	virtual void s0();
	virtual void s1();
	virtual void s2();
	virtual void s3();
	virtual Bool s4();					// slot 4, +0x10
	virtual void s5();
	virtual void s6();
	virtual void s7();
	virtual void s8();
	virtual void s9();
	virtual void s10();
	virtual void s11();
	virtual void s12();
	virtual void s13();
	virtual void s14();
	virtual void s15();
	virtual void s16();
	virtual void s17();
	virtual void s18();
	virtual void s19();
	virtual void s20();
	virtual void s21();
	virtual void s22();
	virtual void s23();
	virtual void s24();
	virtual void s25();
	virtual void s26();
	virtual void s27();
	virtual void s28();
	virtual void s29();
	virtual void s30();
	virtual void s31();
	virtual void s32();
	virtual void s33();
	virtual void s34();
	virtual void s35();
	virtual void s36();
	virtual void s37();
	virtual void s38();
	virtual void s39();
	virtual void s40();
	virtual void s41();
	virtual void s42();
	virtual void s43();
	virtual void s44();
	virtual void s45();
	virtual void s46();
	virtual void s47();
	virtual void s48();
	virtual void s49();
	virtual void s50();
	virtual void s51();
	virtual void s52();
	virtual void s53();
	virtual void s54();
	virtual void s55();
	virtual void s56();
	virtual void s57();
	virtual void s58();
	virtual void s59();
	virtual void s60();
	virtual void s61();
	virtual void s62();
	virtual void s63();
	virtual void s64();
	virtual void s65();
	virtual void s66();
	virtual void s67();
	virtual void s68();
	virtual void s69();
	virtual void s70();
	virtual void s71();
	virtual void s72();
	virtual Int s73();					// slot 73, +0x124
};

class Rva004A2BF0
{
public:
	Bool allow(Object *obj);

	UnsignedByte m_unk0x00[8];
	const Player *m_player;								// 0x08
};
// SIZE 0xC

#endif
