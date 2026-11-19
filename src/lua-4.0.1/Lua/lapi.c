// cl-profile: upstream
// cl: -O2 -GR- -EHsc- /MD /Iinclude/upstream/lua-4.0.1-bfme/src /Iinclude/upstream/lua-4.0.1-bfme/include /Iinclude/upstream/lua-4.0.1/include /Iinclude/upstream/lua-4.0.1/src
// UPSTREAM: lua-4.0.1 ALTERED; see include/upstream/lua-4.0.1-bfme/PROVENANCE.md
// FUNCTION: BFME2 0x00746EA0
// FUNCTION: BFME2 0x00746EC0
// FUNCTION: BFME2 0x00746EE0
// FUNCTION: BFME2 0x00746F20
// FUNCTION: BFME2 0x00746F30
// FUNCTION: BFME2 0x00746F40
// FUNCTION: BFME2 0x00746F70
// FUNCTION: BFME2 0x00746FD0
// FUNCTION: BFME2 0x00747050
// FUNCTION: BFME2 0x007470A0
// FUNCTION: BFME2 0x007470D0
// FUNCTION: BFME2 0x007470F0
// FUNCTION: BFME2 0x00747130
// FUNCTION: BFME2 0x00747190
// FUNCTION: BFME2 0x007471D0
// FUNCTION: BFME2 0x00747210
// FUNCTION: BFME2 0x00747250
// FUNCTION: BFME2 0x007472B0
// FUNCTION: BFME2 0x00747320
// FUNCTION: BFME2 0x00747370
// FUNCTION: BFME2 0x007473B0
// FUNCTION: BFME2 0x00747400
// FUNCTION: BFME2 0x00747450
// FUNCTION: BFME2 0x00747490
// FUNCTION: BFME2 0x007474D0
// FUNCTION: BFME2 0x00747510
// FUNCTION: BFME2 0x00747540
// FUNCTION: BFME2 0x00747570
// FUNCTION: BFME2 0x007475A0
// FUNCTION: BFME2 0x007475E0
// FUNCTION: BFME2 0x00747650
// FUNCTION: BFME2 0x007476C0
// FUNCTION: BFME2 0x00747720
// FUNCTION: BFME2 0x00747770
// FUNCTION: BFME2 0x007477C0
// FUNCTION: BFME2 0x00747830
// FUNCTION: BFME2 0x00747860
// FUNCTION: BFME2 0x007478E0
// FUNCTION: BFME2 0x00747920
// FUNCTION: BFME2 0x00747950
// FUNCTION: BFME2 0x00747990
// FUNCTION: BFME2 0x007479F0
// FUNCTION: BFME2 0x00747A50
// FUNCTION: BFME2 0x00747A70
// FUNCTION: BFME2 0x00747B20
// FUNCTION: BFME2 0x00747B40
// FUNCTION: BFME2 0x00747B50
// FUNCTION: BFME2 0x00747B60
// FUNCTION: BFME2 0x00747B90
// FUNCTION: BFME2 0x00747BE0
// FUNCTION: BFME2 0x00747C10
// FUNCTION: BFME2 0x00747CB0
// FUNCTION: BFME2 0x00747D40
// FUNCTION: BFME2 0x00747D70
/* BFME-ALTERED: Lua 4.0.1's src/lapi.c (upstream SHA-256 7de4e815bf408b8916a71f785aa3aaac047e16edac047cd263eeeeef3ae9d841) altered to reproduce the fork retail game.dat links; every changed hunk is marked BFME-RETAIL. An ALTERED SOURCE VERSION of Lua, not the original (the original is include/upstream/lua-4.0.1/src/lapi.c); Lua's copyright notice, below, applies unchanged; see include/upstream/lua-4.0.1-bfme/PROVENANCE.md. */
/*
** $Id: lapi.c,v 1.110a 2000/10/30 12:50:09 roberto Exp $
** Lua API
** See Copyright Notice in lua.h
*/


#include <string.h>

#include "lua.h"

#include "lapi.h"
#include "ldo.h"
#include "lfunc.h"
#include "lgc.h"
#include "lmem.h"
#include "lobject.h"
#include "lstate.h"
#include "lstring.h"
#include "ltable.h"
#include "ltm.h"
#include "lvm.h"


const char lua_ident[] = "$Lua: " LUA_VERSION " " LUA_COPYRIGHT " $\n"
                               "$Authors: " LUA_AUTHORS " $";



#define Index(L,i)	((i) >= 0 ? (L->Cbase+((i)-1)) : (L->top+(i)))

#define api_incr_top(L)	incr_top




TObject *luaA_index (lua_State *L, int index) {
  return Index(L, index);
}


static TObject *luaA_indexAcceptable (lua_State *L, int index) {
  if (index >= 0) {
    TObject *o = L->Cbase+(index-1);
    if (o >= L->top) return NULL;
    else return o;
  }
  else return L->top+index;
}


void luaA_pushobject (lua_State *L, const TObject *o) {
  *L->top = *o;
  incr_top;
}

LUA_API int lua_stackspace (lua_State *L) {
  return (L->stack_last - L->top);
}



/*
** basic stack manipulation
*/


LUA_API int lua_gettop (lua_State *L) {
  return (L->top - L->Cbase);
}


LUA_API void lua_settop (lua_State *L, int index) {
  if (index >= 0)
    luaD_adjusttop(L, L->Cbase, index);
  else
    L->top = L->top+index+1;  /* index is negative */
}


LUA_API void lua_remove (lua_State *L, int index) {
  StkId p = luaA_index(L, index);
  while (++p < L->top) *(p-1) = *p;
  L->top--;
}


LUA_API void lua_insert (lua_State *L, int index) {
  StkId p = luaA_index(L, index);
  StkId q;
  for (q = L->top; q>p; q--)
    *q = *(q-1);
  *p = *L->top;
}


LUA_API void lua_pushvalue (lua_State *L, int index) {
  *L->top = *luaA_index(L, index);
  api_incr_top(L);
}



/*
** access functions (stack -> C)
*/


LUA_API int lua_type (lua_State *L, int index) {
  StkId o = luaA_indexAcceptable(L, index);
  return (o == NULL) ? LUA_TNONE : ttype(o);
}

LUA_API const char *lua_typename (lua_State *L, int t) {
  UNUSED(L);
  return (t == LUA_TNONE) ? "no value" : luaO_typenames[t];
}


LUA_API int lua_iscfunction (lua_State *L, int index) {
  StkId o = luaA_indexAcceptable(L, index);
  return (o == NULL) ? 0 : iscfunction(o);
}

LUA_API int lua_isnumber (lua_State *L, int index) {
  TObject *o = luaA_indexAcceptable(L, index);
  return (o == NULL) ? 0 : (tonumber(o) == 0);
}

LUA_API int lua_isstring (lua_State *L, int index) {
  int t = lua_type(L, index);
  return (t == LUA_TSTRING || t == LUA_TNUMBER);
}


LUA_API int lua_tag (lua_State *L, int index) {
  StkId o = luaA_indexAcceptable(L, index);
  return (o == NULL) ? LUA_NOTAG : luaT_tag(o);
}

LUA_API int lua_equal (lua_State *L, int index1, int index2) {
  StkId o1 = luaA_indexAcceptable(L, index1);
  StkId o2 = luaA_indexAcceptable(L, index2);
  if (o1 == NULL || o2 == NULL) return 0;  /* index out-of-range */
  else return luaO_equalObj(o1, o2);
}

LUA_API int lua_lessthan (lua_State *L, int index1, int index2) {
  StkId o1 = luaA_indexAcceptable(L, index1);
  StkId o2 = luaA_indexAcceptable(L, index2);
  if (o1 == NULL || o2 == NULL) return 0;  /* index out-of-range */
  else return luaV_lessthan(L, o1, o2, L->top);
}



LUA_API double lua_tonumber (lua_State *L, int index) {
  StkId o = luaA_indexAcceptable(L, index);
  return (o == NULL || tonumber(o)) ? 0 : nvalue(o);
}

LUA_API const char *lua_tostring (lua_State *L, int index) {
  StkId o = luaA_indexAcceptable(L, index);
  return (o == NULL || tostring(L, o)) ? NULL : svalue(o);
}

LUA_API size_t lua_strlen (lua_State *L, int index) {
  StkId o = luaA_indexAcceptable(L, index);
  return (o == NULL || tostring(L, o)) ? 0 : tsvalue(o)->len;
}

LUA_API lua_CFunction lua_tocfunction (lua_State *L, int index) {
  StkId o = luaA_indexAcceptable(L, index);
  return (o == NULL || !iscfunction(o)) ? NULL : clvalue(o)->f.c;
}

LUA_API void *lua_touserdata (lua_State *L, int index) {
  StkId o = luaA_indexAcceptable(L, index);
  return (o == NULL || ttype(o) != LUA_TUSERDATA) ? NULL :
                                                    tsvalue(o)->u.d.value;
}

LUA_API const void *lua_topointer (lua_State *L, int index) {
  StkId o = luaA_indexAcceptable(L, index);
  if (o == NULL) return NULL;
  switch (ttype(o)) {
    case LUA_TTABLE: 
      return hvalue(o);
    case LUA_TFUNCTION:
      return clvalue(o);
    default: return NULL;
  }
}



/*
** push functions (C -> stack)
*/


LUA_API void lua_pushnil (lua_State *L) {
  ttype(L->top) = LUA_TNIL;
  api_incr_top(L);
}


/* BFME-RETAIL 0x00747540: pushes a boolean (tag 6); not in stock Lua; retail 0x00747540, the name is the ledger's (bfmeGo1039E), not recovered; later Lua calls this lua_pushboolean */
LUA_API void bfmeGo1039E (lua_State *L, int b) {
  ttype(L->top) = 6;
  bvalue(L->top) = b;
  api_incr_top(L);
}


LUA_API void lua_pushnumber (lua_State *L, double n) {
  nvalue(L->top) = n;
  ttype(L->top) = LUA_TNUMBER;
  api_incr_top(L);
}


LUA_API void lua_pushlstring (lua_State *L, const char *s, size_t len) {
  tsvalue(L->top) = luaS_newlstr(L, s, len);
  ttype(L->top) = LUA_TSTRING;
  api_incr_top(L);
}


LUA_API void lua_pushstring (lua_State *L, const char *s) {
  if (s == NULL)
    lua_pushnil(L);
  else
    lua_pushlstring(L, s, strlen(s));
}


LUA_API void lua_pushcclosure (lua_State *L, lua_CFunction fn, int n) {
  luaV_Cclosure(L, fn, n);
}


LUA_API void lua_pushusertag (lua_State *L, void *u, int tag) {
  /* ORDER LUA_T */
  if (!(tag == LUA_ANYTAG || tag == LUA_TUSERDATA || validtag(tag)))
    luaO_verror(L, "invalid tag for a userdata (%d)", tag);
  tsvalue(L->top) = luaS_createudata(L, u, tag);
  ttype(L->top) = LUA_TUSERDATA;
  api_incr_top(L);
}



/*
** get functions (Lua -> stack)
*/


LUA_API void lua_getglobal (lua_State *L, const char *name) {
  StkId top = L->top;
  *top = *luaV_getglobal(L, luaS_new(L, name));
  L->top = top;
  api_incr_top(L);
}


LUA_API void lua_gettable (lua_State *L, int index) {
  StkId t = Index(L, index);
  StkId top = L->top;
  *(top-1) = *luaV_gettable(L, t);
  L->top = top;  /* tag method may change top */
}


LUA_API void lua_rawget (lua_State *L, int index) {
  StkId t = Index(L, index);
  LUA_ASSERT(ttype(t) == LUA_TTABLE, "table expected");
  *(L->top - 1) = *luaH_get(L, hvalue(t), L->top - 1);
}


LUA_API void lua_rawgeti (lua_State *L, int index, int n) {
  StkId o = Index(L, index);
  LUA_ASSERT(ttype(o) == LUA_TTABLE, "table expected");
  *L->top = *luaH_getnum(hvalue(o), n);
  api_incr_top(L);
}


LUA_API void lua_getglobals (lua_State *L) {
  hvalue(L->top) = L->gt;
  ttype(L->top) = LUA_TTABLE;
  api_incr_top(L);
}


LUA_API int lua_getref (lua_State *L, int ref) {
  if (ref == LUA_REFNIL)
    ttype(L->top) = LUA_TNIL;
  else if (0 <= ref && ref < L->refSize &&
          (L->refArray[ref].st == LOCK || L->refArray[ref].st == HOLD))
    *L->top = L->refArray[ref].o;
  else
    return 0;
  api_incr_top(L);
  return 1;
}


LUA_API void lua_newtable (lua_State *L, int size) {  /* BFME-RETAIL 0x007478E0: size parameter, retail lua_newtable 0x007478E0 */
  hvalue(L->top) = luaH_new(L, 0, size);  /* BFME-RETAIL 0x007478E9: size is the third argument, pushed first, retail 0x007478E9..0x007478ED */
  ttype(L->top) = LUA_TTABLE;
  api_incr_top(L);
}



/*
** set functions (stack -> Lua)
*/


LUA_API void lua_setglobal (lua_State *L, const char *name) {
  StkId top = L->top;
  luaV_setglobal(L, luaS_new(L, name));
  L->top = top-1;  /* remove element from the top */
}


LUA_API void lua_settable (lua_State *L, int index) {
  StkId t = Index(L, index);
  StkId top = L->top;
  luaV_settable(L, t, top-2);
  L->top = top-2;  /* pop index and value */
}


LUA_API void lua_rawset (lua_State *L, int index) {
  StkId t = Index(L, index);
  LUA_ASSERT(ttype(t) == LUA_TTABLE, "table expected");
  *luaH_set(L, hvalue(t), L->top-2) = *(L->top-1);
  L->top -= 2;
}


LUA_API void lua_rawseti (lua_State *L, int index, int n) {
  StkId o = Index(L, index);
  LUA_ASSERT(ttype(o) == LUA_TTABLE, "table expected");
  *luaH_setint(L, hvalue(o), n) = *(L->top-1);
  L->top--;
}


LUA_API void lua_setglobals (lua_State *L) {
  StkId newtable = --L->top;
  LUA_ASSERT(ttype(newtable) == LUA_TTABLE, "table expected");
  L->gt = hvalue(newtable);
}


LUA_API int lua_ref (lua_State *L,  int lock) {
  int ref;
  if (ttype(L->top-1) == LUA_TNIL)
    ref = LUA_REFNIL;
  else {
    if (L->refFree != NONEXT) {  /* is there a free place? */
      ref = L->refFree;
      L->refFree = L->refArray[ref].st;
    }
    else {  /* no more free places */
      luaM_growvector(L, L->refArray, L->refSize, 1, struct Ref,
                      "reference table overflow", MAX_INT);
      L->nblocks += sizeof(struct Ref);
      ref = L->refSize++;
    }
    L->refArray[ref].o = *(L->top-1);
    L->refArray[ref].st = lock ? LOCK : HOLD;
  }
  L->top--;
  return ref;
}


/*
** "do" functions (run Lua code)
** (most of them are in ldo.c)
*/

LUA_API void lua_rawcall (lua_State *L, int nargs, int nresults) {
  luaD_call(L, L->top-(nargs+1), nresults);
}


/*
** Garbage-collection functions
*/

/* GC values are expressed in Kbytes: #bytes/2^10 */
#define GCscale(x)		((int)((x)>>10))
#define GCunscale(x)		((unsigned long)(x)<<10)

LUA_API int lua_getgcthreshold (lua_State *L) {
  return GCscale(L->GCthreshold);
}

LUA_API int lua_getgccount (lua_State *L) {
  return GCscale(L->nblocks);
}

LUA_API void lua_setgcthreshold (lua_State *L, int newthreshold) {
  if (newthreshold > GCscale(ULONG_MAX))
    L->GCthreshold = ULONG_MAX;
  else
    L->GCthreshold = GCunscale(newthreshold);
  luaC_checkGC(L);
}


/*
** miscellaneous functions
*/

LUA_API void lua_settag (lua_State *L, int tag) {
  luaT_realtag(L, tag);
  switch (ttype(L->top-1)) {
    case LUA_TTABLE:
      hvalue(L->top-1)->htag = tag;
      break;
    case LUA_TUSERDATA:
      tsvalue(L->top-1)->u.d.tag = tag;
      break;
    default:
      luaO_verror(L, "cannot change the tag of a %.20s",
                  luaO_typename(L->top-1));
  }
}


LUA_API void lua_unref (lua_State *L, int ref) {
  if (ref >= 0) {
    LUA_ASSERT(ref < L->refSize && L->refArray[ref].st < 0, "invalid ref");
    L->refArray[ref].st = L->refFree;
    L->refFree = ref;
  }
}


LUA_API int lua_next (lua_State *L, int index) {
  StkId t = luaA_index(L, index);
  Node *n;
  LUA_ASSERT(ttype(t) == LUA_TTABLE, "table expected");
  n = luaH_next(L, hvalue(t), luaA_index(L, -1));
  if (n) {
    *(L->top-1) = *key(n);
    *L->top = *val(n);
    api_incr_top(L);
    return 1;
  }
  else {  /* no more elements */
    L->top -= 1;  /* remove key */
    return 0;
  }
}


LUA_API int lua_getn (lua_State *L, int index) {
  Hash *h = hvalue(luaA_index(L, index));
  const TObject *value = luaH_getstr(h, luaS_new(L, "n"));  /* value = h.n */
  if (ttype(value) == LUA_TNUMBER)
    return (int)nvalue(value);
  else {
    Number max = 0;
    int i = h->size;
    Node *n = h->node;
    while (i--) {
      if (ttype(key(n)) == LUA_TNUMBER &&
          ttype(val(n)) != LUA_TNIL &&
          nvalue(key(n)) > max)
        max = nvalue(key(n));
      n++;
    }
    return (int)max;
  }
}


LUA_API void lua_concat (lua_State *L, int n) {
  StkId top = L->top;
  luaV_strconc(L, n, top);
  L->top = top-(n-1);
  luaC_checkGC(L);
}


LUA_API void *lua_newuserdata (lua_State *L, size_t size) {
  TString *ts = luaS_newudata(L, (size==0) ? 1 : size, NULL);
  tsvalue(L->top) = ts;
  ttype(L->top) = LUA_TUSERDATA;
  api_incr_top(L);
  return ts->u.d.value;
}

