// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// LocalFileSystem.h: the disk file system interface (Zero Hour's LocalFileSystem.h; the target is Win32LocalFileSystem, vftable
// 0x0087A9A8). Slot numbers are byte offsets/4 as FileSystem's virtual calls use them: openFile at +8 (0x00600C34 calls it with
// the path, the access flags and a third word), doesFileExist at +0xC (0x00600D7D).
#pragma once
#ifndef BFME2_LOCALFILESYSTEM_H
#define BFME2_LOCALFILESYSTEM_H

class File;
struct FileInfo;

class LocalFileSystem
{
public:
	virtual ~LocalFileSystem();																// slot 0
	virtual void localSlot1() = 0;															// slot 1 (+4)
	virtual File *openFile(const char *filename, int access, int unknown) = 0;				// slot 2 (+8)
	virtual bool doesFileExist(const char *filename) const = 0;							// slot 3 (+0xC)
	virtual bool getFileInfo(const char *filename, FileInfo *info) const = 0;				// slot 4 (+0x10): 0x00600E3A's local call
};

extern LocalFileSystem *TheLocalFileSystem;	// 0x00A06E54

#endif
