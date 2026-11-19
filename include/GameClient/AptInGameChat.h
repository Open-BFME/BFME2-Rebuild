// AptInGameChat.h: the in-game chat entry screen ("InGameChat.apt").
//
// Target facts (retail game.dat, RVAs; table addresses are VAs):
//  - Constructor 0x004E8B38 (one argument for _bfme_AptGameWindow's constructor 0x0051268C), destructor 0x004E83EB, tables VA
//    0x00862630 (primary) and 0x0086262C (AptCallbackHost secondary). SIZE 0x288 (screen factory 0x002D1F3D). The screen instance is the
//    map's g_Va00E04478 (an int-typed global).
//  - Members: +0x27C the step (0 at construction; OnInitialized moves 0 to 1, rva004E8213 sets 4; 0x004E8620's update closes the
//    chat in step 2 and clears the chat window in step 4), +0x280 not touched by the rows, +0x284 the text entry gadget (0 at
//    construction) that Rva004E84AB clears with GadgetTextEntrySetText.
#pragma once
#ifndef BFME2_APTINGAMECHAT_H
#define BFME2_APTINGAMECHAT_H

#include "GameClient/BfmeAptGameWindow.h"

class AptInGameChat : public _bfme_AptGameWindow
{
public:
	AptInGameChat(void *);							// 0x004E8B38
	virtual ~AptInGameChat();						// 0x004E83EB

	void OnInitialized(const char *);				// 0x004E81FF
	void rva004E8213(const char *);					// 0x004E8213
	void rva004E8220(int);							// 0x004E8220: moves step 1 on to 2
	int rva004E846A(int message, unsigned char key, int state);	// 0x004E846A: the Escape and Enter keys close the chat

	void Send(const char *);						// 0x004E8820
	int rva004E8D3E(int message, int data1, int data2);	// 0x004E8D3E: the screen's window message handler; the text entry's Enter (0x4031) sends the chat

	int m_step;										// +0x27C
	int m_x280;										// +0x280
	void InitGadgets(const char *, void *, GameWindow *);	// 0x004E84FF: binds the chat entry
	GameWindow *m_entry;							// +0x284
};
typedef char AptInGameChatSizeCheck[sizeof(AptInGameChat)==0x288 ? 1 : -1];

#endif
