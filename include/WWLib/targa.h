// targa.h: WWLib's Targa (TGA) image file reader and writer.
// Target facts (retail game.dat): every TARGA.CPP row is a plain member (no vptr). The file is
// held through a FileClass pointer opened with a FileFactoryClass: Close calls FileClass::Close
// then FileFactoryClass::Return_File on the global factory, and File_Read/File_Seek/File_Write
// are one-line forwards to the file's slots. The packed 18-byte file header is the first member,
// so the file pointer is at +0x14 (the header plus alignment), then access mode +0x18, flags
// +0x1C, image buffer +0x20, palette buffer +0x24 and the 495-byte extension area +0x28.
// The record layouts and codes below are the Truevision TGA file format's own: an 18-byte
// header, optional colour map, pixels (raw or run-length coded) and, in version 2 files, a
// 26-byte footer that points at a 495-byte extension area.
#pragma once
#ifndef BFME2_TARGA_H
#define BFME2_TARGA_H

#include "WWLib/wwfile.h"
#include "WWLib/ffactory.h"

#pragma pack(push, 1)

typedef struct _TGAHeader
{
	char idLength;			// bytes of image id text after the header
	char colorMapType;		// 1 when a colour map follows
	char imageType;			// one of the TGA_* image type codes
	short cmapStart;
	short cmapLength;
	char cmapDepth;			// bits per colour map entry
	short xOffset;
	short yOffset;
	short width;
	short height;
	char pixelDepth;		// bits per pixel
	char imageDescriptor;		// attribute bits and the TGAIDF_* origin bits
} TGAHeader;

// Image types.
#define TGA_NOIMAGE		0
#define TGA_CMAPPED		1
#define TGA_TRUECOLOR		2
#define TGA_MONO		3
#define TGA_CMAPPED_ENCODED	9
#define TGA_TRUECOLOR_ENCODED	10
#define TGA_MONO_ENCODED	11

// Image descriptor bits.
#define TGAIDF_XORIGIN		(1 << 4)
#define TGAIDF_YORIGIN		(1 << 5)

// Access modes for Open.
#define TGA_READMODE		0
#define TGA_WRITEMODE		1
#define TGA_RDWRMODE		2

// Error codes returned by the file operations.
#define TGAERR_OPEN		-1
#define TGAERR_READ		-2
#define TGAERR_WRITE		-3
#define TGAERR_SYNTAX		-4
#define TGAERR_NOMEM		-5
#define TGAERR_NOTSUPPORTED	-6

// Flags, in the flags word and in the Load/Save selectors.
#define TGAF_IMAGE		(1 << 0)
#define TGAF_PAL		(1 << 1)
#define TGAF_COMPRESS		(1 << 2)
#define TGAF_TGA2		(1 << 3)

#define TGA_BytesPerPixel(bits) (((bits) + 7) >> 3)

#define TGA2_SIGNATURE "TRUEVISION-XFILE"

typedef struct _TGA2Footer
{
	long extensionOffset;		// file offset of the extension area, 0 when absent
	long developerOffset;
	char signature[16];
	char reservedChar;
	char terminator;
} TGA2Footer;

typedef struct _TGA2DateStamp
{
	short month;
	short day;
	short year;
} TGA2DateStamp;

typedef struct _TGA2TimeStamp
{
	short hour;
	short minute;
	short second;
} TGA2TimeStamp;

typedef struct _TGA2SoftVer
{
	short number;
	char letter;
} TGA2SoftVer;

typedef struct _TGA2Ratio
{
	short numerator;
	short denominator;
} TGA2Ratio;

typedef struct _TGA2Extension
{
	short extSize;
	char authorName[41];
	char authorComment[324];
	TGA2DateStamp date;
	TGA2TimeStamp time;
	char jobName[41];
	TGA2TimeStamp jobTime;
	char softwareId[41];
	TGA2SoftVer softwareVersion;
	long keyColor;
	TGA2Ratio aspect;
	TGA2Ratio gamma;
	long colorCorrectionOffset;
	long postageStampOffset;
	long scanLineOffset;
	char attributes;
} TGA2Extension;

#pragma pack(pop)

class Targa
{
public:
	Targa();							// 0x00614210
	~Targa();							// 0x00614250

	long Open(const char *name, long mode);				// 0x006142B0
	void Close();							// 0x00613DF0

	long Load(const char *name, char *palette, char *image, bool invert_image = true);	// 0x006147B0
	long Load(const char *name, long flags, bool invert_image = true);			// 0x00614950
	long Save(const char *name, long flags, bool addextension = false);			// 0x00614A90

	void XFlip();							// 0x00613E20
	void YFlip();							// 0x00613EF0

	char *SetImage(char *buffer);					// 0x00613FA0
	char *GetImage() const { return mImage; }
	char *SetPalette(char *buffer);					// 0x00613FF0
	char *GetPalette() const { return mPalette; }

	bool IsCompressed();						// 0x00614040
	TGA2Extension *GetExtension();					// 0x00614050

	TGAHeader Header;			// +0x00

protected:
	FileClass *TGAFile;			// +0x14
	long mAccess;				// +0x18
	long mFlags;				// +0x1C
	char *mImage;				// +0x20
	char *mPalette;			// +0x24
	TGA2Extension mExtension;		// +0x28

private:
	long DecodeImage();						// 0x006144B0
	long EncodeImage();						// 0x006145F0
	void InvertImage();						// 0x00614060

	void Clear_File();						// 0x006140D0
	bool Is_File_Open();						// 0x006140E0
	bool File_Open_Read(const char *name);				// 0x006140F0
	bool File_Open_Write(const char *name);				// 0x00614140
	bool File_Open_ReadWrite(const char *name);			// 0x00614180
	int File_Seek(int pos, int dir);				// 0x006141D0
	int File_Read(void *buffer, int size);				// 0x006141E0
	int File_Write(void *buffer, int size);				// 0x006141F0
};

#endif
