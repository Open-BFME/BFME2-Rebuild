// GadgetListBox.h: the list box gadget's free-function interface, declared from the
// names retail's own map carries (decorated names demangled; every one is a plain
// cdecl function that takes the window first). The per-gadget data struct lives in
// the window's instance data, so none of it is exposed here. Each
// declaration's comment is its retail RVA. Evidence: map identities only; the data
// layouts behind them are recovered with the functions that use them.
#pragma once
#ifndef BFME2_GADGETLISTBOX_H
#define BFME2_GADGETLISTBOX_H

#include "GameClient/GameWindow.h"

class AsciiString;
class UnicodeString;
class GameFont;
class Image;

// One column cell of a list box entry: 28 bytes; the justification is at +0x4.
struct GadgetListBoxCell
{
	Int m_cellType;		// +0x0: 1 text, 2 image (computeTotalHeight 0x00324C61)
	Int m_justify;		// +0x4
	unsigned char m_gap0x8[0x4];
	void *m_data;		// +0xC: the DisplayString of a text cell
	unsigned char m_gap0x10[0x8];
	Int m_height;		// +0x18: the height of an image cell
};

// One entry of the list box: 16 bytes; the cell table is at +0x8 and the disabled flag at +0xC.
struct GadgetListBoxEntry
{
	Int m_top;		// +0x0 (IsFull compares it with the visible bottom): the height of the list up to and including this row
	Int m_height;		// +0x4: the row's own height (computeTotalHeight 0x00324C61 stores it)
	GadgetListBoxCell *m_cells;	// +0x8
	UnsignedByte m_disabled;	// +0xC (stored as given)
	unsigned char m_gap0xD[0x3];
};

// The list box user data (GameWindow::winGetUserData). Only the fields the wrappers below
// touch are declared; the rest is not established.
struct _ListboxData
{
	short m_listLength;		// +0x0 (GetListLength and DisableEntry read it)
	short m_numColumns;		// +0x2 (GetNumColumns reads it)
	Int *m_columnPercent;	// +0x4: each column's share of the width in percent (SetColumnWidths 0x003252EA)
	unsigned char m_gap0x8[0x3];
	Bool m_hasLength;		// +0xB: the multi-select flag (AddMultiSelect 0x00325199 sets it)
	Bool m_forceSelect;		// +0xC
	Bool m_scrollIfAtEnd;	// +0xD (AddEntryText 0x00326BEC)
	Bool m_audioFeedback;	// +0xE
	unsigned char m_gap0xF[0x2];	// +0xF: the flag bfmeGo924A writes at +0xF
	unsigned char m_scrollBar;	// +0x11: the list has a scroll bar (Zero Hour's ListboxData::scrollBar; ScrollTo 0x00324B8F and 0x003249D2 read it, bfmeGoENK sets it as a byte)
	Bool m_x12;				// +0x12 (AptOnlineCustomMatch::InitGadgets 0x005A0C6E sets it on the game list)
	unsigned char m_gap0x13;
	Int *m_columnWidths;		// +0x14 (GetColumnWidth reads it)
	GadgetListBoxEntry *m_entries;	// +0x18
	GameWindow *m_upButton;	// +0x1C
	GameWindow *m_downButton;	// +0x20
	GameWindow *m_slider;	// +0x24
	Int m_totalHeight;	// +0x28 (computeTotalHeight 0x00324C61 stores it)
	short m_numEntries;		// +0x2C (GetNumEntries reads it)
	short m_insertPos;		// +0x2E
	Int m_gap0x30;			// +0x30
	Int m_selectPos;		// +0x34 (SetListLength 0x00326E21)
	Int *m_selections;		// +0x38 (GadgetListBoxAddMultiSelect 0x00325199 allocates it)
	short m_clientHeight;	// +0x3C (IsFull)
	unsigned char m_gap0x3E[0x6];
	short m_scrollTop;		// +0x44 (IsFull, the first-visible-entry search)
	unsigned char m_gap0x46[0x2];
	Int m_gap0x48;		// +0x48 (bfmeGoBHF 0x00324968 returns it)

	// Out-of-line helpers of the data record: they take the record as `this` (the map
	// spells them rva00323F78@Rva00323F78 and rva00323F9C@Rva00323F9C).
	int getTopVisibleEntry();		// 0x00323F78: first entry whose top is past the scroll offset
	int getBottomVisibleEntry();	// 0x00323F9C: last entry that starts within the visible area
};
typedef _ListboxData GadgetListBoxData;

GameWindow * GadgetListBoxGetDownButton(GameWindow *);	// 0x002C02D0
GameWindow * GadgetListBoxGetSlider(GameWindow *);	// 0x002C02E7
int GadgetListBoxGetEntryBasedOnXY(GameWindow *, int, int, int &, int &);	// 0x00323F6F
void GadgetListBoxSetColors(GameWindow *, int, int, int, int, int, int, int, int, int, int, int, int);	// 0x003240A6
void GadgetListBoxDisableEntry(GameWindow *, int, unsigned char);	// 0x0032431F
void GadgetListBoxJustifyEntry(GameWindow *, int, int, int);	// 0x0032434A
// The record window message 0x4011 receives for an image entry: 28 bytes.
struct GadgetListBoxImageMessage
{
	Int m_first;			// +0x0
	Int m_second;			// +0x4
	Image const *m_image;	// +0x8
	Int m_kind;			// +0xC (2 in every caller)
	Bool m_flag;			// +0x10
	unsigned char m_gap0x11[0x3];
	Int m_fifth;			// +0x14
	Int m_fourth;			// +0x18
};

int GadgetListBoxAddEntryImage(GameWindow *, Image const *, int, int, int, int, bool, int);	// 0x00324380
int GadgetListBoxAddEntryImage(GameWindow *, Image const *, int, int, bool, int);	// 0x003243D0
void GadgetListBoxSetFont(GameWindow *, GameFont *);	// 0x003243F3
void GadgetListboxCreateScrollbar(GameWindow *);	// 0x0032449E
int GadgetListBoxGetListLength(GameWindow *);	// 0x00324743
int GadgetListBoxGetNumEntries(GameWindow *);	// 0x0032475A
void GadgetListBoxGetSelected(GameWindow *, int *);	// 0x00324773
void GadgetListBoxSetSelected(GameWindow *, int);	// 0x00324798
void GadgetListBoxSetSelected(GameWindow *, int const *, int);	// 0x003247BE
void GadgetListBoxReset(GameWindow *);	// 0x003247E5
bool GadgetListBoxIsFull(GameWindow *);	// 0x00324822
int GadgetListBoxGetTopVisibleEntry(GameWindow *);	// 0x00324860
void GadgetListBoxSetAudioFeedback(GameWindow *, bool);	// 0x0032487B
void BfmeGadgetListBoxSetAudioFeedback(GameWindow *, bool);	// 0x0032497D (the map's spelling; the sibling that writes the flag at +0xE)
int GadgetListBoxGetNumColumns(GameWindow *);	// 0x003248B1
int GadgetListBoxGetColumnWidth(GameWindow *, int);	// 0x003248CA
WindowMsgHandledType GadgetListBoxMultiInput(GameWindow *, unsigned int, unsigned int, unsigned int);	// 0x00324E92
void GadgetListBoxAddMultiSelect(GameWindow *);	// 0x00325199
void GadgetListBoxRemoveMultiSelect(GameWindow *);	// 0x00326DE9 (the map spells it Rva00326DE9Disable)
void GadgetListBoxUpdateColumnWidths(GameWindow *);	// 0x003251F4
void GadgetListBoxSetColumnWidths(GameWindow *, int, int *);	// 0x003252EA
// The pair of ints the item-data messages (0x4021, 0x4020) carry by address.
struct GadgetListBoxItemKey
{
	Int m_first;	// the int parameter stored first
	Int m_second;	// the int parameter stored second
};

void GadgetListBoxSetItemData(GameWindow *, void *, int, int);	// 0x00325388
void * GadgetListBoxGetItemData(GameWindow *, int, int);	// 0x003253BE
void GadgetListBoxSetBottomVisibleEntry(GameWindow *, int);	// 0x003253FD
void GadgetListBoxSetTopVisibleEntry(GameWindow *, int);	// 0x0032544C
WindowMsgHandledType GadgetListBoxInput(GameWindow *, unsigned int, unsigned int, unsigned int);	// 0x00325635
WindowMsgHandledType GadgetListBoxSystem(GameWindow *, unsigned int, unsigned int, unsigned int);	// 0x00325FBF
int GadgetListBoxAddEntryText(GameWindow *, UnicodeString, int, int, int, bool);	// 0x00326BEC
void GadgetListBoxSetListLength(GameWindow *, int);	// 0x00326E21
UnicodeString GadgetListBoxGetTextAndColor(GameWindow *, int *, int, int);	// 0x00326FBC
UnicodeString GadgetListBoxGetText(GameWindow *, int, int);	// 0x00327074

#endif
