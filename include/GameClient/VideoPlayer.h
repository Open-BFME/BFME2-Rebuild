#pragma once
#include "Common/SubsystemInterface.h"
#include "GameClient/Video.h"
class VideoStreamInterface;
class SubtitleEntry;
class SubtitleManager;
struct FieldParse;
typedef SubtitleEntry *(__cdecl *VideoPlayerSubtitleFactory)(AsciiString *,int,
    const AsciiString &,unsigned int,int,int,int,int,int);
// Target table8E48A0 has29 slots; ctor6892D0/dtor6892F0 establish this
// 0xC-byte SubsystemInterface-derived interface. name: donor (ZH), supported
// by identical service ordering and the concrete VideoPlayer's base stores.
// Old Rva006892F0/Rva006892D0Derived views are this interface.
class VideoPlayerInterface : public SubsystemInterface {
public:
    virtual ~VideoPlayerInterface();
    virtual void init()=0;
    virtual void reset()=0;
    virtual void update()=0;
    virtual void deinit()=0; //14; name: donor (ZH)
    virtual void loseFocus()=0; //15; name: donor (ZH)
    virtual void regainFocus()=0; //16; name: donor (ZH)
    virtual VideoStreamInterface *open(AsciiString,unsigned int)=0; //17
    virtual VideoStreamInterface *firstStream()=0; //18, getter6891A0
    virtual void closeAllStreams()=0; //19
    virtual void addVideo(Video *)=0; //20,+50
    virtual void removeVideo(Video *)=0; //21
    virtual int getNumVideos()=0; //22; name: donor (ZH)
    // MSVC7.1 reverses introducing overloads: int is23, string is24.
    virtual const Video *getVideo(AsciiString)=0;
    virtual const Video *getVideo(int)=0;
    //25,+64. Donor name getFieldParse. Temporarily disambiguated because
    // SubsystemInterface's independently inferred slot6 currently has that
    // same signature; using it here would incorrectly override slot6.
    virtual const FieldParse *getVideoFieldParse() const=0;
    virtual void notifyVideoPlayerOfNewProvider(bool)=0; //26; name: donor (ZH)
    virtual SubtitleManager *getSubTitleMgrForVideo(const AsciiString &)=0; //27
    virtual void preserveStreamsOnNextReset()=0; //28, inferred: sets18, reset consumes it
};
// Target689320 stores callbackC/provider10/subtitle factory14; ctor90781
// derives from it and allocation4C590 requests1C. Table8E4918 has29 slots.
class VideoPlayer : public VideoPlayerInterface {
public:
    VideoPlayer(AsciiString (__cdecl *)(),int,VideoPlayerSubtitleFactory);
    virtual ~VideoPlayer();
    virtual void init();
    virtual void reset();
    virtual void update();
    virtual void deinit();
    virtual void loseFocus();
    virtual void regainFocus();
    virtual VideoStreamInterface *open(AsciiString,unsigned int);
    virtual VideoStreamInterface *firstStream();
    virtual void closeAllStreams();
    virtual void addVideo(Video *);
    virtual void removeVideo(Video *);
    virtual int getNumVideos();
    virtual const Video *getVideo(AsciiString);
    virtual const Video *getVideo(int);
    virtual const FieldParse *getVideoFieldParse() const;
    virtual void notifyVideoPlayerOfNewProvider(bool);
    virtual SubtitleManager *getSubTitleMgrForVideo(const AsciiString &);
    virtual void preserveStreamsOnNextReset();
protected:
    AsciiString (__cdecl *m_movieName)(); //C, invoked689596
    int m_provider; //10
    VideoPlayerSubtitleFactory m_createSubtitleEntry; //14, call689A34
    bool m_preserveStreamsOnReset; //18, consumed689180/set4F5FA7
};
typedef char VideoPlayerInterfaceSizeCheck[sizeof(VideoPlayerInterface)==0xC ? 1 : -1];
typedef char VideoPlayerSizeCheck[sizeof(VideoPlayer)==0x1C ? 1 : -1];
extern VideoPlayer *TheVideoPlayer;