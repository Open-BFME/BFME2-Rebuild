#pragma once
// Target file-system output and the old canonical header: four unsigned words.
// Synthesis found one compiled layout and one unresolved shim view.
// GameTextManager embeds this record at+C..+1B. No constructor or vptr.
struct FileInfo {
    unsigned int sizeHigh, sizeLow;
    unsigned int timestampHigh, timestampLow;
};
typedef char FileInfoSize[sizeof(FileInfo)==0x10 ? 1 : -1];
