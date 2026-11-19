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
	virtual ~OptionPreferences();								// 0x002E4272

	int Rva002E42AFDispatch(int index);							// 0x002E42AF: the enum table row at index
	Int getStaticGameDetail();									// 0x002E427D: "StaticGameLOD" against the six static level names
	Int getIdealStaticGameDetail();								// 0x002E429B: "IdealStaticGameLOD", -1 when unset
	void setStaticGameDetail(Int level);						// 0x002E52FC: -1 erases the key
	void setIdealStaticGameDetail(Int level);					// 0x002E537B
	void setAudioLOD(Int level);								// 0x002E53FA: stores the level's name under "AudioLOD"
	void eraseSettings();										// 0x002E54EA: erases the key of every LOD enum table row
	int getModelLOD();											// 0x002E42EE: row 0
	int getAnimationLOD();										// 0x002E42F6: row 1
	int getEffectsLOD();										// 0x002E42FE: row 2
	int getShadowLOD();											// 0x002E4306: row 3
	int getTerrainLOD();										// 0x002E430E: row 4
	int getWaterLOD();											// 0x002E4316: row 5
	int Rva002E431EForward();									// 0x002E431E: row 6
	int Rva002E4326Forward();									// 0x002E4326: row 7
	int Rva002E432EForward();									// 0x002E432E: row 8

	bool usesSystemMapDir();									// 0x002E4653: the system map directory flag
	bool saveCameraInReplays();									// 0x002E46AF
	bool useCameraInReplays();									// 0x002E470B
	bool hasSeenLogoMovies();									// 0x002E498F
	bool hasGotOnline();										// 0x002E49EB
	int getFirewallBehavior();										// 0x002E4815
	Short getFirewallPortAllocationDelta();					// 0x002E4874
	UnsignedShort getFirewallPortOverride();					// 0x002E48CE
	float getBrightness();									// 0x002E4A47
	float getScrollFactor();										// 0x002E45DA
	void getResolution(int *xres, int *yres);					// 0x002E4AA1
	void rva002E43A6(int val);										// 0x002E43A6: FirewallBehavior
	void rva002E43EE(short val);									// 0x002E43EE: FirewallPortAllocationDelta
	void rva002E4438(unsigned short val);						// 0x002E4438: FirewallPortOverride
	void setLANIPAddress(unsigned int ip);							// 0x002E50BB
	void setOnlineIPAddress(unsigned int ip);					// 0x002E526D
	void setLANIPAddress(AsciiString IP);							// 0x002E505A
	void setOnlineIPAddress(AsciiString IP);					// 0x002E520C
	UnsignedInt getLANIPAddress();								// 0x002E4F98
	bool getAllHealthBars();									// 0x002E4CF3
	bool getUseEAX3();										// 0x002E4E76
	void setSetting(Int setting, Int value);					// 0x002E553D
	Int getCampaignDifficulty();								// 0x002E4573: 0..2
	Int getAudioLOD();											// 0x002E4767: -1 when unset
	void setCampaignDifficulty(Int difficulty);					// 0x002E4F1F
	void setVolume(Int index, Real volume);						// 0x002E545A: index 0..4 into g_volumePreferenceKeys
	bool getSendDelay();										// 0x002E47B0
	bool getFirewallNeedToRefresh();							// 0x002E4928
	Real getVolume(Int index);									// 0x002E4B2D: index 0..4 into g_volumePreferenceKeys
	bool getLanguageFilter();									// 0x002E4BC7
	bool getTurnOffMessengerInGame();							// 0x002E4C2B
	bool getDoubleClickGuard();									// 0x002E4C8F
	bool getAlternateMouseSetup();								// 0x002E4D4E
	bool getRefreshNet();										// 0x002E4DAE
	bool getDisplayForeignLanguage();							// 0x002E4E12
};

#endif
