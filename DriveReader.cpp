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
//Узнать литеры дисков
void TForm1::GetLetterDrive()
{
	int n;
	DWORD dr = GetLogicalDrives(); // функция возвращает битовую маску
	for(int x=0;x<26;x++) // проходимся циклом по битам
	{
		 n = ((dr>>x)&1); // узнаём значение текущего бита
		 if(n) // если единица - диск с номером x есть
		 {
			 AnsiString dl = AnsiString((char)(65+x)); // получаем литеру диска
			 AnsiString path = AnsiString((char)(65+x))+":\\";
			 AnsiString out = "Диск " + dl;
			 ListBoxDrive->Items->Add(out);
		 }
	}
}
//---------------------------------------------------------------------------
//Метод для открытия выбранного диска
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
			fileName2, // Имя файла (WCHAR*)
			GENERIC_READ,	  // Режим доступа
			FILE_SHARE_READ | FILE_SHARE_WRITE, // Режим совместной работы
			NULL, // Атрибуты безопасности
			OPEN_EXISTING, // Способ открытия
			FILE_ATTRIBUTE_NORMAL, // Флаги и атрибуты
			NULL // Описатель (идентификатор) файла шаблона с правами доступа GENERIC_READ.
		);
	if(fileHandle == INVALID_HANDLE_VALUE)
	{
		Memo1->Lines->Add("Ошибка открытия диска");// Обработка ошибки
	}
	return fileHandle;
}
//---------------------------------------------------------------------------
//Метод для уcтановки позиции указателя в файле
void TForm1::SetFilePointerMethod(HANDLE fileHandle, ULONGLONG startOffset)
{
	startOffset = (startOffset-1)*sizeCurrentCluster;
	LARGE_INTEGER sectorOffset;
	sectorOffset.QuadPart = startOffset;
	unsigned long currentPosition = SetFilePointer(
			fileHandle,
			sectorOffset.LowPart,
			&sectorOffset.HighPart,
			FILE_BEGIN // Точка в файле, относительно которой необходимо позиционироваться (FILE_BEGIN, FILE_CURRENT, FILE_END)
		);

	if(currentPosition != sectorOffset.LowPart)
	{
		Memo1->Lines->Add("Ошибка позиции указателя файла");// Обработка ошибки
	}
}
//---------------------------------------------------------------------------
//Метод чтения диска
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
		Memo1->Lines->Add("Ошибка чтения сектора");// Обработка ошибки
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
		String info = "В секторе содержится ";
		info += bytePerSector;
		info += " байт";
		Memo1->Lines->Add(info);

		int sectorPerCluster = drive->SectorPerCluster(ss);
		info = "В кластере содержится ";
		info += sectorPerCluster;
		info += " секторов";
		Memo1->Lines->Add(info);

		int bytePerCluster = bytePerSector * sectorPerCluster;
		info = "В кластере содержится ";
		info += bytePerCluster;
		info += " байт";
		sizeCurrentCluster = bytePerCluster;
		Memo1->Lines->Add(info);

		int sectorPerFileSystem = drive->SectorPerFileSystem(ss);
		info = "В томе содержится ";
		info += sectorPerFileSystem;
		info += " секторов";
		Memo1->Lines->Add(info);

		int bytePerFileSystem = sectorPerFileSystem * bytePerSector;
		info = "В томе содержится ";
		info += bytePerFileSystem;
		info += " байт";
		Memo1->Lines->Add(info);

		int clusterPerFileSystem = sectorPerFileSystem / sectorPerCluster;
		info = "В томе содержится ";
		info += clusterPerFileSystem;
		info += " кластеров";
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

