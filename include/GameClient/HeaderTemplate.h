// HeaderTemplate.h: the named font styles ("Header" blocks in HeaderTemplate.ini)
// that windows pick their text fonts from, and the manager that owns them.
//
// Layout evidence (retail game.dat): HeaderTemplate is allocated 0x14 bytes
// (newHeaderTemplate 0x00201C39 pushes 0x14) and built by 0x00201998, which zeroes
// +0x00 (the resolved font), +0x04 (the template name) and +0x08 (the font name,
// the INI token Font), +0x0C (Point) and +0x10 (Bold) and then releases both strings.
// The INI field table 0x00BE3098 (read by parseHeaderTemplateDefinition 0x00201CAF)
// names Font, Point and Bold. The manager is one STLport list (4 bytes, its
// constructor 0x00201B51 only builds it); its global is TheHeaderTemplateManager
// (0x009FE124). populateGameFonts 0x00201B05 resolves every template's font through
// the font library. Member and method names are Zero Hour's (donor), values retail's.
#pragma once
#ifndef BFME2_HEADERTEMPLATE_H
#define BFME2_HEADERTEMPLATE_H

#include "Common/AsciiString.h"
#include "Common/StlList.h"

class GameFont;
class INI;
struct FieldParse;

class HeaderTemplate
{
public:
	HeaderTemplate();													// 0x00201998
	~HeaderTemplate();													// 0x002019E1

	GameFont *m_font;													// +0x00
	AsciiString m_name;													// +0x04
	AsciiString m_fontName;												// +0x08
	int m_point;														// +0x0C
	bool m_bold;														// +0x10

	static const FieldParse m_headerFieldParseTable[];					// 0x00BE3098
};
// SIZE 0x14

class HeaderTemplateManager
{
public:
	HeaderTemplateManager();											// 0x00201B51
	~HeaderTemplateManager();											// 0x00201B64
	void init();														// 0x00201BD0
	HeaderTemplate *findHeaderTemplate(AsciiString name);				// 0x00201A33
	HeaderTemplate *newHeaderTemplate(AsciiString name);				// 0x00201C39
	GameFont *getFontFromTemplate(AsciiString name);					// 0x00201A84
	HeaderTemplate *getFirstHeader();									// 0x00201AD3
	HeaderTemplate *getNextHeader(HeaderTemplate *header);				// 0x00201AE2
	void populateGameFonts();											// 0x00201B05
	void headerNotifyResolutionChange();								// 0x00201C34: a five-byte `jmp` to populateGameFonts

	typedef _STL::list<HeaderTemplate *> HeaderTemplateList;
	HeaderTemplateList m_headerTemplateList;							// +0x00
};
// SIZE 0x04

extern HeaderTemplateManager *TheHeaderTemplateManager;					// 0x009FE124

#endif
