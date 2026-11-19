// GadgetComboBox.h: the combo box gadget's free-function interface, declared from the
// names retail's own map carries (decorated names demangled; every one is a plain
// cdecl function that takes the window first). The per-gadget data struct lives in
// the window's instance data, so none of it is exposed here. Each
// declaration's comment is its retail RVA. Evidence: map identities only; the data
// layouts behind them are recovered with the functions that use them.
#pragma once
#ifndef BFME2_GADGETCOMBOBOX_H
#define BFME2_GADGETCOMBOBOX_H

#include "GameClient/GameWindow.h"

class AsciiString;
class UnicodeString;
class GameFont;
class Image;

// The edit field behind GadgetComboBoxData::m_field: the validation flags at +0xC and the
// maximum character count (16-bit) at +0x10, both mirrored from the combo box data.
struct GadgetComboBoxField
{
	UnsignedByte m_gap0x0[0xC];
	Int m_validationFlags;	// +0xC
	Short m_maxChars;		// +0x10
};

// The combo box user data (GameWindow::winGetUserData). Only +0x0, +0x4, +0x20 to +0x2C are
// recovered so far; the rest is not established.
struct GadgetComboBoxData
{
	Bool m_isEditable;	// +0x0
	Int m_maxDisplay;	// +0x4 (SetMaxDisplay writes it)
	Int m_maxChars;		// +0x8 (SetMaxChars writes it)
	Int m_validationFlags;	// +0xC (SetValidationFlags ORs into it)
	UnsignedByte m_gap0x10[0x4];
	GadgetComboBoxField *m_field;	// +0x14 (the edit field mirrors the two values above)
	UnsignedByte m_gap0x18[0x8];
	Int m_length;		// +0x20 (GetLength reads it)
	GameWindow *m_editBox;		// +0x24 (GetEditBox reads it)
	GameWindow *m_dropDownButton;	// +0x28 (a second child; name not yet confirmed)
	GameWindow *m_listBox;		// +0x2C (GetListBox reads it)
};

class BfmeKeyLC;
void * bfmeGo925A(BfmeKeyLC *);	// 0x002C0315 (the map's spelling): the combo box's list box child, data +0x2C
void GadgetComboDisableEntry(BfmeKeyLC *, int, unsigned char);	// 0x00322714
int Rva00322910(GameWindow *);	// 0x00322910: the selected position, or -1 without a window
void Rva003229C5Hide(GameWindow *);	// 0x003229C5: hides the drop-down list and restores the combo box height
int Rva00322E19Add(GameWindow *, UnicodeString, int);	// 0x00322E19: GCM message 0x4023 (the sibling of AddEntry)
GameWindow * GadgetComboBoxGetEditBox(GameWindow *);	// 0x002C02FE
GameWindow * GadgetComboBoxGetListBox(GameWindow *);	// 0x002C032C
void GadgetComboBoxSetColors(GameWindow *, int, int, int, int, int, int, int, int, int, int, int, int);	// 0x00322463
void GadgetComboBoxSetIsEditable(GameWindow *, bool);	// 0x00322660
void GadgetComboBoxSetValidationFlags(GameWindow *, int);	// 0x00322699
void GadgetComboBoxSetMaxChars(GameWindow *, int);	// 0x003226E7
void GadgetComboBoxSetMaxDisplay(GameWindow *, int);	// 0x00322703
void GadgetComboBoxReset(GameWindow *);	// 0x0032273B
void GadgetComboBoxHideList(GameWindow *);	// 0x0032275D
void GadgetComboBoxHideDropDown(GameWindow *, bool);	// 0x0032277F
void GadgetComboBoxSetFont(GameWindow *, GameFont *);	// 0x003227A0
void GadgetComboBoxSetEnabledTextColors(GameWindow *, int, int);	// 0x00322805
void GadgetComboBoxSetDisabledTextColors(GameWindow *, int, int);	// 0x0032283E
void GadgetComboBoxSetHiliteTextColors(GameWindow *, int, int);	// 0x00322879
void GadgetComboBoxSetIMECompositeTextColors(GameWindow *, int, int);	// 0x003228B2
void GadgetComboBoxGetSelectedPos(GameWindow *, int *);	// 0x003228EB
void GadgetComboBoxSetSelectedPos(GameWindow *, int, bool);	// 0x00322931
void GadgetComboBoxSetItemData(GameWindow *, int, void *);	// 0x0032295A
void * GadgetComboBoxGetItemData(GameWindow *, int);	// 0x00322981
int GadgetComboBoxGetLength(GameWindow *);	// 0x003229B1
WindowMsgHandledType GadgetComboBoxInput(GameWindow *, unsigned int, unsigned int, unsigned int);	// 0x00322C25
UnicodeString GadgetComboBoxGetText(GameWindow *);	// 0x00322D21
void GadgetComboBoxSetText(GameWindow *, UnicodeString);	// 0x00322D63
int GadgetComboBoxAddEntry(GameWindow *, UnicodeString, int);	// 0x00322DC5
WindowMsgHandledType GadgetComboBoxSystem(GameWindow *, unsigned int, unsigned int, unsigned int);	// 0x00322E6D
WindowMsgHandledType GadgetImageComboBoxInput(GameWindow *, unsigned int, unsigned int, unsigned int);	// 0x0032386A
WindowMsgHandledType GadgetImageComboBoxSystem(GameWindow *, unsigned int, unsigned int, unsigned int);	// 0x00323A5B

#endif
