#pragma once
// On-disk CSF records consumed by GameTextTable::readCSF2E5EA5.
// Four-byte tags and lengths, little endian; the string payload is inverted UTF-16.
enum {
    CSF_ID = 0x43534620, CSF_LABEL = 0x4c424c20,
    CSF_STRING = 0x53545220, CSF_STRINGWITHWAVE = 0x53545257
};
struct CSFHeader {
    int id, version;
    unsigned int num_labels;
    int num_strings, skip, langid;
};
struct CSFLabel {
    int id;
    unsigned int num_strings;
    int length;
};
struct CSFString { int id, length; };
