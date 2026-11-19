// AptCampaignReview.h: the campaign review screen ("CampaignReview.apt"). It adds no members to _bfme_AptGameWindow.
//
// Target facts (retail game.dat, RVAs; table addresses are VAs): constructor 0x00512AC8 (one argument for the base constructor
// 0x0051268C), destructor 0x00512894; SIZE 0x27C (screen factory 0x002D1E8F allocates exactly the base's size). The screen instance is the
// global the map names g_obj12F495C (VA 0x00E048C8, typed void*).
#pragma once
#ifndef BFME2_APTCAMPAIGNREVIEW_H
#define BFME2_APTCAMPAIGNREVIEW_H

#include "GameClient/BfmeAptGameWindow.h"

class AptCampaignReview : public _bfme_AptGameWindow
{
public:
	AptCampaignReview(void *);						// 0x00512AC8
	virtual ~AptCampaignReview();					// 0x00512894

	void Continue(const char *);					// 0x00512823
	void TotalCampaignScore(int, char *, bool);		// 0x00512838: extern handler, prints the campaign score with "%d"
	void playerSideType(int, char *, bool);			// 0x00512864: extern handler, "evil" or "good"
	void DisplayVictoryLevel();						// 0x00512948: sets the APT:CmpgnRevResult text
	virtual int rva005127F3(int message, unsigned int data1, unsigned int data2);	// 0x005127F3: the screen's message handler
	static bool OpenScreen();						// 0x0051291C
};
typedef char AptCampaignReviewSizeCheck[sizeof(AptCampaignReview)==0x27C ? 1 : -1];

#endif
