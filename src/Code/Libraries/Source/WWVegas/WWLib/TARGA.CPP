// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// cl: /O2
// Tier B; object=O00613DF0 evidence=Z flags=align=A
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "WWLib/targa.h"

// FUNCTION: BFME2 0x00613DF0 size=39 name=?Close@Targa@@QAEXXZ evidence=N:ledger
void Targa::Close()
{
	if (TGAFile != NULL) {
		TGAFile->Close();
		_TheFileFactory->Return_File(TGAFile);
		TGAFile = NULL;
	}
}

// FUNCTION: BFME2 0x00613E20 size=195 name=?XFlip@Targa@@QAEXXZ evidence=N:ledger
void Targa::XFlip()
{
	char bpp = TGA_BytesPerPixel(Header.pixelDepth);
	long row;
	for (row = 0; row < Header.height; row++) {
		char *left = mImage + (Header.width * bpp) * row;
		char *right = left + (Header.width * bpp - bpp);
		long col;
		for (col = 0; col < Header.width / 2; col++) {
			long k;
			for (k = 0; k < bpp; k++) {
				char a = left[k];
				char b = right[k];
				left[k] = b;
				right[k] = a;
			}
			left += bpp;
			right -= bpp;
		}
	}
}

// FUNCTION: BFME2 0x00613EF0 size=166 name=?YFlip@Targa@@QAEXXZ evidence=N:ledger
void Targa::YFlip()
{
	char bpp = TGA_BytesPerPixel(Header.pixelDepth);
	long row;
	for (row = 0; row < (Header.height >> 1); row++) {
		char *top = mImage + (Header.width * row) * bpp;
		char *bottom = mImage + (Header.width * (Header.height - 1)) * bpp;
		bottom -= (Header.width * row) * bpp;
		long n;
		for (n = 0; n < Header.width * bpp; n++) {
			char a = *top;
			char b = *bottom;
			*top = b;
			*bottom = a;
			top++;
			bottom++;
		}
	}
}

// FUNCTION: BFME2 0x00613FA0 size=69 name=?SetImage@Targa@@QAEPADPAD@Z evidence=N:ledger
char *Targa::SetImage(char *buffer)
{
	char *previous = NULL;
	if (mImage != NULL && (mFlags & TGAF_IMAGE)) {
		free(mImage);
		mImage = NULL;
		mFlags &= ~TGAF_IMAGE;
	}
	if (mImage != NULL) previous = mImage;
	mImage = buffer;
	return previous;
}

// FUNCTION: BFME2 0x00613FF0 size=69 name=?SetPalette@Targa@@QAEPADPAD@Z evidence=N:ledger
char *Targa::SetPalette(char *buffer)
{
	char *previous = NULL;
	if (mPalette != NULL && (mFlags & TGAF_PAL)) {
		free(mPalette);
		mPalette = NULL;
		mFlags &= ~TGAF_PAL;
	}
	if (mPalette != NULL) previous = mPalette;
	mPalette = buffer;
	return previous;
}

// FUNCTION: BFME2 0x00614040 size=8 name=?IsCompressed@Targa@@QAE_NXZ evidence=N:ledger
bool Targa::IsCompressed()
{
	return Header.imageType > 8 ? true : false;
}

// FUNCTION: BFME2 0x00614050 size=13 name=?GetExtension@Targa@@QAEPAU_TGA2Extension@@XZ evidence=N:ledger
TGA2Extension *Targa::GetExtension()
{
	if (mFlags & TGAF_TGA2) return &mExtension;
	return NULL;
}

// FUNCTION: BFME2 0x00614060 size=99 name=?InvertImage@Targa@@AAEXXZ evidence=N:ledger
void Targa::InvertImage()
{
	char *pixel = mImage;
	long bpp = TGA_BytesPerPixel(Header.pixelDepth);
	long remaining = Header.width * Header.height;
	if (bpp > 2) {
		while (remaining > 0) {
			long k;
			for (k = 0; k < bpp / 2; k++) {
				char tmp = pixel[k];
				pixel[k] = pixel[(bpp - 1) - k];
				pixel[(bpp - 1) - k] = tmp;
			}
			remaining--;
			pixel += bpp;
		}
	}
}

// FUNCTION: BFME2 0x006140D0 size=8 name=?Clear_File@Targa@@AAEXXZ evidence=N:ledger
void Targa::Clear_File()
{
	TGAFile = NULL;
}

// FUNCTION: BFME2 0x006140E0 size=9 name=?Is_File_Open@Targa@@AAE_NXZ evidence=N:ledger
bool Targa::Is_File_Open()
{
	return TGAFile != NULL;
}

// FUNCTION: BFME2 0x006140F0 size=65 name=?File_Open_Read@Targa@@AAE_NPBD@Z evidence=N:ledger
bool Targa::File_Open_Read(const char *name)
{
	TGAFile = _TheFileFactory->Get_File(name);
	if (TGAFile != NULL && TGAFile->Is_Available()) return TGAFile->Open(FileClass::READ) != 0;
	return false;
}

// FUNCTION: BFME2 0x00614140 size=50 name=?File_Open_Write@Targa@@AAE_NPBD@Z evidence=N:ledger
bool Targa::File_Open_Write(const char *name)
{
	TGAFile = _TheWritingFileFactory->Get_File(name);
	if (TGAFile != NULL) return TGAFile->Open(FileClass::WRITE) != 0;
	return false;
}

// FUNCTION: BFME2 0x00614180 size=65 name=?File_Open_ReadWrite@Targa@@AAE_NPBD@Z evidence=N:ledger
bool Targa::File_Open_ReadWrite(const char *name)
{
	TGAFile = _TheWritingFileFactory->Get_File(name);
	if (TGAFile != NULL && TGAFile->Is_Available()) return TGAFile->Open(FileClass::READ | FileClass::WRITE) != 0;
	return false;
}

// FUNCTION: BFME2 0x006141D0 size=8 name=?File_Seek@Targa@@AAEHHH@Z evidence=N:ledger
int Targa::File_Seek(int pos, int dir)
{
	return TGAFile->Seek(pos, dir);
}

// FUNCTION: BFME2 0x006141E0 size=8 name=?File_Read@Targa@@AAEHPAXH@Z evidence=N:ledger
int Targa::File_Read(void *buffer, int size)
{
	return TGAFile->Read(buffer, size);
}

// FUNCTION: BFME2 0x006141F0 size=8 name=?File_Write@Targa@@AAEHPAXH@Z evidence=N:ledger
int Targa::File_Write(void *buffer, int size)
{
	return TGAFile->Write(buffer, size);
}

// FUNCTION: BFME2 0x00614200 size=5 name=?rva00614200@@YAPAXPAX@Z evidence=?:placeholder
void *rva00614200(void *p)
{
	return p;
}

// FUNCTION: BFME2 0x00614210 size=52 name=??0Targa@@QAE@XZ evidence=N:ledger
Targa::Targa()
{
	mImage = NULL;
	mPalette = NULL;
	Clear_File();
	mAccess = TGA_READMODE;
	mFlags = 0;
	memset(&Header, 0, sizeof(TGAHeader));
	memset(&mExtension, 0, sizeof(TGA2Extension));
}

// FUNCTION: BFME2 0x00614250 size=85 name=??1Targa@@QAE@XZ evidence=N:ledger
Targa::~Targa()
{
	Close();
	if (mPalette != NULL && (mFlags & TGAF_PAL)) free(mPalette);
	if (mImage != NULL && (mFlags & TGAF_IMAGE)) free(mImage);
}

// FUNCTION: BFME2 0x006142B0 size=508 name=?Open@Targa@@QAEJPBDJ@Z evidence=N:ledger
long Targa::Open(const char *name, long mode)
{
	TGA2Footer footer;
	long got;
	long err = 0;
	memset(&footer, 0, sizeof(footer));
	if (Is_File_Open() && mAccess == mode) return 0;
	Close();
	mAccess = mode;
	mFlags &= ~TGAF_TGA2;
	switch (mode) {
	case TGA_READMODE:
		if (File_Open_Read(name)) {
			// The footer sits in the last 26 bytes; when its signature matches and it names
			// an extension area, that area is read too.
			if (File_Seek(-26, SEEK_END) == -1) err = TGAERR_READ;
			if (err == 0) {
				if (File_Read(&footer, sizeof(TGA2Footer)) != sizeof(TGA2Footer)) {
					err = TGAERR_READ;
				} else {
					if (strncmp(footer.signature, TGA2_SIGNATURE, 16) == 0) {
						if (footer.extensionOffset != 0) mFlags |= TGAF_TGA2;
					}
				}
			}
			if (err == 0 && (mFlags & TGAF_TGA2)) {
				if (File_Seek(footer.extensionOffset, SEEK_SET) == -1) err = TGAERR_READ;
				if (err == 0) {
					if (File_Read(&mExtension, sizeof(TGA2Extension)) != sizeof(TGA2Extension)) err = TGAERR_READ;
				}
			}
			if (err == 0 && File_Seek(0, SEEK_SET) == -1) {
				err = TGAERR_READ;
			} else {
				got = File_Read(&Header, sizeof(TGAHeader));
				if (got != sizeof(TGAHeader)) err = TGAERR_READ;
			}
			if (err == 0 && Header.idLength != 0) {
				if (File_Seek(Header.idLength, SEEK_CUR) == -1) err = TGAERR_READ;
			}
		} else {
			err = TGAERR_OPEN;
		}
		break;
	case TGA_WRITEMODE:
		if (!File_Open_Write(name)) err = TGAERR_OPEN;
		break;
	case TGA_RDWRMODE:
		if (File_Open_ReadWrite(name)) {
			got = File_Read(&Header, sizeof(TGAHeader));
			if (got != sizeof(TGAHeader)) err = TGAERR_READ;
			if (err == 0 && Header.idLength != 0) {
				if (File_Seek(Header.idLength, SEEK_CUR) == -1) err = TGAERR_READ;
			}
		} else {
			err = TGAERR_OPEN;
		}
		break;
	}
	if (err != 0) Close();
	return err;
}

// FUNCTION: BFME2 0x006144B0 size=318 name=?DecodeImage@Targa@@AAEJXZ evidence=N:ledger
long Targa::DecodeImage()
{
	char *out;
	char *first;
	unsigned char packet;
	unsigned char bpp;
	unsigned long left;
	unsigned long bytes;
	unsigned long rep, b;
	long err = 0;
	out = mImage;
	bpp = TGA_BytesPerPixel(Header.pixelDepth);
	left = Header.width * Header.height;
	while (left > 0 && err == 0) {
		if (File_Read(&packet, 1) == 1) {
			if (packet & 0x80) {
				// A repeat packet: one pixel, repeated.
				packet &= 0x7F;
				packet++;
				if (File_Read(out, bpp) == bpp) {
					first = out;
					out += bpp;
					for (rep = 1; rep < packet; rep++) {
						for (b = 0; b < bpp; b++) *out++ = first[b];
					}
				} else {
					err = TGAERR_READ;
				}
			} else {
				// A raw packet: that many pixels follow as they are.
				packet++;
				bytes = packet * bpp;
				if ((unsigned)File_Read(out, bytes) == bytes) {
					out += bytes;
				} else {
					err = TGAERR_READ;
				}
			}
			left -= packet;
		} else {
			err = TGAERR_READ;
		}
	}
	return err;
}

// FUNCTION: BFME2 0x006145F0 size=434 name=?EncodeImage@Targa@@AAEJXZ evidence=N:ledger
long Targa::EncodeImage()
{
	long err = 0;
	long bpp = TGA_BytesPerPixel(Header.pixelDepth);
	char *packet = (char *)malloc(128 * bpp);
	if (packet != NULL) {
	long pixelsLeft = Header.width * Header.height;
	char *runStart = mImage;
	char *cursor = runStart;
	long repeats = 0;
	long fill;
	packet[0] = 0;
	fill = 1;
	while (pixelsLeft != 0 && err == 0) {
		long same = 1;
		cursor += bpp;
		pixelsLeft--;
		long k;
		for (k = 0; k < bpp; k++) {
			if (runStart[k] != cursor[k]) {
				same = 0;
				break;
			}
		}
		if (same == 1) {
			repeats++;
			if (repeats < 128) {
				if (packet[0] == 0) continue;
			} else {
				repeats--;
			}
		}
		if (repeats != 0 && packet[0] == 0) {
			packet[0] = (char)(repeats | 0x80);
			for (k = 0; k < bpp; k++) packet[k + 1] = runStart[k];
			if (File_Write(packet, bpp + 1) != bpp + 1) err = TGAERR_WRITE;
			runStart = cursor;
			repeats = 0;
			packet[0] = 0;
		} else {
			if (repeats == 0) {
				for (k = 0; k < bpp; k++) packet[fill + k] = runStart[k];
				packet[0]++;
				runStart = cursor;
				fill += bpp;
			}
			if (packet[0] == 127 || repeats != 0 || pixelsLeft == 0) {
				k = packet[0];
				packet[0]--;
				if (File_Write(packet, k * bpp + 1) != k * bpp + 1) err = TGAERR_WRITE;
				fill = 1;
				packet[0] = 0;
			}
		}
	}
	free(packet);
	} else {
		err = TGAERR_NOMEM;
	}
	return err;
}

// FUNCTION: BFME2 0x006147B0 size=364 name=?Load@Targa@@QAEJPBDPAD1_N@Z evidence=N:ledger
long Targa::Load(const char *name, char *palette, char *image, bool invert_image)
{
	long bytes;
	long bpp;
	long err = 0;
	if (Open(name, TGA_READMODE) == 0) {
	if (Header.colorMapType == 1) {
		bpp = Header.cmapDepth >> 3;
		bytes = Header.cmapLength * bpp;
		if (palette != NULL && Header.cmapLength > 0) {
			palette += Header.cmapStart * bpp;
			if (File_Read(palette, bytes) != bytes) err = TGAERR_READ;
		} else {
			if (File_Seek(bytes, SEEK_CUR) == -1) err = TGAERR_READ;
		}
	}
	if (err == 0 && image != NULL) {
		bpp = TGA_BytesPerPixel(Header.pixelDepth);
		bytes = Header.width * Header.height * bpp;
		switch (Header.imageType) {
		case TGA_CMAPPED:
			if (File_Read(image, bytes) != bytes) err = TGAERR_READ;
			break;
		case TGA_TRUECOLOR:
			if (File_Read(image, bytes) == bytes) {
				if (invert_image) InvertImage();
			} else {
				err = TGAERR_READ;
			}
			break;
		case TGA_MONO:
			if (File_Read(image, bytes) != bytes) err = TGAERR_READ;
			break;
		case TGA_CMAPPED_ENCODED:
			err = DecodeImage();
			break;
		case TGA_TRUECOLOR_ENCODED:
			err = DecodeImage();
			if (err == 0 && invert_image) InvertImage();
			break;
		default:
			err = TGAERR_NOTSUPPORTED;
			break;
		}
		if (err == 0) {
			// The image is delivered with its origin at the bottom left, so flip whichever
			// axis the file stored the other way round and clear the flag.
			if (Header.imageDescriptor & TGAIDF_XORIGIN) {
				XFlip();
				Header.imageDescriptor &= ~TGAIDF_XORIGIN;
			}
			if (Header.imageDescriptor & TGAIDF_YORIGIN) {
				YFlip();
				Header.imageDescriptor &= ~TGAIDF_YORIGIN;
			}
		}
	}
	Close();
	} else {
		err = TGAERR_OPEN;
	}
	return err;
}

// FUNCTION: BFME2 0x00614950 size=310 name=?Load@Targa@@QAEJPBDJ_N@Z evidence=N:ledger
long Targa::Load(const char *name, long flags, bool invert_image)
{
	long bytes;
	long err = 0;
	if (Open(name, TGA_READMODE) == 0) {
	if ((flags & TGAF_PAL) && Header.colorMapType == 1) {
		if (mPalette != NULL && (mFlags & TGAF_PAL)) {
			free(mPalette);
			mPalette = NULL;
			mFlags &= ~TGAF_PAL;
		}
		if (mPalette == NULL && !(mFlags & TGAF_PAL)) {
			bytes = Header.cmapLength * (Header.cmapDepth >> 3);
			if (bytes != 0) {
				mPalette = (char *)malloc(bytes);
				if (mPalette != NULL) {
					mFlags |= TGAF_PAL;
				} else {
					err = TGAERR_NOMEM;
				}
			}
		}
	}
	if (err == 0 && (flags & TGAF_IMAGE)) {
		if (mImage != NULL && (mFlags & TGAF_IMAGE)) {
			free(mImage);
			mImage = NULL;
			mFlags &= ~TGAF_IMAGE;
		}
		if (mImage == NULL && !(mFlags & TGAF_IMAGE)) {
			bytes = Header.width * Header.height * TGA_BytesPerPixel(Header.pixelDepth);
			if (bytes != 0) {
				mImage = (char *)malloc(bytes);
				if (mImage != NULL) {
					mFlags |= TGAF_IMAGE;
				} else {
					err = TGAERR_NOMEM;
				}
			}
		}
	}
	if (err == 0) err = Load(name, mPalette, mImage, invert_image);
	Close();
	} else {
		err = TGAERR_OPEN;
	}
	return err;
}

// FUNCTION: BFME2 0x00614A90 size=616 name=?Save@Targa@@QAEJPBDJ_N@Z evidence=N:ledger
long Targa::Save(const char *name, long flags, bool addextension)
{
	long bytes;
	long bpp;
	long err = 0;
	TGA2Footer footer;
	if (Open(name, TGA_WRITEMODE) == 0) {
	Header.idLength = 0;
	if (flags & TGAF_COMPRESS) {
		switch (Header.imageType) {
		case TGA_CMAPPED:
		case TGA_TRUECOLOR:
		case TGA_MONO:
			Header.imageType += 8;
			break;
		case TGA_CMAPPED_ENCODED:
		case TGA_TRUECOLOR_ENCODED:
		case TGA_MONO_ENCODED:
			break;
		default:
			flags &= ~TGAF_COMPRESS;
			break;
		}
	}
	if (File_Write(&Header, sizeof(TGAHeader)) != sizeof(TGAHeader)) err = TGAERR_WRITE;
	if (err == 0 && (flags & TGAF_PAL) && mPalette != NULL && Header.cmapLength > 0) {
		bpp = Header.cmapDepth >> 3;
		char *source = mPalette + Header.cmapStart * bpp;
		bytes = Header.cmapLength * bpp;
		char *copy = (char *)malloc(bytes);
		if (copy != NULL) {
			memcpy(copy, source, bytes);
			if (File_Write(copy, bytes) != bytes) err = TGAERR_WRITE;
			free(copy);
		} else {
			err = TGAERR_NOMEM;
		}
	}
	if (err == 0 && (flags & TGAF_IMAGE) && mImage != NULL) {
		bool swapped;
		if (Header.imageType == TGA_TRUECOLOR || Header.imageType == TGA_TRUECOLOR_ENCODED) {
			InvertImage();
			swapped = true;
		} else {
			swapped = false;
		}
		if (flags & TGAF_COMPRESS) {
			EncodeImage();
		} else {
			bpp = TGA_BytesPerPixel(Header.pixelDepth);
			bytes = Header.width * Header.height * bpp;
			if (File_Write(mImage, bytes) != bytes) err = TGAERR_WRITE;
		}
		if (swapped) InvertImage();
	}
	if (addextension) {
		if (err == 0) {
			mExtension.extSize = 495;
			strncpy(mExtension.softwareId, "Denzil's Targa Code", 41);
			mExtension.softwareVersion.number = 1 * 100;
			mExtension.softwareVersion.letter = 0;
			footer.extensionOffset = File_Seek(0, SEEK_CUR);
			if (footer.extensionOffset == -1) err = TGAERR_WRITE;
			if (err == 0 && File_Write(&mExtension, sizeof(TGA2Extension)) != sizeof(TGA2Extension)) err = TGAERR_WRITE;
		}
	} else {
		footer.extensionOffset = 0;
	}
	if (err == 0) {
		footer.developerOffset = 0;
		strncpy(footer.signature, TGA2_SIGNATURE, 16);
		footer.reservedChar = '.';
		footer.terminator = 0;
		if (File_Write(&footer, sizeof(TGA2Footer)) != sizeof(TGA2Footer)) err = TGAERR_WRITE;
	}
	Close();
	} else {
		err = TGAERR_OPEN;
	}
	return err;
}

