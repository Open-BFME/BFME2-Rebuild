// Recorder.h: the replay recorder and player (TheRecorder). Evidence, in order: the binary
// (createRecorder 0x0037BCC4 allocates 0xE7C and runs the constructor 0x0037BB98; its table VA
// 0x00C18828 has 14 slots; init 0x0037B326 stores FILE +0x10, position +0x18, mode +0x1C;
// isMultiplayer 0x0037B18C calls GameInfo::getSlot on the member at +0x24; update 0x0037DC36
// switches on the mode at +0x1C: 0 and 2 -> updateRecord, 1 -> playback update; initControls
// 0x0037BD81 hides ReplayControl.wnd's parent unless the mode is 1); the map's own method names;
// then Zero Hour's recorder by role (names only).
// Table: slots 0 (deleting dtor 0x0037BD48), 1 init, 9 reset 0x0037B3D1, 10 update 0x0037DC36 are the
// class's own; every other slot is SubsystemInterface's. The constructor/destructor rows keep the
// ledger's placeholder owner (Rva0037BBED) until the map renames them.
#pragma once
#ifndef BFME2_RECORDER_H
#define BFME2_RECORDER_H

#include <stddef.h>
#include <stdio.h>
#include "Common/SubsystemInterface.h"
#include "Common/UnicodeString.h"
#include "Common/AsciiString.h"
#include "Common/GameMessageArgument.h"
#include "GameNetwork/SkirmishGameInfo.h"

class GameMessage;
class Rva0037D076;
class Rva0037B7C7;

// The values update 0x0037DC36 and initControls 0x0037BD81 test (names are Zero Hour's).
enum RecorderModeType { RECORDERMODETYPE_RECORD = 0, RECORDERMODETYPE_PLAYBACK = 1, RECORDERMODETYPE_NONE = 2 };

class RecorderClass : public SubsystemInterface
{
public:
	RecorderClass();							// 0x0037BB98
	virtual ~RecorderClass();					// 0x0037BBED
	virtual void init();						// slot 1, 0x0037B326
	virtual void reset();						// slot 9, 0x0037B3D1
	virtual void update();						// slot 10, 0x0037DC36

	RecorderModeType getMode();					// 0x0030F2C7: a folded `mov eax,[ecx+0x1C]` getter; callers (switchControlBarStage 0x0031B99A) call it out of line
	UnicodeString getCurrentReplayFilename();	// 0x0037B5A0: the replay being played back, else the empty string (map class Rva0037B5A0)
	bool isMultiplayer();						// 0x0037B18C
	void initControls();						// 0x0037BD81
	void stopRecording();						// 0x0037B42D
	void logCRCMismatch();						// 0x0037ADB6
	void logGameEnd();							// 0x0037AE4E
	void rva0037AEE2();							// 0x0037AEE2
	void logPlayerDisconnect(UnicodeString playerName, int slot);	// 0x0037B287
	bool playbackFile(UnicodeString fileName);	// 0x0037D1E6
	bool copyCurrentRecordingToFile(UnicodeString *source, UnicodeString *destination);	// 0x0037D6E8
	void writeArgument(int type, GameMessageArgumentType argument);	// 0x0037AF05
	void readArgument(int type, GameMessage *message);	// 0x0037AF88
	UnicodeString readUnicodeString();			// 0x0037BCA8: reads the next 0-terminated wide string from the replay file (forwards m_file to the file-level readUnicodeString 0x0037B648)
	static UnicodeString getReplayDir();		// 0x0037B9D4 (Zero Hour's name; map class-less Rva0037B9D4Get): the user data path plus "Replays\\"
	static UnicodeString getReplayExtention();	// 0x0037BA48 (Zero Hour's name; map class-less Rva0037BA48Get): ".BfME2Replay"

protected:
	void writeToFile(GameMessage *message);	// 0x0037B461
	void appendNextCommand();					// 0x0037B7F6
	void updatePlayback();						// 0x0037BC65 (Zero Hour's name; map class Rva0037B7C7)
	void cullBadCommands();						// 0x0037B146 (Zero Hour's name; map class Rva0037B146)
	void stopPlayback();						// 0x0037B3F7 (Zero Hour's name; map class Rva0037B3F7)
	void readNextFrame();						// 0x0037B7C7 (Zero Hour's name; map class Rva0037B7C7)
	void updateRecord();						// 0x0037D42B
	AsciiString readAsciiString();				// 0x0037B70A (map class Rva0037BBED)
	void logGameStart(AsciiString mapName);		// 0x0037B1FA
	void startRecording(int, int, int, int);	// 0x0037C57E

public:
	Rva0037D076 *m_playbackInfo;				// +0x0C (playbackFile 0x0037D1E6 stores it; the CRC dispatcher 0x0037D0A7 reads it)
	FILE *m_file;								// +0x10
	UnicodeString m_fileName;					// +0x14
	int m_currentFilePosition;					// +0x18
	int m_mode;									// +0x1C (RecorderModeType)
	UnicodeString m_currentReplayFilename;		// +0x20
	SkirmishGameInfo m_gameInfo;				// +0x24 (the record isMultiplayer reads its slots from)
	int m_unk0xE60;								// +0xE60
	int m_unk0xE64;								// +0xE64
	int m_unk0xE68;								// +0xE68
	int m_unk0xE6C;								// +0xE6C
	bool m_doingAnalysis;						// +0xE70
	unsigned char m_pad0xE71[3];				// +0xE71
	int m_originalGameMode;						// +0xE74
	unsigned int m_nextFrame;					// +0xE78
};
typedef char RecorderClassSize[(sizeof(RecorderClass) == 0xE7C) ? 1 : -1];
typedef char RecorderClassGameInfoOffset[(offsetof(RecorderClass, m_gameInfo) == 0x24) ? 1 : -1];
typedef char RecorderClassModeOffset[(offsetof(RecorderClass, m_mode) == 0x1C) ? 1 : -1];

extern RecorderClass *TheRecorder;
RecorderClass *createRecorder();
UnicodeString readUnicodeString(FILE *file);	// 0x0037B648: a file-level function, not a member (the member forwarder 0x0037BCA8 calls it)

#endif
