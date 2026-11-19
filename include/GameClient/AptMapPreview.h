// AptMapPreview.h: the map preview sub-panel of the multiplayer setup screens (the panel embedded at +0x60 of AptMpGameSetup).
//
// Target facts (retail game.dat, RVAs; table addresses are VAs):
//  - Constructor 0x0057CF43 (one argument, the setup panel's GameInfoTarget, whose reference count at +4 it raises), destructor
//    0x0057CF0B. SIZE 0x70. It stores the table VA 0x0086F390 at +0, the table VA 0x0086F31C at +4 and its own address at +8,
//    fills the 12-byte container at +0x0C through 0x00211E58 and allocates a 0x100-byte MapMetaData into +0x64.
//  - Members: +0x18 the target, +0x1C the mode (-1 at construction; 0 "OpenPlay", 1 "Strategic": GameMapType 0x0057C549 answers
//    it), +0x2C the description list box (bfmeSetMapDescription 0x0057C892 refills it), +0x30 the eight start-position buttons
//    (rva0057C57B finds the index of a button, rva0057C597 enables them all), +0x50 the strategic scenario combo box,
//    +0x60 and +0x61 flags, +0x64 the displayed map's MapMetaData, +0x68 and +0x6C ints (zero at construction).
#pragma once
#ifndef BFME2_APTMAPPREVIEW_H
#define BFME2_APTMAPPREVIEW_H

class GameWindow;
class GameInfoTarget;
class MapMetaData;
class GameSlot;

class AptMapPreview
{
public:
	AptMapPreview(GameInfoTarget *);									// 0x0057CF43
	~AptMapPreview();													// 0x0057CF0B

	void GameMapType(int index, char *out, bool skip);					// 0x0057C549: Apt query, "OpenPlay" or "Strategic" by the mode
	int rva0057C57B(int button);										// 0x0057C57B: the start position under a start-position button, or -1
	void rva0057C597(bool enable);										// 0x0057C597: enables or disables the start-position buttons
	int GetNextSelectablePlayer(int startPos);							// 0x0057CA78: the first start position from startPos a slot can take, or -1
	int GetCurrentStrategicScenario();									// 0x0057C621
	void *GetStrategicScenarioComboBoxSelectedCampaign();				// 0x0057C649: the item data of the scenario combo box's selection
	void SelectCampaign(int campaign);									// 0x0057CA2D
	void rva0057CD66();													// 0x0057CD66: selects the campaign the scenario combo box shows
	int rva0057CDA1(unsigned int message, unsigned int data1, unsigned int data2);	// 0x0057CDA1: the scenario combo box's selection message (0x4026) selects its campaign
	void rva0057E24B(int game);													// 0x0057E24B: stores the game handle in the target and refreshes the preview
	void *GetStartPositionInfoForSlot(int);								// 0x0057C71F: the start-position record of a slot (its team at +4), or null
	void UpdateMapTitle(MapMetaData *map);									// 0x0057C8D1: shows the map's display name in the title field (a blank for no map)
	void bfmeSetMapDescription(MapMetaData *map);						// 0x0057C892: refills the description list with the map's first line
	void rva0057E058();													// 0x0057E058
	void setMode(int mode);												// 0x0057C7BA (the map spells its owner Rva0057E3DB): stores the mode and retitles the lobby map label
	int getViewerSlot();												// 0x0057CB06 (the map spells its owner Rva0057CB06): the local slot, or for an observing host the first AI slot
	GameSlot *rva0057C688(int startPos);								// 0x0057C688: the slot (of the first eight) holding a start position, or null
	void UpdateStrategicStartSpots();									// 0x0057DB97
	bool UpdateStartSpots(bool);										// 0x0057DFFB (the map spells its owner Rva0057DFFB): mode 0 refreshes the buttons from the game, mode 1 the strategic spots

	unsigned char m_x00[0x0C];											// +0x00: two table pointers and the self link
	unsigned char m_x0C[0x0C];											// +0x0C: a container
	GameInfoTarget *m_target;											// +0x18
	int m_mode;															// +0x1C
	int m_x20;															// +0x20
	int m_x24;															// +0x24
	int m_x28;															// +0x28
	GameWindow *m_descriptionList;										// +0x2C
	GameWindow *m_startButtons[8];										// +0x30
	GameWindow *m_scenarioCombo;										// +0x50
	int m_x54;															// +0x54
	int m_x58;															// +0x58
	int m_x5C;															// +0x5C
	bool m_flag60;														// +0x60
	bool m_flag61;														// +0x61
	unsigned char m_pad62[2];
	MapMetaData *m_map;													// +0x64
	int m_x68;															// +0x68
	int m_x6C;															// +0x6C
};
typedef char AptMapPreviewSizeCheck[sizeof(AptMapPreview)==0x70 ? 1 : -1];

#endif
