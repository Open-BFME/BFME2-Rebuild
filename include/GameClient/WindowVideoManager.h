// WindowVideoManager.h: plays a movie into a window's video buffer. WindowVideo is
// one window's playing movie; WindowVideoManager keeps them in a hash map keyed by
// window and walks them each frame.
//
// Layout evidence (retail game.dat): WindowVideo's constructor 0x0053F0D0 clears
// +0x00..+0x08, releases the string at +0x0C and stores 1 at +0x10 (allocation size
// 0x14, 0x0053F63D pushes 0x14); init 0x0053F165 stores the window, the movie name,
// the play type's value into +0x00 and the stream into +0x08, and the state 3 at
// +0x10; setWindowState 0x0053F06C stores the state at +0x10. WindowVideoManager's
// constructor 0x0053F85C builds a SubsystemInterface, the hash map at +0x0C (an
// STLport hash_map<const GameWindow *, WindowVideo *> whose hash functor is the
// nested hashConstGameWindowPtr; the map's own mangled names carry that), then clears
// the bytes at +0x20/+0x21 (size 0x24). Its table 0x008694A4 overrides init, reset and
// update. State values 1..4 are the constants setWindowState's callers push (stop 0x0053F331,
// pause 0x0053F2B9, resume/play 0x0053F301, hide 0x0053F2DD); names are Zero Hour's.
#pragma once
#ifndef BFME2_WINDOWVIDEOMANAGER_H
#define BFME2_WINDOWVIDEOMANAGER_H

#include "Common/SubsystemInterface.h"
#include "Common/AsciiString.h"
#include "Common/StlHashMap.h"

class GameWindow;
class VideoBuffer;
class VideoStreamInterface;

enum WindowVideoStates
{
	WINDOW_VIDEO_STATE_STOP = 1,
	WINDOW_VIDEO_STATE_PAUSE = 2,
	WINDOW_VIDEO_STATE_PLAY = 3,
	WINDOW_VIDEO_STATE_HIDE = 4
};

// Values from WindowVideoManager::update 0x0053F53C: type 1 loops (it asks the stream to
// loop), type 2 pauses when the stream wraps to frame 0, every other value stops there.
// Names are Zero Hour's.
enum WindowVideoPlayType
{
	WINDOW_PLAY_MOVIE_ONCE = 0,
	WINDOW_PLAY_MOVIE_LOOP = 1,
	WINDOW_PLAY_MOVIE_SHOW_LAST_FRAME = 2,
	WINDOW_PLAY_MOVIE_MAX
};

class WindowVideo
{
public:
	WindowVideo();															// 0x0053F0D0
	~WindowVideo();															// 0x0053F10E
	void init(GameWindow *win, AsciiString movieName, WindowVideoPlayType playType,
		VideoStreamInterface *videoStream);									// 0x0053F165
	void setWindowState(WindowVideoStates state);							// 0x0053F06C
	VideoBuffer *getVideoBuffer();											// 0x0053F0BF

	int m_playType;															// +0x00 (WindowVideoPlayType)
	GameWindow *m_win;														// +0x04
	VideoStreamInterface *m_videoStream;									// +0x08
	AsciiString m_movieName;												// +0x0C
	int m_state;															// +0x10 (WindowVideoStates)
};
// SIZE 0x14

class WindowVideoManager : public SubsystemInterface
{
public:
	WindowVideoManager();													// 0x0053F85C
	virtual ~WindowVideoManager();											// 0x0053F45C
	virtual void init();													// 0x0053F2A4
	virtual void reset();													// 0x0053F4E4
	virtual void update();													// 0x0053F53C

	void playMovie(GameWindow *win, AsciiString movieName, WindowVideoPlayType playType);	// 0x0053F63D
	void pauseMovie(GameWindow *win);										// 0x0053F2B9
	void hideMovie(GameWindow *win);										// 0x0053F2DD
	void resumeMovie(GameWindow *win);										// 0x0053F301
	void stopMovie(GameWindow *win);										// 0x0053F331
	int getWinState(GameWindow *win);										// 0x0053F355
	void stopAndRemoveMovie(GameWindow *win);								// 0x0053F3FF
	void stopAllMovies();													// 0x0053F721
	void pauseAllMovies();													// 0x0053F761
	void resumeAllMovies();													// 0x0053F7A1

	struct hashConstGameWindowPtr
	{
		unsigned int operator()(const GameWindow *p) const { return (unsigned int)p; }
	};

	typedef _STL::hash_map<const GameWindow *, WindowVideo *, hashConstGameWindowPtr, _STL::equal_to<const GameWindow *> > WindowVideoMap;
	WindowVideoMap m_playingVideos;	// +0x0C
	bool m_stopAllMovies;													// +0x20
	bool m_pauseAllMovies;													// +0x21
};
// SIZE 0x24

#endif
