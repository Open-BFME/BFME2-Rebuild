// Retail global at absolute 0x00DFE4CC (RVA 0x009FE4CC): the Apt window manager object, installed by the
// AptPlayer constructor 0x00224E10 (0x00224F35). The map's name for the address is
// g_bfmeAptWindowManager; its class has no layout here yet, so only the global is declared.
// Apt callbacks reach it through the view classes below (casts), as retail does.
#pragma once

class AsciiString;
class UnicodeString;

class BfmeAptWindowManager
{
public:
	void bfmeSetText(const AsciiString &, const UnicodeString &, bool);	// 0x00225301: sets the text of the named Apt text field
};
extern BfmeAptWindowManager *g_bfmeAptWindowManager;

// The window manager's rowed byte setter at 0x00222479 (8 bytes, `enable`); CloseWindow tail-jumps to it
// with the global as `this`. Declared here so callers can name it; its body is owned by the AptPlayer rows.
class Rva00222479ByteOneSetter
{
public:
	void enable();
};

// The window manager's invoke (0x00222A8B, `invoke@Rva00222A8BTarget`): the callback owner, the Apt
// method name, an int, a text argument and four pointer-sized extras (callers pass them as 0).
class Rva00222A8BTarget
{
public:
	int invoke(void *, const char *, int, const char *, void *, void *, void *, void *);
	void rva00222F55(bool);								// 0x00222F55
	void rva002233A6(int);								// 0x002233A6 (map: ?rva002233A6@Rva00222A8BTarget@@QAEXH@Z): the Apt screens call it with 1 after pushing their movie

	unsigned char m_gap0x000[0x31C];		// the window manager object's other members are not recovered
	int m_backgroundMode;					// +0x31C: 0 none, 1 front end, 2 in game (rva002233A6 stores it)
	int m_flag320;							// +0x320: cleared when a background is shown
	void *m_backgroundLevel;				// +0x324: the Apt level the background calls are sent to
};
