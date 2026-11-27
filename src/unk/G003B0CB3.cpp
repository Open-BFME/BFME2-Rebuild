// cl: /O1
// Portions derived from Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
#include "Common/Money.h"
#include "Common/GameAudio.h"
#include "Common/MiscAudio.h"
#include "Common/AudioEventRTS.h"
#include "Common/Player.h"
#include "Common/PlayerList.h"
#include "Common/ScoreKeeper.h"
#include "Common/StatsCollector.h"
// Generated from the ledger; map snapshot c591f7146f88c8a8248ebb8ae22f0c978c22aed463db64e05ef57ff0dfae99cc
// object=G003B0CB3 name-evidence=invented
// Extents and edges are hypotheses; byte verification alone does not establish original TU boundaries.
// FUNCTION: BFME2 0x003B0CB3 size=201 name=?withdraw@Money@@QAEIIPAVScoreKeeper@@_N@Z evidence=X:map request (link (from standalone link 0cf3e833)) tier=B hypothesis:mm1-gap-unit
unsigned int Money::withdraw(unsigned int amount, ScoreKeeper *tracker, bool playSound)
{
	if (amount > m_money)
		amount = m_money;

	if (amount == 0)
		return amount;

	if (playSound)
	{
		AudioEventRTS event(TheAudio->getMiscAudio()->m_moneyWithdrawSound, 0);
		event.setPlayerIndex(m_playerIndex);
		TheAudio->addAudioEvent(&event);
	}

	m_money -= amount;

	if (TheStatsCollector && ThePlayerList && ThePlayerList->m_local && ThePlayerList->m_local->getPlayerIndex() == m_playerIndex)
		TheStatsCollector->m_moneyWithdrawn += amount;

	if (tracker)
		tracker->rva0039B795(amount);

	return amount;
}
// FUNCTION: BFME2 0x003B0D7C size=188 name=?deposit@Money@@QAEXHPAVScoreKeeper@@_N@Z evidence=X:map request (link (from standalone link 0cf3e833)) tier=B hypothesis:mm1-gap-unit
void Money::deposit(int amount, ScoreKeeper *tracker, bool playSound)
{
	if (amount == 0)
		return;

	if (playSound)
	{
		AudioEventRTS event(TheAudio->getMiscAudio()->m_moneyDepositSound, 0);
		event.setPlayerIndex(m_playerIndex);
		TheAudio->addAudioEvent(&event);
	}

	m_money += amount;

	if (TheStatsCollector && ThePlayerList && ThePlayerList->m_local && ThePlayerList->m_local->getPlayerIndex() == m_playerIndex)
		TheStatsCollector->m_moneyDeposited += amount;

	if (tracker)
		tracker->rva0039B7AD(amount);
}
// FUNCTION: BFME2 0x003B0E57 size=6 name=?Rva003B0E57Get@@YAHXZ evidence=?:placeholder tier=B hypothesis:mm1-gap-unit
// FUNCTION: BFME2 0x003B0E75 size=14 name=?rva003B0E75@Rva003B0E75@@QAEXXZ evidence=?:placeholder tier=B hypothesis:mm1-gap-unit
// FUNCTION: BFME2 0x003B0E83 size=16 name=?rva003B0E83@Rva003B0E83@@QAEPAV1@XZ evidence=?:placeholder tier=B hypothesis:mm1-gap-unit
// FUNCTION: BFME2 0x003B0E93 size=15 name=?Rva003B0E93@@YAHPBURva003B0E93Record@@@Z evidence=?:placeholder tier=B hypothesis:mm1-gap-unit
