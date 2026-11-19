// ftp.h: the Cftp class (Zero Hour's WWDownload/ftp.h, GPL-3.0-or-later).
// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// Target facts: Cftp has a vtable (the virtual destructor) at +0, so the command socket sits at +4 and the data
// socket at +8: CloseSockets 0x006C9EA0 closes +8 then +4; ZeroStuff 0x006C9ED0 clears the strings at +0x38,
// +0xB8, +0x138, +0x1B8, +0x238, +0x2B8, +0x338 (128 bytes each) and the integers behind them.
#pragma once
#ifndef BFME2_FTP_H
#define BFME2_FTP_H

#include <windows.h>
#include <winsock.h>
#include <stdio.h>

#include "WWDownload/ftpdefs.h"

// FTP server return codes.  See RFC 959

#define FTPREPLY_SERVEROK		220
#define FTPREPLY_PASSWORD		331
#define FTPREPLY_LOGGEDIN		230
#define FTPREPLY_PORTOK			200
#define FTPREPLY_TYPEOK			200
#define FTPREPLY_RESTARTOK		350
#define FTPREPLY_CWDOK			250
#define FTPREPLY_OPENASCII		150
#define FTPREPLY_OPENBINARY		150
#define FTPREPLY_COMPLETE		226
#define FTPREPLY_CONTROLCLOSED	421

// Temporary download file name

#define FTP_TEMPFILENAME	"..\\__~DOWN_L~D"

/////////////////////////////////////////////////////////////////////////////
// Cftp
class Cftp
{
private:
	friend class CDownload;

	int		m_iCommandSocket;							// Socket for commands
	int		m_iDataSocket;								// Socket for data

	struct sockaddr_in m_CommandSockAddr;				// Address for commands
	struct sockaddr_in m_DataSockAddr;					// Address for data

	int		m_iFilePos;									// Byte offset into file
	int		m_iBytesRead;								// Number of bytes downloaded
	int		m_iFileSize;								// Total size of the file
	char	m_szRemoteFilePath[128];
	char	m_szRemoteFileName[128];
	char	m_szLocalFilePath[128];
	char	m_szLocalFileName[128];
	char	m_szServerName[128];
	char	m_szUserName[128];
	char	m_szPassword[128];
	FILE *	m_pfLocalFile;
	int		m_iStatus;

	int		m_sendNewPortStatus;
	int		m_findStart;

	HRESULT	SendCommand( LPCSTR pCommand, int iSize );
public:
	void		CloseSockets(void);				// 0x006C9EA0
	void		ZeroStuff(void);
	int		AsyncGetHostByName( char * szName, struct sockaddr_in &address );
	int		SendNewPort();
	int		OpenDataConnection();						// 0x006CA430
	void		CloseDataConnection();			// 0x006CA470
	int		SendData( char * pData, int iSize );
	int		RecvData( char * pData, int iSize );
				// Convert a local filename into a temp filename to download into
	void		GetDownloadFilename( const char* localname, char* downloadname);	// 0x006CA4E0

public:
	Cftp();
	virtual ~Cftp();

public:
	HRESULT ConnectToServer(LPCSTR szServerName);
	HRESULT DisconnectFromServer();

	HRESULT LoginToServer( LPCSTR szUserName, LPCSTR szPassword );
	HRESULT LogoffFromServer( void );

	HRESULT FindFile( LPCSTR szRemoteFileName, int * piSize );

	HRESULT FileRecoveryPosition( LPCSTR szLocalFileName, LPCSTR szRegistryRoot );
	HRESULT RestartFrom( int i ) { m_iFilePos = i; return FTP_SUCCEEDED;  };

	HRESULT GetNextFileBlock( LPCSTR szLocalFileName, int * piTotalRead );

	HRESULT RecvReply( LPCSTR pReplyBuffer, int iSize, int * piRetCode );

};

#endif //__FTP_H_
