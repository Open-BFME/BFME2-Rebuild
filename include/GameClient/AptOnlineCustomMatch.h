// AptOnlineCustomMatch: the GameSpy custom-match screen (retail constructor 0x005A5BC7, SIZE 0x4E0).
// Target facts (retail game.dat; RVAs, table addresses are VAs):
//  - Primary table VA 0x00871414, 11 slots: 0 deleting destructor (0x005A0B62), 1 the shared stub 0x000B3FD0, 2 InitGadgets
//    0x0059EFA0, 3 Update 0x005A61C6, 4 0x005A6582, 5 GameWindowSystem 0x005A52FE, 6 0x005A2DB7 (ProcessGameSpyResponses, which the
//    map spells non-virtual), 7 0x0050B5C6 (inherited), 8 0x0059EF49, 9 0x005A6697, 10 __purecall: the Apt movie prefix a
//    derived screen supplies (every `AptCall` of this class passes `slot10()` as the Apt path prefix).
//  - Layout (constructor 0x005A5BC7): the Apt screen base (constructor 0x0056DC4C: AptCallbackHost's 0x58 bytes, then the owner
//    screen pointer at +0x58 and a zeroed word at +0x5C), the MpGameSetupOwner subobject at +0x60 (constructor 0x004444AE, table VA
//    0x008713B8), a listener vptr at +0x6C (table VA 0x008713AC), the embedded AptMpGameSetup panel at +0x70 (0x3E0 bytes, mode
//    0x200001F0), a GameSorter at +0x450 (0x1C bytes) and a preferences object at +0x46C (0x1C bytes, ctor 0x0054F52F), then the
//    members below. The constructor leaves +0x494 untouched (InitGadgets stores the GameInfo list there).
//  - `m_screen->m_callbackOwner` (+0x274 of the owner screen) is the Apt level every `AptCall` is sent to; the registered Apt
//    callbacks are AptOnline::CustomMatch::{CreateGame 0x0059EC72, JoinGame 0x0059ECAD, LoadGame 0x0059ECC1, CancelPopUpJoin
//    0x0059ECD2, CancelPopUpCreate 0x0059ED18, CancelPopUpHost 0x0059ED2C, ...}.
#pragma once
#include <stddef.h>
#include "GameClient/BfmeAptGameWindow.h"
#include "GameClient/AptMpGameSetup.h"
#include "GameClient/AptCallbackHost.h"
#include "GameClient/AptOnlineScreen.h"
#include "GameClient/MpGameSetupOwner.h"
#include "GameNetwork/GameSlot.h"
#include "GameNetwork/PortNegotiationListener.h"
#include "Common/AsciiString.h"
#include "Common/GameModePreferences.h"
#include "GameClient/GameSorter.h"
class AptConnectionScreen;
class GameWindow;
class GameSpyStagingRoom;
class AptOnlineCustomMatch : public AptOnlineScreen, public MpGameSetupOwner, public PortNegotiationListener {
public:
  virtual ~AptOnlineCustomMatch(); // slot 0, deleting destructor 0x005A0B62 over 0x005A0009
  virtual void InitGadgets(); // slot 2, 0x0059EFA0
  virtual void Update(); // slot 3, 0x005A61C6
  virtual int rva005A6582(int, int, unsigned char, int); // slot 4
  virtual int GameWindowSystem(GameWindow *, unsigned int, unsigned int, unsigned int); // slot 5, 0x005A52FE
  virtual void slot6(); // 0x005A2DB7 (the map's ProcessGameSpyResponses, spelled non-virtual)
  virtual bool rva0059EF49(); // slot 8: the setup panel is not busy starting
  virtual bool rva005A6697(); // slot 9: the idle step of the state machine
  virtual const char *slot10() = 0; // __purecall: the Apt movie prefix
  virtual void onNegotiationChanged(void *schema, int row, int column);
  virtual void onPairChanged(void *schema, int row, int column);
  void UpdatePings();

  // The MpGameSetupOwner overrides (table VA 0x008713B8).
  virtual bool MpOwnerOnReadyChecked(bool); // slot 3, 0x005A1DCE
  virtual bool MpOwnerSelectColor(GameSlot *, int); // slot 4, 0x005A0F4B
  virtual bool MpOwnerSelectHandicap(GameSlot *, int); // slot 5, 0x005A152F
  virtual bool MpOwnerSelectHero(GameSlot *); // slot 6, 0x005A1281
  virtual bool MpOwnerSelectMap(const AsciiString &); // slot 7, 0x005A0B7E
  virtual bool MpOwnerSelectPlayer(GameSlot *, SlotState, const UnicodeString &); // slot 9, 0x005A168D
  virtual bool MpOwnerSelectPlayerTemplate(GameSlot *, int); // slot 10, 0x005A10DA
  virtual bool MpOwnerSelectStartPosition(GameSlot *, int); // slot 11, 0x005A186B
  virtual bool MpOwnerSelectTeam(GameSlot *, int); // slot 12, 0x005A13D8
  virtual bool MpOwnerSetClanID(GameSlot *, const UnicodeString &); // slot 13, 0x005A19C0
  virtual void MpOwnerGetLocalPlayerName(UnicodeString &); // slot 14, 0x0059EBAF
  virtual void MpOwnerPrintMessage(const UnicodeString &, int); // slot 17, 0x0059F043
  virtual void enableSetupUI(); // slot 15, 0x005A1D32 (the map holds it as rva005A1D32)
  virtual void setBusy(bool); // slot 16, 0x005A1C87 (the map holds it as rva005A1C87)
  void rva0059ECAD(int); // State 2 moves on to 9
  unsigned char rva0059EF33() const; // Whether the state is 6 or 0xC
  void rva0059EE87();
  void resetState(); // State back to 0
  void OpenConnectionScreen(bool);
  void OnOpenCreateDialog(const char *);
  void rva0059EC72(int);
  void InitGadgets(const char *, int, GameWindow *);
  void rva005A083C();
  static void GamesListTooltipFunc(GameWindow *, class WinInstanceData *, unsigned int);
  void FillGameNameTextEntry(bool);
  void Refresh(const char *); // rva005A0D61(true)
  void PlayGame(const char *); // 0x0059EC65: the Apt callback that moves the lobby to state 7
  void LoadGame(const char *); // Opens the load screen for the multiplayer saves (kind 2, filter 0x10)
  void OnOpenConnectionsScreen(const char *);
  void OnClosingConnectionsScreen(const char *);
  void CancelPopUpCreate(const char *);
  void rva0052493F(); // (the map holds it as a method of a placeholder class)
  void CancelPopUpJoin(int); // 0x0059ECD2: closes the password pop-up of the join dialog
  void CancelPopUpHost(const char *); // Closes the password pop-up of the host dialog
  void setPlayButtonEnabled(unsigned char enabled, unsigned char force); // 0x0059EE96: tells the movie to enable or disable the play button
  void OnBttnCancel(int);
  void saveSlotPreferences(); // 0x0059F950: stores the local slot's color, faction and start position and the map in the preferences object
  void UpdateHostStartGameButton(); // The host may start once two humans sit in the room
  GameSpyStagingRoom *GetGameToJoin();
  void AcceptJoinGame(const char *); // The join dialog's accept button
  bool RequestJoinGame(const char *password); // Asks the peer thread to join the selected room
  void rva005A5BAF();
  void PopulateGamesListBox();
  void PopulateLobbyComboBox(); // Refills the "Lobbies" combo box with the group rooms of room type 1
  void rva005A0D61(bool force); // Refreshes the games list when it is due (force, the second since the last, a sort or a room list change)
  bool rva005A57BA(bool);
  bool rva005A5B38(); // 0x005A5B38
  AptMpGameSetup m_setup; // +0x70 (0x3E0 bytes)
  GameSorter m_sorter; // +0x450 the games list's sort state
  GameModePreferences m_prefs; // +0x46C the custom-match preferences (a GameModePreferences, 0x1C bytes; ctor 0x0054F52F)
  int m_state; // +0x488 (CancelPopUpCreate 0x0059ED18 sets 1; rva005A5BAF tests 0xD; rva005A6697 switches on it)
  GameWindow *m_gameList; // +0x48C the "GameList" list box (InitGadgets 0x005A0C6E)
  GameWindow *m_lobbies; // +0x490 the "Lobbies" window
  GameWindow *m_gameInfoList; // +0x494 the "GameInfo" list box
  GameWindow *m_gameNameEntry; // +0x498, GameName gadget
  GameWindow *m_gamePasswordEntry; // +0x49C, GamePassword gadget
  bool m_x4A0; // +0x4A0 cleared by CancelPopUpCreate and rva005A5BAF
  unsigned char m_pad0x4A1[3]; // +0x4A1
  int m_x4A4; // +0x4A4
  int m_x4A8; // +0x4A8 set to -1 by rva0059EE87 0x0059EE87
  unsigned int m_lastRefreshTime; // +0x4AC (rva005A0D61 0x005A0D61 stamps timeGetTime)
  unsigned int m_lastPingUpdateTime; // +0x4B0: timeGetTime, checked for a 10 s timeout
  bool m_x4B4; // +0x4B4
  unsigned char m_pad0x4B5[3]; // +0x4B5
  int m_gameToJoinID; // +0x4B8 (GetGameToJoin 0x005A0808 looks the staging room up by it)
  int m_connectingPopupCount; // +0x4BC
  bool m_x4C0; // +0x4C0
  unsigned char m_playButtonEnabled; // +0x4C1 (1 at construction; setPlayButtonEnabled 0x0059EE96)
  bool m_x4C2; // +0x4C2 (1 at construction)
  unsigned char m_pad0x4C3; // +0x4C3
  int m_x4C4; // +0x4C4
  int m_x4C8; // +0x4C8
  int m_x4CC; // +0x4CC
  AsciiString m_x4D0; // +0x4D0 (constructed from the literal "APT:NULL")
  AptConnectionScreen *m_connectionScreen; // +0x4D4: created by 0x5DB6E2 and released on response completion
  bool m_connectionsOpen; // +0x4D8 set by OnOpenConnectionsScreen 0x0059ED69, cleared by OnClosingConnectionsScreen 0x0059ED7A
  unsigned char m_pad0x4D9[3]; // +0x4D9
  unsigned int m_x4DC; // +0x4DC (timeGetTime stamp stored by 0x005A5B38)
};
typedef char AptOnlineCustomMatchSizeCheck[sizeof(AptOnlineCustomMatch) == 0x4E0 ? 1 : -1];
typedef char AptOnlineCustomMatchStateOffset[offsetof(AptOnlineCustomMatch, m_state) == 0x488 ? 1 : -1];
typedef char AptOnlineCustomMatchSetupOffset[offsetof(AptOnlineCustomMatch, m_setup) == 0x70 ? 1 : -1];
