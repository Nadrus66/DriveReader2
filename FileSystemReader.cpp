//---------------------------------------------------------------------------

#pragma hdrstop

#include "FileSystemReader.h"
#include <sstream>
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
using namespace std;

//NTFS_FileSystemReader
int NTFS_FileSystemReader::BytePerSector(AnsiString ss)
{
	stringstream ssThis;
	ssThis << ss[25] << ss[26] << ss[23] << ss[24];
	int hexValue;
	ssThis >> hex >> hexValue;
	return hexValue;
}
int NTFS_FileSystemReader::SectorPerCluster(AnsiString ss)
{
	stringstream ssThis;
	ssThis << ss[27] << ss[28];
	int hexValue;
	ssThis >> hex >> hexValue;
	return hexValue;
}
int NTFS_FileSystemReader::SectorPerFileSystem(AnsiString ss)
{
	stringstream ssThis;
	ssThis << ss[95] << ss[96]<< ss[93] << ss[94]<< ss[91] << ss[92]<< ss[89] << ss[90]<< ss[87] << ss[88]<< ss[85] << ss[86]<< ss[83] << ss[84]<< ss[81] << ss[82];
	int hexValue;
	ssThis >> hex >> hexValue;
	return hexValue;
}
//---------------------------------------------------------------------------
//FAT_FileSystemReader
int FAT_FileSystemReader::BytePerSector(AnsiString ss)
{
	stringstream ssThis;
	ssThis << ss[25] << ss[26] << ss[23] << ss[24];
	int hexValue;
	ssThis >> hex >> hexValue;
	return hexValue;
}
int FAT_FileSystemReader::SectorPerCluster(AnsiString ss)
{
	stringstream ssThis;
	ssThis << ss[27] << ss[28];
	int hexValue;
	ssThis >> hex >> hexValue;
	return hexValue;
}
int FAT_FileSystemReader::SectorPerFileSystem(AnsiString ss)
{
	stringstream ssThis;
	ssThis << ss[71] << ss[72]<< ss[69] << ss[70]<< ss[67] << ss[68]<< ss[65] << ss[66];
	int hexValue;
	ssThis >> hex >> hexValue;
	return hexValue;
}
//---------------------------------------------------------------------------
//ExFAT_FileSystemReader
int ExFAT_FileSystemReader::BytePerSector(AnsiString ss)
{
	stringstream ssThis;
	ssThis << ss[217] << ss[218];
	int hexValue;
	ssThis >> hex >> hexValue;
	return pow(2,hexValue);
}
int ExFAT_FileSystemReader::SectorPerCluster(AnsiString ss)
{
	stringstream ssThis;
	ssThis << ss[219] << ss[220];
	int hexValue;
	ssThis >> hex >> hexValue;
	return pow(2,hexValue);
}
int ExFAT_FileSystemReader::SectorPerFileSystem(AnsiString ss)
{
	stringstream ssThis;
	ssThis << ss[159] << ss[160]<< ss[157] << ss[158]<< ss[155] << ss[156]<< ss[153] << ss[154]<< ss[151] << ss[152]<< ss[149] << ss[150]<< ss[147] << ss[148]<< ss[145] << ss[146];
	int hexValue;
	ssThis >> hex >> hexValue;
	return hexValue;
}
