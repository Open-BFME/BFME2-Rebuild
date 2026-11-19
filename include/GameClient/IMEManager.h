// IMEManager.h: the input-method (IME) manager the text-entry gadgets talk to.
// IMEManagerInterface is the abstract surface (a SubsystemInterface plus eighteen
// virtuals); IMEManager is the one implementation.
//
// Evidence (retail game.dat): the interface's constructor 0x00232DA2 builds a
// SubsystemInterface and stores table 0x007E8238 (size 0xC: it does not touch +0x0C, which
// the IMEManager constructor clears, so m_result is IMEManager's) (32 slots: the 14 inherited
// ones then the eighteen below; its destructor 0x00232D7B restores it). IMEManager's
// constructor 0x0023309A stores table 0x007E82B8 (the same 32 slots) and clears
// +0x0C..+0x20, the two 2049-wide-character buffers at +0x22 and +0x1024 (one loop of
// 0x801 words; a third buffer of the same size sits between them and +0x3028), and
// +0x3028..+0x3060, with 1 at +0x3030. Its size is 0x3064. init
// 0x002336CA creates the candidate window and its four children (+0x3050..+0x3060),
// the status window (+0x3054) and the IME context (+0x14/+0x18); the getters pin the
// candidate counters (+0x3034..+0x3040). Slot 0x7C is the folded `mov eax,[ecx+0xC]`
// (0x001DB0A8), slot 0x50 the folded `mov eax,[ecx+0x10]` (0x001DB09D). Names are Zero
// Hour's IMEManager (donor); retail spells detatch with the same typo.
#pragma once
#ifndef BFME2_IMEMANAGER_H
#define BFME2_IMEMANAGER_H

#include "Common/SubsystemInterface.h"
#include "Common/UnicodeString.h"

class GameWindow;

class IMEManagerInterface : public SubsystemInterface
{
public:
	// No user-declared constructor: the implicit one is inline, and IMEManager's constructor calls SubsystemInterface's 0x001B4E63 directly.
	virtual ~IMEManagerInterface() {}														// out-of-line copy 0x00232D7B

	virtual void attach(GameWindow *window) = 0;											// 0x38
	virtual void detatch() = 0;																// 0x3C
	virtual void enable() = 0;																// 0x40
	virtual void disable() = 0;																// 0x44
	virtual bool isEnabled() = 0;															// 0x48
	virtual bool isAttachedTo(GameWindow *window) = 0;										// 0x4C
	virtual GameWindow *getWindow() = 0;													// 0x50
	virtual bool isComposing() = 0;															// 0x54
	virtual void getCompositionString(UnicodeString &string) = 0;							// 0x58
	virtual int getCompositionCursorPosition() = 0;											// 0x5C
	virtual int getIndexBase() = 0;															// 0x60
	virtual int getCandidateCount() = 0;													// 0x64
	virtual UnicodeString *getCandidate(int index) = 0;										// 0x68
	virtual int getSelectedCandidateIndex() = 0;											// 0x6C
	virtual int getCandidatePageSize() = 0;													// 0x70
	virtual int getCandidatePageStart() = 0;												// 0x74
	virtual bool serviceIMEMessage(void *windowsHandle, unsigned int message, int wParam, int lParam) = 0;	// 0x78
	virtual int result() = 0;																// 0x7C
};
// SIZE 0x0C

class IMEManager : public IMEManagerInterface
{
public:
	IMEManager();																			// 0x0023309A
	virtual ~IMEManager();																	// 0x0023361E

	virtual void init();																	// 0x002336CA
	virtual void reset() {}																// slot 9: retail table 0x007E82B8 holds the folded empty body 0x000B3FD0 here
	virtual void update() {}																// slot 10: same
	virtual void attach(GameWindow *window);												// 0x00233138
	virtual void detatch();																	// 0x00232DB4
	virtual void enable();																	// 0x00232E01
	virtual void disable();																	// 0x00232E1E
	virtual bool isEnabled();																// 0x00232E2F
	virtual bool isAttachedTo(GameWindow *window) { return m_window == window; }			// folded 0x0056A9CF
	virtual GameWindow *getWindow() { return m_window; }									// folded 0x001DB09D
	virtual bool isComposing() { return m_composing; }										// folded 0x00232E8F
	virtual void getCompositionString(UnicodeString &string);								// 0x00233186
	virtual int getCompositionCursorPosition();												// 0x00232E93
	virtual int getIndexBase();																// 0x00232F7D
	virtual int getCandidateCount();														// 0x0023303C
	virtual UnicodeString *getCandidate(int index);											// 0x002339AA
	virtual int getSelectedCandidateIndex();												// 0x00233043
	virtual int getCandidatePageSize();														// 0x0023304A
	virtual int getCandidatePageStart();													// 0x00233051
	virtual bool serviceIMEMessage(void *windowsHandle, unsigned int message, int wParam, int lParam);	// 0x00233C0B
	virtual int result() { return m_result; }												// folded 0x001DB0A8


protected:
	void openStatusWindow();																// 0x00233058
	void closeStatusWindow();																// 0x0023306A
	unsigned short convertCharToWide(unsigned int charCode, unsigned int codePage);		// 0x00232E3D
	void updateCompositionString();															// 0x00232E9A
	void getResultsString();																// 0x00232F0C
	void updateProperties();																// 0x00232F4F
	void resizeCandidateWindow(int pageSize);												// 0x00232F84
	void convertToUnicode(char *source, UnicodeString &destination);						// 0x0023323B
	void closeCandidateList(int candidateFlags);											// 0x002332B2
	void updateCandidateList(int candidateFlags);											// 0x002332FB
	void openCandidateList(int candidateFlags);												// 0x002338B4

	int m_result;																			// +0x0C
	GameWindow *m_window;																	// +0x10
	void *m_context;																		// +0x14 (HIMC)
	void *m_oldContext;																		// +0x18 (HIMC)
	int m_disabled;																			// +0x1C
	bool m_composing;																		// +0x20
	unsigned char m_pad21;
	unsigned short m_compositionString[2049];												// +0x22
	unsigned short m_resultsString[2049];													// +0x1024
	unsigned short m_unk0x2026[2049];														// +0x2026, a third wide buffer the constructor does not clear
	int m_compositionCursorPos;																// +0x3028
	int m_compositionStringLength;															// +0x302C
	int m_indexBase;																		// +0x3030 (1 after construction)
	int m_candidatePageStart;																// +0x3034
	int m_candidatePageSize;																// +0x3038
	int m_selectedIndex;																	// +0x303C
	int m_candidateCount;																	// +0x3040
	UnicodeString *m_candidateString;														// +0x3044
	bool m_unicodeIME;																		// +0x3048
	unsigned char m_pad3049[3];
	int m_compositionCharsDisplayed;																		// +0x304C, cleared by the constructor
	GameWindow *m_candidateWindow;															// +0x3050
	GameWindow *m_statusWindow;																// +0x3054
	GameWindow *m_candidateTextArea;														// +0x3058
	GameWindow *m_candidateUpArrow;															// +0x305C
	GameWindow *m_candidateDownArrow;														// +0x3060
};
// SIZE 0x3064

extern IMEManager *TheIMEManager;

#endif
