// OptionPreferences.h: the option settings of the game, a UserPreferences with
// no members of its own. SIZE 0x14.
//
// Target facts: the constructor 0x002E434E calls UserPreferences' constructor and
// stores its own vtable (VA 0x00C04CE8) at +0; the deleting destructor 0x002E42D2
// calls 0x002E4272. The vtable's slots 1 to 12 are UserPreferences' own targets.
// Only the accessors this unit defines are declared here; the rest of the
// option getters and setters are rowed in their own units.
#pragma once
#ifndef BFME2_OPTIONPREFERENCES_H
#define BFME2_OPTIONPREFERENCES_H

#include "Common/UserPreferences.h"

// One row of the enum table the LOD settings index: a setting key, its name list
// and the list's length. SIZE 0xC. The table itself is data at VA 0x00DBD120.
struct BfmeEnumTableEntry
{
	const char *m_key;
	const char **m_subtable;
	Int m_count;
};

class OptionPreferences : public UserPreferences
{
public:
	OptionPreferences();										// 0x002E434E
	virtual ~OptionPreferences();

	int Rva002E42AFDispatch(int index);							// The enum table row at index
	Int getStaticGameDetail();									// "StaticGameLOD" against the six static level names
	Int getIdealStaticGameDetail();								// "IdealStaticGameLOD", -1 when unset
	void setStaticGameDetail(Int level);						// 0x002E52FC: -1 erases the key
	void setIdealStaticGameDetail(Int level);					// 0x002E537B
	void setAudioLOD(Int level);								// 0x002E53FA: stores the level's name under "AudioLOD"
	void eraseSettings();										// 0x002E54EA: erases the key of every LOD enum table row
	int getModelLOD();											// Row 0
	int getAnimationLOD();										// Row 1
	int getEffectsLOD();										// Row 2
	int getShadowLOD();											// Row 3
	int getTerrainLOD();										// Row 4
	int getWaterLOD();											// Row 5
	int Rva002E431EForward();									// Row 6
	int Rva002E4326Forward();									// Row 7
	int Rva002E432EForward();									// Row 8

	bool usesSystemMapDir();									// The system map directory flag
	bool saveCameraInReplays();
	bool useCameraInReplays();
	bool hasSeenLogoMovies();
	bool hasGotOnline();
	int getFirewallBehavior();
	Short getFirewallPortAllocationDelta();
	UnsignedShort getFirewallPortOverride();
	float getBrightness();
	float getScrollFactor();
	void getResolution(int *xres, int *yres);
	void rva002E43A6(int val);										// FirewallBehavior
	void rva002E43EE(short val);									// FirewallPortAllocationDelta
	void rva002E4438(unsigned short val);						// FirewallPortOverride
	void setLANIPAddress(unsigned int ip);							// 0x002E50BB
	void setOnlineIPAddress(unsigned int ip);					// 0x002E526D
	void setLANIPAddress(AsciiString IP);							// 0x002E505A
	void setOnlineIPAddress(AsciiString IP);					// 0x002E520C
	UnsignedInt getLANIPAddress();
	UnsignedInt getOnlineIPAddress();
	bool getAllHealthBars();
	bool getUseEAX3();
	void setSetting(Int setting, Int value);
	Int getCampaignDifficulty();
	Int getAudioLOD();											// -1 when unset
	void setCampaignDifficulty(Int difficulty);
	void setVolume(Int index, Real volume);						// Index 0..4 into g_volumePreferenceKeys
	bool getSendDelay();
	bool getFirewallNeedToRefresh();
	Real getVolume(Int index);									// Index 0..4 into g_volumePreferenceKeys
	bool getLanguageFilter();
	bool getTurnOffMessengerInGame();
	bool getDoubleClickGuard();
	bool getAlternateMouseSetup();
	bool getRefreshNet();
	bool getDisplayForeignLanguage();
};

#endif
