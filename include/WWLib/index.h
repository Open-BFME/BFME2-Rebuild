// index.h: WWLib's sorted lookup table from a key to a stored value (key, value pairs kept in
// an array and sorted lazily on the first search).
// Target facts (retail game.dat), from the instances ini.cpp's unit emits (IndexClass<int,
// INIEntry *> and IndexClass<int, INISection *>, which ICF folds into one body each):
//  - SIZE 0x14 with no vptr: table pointer +0, element count +4, capacity +8, sorted flag
//    +0xC (a byte), last-found element +0x10. The constructor 0x006160E0 zeroes all five.
//  - An element is 8 bytes, key then value. The table is allocated as a counted array of
//    elements (the array constructor loop runs an empty element constructor) and freed with
//    the plain operator delete; Clear 0x00616170 frees it and zeroes the five fields, and the
//    destructor body 0x00616960 is Clear.
//  - Add_Index 0x006166D0 grows by the current capacity (ten when empty) through
//    Increase_Table_Size 0x00616190, appends, and marks the table unsorted. Remove_Index
//    0x00616100 finds the key linearly, shifts the tail down, clears the vacated slot and the
//    last-found pointer, and stays sorted. Search_For_Node 0x00616730 sorts with qsort
//    (comparison 0x00615D20) when unsorted, then binary-searches (0x00616260).
//  - Is_Present 0x00616A20 and operator[] 0x006169E0 first compare the key with the last-found
//    element's. A missing key gives a reference to a function-local zero value, not a fault.
#pragma once
#ifndef BFME2_INDEX_H
#define BFME2_INDEX_H

#include <stdlib.h>
#include "WWLib/bsearch.h"

template<class K, class V>
class IndexClass
{
	struct NodeElement
	{
		NodeElement() {}						// folded empty body, 0x00581450
		bool operator<(const NodeElement &other) const { return ID < other.ID; }	// 0x00615D50
		bool operator==(const NodeElement &other) const { return ID == other.ID; }

		K ID;			// +0x00
		V Data;			// +0x04
	};

public:
	IndexClass() : IndexTable(0), IndexCount(0), IndexSize(0), IsSorted(false), ArchivePtr(0) {}	// 0x006160E0
	~IndexClass() { Clear(); }						// 0x00616960

	void Clear()								// 0x00616170
	{
		delete[] IndexTable;
		IndexTable = 0;
		IndexCount = 0;
		IndexSize = 0;
		IsSorted = false;
		ArchivePtr = 0;
	}
	int Count() const { return IndexCount; }

	bool Add_Index(const K &id, const V &data)				// 0x006166D0
	{
		if (IndexCount + 1 > IndexSize) {
			if (!Increase_Table_Size(IndexSize != 0 ? IndexSize : 10)) return false;
		}
		IndexTable[IndexCount].ID = id;
		IndexTable[IndexCount].Data = data;
		IndexCount++;
		IsSorted = false;
		return true;
	}

	bool Remove_Index(const K &id)						// 0x00616100
	{
		int found = -1;
		for (int i = 0; i < IndexCount; i++) {
			if (IndexTable[i].ID == id) {
				found = i;
				break;
			}
		}
		if (found == -1) return false;
		for (int i = found + 1; i < IndexCount; i++) IndexTable[i - 1] = IndexTable[i];
		IndexCount--;
		IndexTable[IndexCount].ID = K();
		IndexTable[IndexCount].Data = V();
		ArchivePtr = 0;
		return true;
	}

	bool Is_Present(const K &id) const					// 0x00616A20
	{
		if (IndexCount == 0) return false;
		if (Is_Archive_Same(id)) return true;
		const NodeElement *node = Search_For_Node(id);
		if (node != 0) {
			ArchivePtr = node;
			return true;
		}
		return false;
	}

	const V &operator[](const K &id) const					// 0x006169E0
	{
		if (Is_Present(id)) return ArchivePtr->Data;
		static V missing;
		return missing;
	}

private:
	bool Is_Archive_Same(const K &id) const					// 0x00615D00
	{
		return ArchivePtr != 0 && ArchivePtr->ID == id;
	}
	void Invalidate_Archive() const { ArchivePtr = 0; }			// 0x00196340

	bool Increase_Table_Size(int amount)					// 0x00616190
	{
		if (amount < 0) return false;
		NodeElement *table = new NodeElement[IndexSize + amount];
		if (table == 0) return false;
		for (int i = 0; i < IndexCount; i++) table[i] = IndexTable[i];
		delete[] IndexTable;
		IndexSize += amount;
		IndexTable = table;
		ArchivePtr = 0;
		return true;
	}

	const NodeElement *Search_For_Node(const K &id) const			// 0x00616730
	{
		if (IndexCount == 0) return 0;
		if (!IsSorted) {
			qsort(IndexTable, IndexCount, sizeof(NodeElement), search_compfunc);
			ArchivePtr = 0;
			IsSorted = true;
		}
		NodeElement probe;
		probe.ID = id;
		return Binary_Search(IndexTable, IndexCount, probe);
	}

	static int __cdecl search_compfunc(const void *a, const void *b)	// 0x00615D20
	{
		const K &ka = ((const NodeElement *)a)->ID;
		const K &kb = ((const NodeElement *)b)->ID;
		if (ka == kb) return 0;
		return (ka < kb) ? -1 : 1;
	}

	NodeElement *IndexTable;			// +0x00
	int IndexCount;				// +0x04
	int IndexSize;				// +0x08
	mutable bool IsSorted;			// +0x0C
	mutable const NodeElement *ArchivePtr;	// +0x10
};
// SIZE 0x14

#endif
