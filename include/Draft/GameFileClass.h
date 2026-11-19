// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// GameFileClass.h (kept at its Draft path: src/unk/O00574365.cpp includes it): the W3D asset file adapter that W3DFileSystem hands to the renderer's loaders
// (Zero Hour's W3DDevice/GameClient/W3DFileSystem.h). It resolves a W3D or image name through the game's own file system and reads it through a File.
//
// Target facts (retail game.dat 1.06, RVAs):
//  - Allocation 0x214 (W3DFileSystem::Get_File 0x00078780 calls operator new with 0x214 before the constructor).
//  - Constructors 0x000782A5 and 0x00078737 store the vftable 0x007C6788 and clear the File pointer (+4), the exists flag (+8), the first byte of the
//    path (+9) and of the file name (+0x10D); the path buffer at +9 is passed to FileSystem::openFile by Open 0x000783F8, so the two buffers are
//    _MAX_PATH (260) bytes each: 9 + 260 = 0x10D, 0x10D + 260 = 0x211, padded to 0x214.
//  - FileClass's 18 slots (WWLib/wwfile.h) are all overridden: Create and Delete are the shared `xor eax,eax; inc eax; ret` fold 0x000910FB (it returns 1), Write is the
//    shared stub 0x005748B2 (src/unk/O00574365.cpp), Tell and the two date slots are inherited.
//  - Create and Delete return 1, not 0: a diagnostic image whose only difference from the rebuilt 364-file plan was these two bodies returning 0 diverged from retail on Fords of Isen II (seed 2, 0 of 4 frames equal from frame 25); with retail's `xor eax,eax; inc eax; ret` they were 4 of 4 equal to frame 100. The value comes from retail's vtable slots 3 and 4 (the fold 0x000910FB), never from a default.
//  - Destructor 0x000783C5: restores the vftable, calls Close, then the base destructor's vftable store.
#pragma once
#ifndef BFME2_GAMEFILECLASS_H
#define BFME2_GAMEFILECLASS_H

#include <stddef.h>
#include "Common/GameType.h"
#include "WWLib/wwfile.h"

class File;

class GameFileClass : public FileClass
{
public:
	GameFileClass();									// 0x000782A5
	GameFileClass(const char *filename);				// 0x00078737
	virtual ~GameFileClass();							// slot 0: deleting destructor 0x00078453, destructor 0x000783C5

	virtual const char *File_Name() const;				// slot 1, 0x000782BF
	virtual const char *Set_Name(const char *filename);	// slot 2, 0x0007846F
	virtual int Create() { return 1; }					// slot 3, the fold 0x000910FB: retail returns 1
	virtual int Delete() { return 1; }					// slot 4, the same fold (returns 1)
	virtual bool Is_Available(int forced = false);		// slot 5, 0x000782E1
	virtual bool Is_Open() const;						// slot 6, 0x000782E7
	virtual int Open(const char *filename, int rights = READ);	// slot 8, 0x000782F0
	virtual int Open(int rights = READ);				// slot 7, 0x000783F8
	virtual int Read(void *buffer, int size);			// slot 9, 0x0007831C
	virtual int Seek(int pos, int dir = SEEK_CUR);		// slot 10, 0x0007832F
	virtual int Size();									// slot 12, 0x00078363
	virtual int Write(const void *buffer, int size);	// slot 13, 0x005748B2 (src/unk/O00574365.cpp)
	virtual void Close();								// slot 14, 0x00078375

	File *m_theFile;									// +0x04
	Bool m_fileExists;									// +0x08
	char m_filePath[260];								// +0x09 (the path the file system opens)
	char m_filename[260];								// +0x10D (the name as given)
};
// SIZE 0x214
typedef char GameFileClassSize[(sizeof(GameFileClass) == 0x214) ? 1 : -1];
typedef char GameFileClassPathOffset[(offsetof(GameFileClass, m_filePath) == 0x09) ? 1 : -1];
typedef char GameFileClassNameOffset[(offsetof(GameFileClass, m_filename) == 0x10D) ? 1 : -1];

#endif
