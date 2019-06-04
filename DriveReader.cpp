//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DriveReader.h"
#include "FileSystemReader.h"
#include "IteratorDecorator.h"
#include <sstream>
#include <iomanip>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

using namespace std;

int sizeCurrentCluster = 0;
int maxCurrentCluster = 0;

TForm1 *Form1;
//---------------------------------------------------------------------------
//������ ������ ������
void TForm1::GetLetterDrive()
{
	int n;
	DWORD dr = GetLogicalDrives(); // ������� ���������� ������� �����
	for(int x=0;x<26;x++) // ���������� ������ �� �����
	{
		 n = ((dr>>x)&1); // ����� �������� �������� ����
		 if(n) // ���� ������� - ���� � ������� x ����
		 {
			 AnsiString dl = AnsiString((char)(65+x)); // �������� ������ �����
			 AnsiString path = AnsiString((char)(65+x))+":\\";
			 AnsiString out = "���� " + dl;
			 ListBoxDrive->Items->Add(out);
		 }
	}
}
//---------------------------------------------------------------------------
//����� ��� �������� ���������� �����
HANDLE TForm1::OpenDriveMethod()
{
	const char d = ListBoxDrive->Items->Strings[ListBoxDrive->ItemIndex][6];
	const char *fileName = "\\\\.\\:";
	size_t len = strlen(fileName);
	char* fileName2 = new char[len+1];
	strcpy(fileName2, fileName);
	fileName2[4] = d;
	fileName2[5] = ':';
	Memo1->Lines->Add(fileName2);
	HANDLE fileHandle = CreateFileA(
			fileName2, // ��� ����� (WCHAR*)
			GENERIC_READ,	  // ����� �������
			FILE_SHARE_READ | FILE_SHARE_WRITE, // ����� ���������� ������
			NULL, // �������� ������������
			OPEN_EXISTING, // ������ ��������
			FILE_ATTRIBUTE_NORMAL, // ����� � ��������
			NULL // ��������� (�������������) ����� ������� � ������� ������� GENERIC_READ.
		);
	if(fileHandle == INVALID_HANDLE_VALUE)
	{
		Memo1->Lines->Add("������ �������� �����");// ��������� ������
	}
	return fileHandle;
}
//---------------------------------------------------------------------------
//����� ��� �c������� ������� ��������� � �����
void TForm1::SetFilePointerMethod(HANDLE fileHandle, ULONGLONG startOffset)
{
	startOffset = (startOffset-1)*sizeCurrentCluster;
	LARGE_INTEGER sectorOffset;
	sectorOffset.QuadPart = startOffset;
	unsigned long currentPosition = SetFilePointer(
			fileHandle,
			sectorOffset.LowPart,
			&sectorOffset.HighPart,
			FILE_BEGIN // ����� � �����, ������������ ������� ���������� ����������������� (FILE_BEGIN, FILE_CURRENT, FILE_END)
		);

	if(currentPosition != sectorOffset.LowPart)
	{
		Memo1->Lines->Add("������ ������� ��������� �����");// ��������� ������
	}
}
//---------------------------------------------------------------------------
//����� ������ �����
AnsiString TForm1::DriveReadMethod(HANDLE fileHandle, int size)
{
	BYTE dataBuffer[size];
	DWORD bytesRead;
	bool readResult = ReadFile(
			fileHandle,
			dataBuffer,
			sizeof(dataBuffer),
			&bytesRead,
			NULL
			);

	if(!readResult || bytesRead != sizeof(dataBuffer))
	{
		Memo1->Lines->Add("������ ������ �������");// ��������� ������
	}
	wstringstream ss;
	for(int i = 0; i < size; i++)
	{
			ss << hex << setfill(L'0') << setw(2) <<  dataBuffer[i];
	}
	return (ss.str()).c_str();
}
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	ButtonReadCluster->Enabled = False;
	ButtonDriveInfo->Enabled = False;
	ListBoxDrive->Items->Clear();
	Memo1->Lines->Clear();
	GetLetterDrive();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ShowDriveInfo(TObject *Sender)
{
	Memo1->Lines->Clear();
	FileSystemReader* drive = NULL;
	HANDLE fileHandle = OpenDriveMethod();
	AnsiString ss = DriveReadMethod(fileHandle, 512);
	if ((ss[7] == '4') && (ss[8] == 'e')) {
		Memo1->Lines->Add("NTFS");
		drive = new NTFS_FileSystemReader;
	}else if ((ss[7] == '4') && (ss[8] == '5')) {
				Memo1->Lines->Add("exFAT");
				drive = new ExFAT_FileSystemReader;
		  }else if ((ss[7] == '4') && (ss[8] == 'd')) {
					Memo1->Lines->Add("FAT");
					drive = new FAT_FileSystemReader;
				}
	if (drive != NULL)
	{
		int bytePerSector = drive->BytePerSector(ss);
		String info = "� ������� ���������� ";
		info += bytePerSector;
		info += " ����";
		Memo1->Lines->Add(info);

		int sectorPerCluster = drive->SectorPerCluster(ss);
		info = "� �������� ���������� ";
		info += sectorPerCluster;
		info += " ��������";
		Memo1->Lines->Add(info);

		int bytePerCluster = bytePerSector * sectorPerCluster;
		info = "� �������� ���������� ";
		info += bytePerCluster;
		info += " ����";
		sizeCurrentCluster = bytePerCluster;
		Memo1->Lines->Add(info);

		int sectorPerFileSystem = drive->SectorPerFileSystem(ss);
		info = "� ���� ���������� ";
		info += sectorPerFileSystem;
		info += " ��������";
		Memo1->Lines->Add(info);

		int bytePerFileSystem = sectorPerFileSystem * bytePerSector;
		info = "� ���� ���������� ";
		info += bytePerFileSystem;
		info += " ����";
		Memo1->Lines->Add(info);

		int clusterPerFileSystem = sectorPerFileSystem / sectorPerCluster;
		info = "� ���� ���������� ";
		info += clusterPerFileSystem;
		info += " ���������";
        maxCurrentCluster = clusterPerFileSystem;
		Memo1->Lines->Add(info);
	}

	delete drive;
	CloseHandle(fileHandle);
	ButtonReadCluster->Enabled = True;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ClustersRead(TObject *Sender)
{
    Memo1->Lines->Clear();
	HANDLE fileHandle = OpenDriveMethod();
	int clusterFrom = StrToInt(EditFrom->Text);
	int clusterTo = StrToInt(EditTo->Text);
	Iterator *iter = new ClusterIterator;
	iter->First(fileHandle, clusterFrom, sizeCurrentCluster, maxCurrentCluster);
	for(clusterFrom; clusterTo; clusterFrom++)
	{
		String currentCluster = iter->GetCurrent(fileHandle, sizeCurrentCluster);
        ShowTxtCluster(clusterFrom, currentCluster);
        if (iter->IsDone(clusterFrom, clusterTo, maxCurrentCluster) == true) {
            break;
        }
		iter->Next(fileHandle, clusterFrom + 1, sizeCurrentCluster, maxCurrentCluster);
	}

	CloseHandle(fileHandle);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ListBoxClick(TObject *Sender)
{
	ButtonReadCluster->Enabled = False;
	ButtonDriveInfo->Enabled = True;
	sizeCurrentCluster = 0;
    maxCurrentCluster = 0;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SaveAs(TObject *Sender)
{
    if (SaveTextFileDialog1->Execute())
	{
		Memo1->Lines->SaveToFile(SaveTextFileDialog1->FileName);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::SearchTxt(TObject *Sender)
{
    Memo1->Lines->Clear();
    HANDLE fileHandle = OpenDriveMethod();
	Iterator *decor = new TxtClusterIteratorDecorator;
	decor->First(fileHandle, 1, sizeCurrentCluster, maxCurrentCluster);
	CloseHandle(fileHandle);
}
//---------------------------------------------------------------------------
void TForm1::ShowTxtCluster(int i, AnsiString c)
{
	Memo1->Lines->Add(i);
    Memo1->Lines->Add(c);
}

