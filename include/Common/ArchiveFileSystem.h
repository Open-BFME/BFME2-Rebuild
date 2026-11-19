// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// ArchiveFileSystem.h: the archive (.big) file system interface (Zero Hour's ArchiveFileSystem.h; the target is
// Win32BIGFileSystem). openFile at +0xC (0x00600C34), doesFileExist at +0x14 (0x00600D7D).
#pragma once
#ifndef BFME2_ARCHIVEFILESYSTEM_H
#define BFME2_ARCHIVEFILESYSTEM_H

class File;
struct FileInfo;

class ArchiveFileSystem
{
public:
	virtual ~ArchiveFileSystem();															// slot 0
	virtual void archiveSlot1() = 0;														// slot 1 (+4)
	virtual File *openFileWide(const unsigned short *path, int access, int unknown) = 0;	// slot 2 (+8): FileSystem 0x00600676 forwards its wide-path open here
	virtual File *openFile(const char *filename, int access, int unknown) = 0;				// slot 3 (+0xC)
	virtual void archiveSlot4() = 0;														// slot 4 (+0x10)
	virtual bool doesFileExist(const char *filename) const = 0;							// slot 5 (+0x14)
	virtual void archiveSlot6() = 0;														// slot 6 (+0x18)
	virtual void archiveSlot7() = 0;														// slot 7 (+0x1C)
	virtual void archiveSlot8() = 0;														// slot 8 (+0x20)
	virtual void archiveSlot9() = 0;														// slot 9 (+0x24)
	virtual bool getFileInfo(const char *filename, FileInfo *info) const = 0;				// slot 10 (+0x28): 0x00600E3A's archive call
	virtual bool doesFileExistWide(const char *path) = 0;									// slot 11 (+0x2C): FileSystem 0x006006A9 forwards its wide-path test here (the text is passed as it comes)
	virtual bool archiveSlot12(const char *path) = 0;										// slot 12 (+0x30): 0x00600695's forward
};

extern ArchiveFileSystem *TheArchiveFileSystem;	// 0x00A06E5C

#endif
