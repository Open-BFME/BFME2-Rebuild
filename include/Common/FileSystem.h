#pragma once
class File;
class AsciiString;
struct FileInfo;
// Native factory225F44 allocates4; ctor600611 stores table87A660, whose
// only slot is deleting dtor6006BD. No SubsystemInterface or members.
// openFile600C34/getFileInfo600E3A use the global disk/archive services.
class FileSystem {
public:
    FileSystem();
    virtual ~FileSystem();
    File *openFile(const char *, int, int = 0);
    File *rva00600676(const unsigned short *, int, int);	// 0x00600676: the wide-path openFile
    bool rva006006A9(const unsigned short *path);	// 0x006006A9: the wide-path doesFileExist (the archive's slot 11)
    File *rva000783A1(const char *filename, int access, int first, int second);	// 0x000783A1 (the map spells it rva000783A1@FileSystem): openFile with the two ints passed as a pair (GetGameFilePart 0x0007882F)
    File *openFile(const unsigned short *path, int access);	// 0x002DBA07 (map: rva002DBA07@FileSystem): rva00600676 with flags 0
    bool createDirectory(const AsciiString &); // 0x003006C4 (the map spells it rva003006C4@Rva003006C4); ignores this, passes the path to 0x00600695
    bool doesFileExist(const char *) const;
    bool getFileInfo(const char *, FileInfo *) const;
    bool getFileInfo(const AsciiString &, FileInfo *) const;
};
typedef char FileSystemSize[sizeof(FileSystem)==4 ? 1 : -1];
extern FileSystem *TheFileSystem;
