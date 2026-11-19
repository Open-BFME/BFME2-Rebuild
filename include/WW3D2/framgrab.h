// The movie-capture role and GetBuffer name follow Zero Hour's framgrab.h.
// BFME II allocates a batch of frames and writes them through Video for Windows.
// The constructor and deleting destructor agree on the buffer and AVI ownership.
#ifndef BFME2_FRAMEGRAB_H
#define BFME2_FRAMEGRAB_H

struct IAVIFile;
struct IAVIStream;

class FrameGrabClass
{
public:
	FrameGrabClass(const char *filename, int width, int height, int bitdepth,
		float framerate, int buffer_count, bool compressed);
	virtual ~FrameGrabClass();

	long *GetBuffer();

protected:
	void Flush_Buffered_Frames();

	int FrameSize;
	int BufferCount;
	unsigned char *Buffer;
	int WrittenFrames;
	int BufferedFrames;
	IAVIFile *AVIFile;
	IAVIStream *AVIStream;
};

#endif
