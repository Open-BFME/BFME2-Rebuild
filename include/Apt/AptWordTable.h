#pragma once
// SIZE 0x8. A zero-fillable table of 32-bit words: a used count, the number of words, and the block they live in.
// 0x006E3C40 reads the word count and 0x006E3C50 empties the table (the map files them under the placeholder
// classes Rva006E3C40WordIntField and Rva006E3C50Table; the names here are this header's).
struct AptWordTable
{
    unsigned short mnUsed;  // 0x0
    unsigned short mnWords; // 0x2
    unsigned int *mpWords;  // 0x4

    int getWordCount() const; // 0x006E3C40 (AptAnimation.cpp)
    void clear();             // 0x006E3C50 (AptAnimation.cpp)
};
