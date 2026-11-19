/* BFME-ALTERED: Lua 4.0.1's lobject.h (upstream SHA-256 e051401d75cf5793a55eba62c768beec8a401bb0c671ca02bd4d412dd765eb0c) altered to reproduce the fork retail game.dat links; every changed hunk is marked BFME-RETAIL. An ALTERED SOURCE VERSION of Lua, not the original (the original is include/upstream/lua-4.0.1/src/lobject.h); Lua's copyright notice, in lua.h, applies unchanged; see include/upstream/lua-4.0.1-bfme/PROVENANCE.md. */
/*
** $Id: lobject.h,v 1.82 2000/10/30 17:49:19 roberto Exp $
** Type definitions for Lua objects
** See Copyright Notice in lua.h
*/

#ifndef lobject_h
#define lobject_h


#include "llimits.h"
#include "lua.h"


#ifdef LUA_DEBUG
#undef NDEBUG
#include <assert.h>
#define LUA_INTERNALERROR(s)	assert(((void)s,0))
#define LUA_ASSERT(c,s)		assert(((void)s,(c)))
#else
#define LUA_INTERNALERROR(s)	/* empty */
#define LUA_ASSERT(c,s)		/* empty */
#endif


#ifdef LUA_DEBUG
/* to avoid warnings, and make sure value is really unused */
#define UNUSED(x)	(x=0, (void)(x))
#else
#define UNUSED(x)	((void)(x))	/* to avoid warnings */
#endif


/* mark for closures active in the stack */
#define LUA_TMARK	7  /* BFME-RETAIL 0x0074D924: was 6; retail luaD_call stores tag 7 for an active closure, 0x0074D924 */


/* tags for values visible from Lua == first user-created tag */
#define NUM_TAGS	7  /* BFME-RETAIL 0x0074FB56: was 6; retail luaT_init sizes the tag tables by 7 and sets last_tag 6, 0x0074FB56, 0x0074FB74 */


/* check whether `t' is a mark */
#define is_T_MARK(t)	((t) == LUA_TMARK)


typedef union {
  struct TString *ts;	/* LUA_TSTRING, LUA_TUSERDATA */
  struct Closure *cl;	/* LUA_TFUNCTION */
  struct Hash *a;	/* LUA_TTABLE */
  struct CallInfo *i;	/* LUA_TLMARK */
  Number n;		/* LUA_TNUMBER */
  int b;		/* BFME-RETAIL 0x0074EF1F: boolean payload, tag 6; the name is borrowed from Lua 5.0 for readability, not recovered; stored at +8 by 0x0074EF1F */
} Value;


/* Macros to access values */
#define ttype(o)        ((o)->ttype)
#define nvalue(o)       ((o)->value.n)
#define bvalue(o)       ((o)->value.b)  /* BFME-RETAIL 0x0074EF1F: accessor for the boolean payload; name borrowed from Lua 5.0, not recovered; see 0x0074EF1F */
#define tsvalue(o)      ((o)->value.ts)
#define clvalue(o)      ((o)->value.cl)
#define hvalue(o)       ((o)->value.a)
#define infovalue(o)	((o)->value.i)
#define svalue(o)       (tsvalue(o)->str)


typedef struct lua_TObject {
  int ttype;
  Value value;
} TObject;


/*
** String headers for string table
*/

/*
** most `malloc' libraries allocate memory in blocks of 8 bytes. TSPACK
** tries to make sizeof(TString) a multiple of this granularity, to reduce
** waste of space.
*/
#define TSPACK	((int)sizeof(int))

typedef struct TString {
  union {
    struct {  /* for strings */
      unsigned long hash;
      int constindex;  /* hint to reuse constants */
    } s;
    struct {  /* for userdata */
      int tag;
      void *value;
    } d;
  } u;
  size_t len;
  struct TString *nexthash;  /* chain for hash table */
  int marked;
  char str[TSPACK];   /* variable length string!! must be the last field! */
} TString;


/*
** Function Prototypes
*/
typedef struct Proto {
  Number *knum;  /* Number numbers used by the function */
  int nknum;  /* size of `knum' */
  struct TString **kstr;  /* strings used by the function */
  int nkstr;  /* size of `kstr' */
  struct Proto **kproto;  /* functions defined inside the function */
  int nkproto;  /* size of `kproto' */
  Instruction *code;
  int ncode;  /* size of `code'; when 0 means an incomplete `Proto' */
  short numparams;
  short is_vararg;
  short maxstacksize;
  short marked;
  struct Proto *next;
  /* debug information */
  int *lineinfo;  /* map from opcodes to source lines */
  int nlineinfo;  /* size of `lineinfo' */
  int nlocvars;
  struct LocVar *locvars;  /* information about local variables */
  int lineDefined;
  TString  *source;
} Proto;


typedef struct LocVar {
  TString *varname;
  int startpc;  /* first point where variable is active */
  int endpc;    /* first point where variable is dead */
} LocVar;


/*
** Closures
*/
typedef struct Closure {
  union {
    lua_CFunction c;  /* C functions */
    struct Proto *l;  /* Lua functions */
  } f;
  struct Closure *next;
  struct Closure *mark;  /* marked closures (point to itself when not marked) */
  short isC;  /* 0 for Lua functions, 1 for C functions */
  short nupvalues;
  TObject upvalue[1];
} Closure;


#define iscfunction(o)	(ttype(o) == LUA_TFUNCTION && clvalue(o)->isC)


typedef struct Node {
  TObject key;
  TObject val;
  struct Node *next;  /* for chaining */
} Node;

typedef struct Hash {
  Node *node;
  int htag;
  int size;
  int sizehint;  /* BFME-RETAIL 0x00750770: neutral name; luaH_new stores its third argument here, +0xC: 0x00750770 (0x0075079A); firstfree moves to +0x10: 0x007506E0 (0x00750762) */
  Node *firstfree;  /* this position is free; all positions after it are full */
  struct Hash *next;
  struct Hash *mark;  /* marked tables (point to itself when not marked) */
} Hash;


/* unmarked tables and closures are represented by pointing `mark' to
** themselves
*/
#define ismarked(x)	((x)->mark != (x))


/*
** informations about a call (for debugging)
*/
typedef struct CallInfo {
  struct Closure *func;  /* function being called */
  const Instruction **pc;  /* current pc of called function */
  int lastpc;  /* last pc traced */
  int line;  /* current line */
  int refi;  /* current index in `lineinfo' */
} CallInfo;


extern const TObject luaO_nilobject;
extern const char *const luaO_typenames[];


#define luaO_typename(o)	(luaO_typenames[ttype(o)])


lint32 luaO_power2 (lint32 n);
char *luaO_openspace (lua_State *L, size_t n);

int luaO_equalObj (const TObject *t1, const TObject *t2);
int luaO_str2d (const char *s, Number *result);

void luaO_verror (lua_State *L, const char *fmt, ...);
void luaO_chunkid (char *out, const char *source, int len);


#endif
