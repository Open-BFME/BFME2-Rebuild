/* GameSpy SDK (the snapshot retail BFME 2 links); GameSpy Industries' own copyright header is kept in each file.
   Used with the project owner's permission, see NOTICE. Retail linked the DLL CRT, so libc calls are __imp__ indirect
   (/MD makes those call sites byte-exact) and built the SDK __cdecl. Win32 declarations come from
   include/upstream/gamespy-sdk/shims/. */
/*
gpiUnique.h
GameSpy Presence SDK 
Dan "Mr. Pants" Schoenblum

Copyright 1999-2003 GameSpy Industries, Inc

18002 Skypark Circle
Irvine, California 92614
949.798.4200 (Tel)
949.798.4299 (Fax)
devsupport@gamespy.com

***********************************************************************
Please see the GameSpy Presence SDK documentation for more information
**********************************************************************/

#ifndef _GPIUNIQUE_H_
#define _GPIUNIQUE_H_

//INCLUDES
//////////
#include "gpi.h"

//FUNCTIONS
///////////
GPResult gpiRegisterUniqueNick(
  GPConnection * connection,
  const char uniquenick[GP_UNIQUENICK_LEN],
  const char cdkey[GP_CDKEY_LEN],
  GPEnum blocking,
  GPCallback callback,
  void * param
);

GPResult gpiProcessRegisterUniqueNick(
  GPConnection * connection,
  GPIOperation * operation,
  const char * input
);

#endif
