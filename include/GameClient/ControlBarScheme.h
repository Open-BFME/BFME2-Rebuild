// ControlBarScheme.h: the control bar's look (images, colours, button rectangles,
// layered pictures and slide animations) and the manager that picks one.
//
// Layout evidence (retail game.dat): ControlBarScheme is allocated 0x174 bytes
// (newControlBarScheme 0x0032053E) and built by 0x00320291, which also fixes the
// colour defaults (0x80000000 at +0x18, 0x00FFFFFF at +0x1C..+0x2C) and the six
// layer lists at +0x158 (an array constructor, count 6, element size 4) and the
// animation list at +0x170. The INI field table at 0x0080CD88 gives every other
// offset and name (token spellings are retail's). ControlBarSchemeImage (0x1C,
// ctor 0x0031F7AB) and ControlBarSchemeAnimation (0x24, ctor 0x0031F7EB) are the
// list elements; their destructors (0x002004FD, 0x0031F831) clear the image
// pointer and release the name. ControlBarSchemeManager is 0x10 (ctor 0x003200EB):
// current scheme, a Coord2D multiplier initialised to 1.0, and the scheme list.
// Member names follow Zero Hour's ControlBarScheme.h where it agrees with the
// field table; the INI token wins where it does not.
#pragma once
#ifndef BFME2_CONTROLBARSCHEME_H
#define BFME2_CONTROLBARSCHEME_H

#include "Common/AsciiString.h"
#include "Common/Coord2D.h"
#include "Common/ICoord2D.h"
#include "Common/StlList.h"

struct FieldParse;
class Image;
class Player;
class PlayerTemplate;

class ControlBarSchemeImage
{
public:
	ControlBarSchemeImage();			// 0x0031F7AB
	~ControlBarSchemeImage();			// 0x002004FD (shared with the shell menu scheme's identical body)

	AsciiString m_name;					// +0x00
	ICoord2D m_position;				// +0x04
	ICoord2D m_size;					// +0x0C
	Image *m_image;						// +0x14
	int m_layer;						// +0x18
};
// SIZE 0x1C

class ControlBarSchemeAnimation
{
public:
	ControlBarSchemeAnimation();		// 0x0031F7EB
	~ControlBarSchemeAnimation();		// 0x0031F831

	AsciiString m_name;					// +0x00
	int m_animType;						// +0x04: 0 is the slide-right animation (updateAnim 0x0031F799)
	ControlBarSchemeImage *m_animImage;	// +0x08
	unsigned int m_animDuration;		// +0x0C
	ICoord2D m_finalPos;				// +0x10
	ICoord2D m_startPos;				// +0x18
	unsigned int m_currentFrame;		// +0x20
};
// SIZE 0x24

class ControlBarScheme
{
public:
	ControlBarScheme();					// 0x00320291
	~ControlBarScheme();				// 0x0032002C
	void init();						// 0x0031ED2C
	void update();						// 0x0031F872
	void updateAnim(ControlBarSchemeAnimation *anim);	// 0x0031F799
	void drawForeground(Coord2D multiplier, ICoord2D offset);	// 0x0031F89E
	void drawBackground(Coord2D multiplier, ICoord2D offset);	// 0x0031F96F
	void reset();						// 0x0031FAF0
	void addAnimation(ControlBarSchemeAnimation *animation);	// 0x003200A2
	void addImage(ControlBarSchemeImage *image);				// 0x003200BC

	typedef _STL::list<ControlBarSchemeImage *> ControlBarSchemeImageList;
	typedef _STL::list<ControlBarSchemeAnimation *> ControlBarSchemeAnimationList;
	enum { MAX_CONTROL_BAR_SCHEME_IMAGE_LAYERS = 6, CONTROL_BAR_SCHEME_FOREGROUND_IMAGE_LAYERS = 3 };

	AsciiString m_name;											// +0x00
	ICoord2D m_screenCreationRes;								// +0x04 ScreenCreationRes
	AsciiString m_side;											// +0x0C Side
	const Image *m_buttonQueueImage;							// +0x10 QueueButtonImage
	const Image *m_rightHUDImage;								// +0x14 RightHUDImage
	int m_buildUpClockColor;									// +0x18 BuildUpClockColor (packed colour)
	int m_buttonBorderBuildColor;								// +0x1C
	int m_buttonBorderActionColor;								// +0x20
	int m_buttonBorderUpgradeColor;								// +0x24
	int m_buttonBorderSystemColor;								// +0x28
	int m_buttonBorderAlteredColor;								// +0x2C
	int m_commandBarBorderColor;								// +0x30
	const Image *m_optionsButtonEnable;							// +0x34 OptionsButtonEnable
	const Image *m_optionsButtonHighlited;						// +0x38 OptionsButtonHightlited (INI spelling)
	const Image *m_optionsButtonPushed;							// +0x3C
	const Image *m_optionsButtonDisabled;						// +0x40
	const Image *m_idleWorkerButtonEnable;						// +0x44
	const Image *m_idleWorkerButtonHighlited;					// +0x48
	const Image *m_idleWorkerButtonPushed;						// +0x4C
	const Image *m_idleWorkerButtonDisabled;					// +0x50
	const Image *m_buddyButtonEnable;							// +0x54
	const Image *m_buddyButtonHighlited;						// +0x58
	const Image *m_buddyButtonPushed;							// +0x5C
	const Image *m_buddyButtonDisabled;							// +0x60
	const Image *m_beaconButtonEnable;							// +0x64
	const Image *m_beaconButtonHighlited;						// +0x68
	const Image *m_beaconButtonPushed;							// +0x6C
	const Image *m_beaconButtonDisabled;						// +0x70
	const Image *m_genBarButtonIn;								// +0x74
	const Image *m_genBarButtonOn;								// +0x78
	const Image *m_toggleButtonUpIn;							// +0x7C
	const Image *m_toggleButtonUpOn;							// +0x80
	const Image *m_toggleButtonUpPushed;						// +0x84
	const Image *m_toggleButtonDownIn;							// +0x88
	const Image *m_toggleButtonDownOn;							// +0x8C
	const Image *m_toggleButtonDownPushed;						// +0x90
	const Image *m_generalButtonEnable;							// +0x94
	const Image *m_generalButtonHighlited;						// +0x98
	const Image *m_generalButtonPushed;							// +0x9C
	const Image *m_generalButtonDisabled;						// +0xA0
	const Image *m_uAttackButtonEnable;							// +0xA4
	const Image *m_uAttackButtonHighlited;						// +0xA8
	const Image *m_uAttackButtonPushed;							// +0xAC
	const Image *m_minMaxButtonEnable;							// +0xB0
	const Image *m_minMaxButtonHighlited;						// +0xB4
	const Image *m_minMaxButtonPushed;							// +0xB8
	const Image *m_genArrow;									// +0xBC GenArrow
	ICoord2D m_moneyUL;											// +0xC0
	ICoord2D m_moneyLR;											// +0xC8
	ICoord2D m_minMaxUL;										// +0xD0
	ICoord2D m_minMaxLR;										// +0xD8
	ICoord2D m_generalUL;										// +0xE0
	ICoord2D m_generalLR;										// +0xE8
	ICoord2D m_uAttackUL;										// +0xF0
	ICoord2D m_uAttackLR;										// +0xF8
	ICoord2D m_optionsUL;										// +0x100
	ICoord2D m_optionsLR;										// +0x108
	ICoord2D m_workerUL;										// +0x110
	ICoord2D m_workerLR;										// +0x118
	ICoord2D m_chatUL;											// +0x120
	ICoord2D m_chatLR;											// +0x128
	ICoord2D m_beaconUL;										// +0x130
	ICoord2D m_beaconLR;										// +0x138
	ICoord2D m_powerBarUL;										// +0x140
	ICoord2D m_powerBarLR;										// +0x148
	const Image *m_expBarForegroundImage;						// +0x150 ExpBarForegroundImage
	const Image *m_commandMarkerImage;							// +0x154 CommandMarkerImage
	ControlBarSchemeImageList m_layer[MAX_CONTROL_BAR_SCHEME_IMAGE_LAYERS];	// +0x158
	ControlBarSchemeAnimationList m_animations;					// +0x170
};
// SIZE 0x174

class ControlBarSchemeManager
{
public:
	ControlBarSchemeManager();			// 0x003200EB
	~ControlBarSchemeManager();			// 0x003204D9
	void init();						// 0x00320205
	void update();						// 0x0031FA8E (Zero Hour's name)
	ControlBarScheme *findControlBarScheme(AsciiString name);		// 0x0031FC08
	ControlBarScheme *newControlBarScheme(AsciiString name);		// 0x0032053E
	void setControlBarScheme(AsciiString name);						// 0x0031FC6A
	void setControlBarSchemeByPlayerTemplate(const PlayerTemplate *playerTemplate, bool useSmall);	// 0x0031FD0D
	void setControlBarSchemeByPlayer(Player *player);				// 0x0031FE49
	void drawForeground(ICoord2D offset);							// 0x0031FA9A
	void drawBackground(ICoord2D offset);							// 0x0031FAC5

	// INI block parser 0x001DB68C passes this table (VA 0x00C0CD88).
	static const FieldParse m_controlBarSchemeFieldParseTable[];

	typedef _STL::list<ControlBarScheme *> ControlBarSchemeList;

	ControlBarScheme *m_currentScheme;	// +0x00
	Coord2D m_multiplier;				// +0x04, 1.0 x 1.0 after construction
	ControlBarSchemeList m_schemeList;	// +0x0C
};
// SIZE 0x10

#endif
