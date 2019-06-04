//---------------------------------------------------------------------------

#pragma hdrstop

#include "IteratorDecorator.h"
#include "DriveReader.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
using namespace std;
TForm1 *Form2;
//---------------------------------------------------------------------------
//Итератор
//---------------------------------------------------------------------------
void ClusterIterator::First(HANDLE fileHandle, ULONGLONG startOffset, int sizeCurrentCluster, int maxCurrentCluster)
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
		Form2->Memo1->Lines->Add("Ошибка позиции указателя файла");// Обработка ошибки
	}
}
//---------------------------------------------------------------------------
AnsiString ClusterIterator::GetCurrent(HANDLE fileHandle, int sizeCurrentCluster)
{
	AnsiString currentCluster = Form1->DriveReadMethod(fileHandle, sizeCurrentCluster);
	return currentCluster;
}
//---------------------------------------------------------------------------
void ClusterIterator::Next(HANDLE fileHandle, ULONGLONG startOffset, int sizeCurrentCluster, int maxCurrentCluster)
{
    First(fileHandle, startOffset, sizeCurrentCluster, maxCurrentCluster);
}
//---------------------------------------------------------------------------
bool ClusterIterator::IsDone(int clusterFrom, int clusterTo, int maxCurrentCluster)
{
    if (clusterFrom == clusterTo) {
        return true;
    }
    if (clusterFrom == maxCurrentCluster) {
        return true;
    }
    return false;
}
//---------------------------------------------------------------------------
//Декоратор
//---------------------------------------------------------------------------
Iterator *simpleIterator = new ClusterIterator;
void TxtClusterIteratorDecorator::First(HANDLE fileHandle, ULONGLONG startOffset, int sizeCurrentCluster, int maxCurrentCluster)
{
    simpleIterator->First(fileHandle, startOffset, sizeCurrentCluster, maxCurrentCluster);
    int i = 1;
    int n = 0;
    AnsiString c = "";
    for (i; maxCurrentCluster; i++) {
    	c = simpleIterator->GetCurrent(fileHandle, sizeCurrentCluster);
        n = c.Pos("2e007400780074");//2e007400780074
        if(n != 0) {
            Form2->ShowTxtCluster(i, c);
        	break;
        }
        if (simpleIterator->IsDone(i, maxCurrentCluster, maxCurrentCluster) == true) {
            Form2->Memo1->Lines->Add("Ничего нет");
            break;
        }
        simpleIterator->Next(fileHandle, i+1, sizeCurrentCluster, maxCurrentCluster);
    }

}
//---------------------------------------------------------------------------
AnsiString TxtClusterIteratorDecorator::GetCurrent(HANDLE fileHandle, int sizeCurrentCluster)
{
    return simpleIterator->GetCurrent(fileHandle, sizeCurrentCluster);
}
//---------------------------------------------------------------------------
void TxtClusterIteratorDecorator::Next(HANDLE fileHandle, ULONGLONG startOffset, int sizeCurrentCluster, int maxCurrentCluster)
{
    return simpleIterator->Next(fileHandle, startOffset, sizeCurrentCluster, maxCurrentCluster);
}
//---------------------------------------------------------------------------
bool TxtClusterIteratorDecorator::IsDone(int clusterFrom, int clusterTo, int maxCurrentCluster)
{
    return simpleIterator->IsDone(clusterFrom, clusterTo, maxCurrentCluster);
}
//---------------------------------------------------------------------------
