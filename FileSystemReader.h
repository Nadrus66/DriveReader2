//---------------------------------------------------------------------------

#ifndef FileSystemReaderH
#define FileSystemReaderH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
//---------------------------------------------------------------------------
class FileSystemReader
{
public:
	virtual int BytePerSector(AnsiString ss) = 0;
	virtual int SectorPerCluster(AnsiString ss) = 0;
	virtual int SectorPerFileSystem(AnsiString ss) = 0;
	virtual ~FileSystemReader(){}
};

class NTFS_FileSystemReader : public FileSystemReader
{
public:
	virtual int BytePerSector(AnsiString ss);
	virtual int SectorPerCluster(AnsiString ss);
	virtual int SectorPerFileSystem(AnsiString ss);
	virtual ~NTFS_FileSystemReader(){}
};

class FAT_FileSystemReader : public FileSystemReader
{
public:
	virtual int BytePerSector(AnsiString ss);
	virtual int SectorPerCluster(AnsiString ss);
	virtual int SectorPerFileSystem(AnsiString ss);
	virtual ~FAT_FileSystemReader(){}
};

class ExFAT_FileSystemReader : public FileSystemReader
{
public:
	virtual int BytePerSector(AnsiString ss);
	virtual int SectorPerCluster(AnsiString ss);
	virtual int SectorPerFileSystem(AnsiString ss);
	virtual ~ExFAT_FileSystemReader(){}
};
#endif
