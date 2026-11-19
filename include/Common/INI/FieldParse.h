// FieldParse.h: SAGE INI field-table entries and the multi-table parse list.
// Target facts (old repo Code/GameEngine/Include/Common/INI/FieldParse.h):
// FieldParse has no virtual functions or bases; its 0x10-byte entries hold
// token +0, parse +4, userData +8, offset +0x0C. MultiIniFieldParse's ctor
// (RVA 0x0002BAA0) zeroes 16 pointers at +0, 16 offsets at +0x40 and the count
// at +0x80; add (RVA 0x0002BC6E) checks count < 16.
#pragma once
#ifndef BFME2_FIELDPARSE_H
#define BFME2_FIELDPARSE_H

class INI;

typedef void (*INIFieldParseProc)(INI *, void *, void *, const void *);

struct FieldParse
{
	const char *token;
	INIFieldParseProc parse;
	const void *userData;
	int offset;

	void set(const char *t, INIFieldParseProc p, const void *u, int o)
	{
		token = t;
		parse = p;
		userData = u;
		offset = o;
	}
};

class MultiIniFieldParse
{
public:
	enum { MAX_MULTI_FIELDS = 16 };

	MultiIniFieldParse();
	void add(const FieldParse *fields, unsigned int extraOffset = 0);
	int getCount() const { return m_count; }
	const FieldParse *getNthFieldParse(int i) const;
	unsigned int getNthExtraOffset(int i) const;

	const FieldParse *m_fieldParse[MAX_MULTI_FIELDS];
	unsigned int m_extraOffset[MAX_MULTI_FIELDS];
	int m_count;
};

#endif
