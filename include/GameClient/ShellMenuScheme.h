// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// ShellMenuScheme.h: the shell's menu backdrop schemes (ShellMenuSchemeManager and the
// scheme, image and line records it keeps). Names and member order: Zero Hour's
// ShellMenuScheme.h (GPL-3.0-or-later, see NOTICE), which BFME 2 kept unchanged.
// Target facts: ShellMenuSchemeLine ctor 0x002004A5 (SIZE 0x18: start +0, end +8, width +0x10,
// colour +0x14), ShellMenuSchemeImage ctor 0x002004C3 (SIZE 0x18: name +0, position +4, size +0xC,
// image +0x14), ShellMenuScheme ctor 0x0020066C and dtor 0x002006B0 (SIZE 0xC: name +0, image
// list +4, line list +8; the dtor erases each node and deletes the record), ShellMenuSchemeManager
// ctor 0x00200774 / dtor 0x002007D5 (SIZE 0x8: scheme list +0, current scheme +4; Shell's ctor
// 0x0035BFBC allocates it, ShellMenuSchemeManager::draw reaches the scheme's draw 0x00200506).
#pragma once
#ifndef BFME2_SHELLMENUSCHEME_H
#define BFME2_SHELLMENUSCHEME_H

#include "Common/AsciiString.h"
#include "Common/ICoord2D.h"
#include "Common/StlList.h"
#include "Common/INI/FieldParse.h"

class Image;
class INI;

class ShellMenuSchemeLine
{
public:
	ShellMenuSchemeLine();									// 0x002004A5
	~ShellMenuSchemeLine() {}

	ICoord2D m_startPos;									// +0x00
	ICoord2D m_endPos;										// +0x08
	int m_width;											// +0x10
	int m_color;											// +0x14
};

class ShellMenuSchemeImage
{
public:
	ShellMenuSchemeImage();									// 0x002004C3
	~ShellMenuSchemeImage() { m_image = 0; }

	AsciiString m_name;										// +0x00
	ICoord2D m_position;									// +0x04
	ICoord2D m_size;										// +0x0C
	Image *m_image;											// +0x14
};

class ShellMenuScheme
{
public:
	ShellMenuScheme();										// 0x0020066C
	~ShellMenuScheme();										// 0x002006B0

	void draw();											// 0x00200506
	void addImage(ShellMenuSchemeImage *schemeImage);		// 0x002007BE
	void addLine(ShellMenuSchemeLine *schemeLine);

	AsciiString m_name;										// +0x00
	typedef _STL::list<ShellMenuSchemeImage *> ShellMenuSchemeImageList;
	typedef _STL::list<ShellMenuSchemeLine *> ShellMenuSchemeLineList;
	ShellMenuSchemeImageList m_imageList;					// +0x04
	ShellMenuSchemeLineList m_lineList;						// +0x08
};
typedef char ShellMenuSchemeSize[(sizeof(ShellMenuScheme) == 0xC) ? 1 : -1];

class ShellMenuSchemeManager
{
public:
	ShellMenuSchemeManager();								// 0x00200774 (the map spells it Rva002007D5)
	~ShellMenuSchemeManager();								// 0x002007D5

	void init();
	void update();											// folded empty body 0x000B3FD0
	void setShellMenuScheme(AsciiString name);				// 0x002005DE
	void draw();
	const FieldParse *getFieldParse() const { return m_shellMenuSchemeFieldParseTable; }
	static const FieldParse m_shellMenuSchemeFieldParseTable[];	// 0x00BE29EC
	static void parseImagePart(INI *ini, void *instance, void *store, const void *userData);	// 0x00200842
	ShellMenuScheme *newShellMenuScheme(AsciiString name);	// 0x002008C8

private:
	typedef _STL::list<ShellMenuScheme *> ShellMenuSchemeList;
	ShellMenuSchemeList m_schemeList;						// +0x00
	ShellMenuScheme *m_currentScheme;						// +0x04
};
typedef char ShellMenuSchemeManagerSize[(sizeof(ShellMenuSchemeManager) == 0x8) ? 1 : -1];

#endif
