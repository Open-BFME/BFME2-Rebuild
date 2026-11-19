// bsearch.h: binary search over a sorted array of records.
// Target facts (retail game.dat): the only instance is for IndexClass<int, INIEntry *>'s
// NodeElement, 0x00616260 (70 bytes), called from Search_For_Node 0x00616730 with the
// table, the element count and a probe record on the stack. It compares with the record's
// operator< (0x00615D50, inline at the call) and finds equality by comparing the key.
#pragma once
#ifndef BFME2_BSEARCH_H
#define BFME2_BSEARCH_H

template<class T>
T *Binary_Search(T *table, int count, const T &key)
{
	const T *base = table;
	int remaining = count;
	while (remaining > 0) {
		int half = remaining / 2;
		const T *probe = base + half;
		if (key < *probe) {
			remaining = half;
		} else if (*probe == key) {
			return (T *)probe;
		} else {
			base = probe + 1;
			remaining -= half + 1;
		}
	}
	return 0;
}

#endif
