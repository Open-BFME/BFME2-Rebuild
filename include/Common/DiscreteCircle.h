// DiscreteCircle.h: a filled circle as horizontal spans, upper half only.
// Target facts (retail game.dat): the constructor 0x002DF93D stores the centre row at
// +0xC and twice the centre row at +0x10 after an STLport vector of 12-byte spans at +0,
// so SIZE is 0x14. A span is three ints (row, first column, last column; 12 bytes, the
// stride the vector code in generateEdgePairs 0x002DF8CF uses). The constructor reserves
// room for twice the radius, calls generateEdgePairs 0x002DF8CF, then removeDuplicates
// 0x002DF872, which drops a span whose row equals the next one's. drawCircle 0x002DF82C
// calls the callback once per stored span and a second time with the mirrored row
// (twice the centre row minus the span row) unless that is the centre row itself.
#pragma once
#ifndef BFME2_DISCRETECIRCLE_H
#define BFME2_DISCRETECIRCLE_H

#include <vector>

struct HorzLine
{
	int yPos;		// +0x00
	int xStart;		// +0x04
	int xEnd;		// +0x08
};

typedef _STL::vector<HorzLine> VecHorzLine;
typedef VecHorzLine::iterator VecHorzLineIt;

typedef void (*ScanlineDrawFunc)(int xStart, int xEnd, int yPos, void *otherParms);

class DiscreteCircle
{
	VecHorzLine m_edges;		// +0x00
	int m_yPos;			// +0x0C
	int m_yPosDoubled;		// +0x10

public:
	DiscreteCircle(int xCenter, int yCenter, int radius);				// 0x002DF93D
	const VecHorzLine &getEdges() const { return m_edges; }
	int getEdgeCount() const { return m_edges.size(); }
	void drawCircle(ScanlineDrawFunc functionToDrawWith, void *parmToPass);	// 0x002DF82C

protected:
	void generateEdgePairs(int xCenter, int yCenter, int radius);			// 0x002DF8CF
	void removeDuplicates();							// 0x002DF872
};
// SIZE 0x14

#endif
