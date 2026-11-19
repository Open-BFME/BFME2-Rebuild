// Target4376FA/4376BB: AsciiString+0, integral counters4..54,
// scrolling flag58, time count5C, last update60, start frame64.
// Member names follow ZH StatsCollector.h where the target accesses agree.
#pragma once
#include "Common/GameType.h"
#include "Common/AsciiString.h"
class GameMessage;
class StatsCollector
{
public:
	StatsCollector();
	void reset();	// 0x00437DF8 (Zero Hour's StatsCollector::reset: the stats directory, the file name, the initial info, a first collection, then zeroOutStats)
	void update();
	void collectUnitCountStats();
	void collectScoreKeeperStats();
	void collectMsgStats(const GameMessage *msg);
	void incrementMoveCount() { m_moveCommands++; }		// inline in retail: `inc [eax+0x10]` (CommandTranslator::issueMoveToLocationCommand 0x004292F6)
	void incrementAttackCount() { m_attackCommands++; }	// inline in retail: `inc [eax+0x14]` (issueAttackCommand 0x004297F0)
	void startScrollTime();
	void endScrollTime();
	void writeFileEnd();

private:
	void createFileName();
	void writeStatInfo();
	void writeInitialFileInfo();
	void zeroOutStats();

	AsciiString m_statsFileName;
	UnsignedInt m_moneyWithdrawn;
	UnsignedInt m_moneyDeposited;
	UnsignedInt m_buildCommands;
	UnsignedInt m_moveCommands;
	UnsignedInt m_attackCommands;
	UnsignedInt m_scrollMapCommands;
	UnsignedInt m_aiUnits;
	UnsignedInt m_playerUnits;
	UnsignedInt m_alliesKilled;
	UnsignedInt m_neutralsKilled;
	UnsignedInt m_enemiesKilled;
	UnsignedInt m_scoreKeeperMoneySpent;
	UnsignedInt m_scoreKeeperMoneyEarned;
	UnsignedInt m_scoreKeeperUnitsDestroyed;
	UnsignedInt m_scoreKeeperUnitsBuilt;
	UnsignedInt m_scoreKeeperUnitsLost;
	UnsignedInt m_scoreKeeperBuildingsDestroyed;
	UnsignedInt m_scoreKeeperBuildingsBuilt;
	UnsignedInt m_scoreKeeperBuildingsLost;
	UnsignedInt m_scrollBeginTime;
	UnsignedInt m_scrollTime;
	bool m_isScrolling;
	Int m_timeCount;
	Int m_lastUpdate;
	Int m_startFrame;
};


// The global statistics object (VA 0x00E032F8; the map has no name for it, role name from the class).
extern StatsCollector *TheStatsCollector;

typedef char StatsCollectorSizeCheck[sizeof(StatsCollector)==0x68 ? 1 : -1];

