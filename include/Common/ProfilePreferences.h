// ProfilePreferences.h: a player profile's saved statistics (a UserPreferences whose keys are the stat names).
// Target facts (retail game.dat): the stat accessors read and write the string-keyed map through the virtual
// getInt/setInt/getReal/setReal (slots 6, 11, 5 and 10 of the UserPreferences table); the key is a string literal
// ("Points", "Wins", "LongestGameTime", ...). StatsReporter 0x005BF4BC and ProcessStrategicSinglePlayerGame
// 0x005BFD35 call them on a profile preferences object. ProfilePreferences::loadStatsFile 0x00537D33 sits in the same
// object and StrategicStatsPreferences / RealTimeStatsPreferences derive from the class (deleting destructor 0x00537E60).
// The map names these rows with a placeholder class (?rva0053587C@UserPreferences@@...). The accessors that take a leading
// AsciiString append the stat name to it (AsciiString::concat 0x00005629) and use the result as the key: the callers
// pass an empty or prefixed string; the others build the key from the literal alone.
#pragma once
#ifndef BFME2_PROFILEPREFERENCES_H
#define BFME2_PROFILEPREFERENCES_H

#include "Common/UserPreferences.h"

class ProfilePreferences : public UserPreferences
{
public:
	ProfilePreferences(Int kind);	// 0x00535762 (the argument is unused: callers pass 0 for the real-time and 1 for the strategic statistics)
	virtual ~ProfilePreferences();	// 0x00535776, deleting 0x00537E60 (shared with both derived classes)
	// Slot 13 of the 0x00C68DD8 table is __purecall; the two statistics classes override it.
	virtual void loadProfileStats(const UnicodeString &profilePath) = 0;
	void setPoints(AsciiString key, Int val);	// 0x0053587C
	Int getPoints(AsciiString key);	// 0x005358C3
	Real getTimePlayed(AsciiString key);	// 0x0053590D
	void setLongestGameTime(AsciiString key, Real val);	// 0x0053595E
	Real getLongestGameTime(AsciiString key);	// 0x005359A9
	void setShortestGameTime(AsciiString key, Real val);	// 0x005359FA
	Real getShortestGameTime(AsciiString key);	// 0x00535A45
	void setAverageGameTime(AsciiString key, Real val);	// 0x00535A96
	Real getAverageGameTime(AsciiString key);	// 0x00535AE1
	void setWins(AsciiString key, Int val);	// 0x00535BAF
	Int getWins(AsciiString key);	// 0x00535BF6
	void setLosses(AsciiString key, Int val);	// 0x00535C9D
	Int getLosses(AsciiString key);	// 0x00535CE4
	void setWinStreak(AsciiString key, Int val);	// 0x00535D2E
	Int getWinStreak(AsciiString key);	// 0x00535D75
	void setLossStreak(AsciiString key, Int val);	// 0x00535DBF
	Int getLossStreak(AsciiString key);	// 0x00535E06
	void setBestWinStreak(AsciiString key, Int val);	// 0x00535E50
	Int getBestWinStreak(AsciiString key);	// 0x00535E97
	void setWorstLossStreak(AsciiString key, Int val);	// 0x00535EE1
	Int getWorstLossStreak(AsciiString key);	// 0x00535F28
	void setOverallWinStreak(Int val);	// 0x00535F72
	Int getOverallWinStreak();	// 0x00535FBA
	void setOverallBestWinStreak(Int val);	// 0x00536003
	Int getOverallBestWinStreak();	// 0x0053604B
	void setOverallLossStreak(Int val);	// 0x00536094
	Int getOverallLossStreak();	// 0x005360DC
	void setOverallWorstLossStreak(Int val);	// 0x00536125
	Int getOverallWorstLossStreak();	// 0x0053616D
	void setStructuresCreatedRTS(AsciiString key, Int val);	// 0x0053626B
	Int getStructuresCreatedRTS(AsciiString key);	// 0x005362B2
	void setStructuresLostRTS(AsciiString key, Int val);	// 0x005362FC
	Int getStructuresLostRTS(AsciiString key);	// 0x00536343
	void setStructuresKilledRTS(AsciiString key, Int val);	// 0x0053638D
	Int getStructuresKilledRTS(AsciiString key);	// 0x005363D4
	void setUnitsCreatedRTS(AsciiString key, Int val);	// 0x0053641E
	Int getUnitsCreatedRTS(AsciiString key);	// 0x00536465
	void setUnitsLostRTS(AsciiString key, Int val);	// 0x005364AF
	Int getUnitsLostRTS(AsciiString key);	// 0x005364F6
	void setUnitsKilledRTS(AsciiString key, Int val);	// 0x00536540
	Int getUnitsKilledRTS(AsciiString key);	// 0x00536587
	void setResourcesGatheredRTS(AsciiString key, Int val);	// 0x005365D1
	Int getResourcesGatheredRTS(AsciiString key);	// 0x00536618
	void setResourcesSpentRTS(AsciiString key, Int val);	// 0x00536662
	Int getResourcesSpentRTS(AsciiString key);	// 0x005366A9
	void setHeroesBuiltRTS(AsciiString key, Int val);	// 0x005366F3
	Int getHeroesBuiltRTS(AsciiString key);	// 0x0053673A
	void setHeroesLostRTS(AsciiString key, Int val);	// 0x00536784
	Int getHeroesLostRTS(AsciiString key);	// 0x005367CB
	Int getTurnsPlayed(AsciiString key);	// 0x00536815
	void setLongestGameTurns(AsciiString key, Int val);	// 0x0053685F
	Int getLongestGameTurns(AsciiString key);	// 0x005368A6
	void setShortestGameTurns(AsciiString key, Int val);	// 0x005368F0
	Int getShortestGameTurns(AsciiString key);	// 0x00536937
	void setAverageGameTurns(AsciiString key, Real val);	// 0x00536981
	Real getAverageGameTurns(AsciiString key);	// 0x005369CC
	void setStructuresLostNonRTS(AsciiString key, Int val);	// 0x00536A1D
	Int getStructuresLostNonRTS(AsciiString key);	// 0x00536A64
	void setStructuresKilledNonRTS(AsciiString key, Int val);	// 0x00536AAE
	Int getStructuresKilledNonRTS(AsciiString key);	// 0x00536AF5
	void setUnitsLostNonRTS(AsciiString key, Int val);	// 0x00536B3F
	Int getUnitsLostNonRTS(AsciiString key);	// 0x00536B86
	void setUnitsKilledNonRTS(AsciiString key, Int val);	// 0x00536BD0
	Int getUnitsKilledNonRTS(AsciiString key);	// 0x00536C17
	void setBattlesLostRTS(AsciiString key, Int val);	// 0x00536C61
	Int getBattlesLostRTS(AsciiString key);	// 0x00536CA8
	void setBattlesWonRTS(AsciiString key, Int val);	// 0x00536CF2
	Int getBattlesWonRTS(AsciiString key);	// 0x00536D39
	void setBattlesLostAutoResolve(AsciiString key, Int val);	// 0x00536D83
	Int getBattlesLostAutoResolve(AsciiString key);	// 0x00536DCA
	void setBattlesWonAutoResolve(AsciiString key, Int val);	// 0x00536E14
	Int getBattlesWonAutoResolve(AsciiString key);	// 0x00536E5B
	void setRegionsConquered(AsciiString key, Int val);	// 0x00536EA5
	Int getRegionsConquered(AsciiString key);	// 0x00536EEC
	void setRegionsLost(AsciiString key, Int val);	// 0x00536F36
	Int getRegionsLost(AsciiString key);	// 0x00536F7D
	void setChallenge(Int val);	// 0x00536FC7
	Int getChallenge();	// 0x0053700F
	Int getHonors();	// 0x005370D2
	void setLoyalGames(Int val);	// 0x005372BD
	Int getLoyalGames();	// 0x00537305
	void recordGameTime(AsciiString key, Real gameTime);	// 0x00537A9B (adds to TimePlayed, longest, shortest and the running average)
	void recordGameTurns(AsciiString key, Int turns);	// 0x005378E9 (the same for TurnsPlayed)
	UnicodeString getTimePlayedString(Real seconds);	// 0x00535B32 ("Apt:TimePlayed" with days and hours; `this` unused)
	void addHonors(Int honors);	// 0x00537058 (ORs into "Honors")
	void setKeyedInt(AsciiString key, Int index, Int val);	// 0x0053711B ("key_index")
	Int getKeyedInt(AsciiString key, Int index);	// 0x00537190
	AsciiString getMostWinsOpponent(AsciiString key);	// 0x0053755A
	AsciiString getMostLossesOpponent(AsciiString key);	// 0x00537616
	AsciiString getMostWinsOpponentOverall();	// 0x005376D2
	AsciiString getMostLossesOpponentOverall();	// 0x005377B6
	Int getTotalGames();	// 0x00537C28
	UnicodeString getFavoriteSideText();	// 0x00537C3F ("Side:<favorite>" through TheGameText)
	AsciiString getProfileCreatedDate();	// 0x00535820
	void stampProfileCreatedDate();	// 0x00535781 (stores today's date as ProfileCreatedDate)
	void setFavoriteSide(AsciiString side);	// 0x005361B6
	AsciiString getFavoriteSide();	// 0x0053620F
	void setLastHouse(AsciiString house);	// 0x00537208
	AsciiString getLastHouse();	// 0x00537261
	void setWinsVs(AsciiString key, const AsciiString &opponent, Int val);	// 0x0053740C (key, "WinsVs", opponent)
	Int getWinsVs(AsciiString key, const AsciiString &opponent);	// 0x0053745E
	void setLossesVs(AsciiString key, const AsciiString &opponent, Int val);	// 0x005374B3
	Int getLossesVs(AsciiString key, const AsciiString &opponent);	// 0x00537505
	Int getGamesPlayed(AsciiString key);	// 0x0053734E (wins plus losses)
	Int getTotalWins();	// 0x00535C40 (getWins summed over the six side names)
	Int getTotalLosses();	// 0x005373AF (getLosses summed over the six side names)

protected:
	void loadStatsFile(const UnicodeString &profilePath, const UnicodeString &leafName);	// 0x00537D33
};

#endif
