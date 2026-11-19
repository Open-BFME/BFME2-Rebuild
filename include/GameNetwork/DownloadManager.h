// DownloadManager.h: the GameSpy download queue's manager (Zero Hour's GameNetwork/DownloadManager.h, GPL-3.0-or-later).
// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
//
// Target facts (retail game.dat): the constructor 0x005E08AB stores the table 0x00C77944, zeroes +0x10 and +0x14
// (the two UnicodeStrings), builds the queue list at +0x18, allocates a 0x5C8-byte CDownload (WWDownload/Download.h)
// into +0x08 with this as its listener, clears the bytes +0x0C and +0x0D, fetches the idle status text into +0x14,
// sets +0x04 and runs WSAStartup(0x0202). update 0x005E048B tail-calls the download's slot 0 (PumpMessages),
// OnEnd 0x005E0492 sets the byte at +0x0D.
#pragma once
#ifndef BFME2_DOWNLOADMANAGER_H
#define BFME2_DOWNLOADMANAGER_H

#include <list>
#include <windows.h>
#include "Common/AsciiString.h"
#include "Common/UnicodeString.h"
#include "WWDownload/downloaddefs.h"
#include "WWDownload/Download.h"

class QueuedDownload
{
public:
	AsciiString server;
	AsciiString userName;
	AsciiString password;
	AsciiString file;
	AsciiString localFile;
	AsciiString regKey;
	Bool tryResume;
};

/////////////////////////////////////////////////////////////////////////////
// DownloadManager

class DownloadManager : public IDownload
{
public:
	DownloadManager();
	virtual ~DownloadManager();

public:
	HRESULT update(void);

	virtual HRESULT OnError(int error);
	virtual HRESULT OnEnd();
	virtual HRESULT OnQueryResume();
	virtual HRESULT OnProgressUpdate(int bytesread, int totalsize, int timetaken, int timeleft);
	virtual HRESULT OnStatusUpdate(int status);
	virtual HRESULT downloadFile(AsciiString server, AsciiString username, AsciiString password, AsciiString file, AsciiString localfile, AsciiString regkey, Bool tryResume);
	AsciiString getLastLocalFile(void);
	Bool isDone(void) { return m_sawEnd || m_wasError; }
	Bool isOk(void) { return m_sawEnd; }
	Bool wasError(void) { return m_wasError; }
	UnicodeString getStatusString(void) { return m_statusString; }
	UnicodeString getErrorString(void) { return m_errorString; }

	void queueFileForDownload(AsciiString server, AsciiString username, AsciiString password, AsciiString file, AsciiString localfile, AsciiString regkey, Bool tryResume);
	Bool isFileQueuedForDownload(void) { return !m_queuedDownloads.empty(); }
	HRESULT downloadNextQueuedFile(void);

private:
	Bool m_winsockInit;				// +0x04
	CDownload *m_download;			// +0x08
	Bool m_wasError;				// +0x0C
	Bool m_sawEnd;					// +0x0D
	UnicodeString m_errorString;	// +0x10
	UnicodeString m_statusString;	// +0x14

protected:
	_STL::list<QueuedDownload> m_queuedDownloads;	// +0x18
};

extern DownloadManager *TheDownloadManager;

#endif // BFME2_DOWNLOADMANAGER_H
