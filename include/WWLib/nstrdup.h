// nstrdup.h: string duplicate that tolerates a null source.
// Target facts: nstrdup 0x00619880 (68 bytes); INIClass::Initialize stores nstrdup("<unknown>")
// as the file name and Put_String and Load copy their section and entry names with it.
#pragma once
#ifndef BFME2_NSTRDUP_H
#define BFME2_NSTRDUP_H

char *nstrdup(const char *str);			// 0x00619880

#endif
