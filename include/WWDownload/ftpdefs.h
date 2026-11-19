// ftpdefs.h: Cftp return codes (Zero Hour's WWDownload/ftpdefs.h, GPL-3.0-or-later).
// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
#pragma once
#ifndef BFME2_FTPDEFS_H
#define BFME2_FTPDEFS_H

// CFtp return codes.
#define FTP_SUCCEEDED		S_OK
#define	FTP_FAILED			MAKE_HRESULT( SEVERITY_ERROR, FACILITY_ITF, 1 )
#define FTP_TRYING			MAKE_HRESULT( SEVERITY_ERROR, FACILITY_ITF, 2 )

#endif
