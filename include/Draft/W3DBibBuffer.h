// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// W3DBibBuffer.h: draw buffer for the building bibs (Zero Hour's W3DDevice/GameClient/W3DBibBuffer.h).
//
// Target facts (retail game.dat 1.06, RVAs):
//  - The constructor 0x000D42B3 stores the buffers at +0/+8 with their sizes at +4/+0xC (0x100 and 0x180) and the two textures at +0x10/+0x14.
//  - The bib records are 0x44 bytes starting at +0x28 (clearAllBibs 0x000D3D35 zeroes the count at +0x109C8 and sets the changed flag at +0x109CC;
//    removeHighlighting 0x000D3D44 clears the byte at +0x58 + n*0x44; removeBib 0x000D3D5F matches the id at +0x60 and removeBibDrawable 0x000D3D97 the id at +0x64).
// Layout of the record and the member order follow Zero Hour's W3DBibBuffer.h; the offsets above agree with it.
#pragma once
#ifndef BFME2_W3DBIBBUFFER_H
#define BFME2_W3DBIBBUFFER_H

#include <stddef.h>
#include "Common/GameType.h"
#include "WWMath/Vector3.h"

class DX8VertexBufferClass;
class DX8IndexBufferClass;
class TextureClass;

/// The individual data for a bib.
struct TBib
{
	Vector3		m_corners[4];	///< drawing location
	Bool		m_highlight;	///< use the highlight texture
	Int			m_color;
	ObjectID	m_objectID;
	DrawableID	m_drawableID;
	Bool		m_unused;		///< true if this bib is currently unused
};

class W3DBibBuffer
{
public:
	W3DBibBuffer();
	~W3DBibBuffer();

	void addBib(Vector3 corners[4], ObjectID id, Bool highlight);
	void addBibDrawable(Vector3 corners[4], DrawableID id, Bool highlight);
	void removeBib(ObjectID id);
	void removeBibDrawable(DrawableID id);
	void clearAllBibs();
	void removeHighlighting();
	void renderBibs();

	enum { INITIAL_BIB_VERTEX = 256, INITIAL_BIB_INDEX = 384, MAX_BIBS = 1000 };

	DX8VertexBufferClass *m_vertexBib;		// +0x00
	Int m_vertexBibSize;					// +0x04
	DX8IndexBufferClass *m_indexBib;		// +0x08
	Int m_indexBibSize;						// +0x0C
	TextureClass *m_bibTexture;				// +0x10
	TextureClass *m_highlightBibTexture;	// +0x14
	Int m_curNumBibVertices;				// +0x18
	Int m_curNumBibIndices;					// +0x1C
	Int m_curNumNormalBibIndices;			// +0x20
	Int m_curNumNormalBibVertex;			// +0x24
	TBib m_bibs[MAX_BIBS];					// +0x28
	Int m_numBibs;							// +0x109C8
	Bool m_anythingChanged;					// +0x109CC
	Bool m_updateAllKeys;					// +0x109CD
	Bool m_initialized;						// +0x109CE
	Bool m_isTerrainPass;					// +0x109CF

	void loadBibsInVertexAndIndexBuffers();
	void allocateBibBuffers();
	void freeBibBuffers();
};
typedef char W3DBibBufferBibsOffset[(offsetof(W3DBibBuffer, m_bibs) == 0x28) ? 1 : -1];
typedef char W3DBibBufferNumBibsOffset[(offsetof(W3DBibBuffer, m_numBibs) == 0x109C8) ? 1 : -1];
typedef char TBibSize[(sizeof(TBib) == 0x44) ? 1 : -1];

#endif
