// cl-profile: upstream
// cl: -O2 -GR- -EHsc- /MD /Iinclude/upstream/lua-4.0.1-bfme/src /Iinclude/upstream/lua-4.0.1-bfme/include /Iinclude/upstream/lua-4.0.1/include /Iinclude/upstream/lua-4.0.1/src
// UPSTREAM: lua-4.0.1 ALTERED; see include/upstream/lua-4.0.1-bfme/PROVENANCE.md
// FUNCTION: BFME2 0x00747DC0
// FUNCTION: BFME2 0x00747DE0
// FUNCTION: BFME2 0x00747E00
// FUNCTION: BFME2 0x007480B0
// FUNCTION: BFME2 0x00748140
// FUNCTION: BFME2 0x007481B0
// FUNCTION: BFME2 0x00748220
// FUNCTION: BFME2 0x00748250
// FUNCTION: BFME2 0x00748280
// FUNCTION: BFME2 0x00748320
// FUNCTION: BFME2 0x00748350
// FUNCTION: BFME2 0x00748380
/* BFME-ALTERED: Lua 4.0.1's src/lib/ldblib.c (upstream SHA-256 41345c982f8a2f440e256170663b0adacf779ddabf5662e257667b0ed4c2f381) altered to reproduce the fork retail game.dat links; every changed hunk is marked BFME-RETAIL. An ALTERED SOURCE VERSION of Lua, not the original (the original is include/upstream/lua-4.0.1/src/lib/ldblib.c); Lua's copyright notice, below, applies unchanged; see include/upstream/lua-4.0.1-bfme/PROVENANCE.md. */
/*
** $Id: ldblib.c,v 1.29 2000/11/06 17:58:38 roberto Exp $
** Interface from Lua to its debug API
** See Copyright Notice in lua.h
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lua.h"

#include "lauxlib.h"
#include "luadebug.h"
#include "lualib.h"



static void settabss (lua_State *L, const char *i, const char *v) {
  lua_pushstring(L, i);
  lua_pushstring(L, v);
  lua_settable(L, -3);
}


static void settabsi (lua_State *L, const char *i, int v) {
  lua_pushstring(L, i);
  lua_pushnumber(L, v);
  lua_settable(L, -3);
}


static int getinfo (lua_State *L) {
  lua_Debug ar;
  const char *options = luaL_opt_string(L, 2, "flnSu");
  char buff[20];
  if (lua_isnumber(L, 1)) {
    if (!lua_getstack(L, (int)lua_tonumber(L, 1), &ar)) {
      lua_pushnil(L);  /* level out of range */
      return 1;
    }
  }
  else if (lua_isfunction(L, 1)) {
    lua_pushvalue(L, 1);
    sprintf(buff, ">%.10s", options);
    options = buff;
  }
  else
    luaL_argerror(L, 1, "function or level expected");
  if (!lua_getinfo(L, options, &ar))
    luaL_argerror(L, 2, "invalid option");
  lua_newtable(L, 0);  /* BFME-RETAIL 0x00747ECC: size argument 0, retail getinfo 0x00747ECC */
  for (; *options; options++) {
    switch (*options) {
      case 'S':
        settabss(L, "source", ar.source);
        if (ar.source)
          settabss(L, "short_src", ar.short_src);
        settabsi(L, "linedefined", ar.linedefined);
        settabss(L, "what", ar.what);
        break;
      case 'l':
        settabsi(L, "currentline", ar.currentline);
        break;
      case 'u':
        settabsi(L, "nups", ar.nups);
        break;
      case 'n':
        settabss(L, "name", ar.name);
        settabss(L, "namewhat", ar.namewhat);
        break;
      case 'f':
        lua_pushstring(L, "func");
        lua_pushvalue(L, -3);
        lua_settable(L, -3);
        break;
    }
  }
  return 1;  /* return table */
}
    

static int getlocal (lua_State *L) {
  lua_Debug ar;
  const char *name;
  if (!lua_getstack(L, luaL_check_int(L, 1), &ar))  /* level out of range? */
    luaL_argerror(L, 1, "level out of range");
  name = lua_getlocal(L, &ar, luaL_check_int(L, 2));
  if (name) {
    lua_pushstring(L, name);
    lua_pushvalue(L, -2);
    return 2;
  }
  else {
    lua_pushnil(L);
    return 1;
  }
}


static int setlocal (lua_State *L) {
  lua_Debug ar;
  if (!lua_getstack(L, luaL_check_int(L, 1), &ar))  /* level out of range? */
    luaL_argerror(L, 1, "level out of range");
  luaL_checkany(L, 3);
  lua_pushstring(L, lua_setlocal(L, &ar, luaL_check_int(L, 2)));
  return 1;
}



/* dummy variables (to define unique addresses) */
static char key1, key2;
#define KEY_CALLHOOK	(&key1)
#define KEY_LINEHOOK	(&key2)


static void hookf (lua_State *L, void *key) {
  lua_getregistry(L);
  lua_pushuserdata(L, key);
  lua_gettable(L, -2);
  if (lua_isfunction(L, -1)) {
    lua_pushvalue(L, 1);
    lua_rawcall(L, 1, 0);
  }
  else
    lua_pop(L, 1);  /* pop result from gettable */
  lua_pop(L, 1);  /* pop table */
}


static void callf (lua_State *L, lua_Debug *ar) {
  lua_pushstring(L, ar->event);
  hookf(L, KEY_CALLHOOK);
}


static void linef (lua_State *L, lua_Debug *ar) {
  lua_pushnumber(L, ar->currentline);
  hookf(L, KEY_LINEHOOK);
}


static void sethook (lua_State *L, void *key, lua_Hook hook,
                     lua_Hook (*sethookf)(lua_State * L, lua_Hook h)) {
  lua_settop(L, 1);
  if (lua_isnil(L, 1))
    (*sethookf)(L, NULL);
  else if (lua_isfunction(L, 1))
    (*sethookf)(L, hook);
  else
    luaL_argerror(L, 1, "function expected");
  lua_getregistry(L);
  lua_pushuserdata(L, key);
  lua_pushvalue(L, -1);  /* dup key */
  lua_gettable(L, -3);   /* get old value */
  lua_pushvalue(L, -2);  /* key (again) */
  lua_pushvalue(L, 1);
  lua_settable(L, -5);  /* set new value */
}


static int setcallhook (lua_State *L) {
  sethook(L, KEY_CALLHOOK, callf, lua_setcallhook);
  return 1;
}


static int setlinehook (lua_State *L) {
  sethook(L, KEY_LINEHOOK, linef, lua_setlinehook);
  return 1;
}


static const struct luaL_reg dblib[] = {
  {"getlocal", getlocal},
  {"getinfo", getinfo},
  {"setcallhook", setcallhook},
  {"setlinehook", setlinehook},
  {"setlocal", setlocal}
};


LUALIB_API void lua_dblibopen (lua_State *L) {
  luaL_openl(L, dblib);
}

