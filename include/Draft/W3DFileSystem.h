// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// W3DFileSystem.h (kept at its Draft path with GameFileClass.h): the file factory the W3D asset manager asks for files (TheW3DFileSystem).
// Target facts (retail game.dat 1.06): FileFactoryClass's three slots (WWLib/ffactory.h): the deleting destructor 0x00078437, Get_File 0x00078780 and
// Return_File 0x004FC957 (the shared `delete file` body).
#pragma once
#ifndef BFME2_W3DFILESYSTEM_H
#define BFME2_W3DFILESYSTEM_H

#include "WWLib/ffactory.h"

class W3DFileSystem : public FileFactoryClass
{
public:
	virtual FileClass *Get_File(const char *filename);			// slot 1, 0x00078780: a new GameFileClass for the name
	virtual void Return_File(FileClass *file) { ::delete file; }	// slot 2, 0x004FC957
};
// SIZE 0x04

#endif
