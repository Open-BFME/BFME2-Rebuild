/* BFME-ALTERED: Lua 4.0.1's llex.h (upstream SHA-256 73aee22a5c454465ac4e2970e4618ddbca9a47370173513f13022e94e5d38eeb) altered to reproduce the fork retail game.dat links; every changed hunk is marked BFME-RETAIL. An ALTERED SOURCE VERSION of Lua, not the original (the original is include/upstream/lua-4.0.1/src/llex.h); Lua's copyright notice, in lua.h, applies unchanged; see include/upstream/lua-4.0.1-bfme/PROVENANCE.md. */
/*
** $Id: llex.h,v 1.31 2000/09/27 17:41:58 roberto Exp $
** Lexical Analyzer
** See Copyright Notice in lua.h
*/

#ifndef llex_h
#define llex_h

#include "lobject.h"
#include "lzio.h"


#define FIRST_RESERVED	257

/* maximum length of a reserved word (+1 for final 0) */
#define TOKEN_LEN	15


/*
* WARNING: if you change the order of this enumeration,
* grep "ORDER RESERVED"
*/
enum RESERVED {
  /* terminal symbols denoted by reserved words */
  TK_AND = FIRST_RESERVED, TK_BREAK,
  TK_DO, TK_ELSE, TK_ELSEIF, TK_END, TK_FOR, TK_FUNCTION, TK_IF, TK_LOCAL,
  TK_NIL, TK_NOT, TK_OR, TK_REPEAT, TK_RETURN, TK_THEN, TK_UNTIL, TK_WHILE,
  TK_TRUE, TK_FALSE,  /* BFME-RETAIL 0x008F3168: two more reserved words after "while", retail reserved-word table 0x008F3168 entries 18 and 19 ("true", "false"); names borrowed from Lua 5.0, not recovered */
  /* other terminal symbols */
  TK_NAME, TK_CONCAT, TK_DOTS, TK_EQ, TK_GE, TK_LE, TK_NE, TK_NUMBER,
  TK_STRING, TK_EOS
};

/* number of reserved words */
#define NUM_RESERVED	((int)(TK_FALSE-FIRST_RESERVED+1))  /* BFME-RETAIL 0x00756F10: was TK_WHILE; luaX_init 0x00756F10 loops to 0x14, 0x00756F39 */


typedef union {
  Number r;
  TString *ts;
} SemInfo;  /* semantics information */


typedef struct Token {
  int token;
  SemInfo seminfo;
} Token;


typedef struct LexState {
  int current;  /* current character */
  Token t;  /* current token */
  Token lookahead;  /* look ahead token */
  struct FuncState *fs;  /* `FuncState' is private to the parser */
  struct lua_State *L;
  struct zio *z;  /* input stream */
  int linenumber;  /* input line counter */
  int lastline;  /* line of last token `consumed' */
  TString *source;  /* current source name */
} LexState;


void luaX_init (lua_State *L);
void luaX_setinput (lua_State *L, LexState *LS, ZIO *z, TString *source);
int luaX_lex (LexState *LS, SemInfo *seminfo);
void luaX_checklimit (LexState *ls, int val, int limit, const char *msg);
void luaX_syntaxerror (LexState *ls, const char *s, const char *token);
void luaX_error (LexState *ls, const char *s, int token);
void luaX_token2str (int token, char *s);


#endif
