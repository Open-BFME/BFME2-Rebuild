// GateOpenBehaviorList.h: the list of every gate module in the world, kept by the skirmish AI manager
// (TheSkirmishAIManager at 0x00DFEEF8, member +0x940). A plain vector of GateOpenAndCloseBehavior pointers:
// begin at +0, end at +4 (the removal 0x004E908C and appendOnce 0x004E9353 walk [+0, +4) comparing each entry
// with the argument; appendOnce pushes back when the argument is not found, the removal erases the entry).
// GateOpenAndCloseBehavior's constructor 0x0049889C appends itself, its destructor 0x00498798 and
// onObjectCreated 0x0049936A (dead owner) and update 0x0049947A (dead owner) remove it.
// Only the methods those rows call are declared; the map spells the class GateOpenBehaviorList (f08).
#pragma once
#ifndef BFME2_GATEOPENBEHAVIORLIST_H
#define BFME2_GATEOPENBEHAVIORLIST_H

class GateOpenBehaviorList
{
public:
	void rva004E908C(void *item);	// 0x004E908C: erases the entry equal to item, if any
	void appendOnce(void *item);	// 0x004E9353: pushes item back unless it is already listed
private:
	void **m_begin;					// +0x00
	void **m_end;					// +0x04
};

#endif
