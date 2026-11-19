// AptCreateAHero.h: the create-a-hero screen ("CreateAHero.apt") and its page interface.
//
// Target facts (retail game.dat, RVAs; table addresses are VAs):
//  - Constructor 0x005142B0 (one argument for _bfme_AptGameWindow's constructor 0x0051268C), destructor 0x00513BAB, deleting
//    destructor 0x00513DB4; tables VA 0x00865D20 (primary: GameWindow's eleven slots + three, 14 in all; slot 5 is 0x005138E8
//    and the two new slots 11/12 are 0x00513838/0x00513988) and 0x00865D1C (the AptCallbackHost secondary). SIZE 0x438 (the
//    screen factory 0x002D1F03 allocates 0x438).
//  - Layout: _bfme_AptGameWindow (0x27C), the embedded AptMyHero at +0x27C (0x190 bytes: constructor 0x005B12C5 takes the
//    screen, destructor 0x005B190D; it derives CreateAHeroHero), +0x40C the argument OpenScreen stores (the hero slot),
//    +0x410 the current page, +0x414 the previous page, +0x418..+0x42B five page pointers, +0x42C..+0x42E three bytes the
//    constructor reads from the global data and the destructor writes back (ShowObjectHealth +0x9BD, +0xD45, +0x68),
//    +0x42F a "picture taken" flag, +0x430..+0x433 the four key flags, +0x434 the picture counter.
//  - The five pages are built by the constructor in this order and each is a distinct class with its own table that
//    stores its owner at +4 (no base constructor: the base is an eight-slot interface): +0x418 Manager (0x28 bytes, ctor
//    0x005B6B93; selected by 'M'), +0x41C Class (0x2C, 0x005B57FA; 'C'), +0x420 Appearance (0x10, 0x005B4F6F; 'A'), +0x424
//    Powers (0x74, 0x005B414F; 'P') and +0x428 a bling page (8 bytes, 0x005B25E7; 'B'). OnShowScreen 0x005139B0 switches on
//    the first character of the page name and moves to that page: it calls slot 4 of the previous page and slot 3 of the new one.
#pragma once
#ifndef BFME2_APTCREATEAHERO_H
#define BFME2_APTCREATEAHERO_H

#include "GameClient/BfmeAptGameWindow.h"

class GameWindow;
class CreateAHeroData;

// The hero list's sort order (the map spells it Rva005B61B3): two columns, the first tried then the second; the column values are 0/1 default hero
// ascending/descending, 2/3 name, 4/5 class and subclass (rva005B61B3 0x005B61B3 switches on them).
class HeroListSorter
{
public:
	HeroListSorter(int primary, int secondary) : m_primary(primary), m_secondary(secondary) {}
	bool operator()(CreateAHeroData *a, CreateAHeroData *b);		// 0x005B61B3
	int m_primary;
	int m_secondary;
};

class AptCreateAHero : public _bfme_AptGameWindow
{
public:
	// The page interface (eight slots; every page class stores its own table and the owner at +4).
	class Page
	{
	public:
		virtual ~Page();								// slot 0
		virtual void slot1();							// slot 1 (the shared empty body 0x000B3FD0)
		virtual void slot2(bool);						// slot 2: told every frame whether it is the current page (the shared stub 0x0047A69C)
		virtual void show();							// slot 3: called on the page a screen switch moves to
		virtual void hide();							// slot 4: called on the page it leaves
		virtual int slot5(int, unsigned int, unsigned int);	// slot 5 (0x005748AD in most pages)
		virtual int slot6(int, unsigned int, unsigned int);	// slot 6
		virtual int slot7(const char *);				// slot 7: rva00513AF4 forwards its argument here
		AptCreateAHero *m_owner;						// +0x04
	};

	// The hero name page ("CreateAHero::HeroName"; the map spells it Appearance). SIZE 0x10: the page interface, the window at +8
	// and two bytes. Table VA 0x0087317C: 3 show 0x005B4CC6, 4 hide 0x005B4D20, 6 0x005B4C97; the rest are the shared stubs.
	class Appearance : public Page
	{
	public:
		void Run();									// 0x005B4CC6 (slot 3 of the page table: the ledger spells it Run and non-virtual)
		virtual void hide();							// slot 4, 0x005B4D20
		virtual int slot6(int, unsigned int, unsigned int);	// slot 6, 0x005B4C97: the name entry's Enter key commits the name

		void AutoChangeBttn(const char *);				// 0x005B488F
		void OnHairColor(const char *);					// 0x005B4934
		void OnSkinColor(const char *);					// 0x005B495E
		void OnPaintColor(const char *);				// 0x005B4988
		void IncreaseAttribute(const char *);			// 0x005B49B2
		void DecreaseAttribute(const char *);			// 0x005B49D7
		void NextAppearance(const char *);				// 0x005B49FC
		void PrevAppearance(const char *);				// 0x005B4A21
		void OnComplete(const char *);					// 0x005B4D6E: leaving for the manager page commits the name and lets the manager take the hero
		void CommitHeroName();							// 0x005B4BDB: reads the name entry, trims it and gives it to the hero
		void ShowHeroName();							// 0x005B4C3C: fills the name entry from the hero's name
		void InitGadgets(const char *, unsigned int, GameWindow *);	// 0x005B4D3F

		GameWindow *m_window;							// +0x08 the name text entry
		bool m_nameLocked;								// +0x0C
		bool m_imeActive;								// +0x0D
		unsigned char m_pad0E[2];
	};

	// The hero class page ("CreateAHero::Class"; selected by 'C'). SIZE 0x2C: the page interface and 0x24 bytes not recovered here.
	// Table VA 0x00873450: 0 0x005B576B, 1 OnMapLoaded 0x005B59FD, 3 show 0x005B5787, the rest are the shared stubs.
	class Class : public Page
	{
	public:
		void Run();										// 0x005B5787 (slot 3, the page's show; the map spells it Run@Rva005B5787Box)
		void OnCloseBttn(const char *);					// 0x005B5418: the done button leaves the screen
		void ExternFunc(int query, char *buffer, bool set);	// 0x005B5485: the movie's count queries: 0 the number of hero classes, 1 the sub-class count of the current hero's class
		void rva005B54ED(unsigned int, unsigned int, bool);	// 0x005B54ED
		void Run(char *);						// 0x005B57C6 (the map spells it Run): two numbers (class, sub-class) picked in the movie
		unsigned char m_pad08[0x24];					// +0x08..+0x2B not recovered here
	};
	typedef char ClassSizeCheck[sizeof(Class)==0x2C ? 1 : -1];

	// The hero manager page ("CreateAHero::Manager"; selected by 'M'). SIZE 0x28: the page interface, the hero list at +8 and the
	// sort and refresh state the sort-button handlers 0x005B6A31/0x005B6A40/0x005B6A4F write through setSortColumn 0x005B5BDC.
	class Manager : public Page
	{
	public:
		virtual void show();							// slot 3, 0x005B692F
		virtual void hide();							// slot 4, 0x005B5AC3
		virtual int slot5(int, unsigned int, unsigned int);	// slot 5, 0x005B5AE3: Enter-style key (message 0x15, key 1, state bit 0) leaves the screen
		virtual int slot6(int, unsigned int, unsigned int);	// slot 6, 0x005B5E03: the hero list's selection message (0x4014) selects the hero
		void clearPendingHero();						// 0x005B5B07
		void OnMyPowerRollOver(char *);					// 0x005B5B39: the power button under the mouse (1-based name) shows its description
		void OnMyPowerRollOut(const char *);			// 0x005B5B92 (the map spells it rva005B5B92@Rva005B5B92): clears the description when the mouse leaves that button
		void OnAcceptHero(const char *);				// 0x005B5B0C: hands the screen's hero to the manager and leaves
		void Run();								// 0x005B5C02 (the map spells it Run): the screen's hero replaces the selected list entry's
		void Run(int refresh);							// 0x005B5C70: gives the selected hero to the screen's hero and tells the movie whether the customize buttons are enabled
		void setSortColumn(unsigned int column);		// 0x005B5BDC: picking the active column again flips it to the next one
		void rva005B6A31(void *);						// 0x005B6A31: sort the hero list by column 0
		void rva005B6A40(void *);						// 0x005B6A40: column 2
		void rva005B6A4F(void *);						// 0x005B6A4F: column 4
		void rva005B6182(bool refresh);					// 0x005B6182: rebuilds the awards when the page is shown and the hero is not being swapped
		void PopulateAwards();							// 0x005B6052
		void rva005B5F2E();								// 0x005B5F2E
		void rva005B6A05();								// 0x005B6A05: gives the pending hero back to the manager's list, then refreshes the hero list
		void PopulateHeroList();						// 0x005B6755
		void InitGadgets(char *name, int, char *window);	// 0x005B6967 (the map types the window as char *): binds the saved hero list and the saved hero stats list

		GameWindow *m_heroList;							// +0x08
		int m_x0C;										// +0x0C
		void *m_x10;									// +0x10 (a CreateAHeroData rva005B6A05 hands back to the manager, then clears)
		bool m_flag14;									// +0x14
		bool m_flag15;									// +0x15
		bool m_flag16;									// +0x16
		unsigned char m_pad17;
		int m_sortColumn;								// +0x18
		int m_previousSortColumn;						// +0x1C
		bool m_listDirty;								// +0x20
		unsigned char m_pad21[3];
		int m_x24;										// +0x24
	};
	typedef char ManagerSizeCheck[sizeof(Manager)==0x28 ? 1 : -1];

	AptCreateAHero(void *);							// 0x005142B0
	virtual ~AptCreateAHero();						// 0x00513BAB

	// Slots 11 and 12 of the primary table are this screen's own virtuals (0x00513838, 0x00513988).
	virtual bool canLeave();						// 0x00513838 (slot 11): true unless a game is running in a mode other than 7, where it asks the window manager to switch and answers false
	virtual void resetHeroAndPages();				// 0x00513988 (slot 12): AptMyHero 0x005B1A6C, then slot 1 of the five pages

	int FrameUpdate();								// 0x005138E8 (slot 5 of the primary table): applies the held camera keys, updates the hero and tells every page whether it is current
	void OnShowScreen(const char *);				// 0x005139B0
	static void OpenScreen(int mode);				// 0x00513CDF: pushes CreateAHero.apt, starts the create-a-hero map and hands the screen its hero slot
	void PrepareToTakePicture(const char *);		// 0x00513A47
	void OnTakePicture(const char *);				// 0x00513A51
	void RenderPictureGuard(const void *, const void *, void *, void *);	// 0x00513A64
	void CreateAHeroDemo(int, char *, bool);		// 0x00513A6D
	void RotateLeft(const char *);					// 0x00513AA8
	void RotateRight(const char *);					// 0x00513ABB
	void ZoomIn(const char *);						// 0x00513ACE
	void ZoomOut(const char *);						// 0x00513AE1
	void rva00513AF4(const char *);					// 0x00513AF4: forwards to the current page's slot 7

	unsigned char m_myHero[0x190];					// +0x27C: AptMyHero (0x190, include/GameClient/AptMyHero.h; abstract until CreateAHeroData overrides loadPostProcess and xfer, so not embedded by value yet)
	int m_heroSlot;									// +0x40C
	Page *m_page;									// +0x410
	Page *m_previousPage;							// +0x414
	Page *m_manager;								// +0x418 ('M')
	Page *m_class;									// +0x41C ('C')
	Page *m_appearance;								// +0x420 ('A')
	Page *m_powers;									// +0x424 ('P')
	Page *m_bling;									// +0x428 ('B')
	bool m_savedShowObjectHealth;					// +0x42C
	bool m_saved42D;								// +0x42D
	bool m_saved42E;								// +0x42E
	bool m_pictureTaken;							// +0x42F
	bool m_rotateLeft;								// +0x430
	bool m_rotateRight;								// +0x431
	bool m_zoomIn;									// +0x432
	bool m_zoomOut;									// +0x433
	int m_pictureCounter;							// +0x434

private:
	static AptCreateAHero *s_instance;				// the open create-a-hero screen (ledger name ?s_instance@AptCreateAHero@@0PAV1@A, VA 0x00E048D4)
	friend void Rva00513866(void);					// 0x00513866: asks for the screen to close
};
typedef char AptCreateAHeroSizeCheck[sizeof(AptCreateAHero)==0x438 ? 1 : -1];

#endif
