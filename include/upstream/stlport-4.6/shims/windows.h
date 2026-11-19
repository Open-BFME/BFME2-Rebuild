/* The Win32 declarations STLport 4.6's library sources use (src/fstream.cpp and src/c_locale_win32/c_locale_win32.c
 * include <windows.h>): the kernel32 file, mapping and national-language calls with their constants and the few
 * structures they touch, with the documented calling convention, import linkage and layout (CRITICAL_SECTION must be
 * 24 bytes: msvcr71's ioinfo, which fstream.cpp walks, embeds one). Retail's library objects imported these from
 * kernel32.dll; no Platform SDK header is used or reproduced. */
#ifndef STLPORT_SHIM_WINDOWS_H
#define STLPORT_SHIM_WINDOWS_H

typedef int BOOL;
typedef unsigned long DWORD;
typedef unsigned short WORD;
typedef long LONG;
typedef __int64 LONGLONG;
typedef void *HANDLE;
typedef void *LPVOID;
typedef const void *LPCVOID;
typedef const char *LPCSTR;
typedef DWORD *LPDWORD;
typedef unsigned int UINT;
typedef unsigned char BYTE;
typedef WORD *LPWORD;
typedef char *LPSTR;
typedef const unsigned short *LPCWSTR;
typedef unsigned short *LPWSTR;
typedef int *LPBOOL;
typedef DWORD LCID;
typedef DWORD LCTYPE;
typedef LONG *PLONG;
struct _SECURITY_ATTRIBUTES;
struct _OVERLAPPED;
typedef struct _SECURITY_ATTRIBUTES *LPSECURITY_ATTRIBUTES;
typedef struct _SECURITY_ATTRIBUTES *PSECURITY_ATTRIBUTES;
typedef struct _OVERLAPPED *LPOVERLAPPED;

typedef union _LARGE_INTEGER {
  struct {
    DWORD LowPart;
    LONG HighPart;
  };
  LONGLONG QuadPart;
} LARGE_INTEGER;

typedef struct _CRITICAL_SECTION {
  void *DebugInfo;
  LONG LockCount;
  LONG RecursionCount;
  HANDLE OwningThread;
  HANDLE LockSemaphore;
  DWORD SpinCount;
} CRITICAL_SECTION;

#define MAX_DEFAULTCHAR 2
#define MAX_LEADBYTES 12
#define MAX_PATH 260
typedef struct _cpinfo {
  UINT MaxCharSize;
  BYTE DefaultChar[MAX_DEFAULTCHAR];
  BYTE LeadByte[MAX_LEADBYTES];
} CPINFO, *LPCPINFO;

typedef struct _OSVERSIONINFOA {
  DWORD dwOSVersionInfoSize;
  DWORD dwMajorVersion;
  DWORD dwMinorVersion;
  DWORD dwBuildNumber;
  DWORD dwPlatformId;
  char szCSDVersion[128];
} OSVERSIONINFO, *LPOSVERSIONINFO;

typedef struct _SYSTEM_INFO {
  union {
    DWORD dwOemId;
    struct {
      WORD wProcessorArchitecture;
      WORD wReserved;
    };
  };
  DWORD dwPageSize;
  LPVOID lpMinimumApplicationAddress;
  LPVOID lpMaximumApplicationAddress;
  DWORD dwActiveProcessorMask;
  DWORD dwNumberOfProcessors;
  DWORD dwProcessorType;
  DWORD dwAllocationGranularity;
  WORD wProcessorLevel;
  WORD wProcessorRevision;
} SYSTEM_INFO, *LPSYSTEM_INFO;

#define FALSE 0
#define TRUE 1
#define WINAPI __stdcall
#define WINBASEAPI __declspec(dllimport)

#define CALLBACK __stdcall
#define VER_PLATFORM_WIN32_NT 2
#define LCID_INSTALLED 0x00000001
#define CP_ACP 0
#define CT_CTYPE1 0x00000001
#define MB_PRECOMPOSED 0x00000001
#define WC_SEPCHARS 0x00000020
#define WC_DEFAULTCHAR 0x00000040
#define WC_COMPOSITECHECK 0x00000200
#define LCMAP_LOWERCASE 0x00000100
#define LCMAP_UPPERCASE 0x00000200
#define LCMAP_SORTKEY 0x00000400
#define LCMAP_LINGUISTIC_CASING 0x01000000
#define LOCALE_USER_DEFAULT 0x0400
#define LOCALE_SENGLANGUAGE 0x00001001
#define LOCALE_SENGCOUNTRY 0x00001002
#define LOCALE_STIMEFORMAT 0x00001003
#define LOCALE_IDEFAULTANSICODEPAGE 0x00001004
#define LOCALE_SABBREVLANGNAME 0x00000003
#define LOCALE_SABBREVCTRYNAME 0x00000007
#define LOCALE_IDEFAULTCODEPAGE 0x0000000B
#define LOCALE_SDECIMAL 0x0000000E
#define LOCALE_STHOUSAND 0x0000000F
#define LOCALE_SGROUPING 0x00000010
#define LOCALE_SCURRENCY 0x00000014
#define LOCALE_SINTLSYMBOL 0x00000015
#define LOCALE_ICURRDIGITS 0x00000019
#define LOCALE_IINTLCURRDIGITS 0x0000001A
#define LOCALE_STIME 0x0000001E
#define LOCALE_SSHORTDATE 0x0000001F
#define LOCALE_SLONGDATE 0x00000020
#define LOCALE_S1159 0x00000028
#define LOCALE_S2359 0x00000029
#define LOCALE_SDAYNAME1 0x0000002A
#define LOCALE_SDAYNAME7 0x00000030
#define LOCALE_SABBREVDAYNAME1 0x00000031
#define LOCALE_SABBREVDAYNAME7 0x00000037
#define LOCALE_SMONTHNAME1 0x00000038
#define LOCALE_SMONTHNAME12 0x00000043
#define LOCALE_SABBREVMONTHNAME1 0x00000044
#define LOCALE_SABBREVMONTHNAME12 0x0000004F
#define LOCALE_SPOSITIVESIGN 0x00000050
#define LOCALE_SNEGATIVESIGN 0x00000051
#define LOCALE_IPOSSIGNPOSN 0x00000052
#define LOCALE_INEGSIGNPOSN 0x00000053
#define LOCALE_IPOSSYMPRECEDES 0x00000054
#define LOCALE_IPOSSEPBYSPACE 0x00000055
#define LOCALE_INEGSYMPRECEDES 0x00000056
#define LOCALE_INEGSEPBYSPACE 0x00000057

typedef BOOL (CALLBACK *LOCALE_ENUMPROCA)(LPSTR);

#define NO_ERROR 0
#define INVALID_HANDLE_VALUE ((HANDLE)(LONG)-1)

#define GENERIC_READ 0x80000000L
#define GENERIC_WRITE 0x40000000L
#define FILE_SHARE_READ 0x00000001
#define FILE_SHARE_WRITE 0x00000002
#define OPEN_EXISTING 3
#define OPEN_ALWAYS 4
#define FILE_ATTRIBUTE_NORMAL 0x00000080
#define FILE_BEGIN 0
#define FILE_CURRENT 1
#define FILE_END 2
#define FILE_TYPE_DISK 0x0001
#define FILE_TYPE_REMOTE 0x8000
#define PAGE_READONLY 0x02
#define FILE_MAP_READ 0x0004

#define CreateFile CreateFileA
#define GetVersionEx GetVersionExA
#define CreateFileMapping CreateFileMappingA

#ifdef __cplusplus
extern "C" {
#endif

WINBASEAPI HANDLE WINAPI CreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
                                     LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition,
                                     DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
WINBASEAPI BOOL WINAPI CloseHandle(HANDLE hObject);
WINBASEAPI DWORD WINAPI GetFileType(HANDLE hFile);
WINBASEAPI DWORD WINAPI GetFileSize(HANDLE hFile, LPDWORD lpFileSizeHigh);
WINBASEAPI DWORD WINAPI GetLastError(void);
WINBASEAPI void WINAPI GetSystemInfo(LPSYSTEM_INFO lpSystemInfo);
WINBASEAPI DWORD WINAPI SetFilePointer(HANDLE hFile, LONG lDistanceToMove, PLONG lpDistanceToMoveHigh,
                                       DWORD dwMoveMethod);
WINBASEAPI BOOL WINAPI SetEndOfFile(HANDLE hFile);
WINBASEAPI BOOL WINAPI ReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead,
                                LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);
WINBASEAPI BOOL WINAPI WriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite,
                                 LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped);
WINBASEAPI HANDLE WINAPI CreateFileMappingA(HANDLE hFile, LPSECURITY_ATTRIBUTES lpAttributes, DWORD flProtect,
                                            DWORD dwMaximumSizeHigh, DWORD dwMaximumSizeLow, LPCSTR lpName);
WINBASEAPI LPVOID WINAPI MapViewOfFile(HANDLE hFileMappingObject, DWORD dwDesiredAccess, DWORD dwFileOffsetHigh,
                                       DWORD dwFileOffsetLow, DWORD dwNumberOfBytesToMap);
WINBASEAPI BOOL WINAPI UnmapViewOfFile(LPCVOID lpBaseAddress);

WINBASEAPI int WINAPI GetLocaleInfoA(LCID Locale, LCTYPE LCType, LPSTR lpLCData, int cchData);
WINBASEAPI int WINAPI MultiByteToWideChar(UINT CodePage, DWORD dwFlags, LPCSTR lpMultiByteStr, int cbMultiByte,
                                          LPWSTR lpWideCharStr, int cchWideChar);
WINBASEAPI int WINAPI WideCharToMultiByte(UINT CodePage, DWORD dwFlags, LPCWSTR lpWideCharStr, int cchWideChar,
                                          LPSTR lpMultiByteStr, int cbMultiByte, LPCSTR lpDefaultChar,
                                          LPBOOL lpUsedDefaultChar);
WINBASEAPI int WINAPI LCMapStringA(LCID Locale, DWORD dwMapFlags, LPCSTR lpSrcStr, int cchSrc, LPSTR lpDestStr,
                                   int cchDest);
WINBASEAPI int WINAPI LCMapStringW(LCID Locale, DWORD dwMapFlags, LPCWSTR lpSrcStr, int cchSrc, LPWSTR lpDestStr,
                                   int cchDest);
WINBASEAPI BOOL WINAPI GetCPInfo(UINT CodePage, LPCPINFO lpCPInfo);
WINBASEAPI BOOL WINAPI GetStringTypeA(LCID Locale, DWORD dwInfoType, LPCSTR lpSrcStr, int cchSrc, LPWORD lpCharType);
WINBASEAPI BOOL WINAPI GetStringTypeW(DWORD dwInfoType, LPCWSTR lpSrcStr, int cchSrc, LPWORD lpCharType);
WINBASEAPI int WINAPI CompareStringA(LCID Locale, DWORD dwCmpFlags, LPCSTR lpString1, int cchCount1, LPCSTR lpString2,
                                     int cchCount2);
WINBASEAPI int WINAPI CompareStringW(LCID Locale, DWORD dwCmpFlags, LPCWSTR lpString1, int cchCount1, LPCWSTR lpString2,
                                     int cchCount2);
/* retail's import table names this call's kernel32 export `lstrcmpi` (not `lstrcmpiA`) */
#define lstrcmpiA lstrcmpi
WINBASEAPI int WINAPI lstrcmpi(LPCSTR lpString1, LPCSTR lpString2);
WINBASEAPI BOOL WINAPI GetVersionExA(LPOSVERSIONINFO lpVersionInformation);
WINBASEAPI BOOL WINAPI EnumSystemLocalesA(LOCALE_ENUMPROCA lpLocaleEnumProc, DWORD dwFlags);

#ifdef __cplusplus
}
#endif

#endif
