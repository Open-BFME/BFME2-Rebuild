// LinearCampaignManager.h: the singleton that owns the linear (story) campaigns and the current one.
// Target facts (retail game.dat 1.06; everything below is read from the binary, nothing is borrowed):
//  - Two bases. Snapshot sits at +0x00 (the constructor 0x001ED3C0 stores Snapshot's table 0x00BBB554 first,
//    then constructs SubsystemInterface at +0x04, then stores this class's tables over both): table 0x00BDF260
//    at +0x00 has Snapshot's four slots (0 deleting destructor 0x001ED52D, 1 loadPostProcess = the shared empty
//    body 0x000B3FD0, 2 GetSnapshotName 0x001EAE69, 3 the xfer override 0x001EBB73, which the map spells
//    ?DoXfer@LinearCampaignManager@@QAEXPAVXfer@@@Z). Table 0x00BDF228 at +0x04 is SubsystemInterface's 14 slots:
//    0 the same destructor through an adjustor (0x001ED413), 1 init and 10 update = the empty body, 9 reset =
//    0x001EB722 (forwards to the current campaign when there is one), the rest are SubsystemInterface's defaults.
//  - Destructor 0x001ED41B (UAE): destroys the vector at +0x14 (element type is the map's class Rva001ED0DE,
//    ~vector 0x001ED363), releases the pointer at +0x10 through 0x000AD6F4, then ~SubsystemInterface.
//  - Members: +0x10 the current campaign (a LinearCampaign, constructor 0x001EBAEA (int, int), allocated 0xC4 bytes
//    in DoXfer; DoXfer first transfers "has a current campaign" = +0x10 != 0), +0x14 an STLport vector built by
//    0x00211E58. SIZE 0x20 (+0x14 plus a three-pointer vector).
//  - NOT proven here: the element type of the vector, the owning-pointer class behind +0x10, the global the game
//    keeps the instance in (the map has no name for it; AptMainMenu::LinearCampaignStart 0x00514D05 reads it).
#pragma once
#ifndef BFME2_LINEARCAMPAIGNMANAGER_H
#define BFME2_LINEARCAMPAIGNMANAGER_H

#include "Common/GameType.h"
#include "Common/Snapshot.h"
#include "Common/SubsystemInterface.h"

class Xfer;
class LinearCampaign;

class LinearCampaignManager : public Snapshot, public SubsystemInterface
{
public:
	LinearCampaignManager();										// 0x001ED3C0
	virtual ~LinearCampaignManager();								// 0x001ED41B; deleting 0x001ED52D

	void rva001ECEF6();												// 0x001ECEF6 (AptCampaignMenu::OnBttnLastMission 0x0052119F calls it with the current campaign)

	virtual void init() {}											// SubsystemInterface slot 1: the shared empty body
	virtual void reset();											// SubsystemInterface slot 9, 0x001EB722
	virtual void update() {}										// SubsystemInterface slot 10: the shared empty body

protected:
	virtual void loadPostProcess() {}								// Snapshot slot 1: the shared empty body
	virtual const char *GetSnapshotName() const;					// Snapshot slot 2, 0x001EAE69
	virtual void xfer(Xfer *xfer);									// Snapshot slot 3, 0x001EBB73 (the map's DoXfer)

private:
	LinearCampaign *m_currentCampaign;								// +0x10 (not proven: the owning-pointer class)
	unsigned char m_campaigns[0xC];									// +0x14 an STLport vector of the map's Rva001ED0DE (0x00211E58 constructs it)
};
// SIZE 0x20

extern LinearCampaignManager *TheLinearCampaignManager;				// VA 0x00DFDC8C (the map's name; AptMainMenu::LinearCampaignStart 0x00514D05 and the campaign menu read it)

#endif
