/* BFME-ALTERED: Lua 4.0.1's lcode.h (upstream SHA-256 71169a0566560a2ec51917ed899ebca3393f2488eabe5293b6e1afd78802bc5e) altered to reproduce the fork retail game.dat links; every changed hunk is marked BFME-RETAIL. An ALTERED SOURCE VERSION of Lua, not the original (the original is include/upstream/lua-4.0.1/src/lcode.h); Lua's copyright notice, in lua.h, applies unchanged; see include/upstream/lua-4.0.1-bfme/PROVENANCE.md. */
/*
** $Id: lcode.h,v 1.16 2000/08/09 14:49:13 roberto Exp $
** Code generator for Lua
** See Copyright Notice in lua.h
*/

#ifndef lcode_h
#define lcode_h

#include "llex.h"
#include "lobject.h"
#include "lopcodes.h"
#include "lparser.h"


/*
** Marks the end of a patch list. It is an invalid value both as an absolute
** address, and as a list link (would link an element to itself).
*/
#define NO_JUMP (-1)


/*
** grep "ORDER OPR" if you change these enums
*/
typedef enum BinOpr {
  OPR_ADD, OPR_SUB, OPR_MULT, OPR_DIV, OPR_POW,
  OPR_CONCAT,
  OPR_NE, OPR_EQ, OPR_LT, OPR_LE, OPR_GT, OPR_GE,
  OPR_AND, OPR_OR,
  OPR_NOBINOPR
} BinOpr;

typedef enum UnOpr { OPR_MINUS, OPR_NOT, OPR_NOUNOPR } UnOpr;


enum Mode {iO, iU, iS, iAB};  /* instruction format */

#define VD	100	/* flag for variable delta */

extern const struct OpProperties {
  char mode;
  unsigned char push;
  unsigned char pop;
} luaK_opproperties[];


void luaK_error (LexState *ls, const char *msg);
int luaK_code0 (FuncState *fs, OpCode o);
int luaK_code1 (FuncState *fs, OpCode o, int arg1);
int luaK_code2 (FuncState *fs, OpCode o, int arg1, int arg2);
int luaK_jump (FuncState *fs);
void luaK_patchlist (FuncState *fs, int list, int target);
void luaK_concat (FuncState *fs, int *l1, int l2);
void luaK_goiftrue (FuncState *fs, expdesc *v, int keepvalue);
int luaK_getlabel (FuncState *fs);
void luaK_deltastack (FuncState *fs, int delta);
void luaK_kstr (LexState *ls, int c);
void luaK_number (FuncState *fs, Number f);
void codepushbool (FuncState *fs, int b);  /* BFME-RETAIL 0x00752970: not in stock Lua; name is the ledger name of 0x00752970, not recovered; pushes the boolean b */
void luaK_adjuststack (FuncState *fs, int n);
int luaK_lastisopen (FuncState *fs);
void luaK_setcallreturns (FuncState *fs, int nresults);
void luaK_tostack (LexState *ls, expdesc *v, int onlyone);
void luaK_storevar (LexState *ls, const expdesc *var);
void luaK_prefix (LexState *ls, UnOpr op, expdesc *v);
void luaK_infix (LexState *ls, BinOpr op, expdesc *v);
void luaK_posfix (LexState *ls, BinOpr op, expdesc *v1, expdesc *v2);


#endif
