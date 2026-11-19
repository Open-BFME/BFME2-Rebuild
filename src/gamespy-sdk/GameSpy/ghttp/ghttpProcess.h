/* GameSpy SDK (the snapshot retail BFME 2 links); GameSpy Industries' own copyright header is kept in each file.
   Used with the project owner's permission, see NOTICE. Retail linked the DLL CRT, so libc calls are __imp__ indirect
   (/MD makes those call sites byte-exact) and built the SDK __cdecl. Win32 declarations come from
   include/upstream/gamespy-sdk/shims/. */
/*
GameSpy GHTTP SDK 
Dan "Mr. Pants" Schoenblum
dan@gamespy.com

Copyright 1999-2001 GameSpy Industries, Inc

18002 Skypark Circle
Irvine, California 92614
949.798.4200 (Tel)
949.798.4299 (Fax)
devsupport@gamespy.com
*/

#ifndef _GHTTPPROCESS_H_
#define _GHTTPPROCESS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ghttpMain.h"
#include "ghttpConnection.h"

void ghiDoHostLookup
(
	GHIConnection * connection
);

void ghiDoConnecting
(
	GHIConnection * connection
);

void ghiDoSendingRequest
(
	GHIConnection * connection
);

void ghiDoPosting
(
	GHIConnection * connection
);

void ghiDoWaiting
(
	GHIConnection * connection
);

void ghiDoReceivingStatus
(
	GHIConnection * connection
);

void ghiDoReceivingHeaders
(
	GHIConnection * connection
);

void ghiDoReceivingFile
(
	GHIConnection * connection
);

#ifdef __cplusplus
}
#endif

#endif
