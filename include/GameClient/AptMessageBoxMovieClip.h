// AptMessageBoxMovieClip.h: the message box's Apt movie clip and its implementation object.
//
// Target facts (retail game.dat, RVAs): the Impl object keeps the movie state and two callback slots: the Apt
// callbacks OnButtonOk/Cancel/Yes/No (0x0054C898..0x0054C8EE) store the result 0 and state 5 and tell the slot at +0x1C
// which button was pressed (0 Ok, 1 Cancel, 2 Yes, 3 No); OnShowing/OnShown/OnHiding (0x0054C90B..0x0054C92F) tell the
// slot at +0x20 the phase (0, 1, 2). A slot is one pointer (the callback target); a slot whose target is null is skipped.
// Retail's invoke (0x0057CC15) is shared with the map preview's tail code. The Impl's size and the members below +0x14
// are not recovered here (its constructor 0x0054CFB8..0x0054D12A builds it from the movie).
#pragma once
#ifndef BFME2_APTMESSAGEBOXMOVIECLIP_H
#define BFME2_APTMESSAGEBOXMOVIECLIP_H

class AptMessageBoxMovieClip
{
public:
	// One callback slot: the target pointer and its invoke (0x0057CC15).
	class CallbackSlot
	{
	public:
		void invoke(int);										// 0x0057CC15
		void *m_target;											// +0x00
	};

	class Impl
	{
	public:
		void OnButtonOk(const char *);							// 0x0054C898
		void OnButtonCancel(const char *);						// 0x0054C8B4
		void OnButtonYes(const char *);							// 0x0054C8D1
		void OnButtonNo(const char *);							// 0x0054C8EE
		void OnShowing(const char *);							// 0x0054C90B
		void OnShown(const char *);								// 0x0054C91D
		void OnHiding(const char *);							// 0x0054C92F

		unsigned char m_pad00[0x14];							// +0x00..+0x13 (not recovered)
		int m_result;											// +0x14 (0 after a button)
		int m_state;											// +0x18 (5 after a button)
		CallbackSlot m_buttonSlot;								// +0x1C
		CallbackSlot m_phaseSlot;								// +0x20
	};
};

#endif
