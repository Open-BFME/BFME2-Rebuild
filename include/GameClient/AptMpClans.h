// Shared clans panel. Retail constructor 0x57F486 installs table VA 0x86F584
// after AptCallbackHost and builds the GameSpy login profile at +0x58.
#pragma once

#include <stddef.h>
#include "Common/AsciiString.h"
#include "Common/GameSpyLoginPreferences.h"
#include "Common/UnicodeString.h"
#include "GameClient/AptCallbackHost.h"

class AptRefTarget;
class GameWindow;

class AptMpClans : public AptCallbackHost
{
public:
	explicit AptMpClans(AptRefTarget *target);
	virtual ~AptMpClans();

	virtual void onClanChanged() = 0;
	virtual void *getOwnerId() = 0;

	void WebSite(const char *name);
	void OnGameInfoChange();
	void PopulateMyClans(const UnicodeString &selected);
	void Delete(const char *name);
	void InitGadgets(const char *name, void *argument, GameWindow *window);
	void rva0057FAB0();

	GameSpyLoginPreferences m_preferences;
	AptRefTarget *m_target;  // +0x9C, reference released by 0x57F2DE
	GameWindow *m_clanCombo;
	GameWindow *m_playersList;
	bool m_unk0xA8;
	AsciiString m_loginName;  // +0xAC, passed as the key by the clan actions
	UnicodeString m_unk0xB0;
};

// AptMpGameSetup builds this specialization in its +0x190 member.
// Retail 0x43DAC8 installs VA 0x83D95C over the common AptMpClans base.
class AptMpSetupClans : public AptMpClans
{
public:
	explicit AptMpSetupClans(AptRefTarget *target);
	virtual ~AptMpSetupClans();
	virtual void onClanChanged();
	virtual void *getOwnerId();
};

typedef char AptMpClansSizeCheck[sizeof(AptMpClans) == 0xB4 ? 1 : -1];
typedef char AptMpSetupClansSizeCheck[sizeof(AptMpSetupClans) == 0xB4 ? 1 : -1];
typedef char AptMpClansTargetOffset[offsetof(AptMpClans, m_target) == 0x9C ? 1 : -1];
typedef char AptMpClansComboOffset[offsetof(AptMpClans, m_clanCombo) == 0xA0 ? 1 : -1];
